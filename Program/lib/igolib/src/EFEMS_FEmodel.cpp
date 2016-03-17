#include "StdAfx.h"
#include "ExternFEMSolver.h"

void EFEMS::CreatFEmodel(char *working_folder) 
{
	int i,j,message,nchar;
	DWORD cb;
	char str[256];

	message = 201;
	WriteFile(hPSEcmdpipe,&message,sizeof(int),&cb,NULL);

	nchar = 256;
	sprintf_s(str,nchar*sizeof(char),"%s",working_folder);
	WriteFile(hPSEdatapipe,&nchar,sizeof(int),&cb,NULL); //длина строки
	WriteFile(hPSEdatapipe,str,nchar*sizeof(char),&cb,NULL); //путь к файлу модели
	
	//ожидание конца выполнения команды решателем
	ReadFile(hPSEcmdpipe,&message,sizeof(int),&cb,NULL);
}

void EFEMS::SendFEMnodesCRD( int NN, int NORT, double *CRD )
{
	int i,j,message;
	DWORD cb;

	message = 202;
	WriteFile(hPSEcmdpipe,&message,sizeof(int),&cb,NULL);

	WriteFile(hPSEdatapipe,&NN,sizeof(int),&cb,NULL);
	WriteFile(hPSEdatapipe,&NORT,sizeof(int),&cb,NULL);
	WriteFile(hPSEdatapipe,CRD,NN*NORT*sizeof(double),&cb,NULL);

	//ожидание конца выполнения команды решателем
	ReadFile(hPSEcmdpipe,&message,sizeof(int),&cb,NULL);
}

void EFEMS::SendFEMelements( int NEL, int neltypes, int **el )
{
	int i,j,message,tmp[30];
	DWORD cb;

	message = 203;
	WriteFile(hPSEcmdpipe,&message,sizeof(int),&cb,NULL);

	WriteFile(hPSEdatapipe,&NEL,sizeof(int),&cb,NULL);
	WriteFile(hPSEdatapipe,&neltypes,sizeof(int),&cb,NULL);
	for (i=0; i<NEL; i++)
	{
		for (j=0; j<(el[i][2]+3); j++) tmp[j] = el[i][j];
		WriteFile(hPSEdatapipe,&tmp[0],30*sizeof(int),&cb,NULL);	
	}

	//ожидание конца выполнения команды решателем
	ReadFile(hPSEcmdpipe,&message,sizeof(int),&cb,NULL);
}

void EFEMS::SendFEMfixing(int NN, int NORT, int *FIXFLAG, double *FIXDISP)
{
	int i,j,message;
	DWORD cb;

	message = 204;
	WriteFile(hPSEcmdpipe,&message,sizeof(int),&cb,NULL);

	WriteFile(hPSEdatapipe,FIXFLAG,NN*NORT*sizeof(int),&cb,NULL);
	WriteFile(hPSEdatapipe,FIXDISP,NN*NORT*sizeof(double),&cb,NULL);

	//ожидание конца выполнения команды решателем
	ReadFile(hPSEcmdpipe,&message,sizeof(int),&cb,NULL);

}

void EFEMS::SendFEMmaterials(int nmat, double **elast_prop)
{
	int i,j,message;
	DWORD cb;
	message = 205;
	WriteFile(hPSEcmdpipe,&message,sizeof(int),&cb,NULL);

	WriteFile(hPSEdatapipe,&nmat,sizeof(int),&cb,NULL);
	for (i=0; i<nmat; i++)
	{
		WriteFile(hPSEdatapipe,elast_prop[i],3*sizeof(double),&cb,NULL);
	}

	//ожидание конца выполнения команды решателем
	ReadFile(hPSEcmdpipe,&message,sizeof(int),&cb,NULL);

}
