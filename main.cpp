#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Mswsock.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include "Fonctions.h"
#include <memory>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Kernel32.lib")

const unsigned int MAX_THREADS = 1500;

const unsigned short DEFAULT_PORT = 80;

int count = 0;

int round = 0;

using namespace std;

CRITICAL_SECTION* zoneCritique = new CRITICAL_SECTION();

std::ifstream* fRu = new std::ifstream();
std::ofstream* fResultats = new std::ofstream();


HANDLE writeEvent;

DWORD WINAPI Thread1( LPVOID lpParameter )
{	
	while (count < round) // std::char_traits<wchar_t>::eof()
	{

				initialisationWinsock();

				SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				if (INVALID_SOCKET == ConnectSocket) // Test de Connection
				{
					printf("Error at socket(): %ld\n", WSAGetLastError());
				}
				int iResult = 0;
				char adresseIp[16];

				EnterCriticalSection(zoneCritique);
				lireFichierIp(adresseIp, 16, fRu);
				LeaveCriticalSection(zoneCritique);

				struct sockaddr_in recpHostInfo;
				recpHostInfo.sin_family = AF_INET;
				recpHostInfo.sin_addr.s_addr = inet_addr(adresseIp);
				recpHostInfo.sin_port = htons(DEFAULT_PORT);
				iResult = connect(ConnectSocket, (sockaddr*) &recpHostInfo, sizeof(recpHostInfo));
				
				if (iResult == SOCKET_ERROR) 
				{
					EnterCriticalSection(zoneCritique);
					printf("Erreur pas de connection %ld !\n", WSAGetLastError());
					WSACleanup();
					LeaveCriticalSection(zoneCritique);

				}
				
				if (iResult != SOCKET_ERROR)
				{
					EnterCriticalSection(zoneCritique);
					printf("Connection etablie !\n");
					fResultats->write(adresseIp, 16);
					LeaveCriticalSection(zoneCritique);
				}

				
				ConnectSocket = INVALID_SOCKET;
				closesocket(ConnectSocket);
				count += 1;

	}
	WSACleanup();
	SetEvent(writeEvent);

	return 0;
}



int main(int argc, PCSTR argv[])
{
	system("TITLE Chercheur Ip Serveur");
	system("COLOR 0A");
	char test01 = 0;
	cout << "================================================================================" << endl;
	cout << "---->                   CHERCHEUR IP SERVEURS V 2.0                        <----" << endl;
	cout << "================================================================================" << endl << endl;
	cout << "  Port test : " << DEFAULT_PORT << "                            Programmed by Marc Sidorenko " << endl;
	cout << "  Nb Ip on test : 45902704                " << endl; 
	cout << "  From : Russia" << endl;
	cout << endl;	
	if(0 == argc) 
	{
		printf("Erreur parametre en trop.\n");
		system("PAUSE");
		return 9;
	} 
		writeEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

		if (writeEvent == NULL) 
		{ 
			printf("Echec de l'event erreur : (%d)\n", GetLastError());
			return 1;
		}
			DWORD threadID[MAX_THREADS];
			cout << "================================================================================" << endl;
			cout << " ";
			cout << MAX_THREADS; 
			cout << " Thread(s) activated" << endl << endl; 
			cout << " Time Process : (almost) 13 days" << endl << endl;
			cout << "================================================================================" << endl;
 			cout << " Beggin Process (y/n)" << endl << endl;
			cin >> test01;
			cout << endl;

			if (test01 != 'y')
			{
				cout << "Process cancelled..." << endl;
				cout << endl << "================================================================================" << endl << endl;
				system("PAUSE");
				fRu->close();
				fResultats->close();
				CloseHandle(writeEvent);
				return 0;
			}

			cout << " How many IPs on test ? (10000 x) if max (30000) " << endl << endl;
			cin >> round;
			
			InitializeCriticalSection(zoneCritique); // pointeur objet

			fResultats->open("C:\\Users\\Marc\\Desktop\\RU\\fResultats.txt");
			fRu->open("C:\\Users\\Marc\\Desktop\\RU\\RU.dat");
			if ( fRu->fail() )
			{
				std::cout << "Erreur ouverture incorrecte !"  << endl;
			} 
			


			for (int relanceThread = 0; relanceThread < MAX_THREADS; relanceThread++)
			{
				CreateThread(NULL, 0, Thread1, NULL, 0, &threadID[relanceThread]);
			}
					
			DWORD attendreResultat;
			
			attendreResultat = WaitForSingleObject(writeEvent, INFINITE);   

			switch (attendreResultat) 
			{
				case WAIT_OBJECT_0: 
					break; 
				default: 
					printf("Erreur prob : (%d)\n", GetLastError()); 
					return 0; 
			}

		fRu->close();
		fResultats->close();
		CloseHandle(writeEvent);
	Sleep (1000);
	attendreResultat = WaitForSingleObject(writeEvent, INFINITE);
	cout << endl << "Process ended well !" << endl << endl;
	DeleteCriticalSection(zoneCritique);
	system("PAUSE");

	return 0;
}
