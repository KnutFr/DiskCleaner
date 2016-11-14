/*
** INCLUDES
*/
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/*
** MACROS
*/
#define MY_SERVICE_NAME  "AutoClean"
#define MY_SERVICE_DESCRIPTOR "AutoClean for Etna"
#define MY_SERVICE_BIN_NAME "AutoClean.exe"
#define MY_KEY_PATH "SOFTWARE\\ETNA\\DiskCleaner"
#define MY_KEY_NAME "path"

/*
** GLOBALS
*/
SERVICE_STATUS g_ServiceStatus;
SERVICE_STATUS_HANDLE g_ServiceStatusHandle;

/*
** STRUCTURES
*/
typedef struct {
	int p;
	int i;
	int d;
	int u;
} s_args;

/*
**PROTOTYPES
*/
//services.c
BOOL InstallMyService();
BOOL DeleteMyService();
void WINAPI ServiceCtrlHandler(DWORD);
void WINAPI ServiceMain(DWORD, LPTSTR*);
//clean.c
BOOL DeleteThisFile(char*);
void CleanThisPath(char*);
void scanThisDir(LPCTSTR, LPCTSTR);
void MyCleanRules(char*);
//registry.c
BOOL SetRegistredPath(char*);
char *GetRegistredPath();
BOOL DeleteRegistredPath();
//util.c
void myUsage();
void logThis(char*);
char *strcat_with_alloc(char*, char*);
void parseArgs(char**, int, int*, s_args*);