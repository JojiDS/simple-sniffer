#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <cstdlib>
#include <iostream>
#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <process.h>

using namespace std;


#define MAX_PACKET_SIZE 0x10000
#define SIO_RCVALL 0x98000001

char Buffer[MAX_PACKET_SIZE]; // 64 Kb



typedef struct IPHeader {
	UCHAR iph_verlen;
	UCHAR iph_tos;
	USHORT iph_length;
	USHORT iph_id;
	USHORT iph_offset;
	UCHAR iph_ttl;
	UCHAR iph_protocol;
	USHORT iph_xsum;
	ULONG iph_src;
	ULONG iph_dest;
} IPHeader;

char src[10];
char dest[10];
char ds[15];
unsigned short lowbyte;
unsigned short hibyte;

int main()
{
	WSADATA wsadata;
	SOCKET s;
	char name[128];
	HOSTENT* phe;
	SOCKADDR_IN sa;
	IN_ADDR sa1;
	unsigned long flag = 1; // Флаг PROMISC Вкл/выкл.

	// инициализация
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	s = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
	gethostname(name, sizeof(name));
	phe = gethostbyname(name);
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = ((struct in_addr*)phe->h_addr_list[0])->s_addr;
	bind(s, (SOCKADDR*)&sa, sizeof(SOCKADDR));

	ioctlsocket(s, SIO_RCVALL, &flag);
	bool flagg = 1;

	do
	{
		int count;
		count = recv(s, Buffer, sizeof(Buffer), 0);
		if (count >= sizeof(IPHeader))
		{
			IPHeader* hdr = (IPHeader*)Buffer;
			sa1.s_addr = hdr->iph_src;
			cout << "Src:";
			printf(inet_ntoa(sa1));
			cout << endl;
			cout << "Dest:";
			sa1.s_addr = hdr->iph_dest;
			printf(inet_ntoa(sa1));
			cout << endl;
			if (hdr->iph_protocol == IPPROTO_TCP) printf("TCP ");
			if (hdr->iph_protocol == IPPROTO_UDP) printf("UDP ");
			printf("\n");
			cout << "Do u wanna repead?" « endl;
			cout << "YES - 1 " « endl « "NO - 0 (Exit)" « endl;
			cin >> flagg;

		}

	} while (flagg);


	closesocket(s);
	WSACleanup();
}
