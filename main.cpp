//#include <winsock2.h>
//#include <ws2tcpip.h>
//#include <Mswsock.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include "fonctions.h"
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

unsigned int MAX_THREADS = 8;

unsigned short DEFAULT_PORT = 80;

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
      warnx("Error socket creation! %s ", strerror(errno));

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
      warnx("Error no connection ! %s", strerror(errno));
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
	system("TITLE IP_Port_Tester");
	system("COLOR 0A");
  char *user = (char*)malloc(sizeof(char) * 81);
  cout << " Enter PORT ?\n";
  cin >> user;
  int port_test = 80;
  try
  {
    port_test = atoi(user);
  }
  catch (const exception e)
  {
    cout << " Error input\n";
  }
  DEFAULT_PORT = port_test;
	cout << "  Port test : " << DEFAULT_PORT << "\n";

  // writeEvent = CreateEvent(NULL, TRUE, FALSE, NULL); FIXME convert to LINUX
  /*
  if (writeEvent == NULL)
  {
    printf("Echec de l'event erreur : (%d)\n", GetLastError());
    return 1;
  }
  */
  cout << " How many threads do you want ?\n";
  free(user);
  user = (char*)malloc(sizeof(char) * 81);
  cin >> user;
  int max_thr = 80;
  try
  {
    max_thr = atoi(user);
  }
  catch (const exception e)
  {
    cout << " Error input\n";
  }
  MAX_THREADS = max_thr;
  cout << " " << MAX_THREADS << " Thread(s) activated\n";
  cout << " Begin Process (y/n)\n";
  free(user);
  user = (char*)malloc(sizeof(char) * 81);
  cin >> user;
  if ('y' != user[0])
  {
    cout << "Process cancelled..." << endl;
    fRu->close();
    fResultats->close();
    // CloseHandle(writeEvent); FIXME convert to Linux !
    free(user);
    return 0;
  }
  free(user);
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
  fRu->open("fEntry");
  if ( fRu->fail() )
    cout << "Erreur ouverture incorrecte !\n";

/*
  pthread_t *threadsTable = (pthread_t*)malloc(MAX_THREADS * sizeof(pthread_t));
  for (int i = 0; i < MAX_THREADS; i++)
    pthread_create(&threadsTable[i], NULL, process_ip, NULL);
*/

  process_ip(NULL); // HERE ALL the MAGIC !!!

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

  sleep(100000);
  fRu->close();
  fResultats->close();
  // CloseHandle(writeEvent); FIXME convert to Linux
  // attendreResultat = WaitForSingleObject(writeEvent, INFINITE);
  cout << "Process ended well !\n";
  pthread_mutex_destroy(&criticalZ);
  return 0;
}
