#pragma once

#include "stdio.h"
#include <stdlib.h>
#include "conio.h"
#include "Windows.h"
#include <Winbase.h>
#include <direct.h>


class EFEMS
{
public:
	EFEMS(void);
public:
	~EFEMS(void);
	
public:
	char *strpipecmd; // сетевое полное имя канала передачи номеров команд
	char *strpipedata; // сетевое полное имя канала передачи данных

	//каналы обмена сообщениями и данными
	HANDLE hPSEcmdpipe;
	HANDLE hPSEdatapipe;



public:

	void ConnectToSolver();
	void StartLocalSolverAndConnect( char *strlocworkpath, char *strexename );
	void ClosePipes();
	void DisconnectAndCloseSolver();
	void SendParallelSettings(int ncores, int npt_matr, int npt_vect, int nTempDir, char**TempDirList, float RAMpermitted, float RAMmatblock );

	//Действия для инициализации модели
	void CreatFEmodel( char *working_folder ); //передача рабочей директории и создание объекта модели на стороне решателя
	void SendFEMnodesCRD( int NN, int NORT, double *CRD ); // передача координат узлов
	void SendFEMelements( int NEL, int neltypes, int **el ); // передача информации об элементах
	void SendFEMfixing(int NN, int NORT, int *FIXFLAG, double *FIXDISP); // передача информации о закреплениях
	void SendFEMmaterials(int nmat, double **elast_prop); // передача информации о материалах

	//Создание суперэлементной модели
	void CreateAutoSE(int SE0elnum, int SEtotalnodes, int SEboundarynodes); //автоматическое создание суперэлементного разбиения

	// Расчет СЧ и СФ
	void EIGFR_SSI(int nfr, double eps, int nblock); 
	int GetFreqRes(int nfr, double *eigfr);
	int GetFormRes(int nfr, int NN, int NORT, double **eigform);
	int GetFormEnergy(int NEL, double **eigfen);

	// Полная очистка решателя от последней решенной задачи
	void ClearExtSolver(); //очистка от решенной задачи и ожидание следующего задания
};