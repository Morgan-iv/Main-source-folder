#pragma once
#ifndef CLIENT_H_  
#define CLIENT_H_  

int IPCinit(void);
char * GetStr(int * pnum);
int FreeStr(int * pnum);
int IPCexit(void);

#endif 
