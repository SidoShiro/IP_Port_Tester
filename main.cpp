//#include <winsock2.h>
//#include <ws2tcpip.h>
//#include <Mswsock.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include "fonctions.h"
#include <memory>
//#include <windows.h>
//#include <tchar.h>
//#include <strsafe.h>
#include <unistd.h>

// Linux Threads
#include <pthread.h>

// Linux Sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

// Errno
#include <errno.h>
#include <err.h>

// Signals
#include <signal.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Kernel32.lib")

const unsigned int MAX_THREADS = 1500;

const unsigned short DEFAULT_PORT = 80;

int count = 0;

int round = 0;

using namespace std;

// CRITICAL_SECTION* zoneCritique = new CRITICAL_SECTION();
pthread_mutex_t criticalZ;

std::ifstream* fRu = new std::ifstream();
std::ofstream* fResultats = new std::ofstream();

// HANDLE writeEvent;
pthread_t *event;

//DWORD WINAPI Thread1( LPVOID lpParameter )
void *process_ip(void *data)
{
  while (count < round) // std::char_traits<wchar_t>::eof()
  {
    int coSockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (-1 == coSockfd) // Socket creation test
      warnx("Error socket creation!");

    int iResult = 0;
    char adresseIp[16];

    pthread_mutex_lock(&criticalZ);
    lireFichierIp(adresseIp, 16, fRu);
    pthread_mutex_unlock(&criticalZ);

    struct sockaddr_in recpHostInfo;
    recpHostInfo.sin_family = AF_INET;
    recpHostInfo.sin_addr.s_addr = inet_addr(adresseIp);
    recpHostInfo.sin_port = htons(DEFAULT_PORT);
    iResult = connect(coSockfd, (sockaddr*) &recpHostInfo,
      sizeof(recpHostInfo));
    if (-1 == iResult)
    {
      pthread_mutex_lock(&criticalZ);
      warnx("Error no connection !");
      // WSACleanup(); -> auto in Linux
      pthread_mutex_unlock(&criticalZ);
    }
    if (iResult != -1)
    {
      pthread_mutex_lock(&criticalZ);
      printf("Connection etablie !\n");
      fResultats->write(adresseIp, 16);
      pthread_mutex_unlock(&criticalZ);
    }
    close(coSockfd); // close fd socket
    count += 1;
  }
  // WSACleanup(); -> auto on Linux
  // SetEvent(writeEvent); FIXME Idk what is this !!! FIXME
  return NULL;
}



int main(int argc, char *argv[])
{
	//system("TITLE Chercheur Ip Serveur");
	//system("COLOR 0A");
	char test01 = 0;
	cout << "  Port test : " << DEFAULT_PORT << endl;
	cout << endl;
	if(0 == argc)
	{
		printf("Erreur parametre en trop.\n");
		system("PAUSE");
		return 9;
	}
  // writeEvent = CreateEvent(NULL, TRUE, FALSE, NULL); FIXME convert to LINUX
  /*
  if (writeEvent == NULL)
  {
    printf("Echec de l'event erreur : (%d)\n", GetLastError());
    return 1;
  }
  */

  cout << " ";
  cout << MAX_THREADS;
  cout << " Thread(s) activated" << endl << endl;
  cout << " Beggin Process (y/n)" << endl << endl;
  cin >> test01;
  cout << endl;

  if (test01 != 'y')
  {
    cout << "Process cancelled..." << endl;
    system("PAUSE");
    fRu->close();
    fResultats->close();
    // CloseHandle(writeEvent); FIXME convert to Linux !
    return 0;
  }

  cout << " How many IPs on test ? (10000 x) if max (30000) " << endl << endl;
  cin >> round;

  // InitializeCriticalSection(zoneCritique); // pointeur objet
  // create mutex attr var
  pthread_mutexattr_t mA;
  // setup recursive mutex for mutex attr
  pthread_mutexattr_settype(&mA, PTHREAD_MUTEX_RECURSIVE_NP);
  // use the mutex attr to create the mutex
  pthread_mutex_init(&criticalZ, &mA);
  // mutex attr can be estroyed
  pthread_mutexattr_destroy(&mA);

  fResultats->open("fResultats");
  fRu->open("RU");
  if ( fRu->fail() )
  {
    std::cout << "Erreur ouverture incorrecte !"  << endl;
  }

  pthread_t *threadsTable = (pthread_t*)malloc(MAX_THREADS * sizeof(pthread_t));
  for (int i = 0; i < MAX_THREADS; i++)
    pthread_create(&threadsTable[i], NULL, process_ip, NULL);

  int attendreResultat;

  // attendreResultat = WaitForSingleObject(writeEvent, INFINITE);

  /* FIXME !!!
  switch (attendreResultat)
  {
    case WAIT_OBJECT_0:
      break;
    default:
      printf("Erreur prob : (%d)\n", GetLastError());
      return 0;
  }
  */

  fRu->close();
  fResultats->close();
  // CloseHandle(writeEvent); FIXME convert to Linux
  // attendreResultat = WaitForSingleObject(writeEvent, INFINITE);
  cout << endl << "Process ended well !" << endl << endl;
  char k;
  cin >> k;
  pthread_mutex_destroy(&criticalZ);
  system("PAUSE");
  return 0;
}
