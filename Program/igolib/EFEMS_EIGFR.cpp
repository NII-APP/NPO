#include "StdAfx.h"
#include "ExternFEMSolver.h"

void EFEMS::EIGFR_SSI(int nfr, double eps, int nblock) // запуск расчета СЧ и СФ
{
	int i,j,message;
	DWORD cb;

	message = 41;
	WriteFile(hPSEcmdpipe,&message,sizeof(int),&cb,NULL);
	
	//передача количества частот, которые требуется найти
	WriteFile(hPSEdatapipe,&nfr,sizeof(int),&cb,NULL); 
	//передача точности
	WriteFile(hPSEdatapipe,&eps,sizeof(double),&cb,NULL); 
	//передача количества частот в блоке
	WriteFile(hPSEdatapipe,&nblock,sizeof(int),&cb,NULL); 

	//ожидание конца выполнения команды решателем
	
	ReadFile(hPSEcmdpipe,&message,sizeof(int),&cb,NULL);

}

int EFEMS::GetFreqRes(int nfr, double *eigfr)
{
	int i,j,message,nfconv;
	DWORD cb;

	message = 42;
	WriteFile(hPSEcmdpipe,&message,sizeof(int),&cb,NULL);
	
	//передача количества найденных частот
	ReadFile(hPSEdatapipe,&nfconv,sizeof(int),&cb,NULL);

	if (nfconv > 0)
	{
		ReadFile(hPSEdatapipe,eigfr,nfconv*sizeof(double),&cb,NULL);
	}
	
	//ожидание конца выполнения команды решателем
	ReadFile(hPSEcmdpipe,&message,sizeof(int),&cb,NULL);

	return(nfconv);
}

int EFEMS::GetFormRes(int nfr, int NN, int NORT, double **eigform)
{
	int i,j,message,nfconv;
	DWORD cb;

	message = 43;
	WriteFile(hPSEcmdpipe,&message,sizeof(int),&cb,NULL);
	
	//передача количества найденных частот
	ReadFile(hPSEdatapipe,&nfconv,sizeof(int),&cb,NULL);

	for (i=0; i<nfconv; i++)
	{
		ReadFile(hPSEdatapipe,eigform[i],NN*NORT*sizeof(double),&cb,NULL);
	}
	
	//ожидание конца выполнения команды решателем
	ReadFile(hPSEcmdpipe,&message,sizeof(int),&cb,NULL);

	return(nfconv);
}