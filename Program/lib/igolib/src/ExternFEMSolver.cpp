#include "StdAfx.h"
#include "ExternFEMSolver.h"

EFEMS::EFEMS(void)
{
	strpipecmd = new char[256];
	strpipedata = new char[256];

	sprintf_s(strpipecmd,256*sizeof(char),"\\\\.\\pipe\\PSEcmdpipe\0");
	sprintf_s(strpipedata,256*sizeof(char),"\\\\.\\pipe\\PSEdatapipe\0");

	hPSEcmdpipe = NULL;
	hPSEdatapipe = NULL;
}

EFEMS::~EFEMS(void)
{
	delete [] strpipecmd;
	delete [] strpipedata;

	ClosePipes();

}

void EFEMS::ClosePipes()
{
	if ( hPSEcmdpipe != NULL && hPSEcmdpipe != INVALID_HANDLE_VALUE ) 
	{
		CloseHandle(hPSEcmdpipe);
		hPSEcmdpipe = NULL;
	}

	if ( hPSEdatapipe != NULL && hPSEdatapipe != INVALID_HANDLE_VALUE ) 
	{
		CloseHandle(hPSEdatapipe);
		hPSEdatapipe = NULL;
	}
}

void EFEMS::DisconnectAndCloseSolver()
{
	int i,j,message,nchar;
	DWORD cb;

	message = 1000;
	WriteFile(hPSEcmdpipe,&message,sizeof(int),&cb,NULL);
	
	//ожидание конца выполнения команды решателем	
	ReadFile(hPSEcmdpipe,&message,sizeof(int),&cb,NULL);

	ClosePipes();
}

void EFEMS::StartLocalSolverAndConnect(char *strlocworkpath, char *strexename)
{
	char strnewdest[256];
	// запуск приложения
	DWORD dw_ret,dw_priority;
	PROCESS_INFORMATION proc_info;
	STARTUPINFOA start_info;
	ZeroMemory(&start_info,sizeof(STARTUPINFO));
	start_info.dwFlags = NULL;
	sprintf_s(strnewdest,256*sizeof(char),"%s\\%s\0",strlocworkpath,strexename);
	dw_priority = BELOW_NORMAL_PRIORITY_CLASS;
	//CreateProcessA(strnewdest,NULL,NULL,NULL,TRUE,CREATE_NO_WINDOW,NULL,strlocworkpath,&start_info,&proc_info);
	CreateProcessA(strnewdest,NULL,NULL,NULL,TRUE,CREATE_NEW_CONSOLE,NULL,strlocworkpath,&start_info,&proc_info);
	
	SetPriorityClass(proc_info.hProcess,dw_priority);
	CloseHandle(proc_info.hThread);
	CloseHandle(proc_info.hProcess);

	ConnectToSolver();
}


void EFEMS::ConnectToSolver()
{
	int i,j,messagecmd = -1,messagedata = -1;
	int it = 0;
	DWORD cb;

	while ( (hPSEcmdpipe == NULL || hPSEcmdpipe == INVALID_HANDLE_VALUE) && it<10 ) 
	{
		hPSEcmdpipe = CreateFileA(LPCSTR(strpipecmd),
			GENERIC_READ|GENERIC_WRITE,
			0,NULL,OPEN_EXISTING,0,NULL);

		//если соединение состоялось решатель записывает в данный канал сообщение №100
		if ( hPSEcmdpipe != NULL && hPSEcmdpipe != INVALID_HANDLE_VALUE ) 
		{
			ReadFile(hPSEcmdpipe,&messagecmd,sizeof(int),&cb,NULL);
		}
		else
		{
			HANDLE NotSignaledEvent;
			NotSignaledEvent = CreateEvent(NULL,TRUE,TRUE,NULL);
			ResetEvent(NotSignaledEvent);
			WaitForSingleObject(NotSignaledEvent,500);
			CloseHandle(NotSignaledEvent);
		}
		it++;
	}
	
	hPSEdatapipe = CreateFileA(LPCSTR(strpipedata),
		GENERIC_READ|GENERIC_WRITE,
		0,NULL,OPEN_EXISTING,0,NULL);
	//если соединение состоялось решатель записывает в канал число 100
	if ( hPSEdatapipe != NULL && hPSEdatapipe != INVALID_HANDLE_VALUE ) 
	{
		ReadFile(hPSEdatapipe,&messagedata,sizeof(int),&cb,NULL);
	}


	if ( messagecmd == 100 && messagedata == 100 )
	{
		printf("\nExtern FEM Solver was connected succefully, messagecmd = %d , messagedata = %d\n",messagecmd,messagedata);
	}
	else
	{
		printf("\nExtern FEM Solver was not connected....  messagecmd = %d , messagedata = %d\n",messagecmd,messagedata);
	}

}

void EFEMS::SendParallelSettings( int ncores, int npt_matr, int npt_vect,  
								 int nTempDir, char**TempDirList, 
								 float RAMpermitted, float RAMmatblock )
{
	int i,j,message,nchar;
	DWORD cb;
	char str[256];
	nchar = 256;

	message = 81;
	WriteFile(hPSEcmdpipe,&message,sizeof(int),&cb,NULL);
	
	WriteFile(hPSEdatapipe,&ncores,sizeof(int),&cb,NULL);
	WriteFile(hPSEdatapipe,&npt_matr,sizeof(int),&cb,NULL);
	WriteFile(hPSEdatapipe,&npt_vect,sizeof(int),&cb,NULL);
	WriteFile(hPSEdatapipe,&nTempDir,sizeof(int),&cb,NULL);
	for (i=0; i<nTempDir; i++)
	{
		sprintf_s(str,nchar*sizeof(char),"%s",TempDirList[i]);
		WriteFile(hPSEdatapipe,str,nchar*sizeof(char),&cb,NULL);
	}
	
	WriteFile(hPSEdatapipe,&RAMpermitted,sizeof(float),&cb,NULL);
	WriteFile(hPSEdatapipe,&RAMmatblock,sizeof(float),&cb,NULL);

	//ожидание конца выполнения команды решателем
	ReadFile(hPSEcmdpipe,&message,sizeof(int),&cb,NULL);

}

void EFEMS::ClearExtSolver() //очистка от решенной задачи
{
	int i,j,message,nchar;
	DWORD cb;

	message = 80;
	WriteFile(hPSEcmdpipe,&message,sizeof(int),&cb,NULL);
	//ожидание конца выполнения команды решателем
	
	ReadFile(hPSEcmdpipe,&message,sizeof(int),&cb,NULL);

}