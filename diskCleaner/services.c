#include "autoclean.h"

BOOL InstallMyService()
{
	char strDir[MAX_PATH + 1];
	SC_HANDLE schSCManager;
	SC_HANDLE schService;
	LPCTSTR lpszBinaryPathName;

	GetCurrentDirectory(1024, strDir);
	strcat(strDir, "\\"MY_SERVICE_BIN_NAME);
	if ((schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) == NULL)
		return TRUE;
	lpszBinaryPathName = strDir;
	schService = CreateService(schSCManager, MY_SERVICE_NAME, MY_SERVICE_DESCRIPTOR,
							SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, SERVICE_AUTO_START, SERVICE_ERROR_NORMAL,
							lpszBinaryPathName, NULL, NULL, NULL, NULL, NULL);
	if (schService == NULL)
		return FALSE;
	CloseServiceHandle(schService);
	return TRUE;
}

BOOL DeleteMyService()
{
	SC_HANDLE schSCManager;
	SC_HANDLE hService;

	if ((schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) == NULL)
		return FALSE;
	if ((hService = OpenService(schSCManager, MY_SERVICE_NAME, SERVICE_ALL_ACCESS)) == NULL)
		return FALSE;
	if (!DeleteService(hService))
		return FALSE;
	if (!CloseServiceHandle(hService))
		return FALSE;
	return TRUE;
}
	
void WINAPI ServiceCtrlHandler(DWORD Opcode)
{
	switch (Opcode)
	{
		case SERVICE_CONTROL_PAUSE:
			g_ServiceStatus.dwCurrentState = SERVICE_PAUSED;
			break;
		case SERVICE_CONTROL_CONTINUE:
			g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
			break;
		case SERVICE_CONTROL_STOP:
			g_ServiceStatus.dwWin32ExitCode = 0;
			g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
			g_ServiceStatus.dwCheckPoint = 0;
			g_ServiceStatus.dwWaitHint = 0;
			SetServiceStatus (g_ServiceStatusHandle, &g_ServiceStatus);
			break;
		case SERVICE_CONTROL_INTERROGATE:
			break;
		default:
			break;
	}
}

void WINAPI ServiceMain(DWORD argc, LPTSTR *argv)
{
	g_ServiceStatus.dwServiceType = SERVICE_WIN32;
	g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	g_ServiceStatus.dwWin32ExitCode = 0;
	g_ServiceStatus.dwServiceSpecificExitCode = 0;
	g_ServiceStatus.dwCheckPoint = 0;
	g_ServiceStatus.dwWaitHint = 0;
	g_ServiceStatusHandle = RegisterServiceCtrlHandler(MY_SERVICE_NAME, ServiceCtrlHandler);
	if (g_ServiceStatusHandle == (SERVICE_STATUS_HANDLE)0)
		return;
	g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	g_ServiceStatus.dwCheckPoint = 0;
	g_ServiceStatus.dwWaitHint = 0;
	SetServiceStatus(g_ServiceStatusHandle, &g_ServiceStatus);
	CleanThisPath(GetRegistredPath());
}