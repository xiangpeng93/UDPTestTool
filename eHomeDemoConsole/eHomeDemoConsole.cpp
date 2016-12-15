// eHomeDemoConsole.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../eHomeDemo/eHomeDemo/eHomeDemo.h"
#pragma comment(lib,"../bin/Release/eHomeDemo.lib")

string testMsg = "<?xml version=\"1.0\" encoding=\"UTF - 8\" ?> <PPVSPMessage><Version>4.0</Version><Sequence>1</Sequence><CommandType>REQUEST</CommandType><Method>REGISTER</Method><Command>REGISTER</Command><Params><SerialNumber>DS-AAAAA</SerialNumber><NetUnitType>PU</NetUnitType><DeviceID>test0123</DeviceID><Password></Password><FirmwareVersion>Vx.y.z buildYYMMDD</FirmwareVersion><LocalIP>10.21.130.53</LocalIP><LocalPort>7660</LocalPort><DevType>1</DevType><Manufacture>0</Manufacture><DetectPackage>TRUE</DetectPackage><ReliableTransmission>TRUE</ReliableTransmission><RegisterServer>10.33.30.218</RegisterServer></Params></PPVSPMessage>";

int _tmain(int argc, _TCHAR* argv[])
{
	Init();
	cin.get();
	Connect("10.33.30.217", 7660);
	SendData((char*)testMsg.c_str());
	char line[3];
	cin.getline(line, 2);
	char buffer[1500] = { 0 };
	do
	{
		GetMsg(buffer);
	} while (buffer != "");
	Fini();
	return 0;
}


