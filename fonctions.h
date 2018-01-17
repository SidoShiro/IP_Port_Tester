#ifndef FONCTIONS_H_
#define FONCTIONS_H_

void lireFichierIp(char* adresseIp, int leng, std::ifstream* fRu);
int initialisationWinsock();
void litUneLigne(char* line, int leng, std::ifstream* fRu);

#endif
