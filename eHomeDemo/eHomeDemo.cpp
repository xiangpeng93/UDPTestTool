// eHomeDemo.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "eHomeDemo.h"
#include <thread>
#include <list>
#include <mutex>

//�����µ��׽���֮ǰ��Ҫ����һ������Ws2_32.dll��ĺ���,����������Ϳͻ������Ӳ���
#pragma comment(lib,"ws2_32.lib")
WSADATA wsaData;                                    //ָ��WinSocket��Ϣ�ṹ��ָ��
SOCKET sockListener = 0;                                //�����׽���
SOCKADDR_IN saUdpServ;                              //ָ��ͨ�Ŷ���Ľṹ��ָ��   
BOOL g_bExit = FALSE;

std::thread myThreadRecv;
std::list<string> MsgQueue;
std::mutex myMutex;

int __stdcall GetMsg(char *buff)
{
	string str = "";
	myMutex.lock();
	if (MsgQueue.size() > 0 )
	{
		str = *(MsgQueue.begin());
		MsgQueue.pop_front();
	}
	myMutex.unlock();
	strncpy(buff, (char *)str.c_str(), str.length()+1);
	//return (char *)str.c_str();
	return 0;
}

void recvThread()
{
	while (!g_bExit)
	{
		if (sockListener != 0)
		{
			char recvBuff[1500] = { 0 };
			int nRet = recvfrom(sockListener, recvBuff, sizeof(recvBuff), 0, NULL, NULL);
			if (nRet >= 0) //������ʧ������ʾ����
			{
				myMutex.lock();
				MsgQueue.push_back(recvBuff);
				myMutex.unlock();

				printf("Recive buffer %s", recvBuff);
			}
			else
			{
				printf("Recive error %d", WSAGetLastError()); 
			}
		}
		Sleep(5);
	}
}

int __stdcall Init()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)    
	{
		printf("Can't initiates windows socket!Program stop.\n");
		return -1;
	}
	sockListener = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&saUdpServ, 0, sizeof(saUdpServ));
	myThreadRecv = std::thread(recvThread);
	myThreadRecv.detach();

	return 0;
}

int __stdcall Connect(char* ip, int port)
{
	saUdpServ.sin_family = AF_INET;
	saUdpServ.sin_addr.s_addr = inet_addr(ip);
	saUdpServ.sin_port = htons(port); 
	//nRet = bind(sockListener, (SOCKADDR FAR *) &saUdpServ, sizeof(saUdpServ));
	return 0;
}

int __stdcall Fini()
{
	g_bExit = TRUE;
	closesocket(sockListener);         //�رռ���socket
	WSACleanup();
	return 0;
}

int __stdcall SendData(char *data)
{
	int nRet = 0;
	nRet = sendto(sockListener, data, strlen(data) + 1, 0, (SOCKADDR *)&saUdpServ, sizeof (saUdpServ));
	if (nRet == SOCKET_ERROR) //������ʧ������ʾ����
	{
		printf("Send Error %d", WSAGetLastError());
	}
	return 0;
}
