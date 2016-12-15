// eHomeDemo.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "eHomeDemo.h"
#include <thread>
#include <list>
#include <mutex>

//创建新的套接字之前需要调用一个引入Ws2_32.dll库的函数,否则服务器和客户端连接不上
#pragma comment(lib,"ws2_32.lib")
WSADATA wsaData;                                    //指向WinSocket信息结构的指针
SOCKET sockListener = 0;                                //创建套接字
SOCKADDR_IN saUdpServ;                              //指向通信对象的结构体指针   
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
			if (nRet >= 0) //若接收失败则提示错误
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
	closesocket(sockListener);         //关闭监听socket
	WSACleanup();
	return 0;
}

int __stdcall SendData(char *data)
{
	int nRet = 0;
	nRet = sendto(sockListener, data, strlen(data) + 1, 0, (SOCKADDR *)&saUdpServ, sizeof (saUdpServ));
	if (nRet == SOCKET_ERROR) //若接收失败则提示错误
	{
		printf("Send Error %d", WSAGetLastError());
	}
	return 0;
}
