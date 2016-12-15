#include <iostream>
#include <WinSock2.h>

using namespace std;
int __stdcall Init();
int __stdcall Fini();
int __stdcall Connect(char* ip,int port);
int __stdcall SendData(char *data);
int __stdcall GetMsg(char *buff);