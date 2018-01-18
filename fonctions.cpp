//#include "stdafx.h"
//#include <winsock2.h>
//#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <istream>
#include <set>
#include "fonctions.h"
#include <memory>
#include <stdlib.h>

// Fonction ecriture dans le fihier, paramètre = Ip
void lireFichierIp(char* adresseIp, int leng, std::ifstream* fRu)
{
	memset(adresseIp, 0, leng);
	fRu->getline(adresseIp, leng);
	std::cout << adresseIp << std::endl;
}

void litUneLigne(char* line, int leng, std::ifstream* fRu)
{
	fRu->getline(line, leng);
}
