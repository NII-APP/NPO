#include "StdAfx.h"
#include "ExternFEMSolver.h"

void EFEMS::CreateAutoSE(int SE0elnum, int SEtotalnodes, int SEboundarynodes) //автоматическое создание суперэлементного разбиения
{
	int i,j,message,nchar;
	DWORD cb;

	message = 220;
	WriteFile(hPSEcmdpipe,&message,sizeof(int),&cb,NULL);
	
	WriteFile(hPSEdatapipe,&SE0elnum,sizeof(int),&cb,NULL);
	WriteFile(hPSEdatapipe,&SEtotalnodes,sizeof(int),&cb,NULL);
	WriteFile(hPSEdatapipe,&SEboundarynodes,sizeof(int),&cb,NULL);
	
	//ожидание конца выполнения команды решателем
	ReadFile(hPSEcmdpipe,&message,sizeof(int),&cb,NULL);

}