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
	char *strpipecmd; // ������� ������ ��� ������ �������� ������� ������
	char *strpipedata; // ������� ������ ��� ������ �������� ������

	//������ ������ ����������� � �������
	HANDLE hPSEcmdpipe;
	HANDLE hPSEdatapipe;



public:

	void ConnectToSolver();
	void StartLocalSolverAndConnect( char *strlocworkpath, char *strexename );
	void ClosePipes();
	void DisconnectAndCloseSolver();
	void SendParallelSettings(int ncores, int npt_matr, int npt_vect, int nTempDir, char**TempDirList, float RAMpermitted, float RAMmatblock );

	//�������� ��� ������������� ������
	void CreatFEmodel( char *working_folder ); //�������� ������� ���������� � �������� ������� ������ �� ������� ��������
	void SendFEMnodesCRD( int NN, int NORT, double *CRD ); // �������� ��������� �����
	void SendFEMelements( int NEL, int neltypes, int **el ); // �������� ���������� �� ���������
	void SendFEMfixing(int NN, int NORT, int *FIXFLAG, double *FIXDISP); // �������� ���������� � ������������
	void SendFEMmaterials(int nmat, double **elast_prop); // �������� ���������� � ����������

	//�������� ��������������� ������
	void CreateAutoSE(int SE0elnum, int SEtotalnodes, int SEboundarynodes); //�������������� �������� ���������������� ���������

	// ������ �� � ��
	void EIGFR_SSI(int nfr, double eps, int nblock); 
	int GetFreqRes(int nfr, double *eigfr);
	int GetFormRes(int nfr, int NN, int NORT, double **eigform);
	int GetFormEnergy(int NEL, double **eigfen);

	// ������ ������� �������� �� ��������� �������� ������
	void ClearExtSolver(); //������� �� �������� ������ � �������� ���������� �������
};