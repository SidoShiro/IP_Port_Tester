//#include "stdafx.h"
//#include <winsock2.h>
//#include <ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <istream>
#include <set>
#include "fonctions.h"
#include <memory>

using namespace std;

//Fonction ecriture dans le fihier, paramètre = Ip
void lireFichierIp(char* adresseIp, int leng, std::ifstream* fRu)
{
	memset(adresseIp, 0, leng); 
	fRu->getline(adresseIp, leng);
	
	std::cout << adresseIp << std::endl;
	
}


int initialisationWinsock()
{
	WSADATA wsaData; // Creation de l'objet Winsockj

	int iResult = 0;

	iResult = WSAStartup(MAKEWORD(2,2), &wsaData); // Initialisation de  Winsock
	if (iResult != 0) 
	{
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}
	return iResult;
}

void litUneLigne(char* line, int leng, std::ifstream* fRu)
{
	fRu->getline(line, leng);
}
