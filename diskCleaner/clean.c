#include "autoclean.h"

void CleanThisPath(char *myPath)
{
	char date[100];
	time_t now = time(NULL);
	struct tm *t = localtime(&now);
	strftime(date, sizeof(date)-1, "%d/%m/%Y %H:%M", t);

	logThis("/******************************BEGIN****************************/");
	logThis(date);
	logThis("/**************************************************************/\n");
	if (!strcmp(myPath, "C:\\"))
		logThis("Path not setted.\nStarting scan for C:\\ by default ...");
	else
		logThis(strcat_with_alloc(strcat_with_alloc("Path : ", myPath),"\nStarting scan ..."));
	MyCleanRules(myPath);
	logThis("\n/****************************END*******************************/\n\n");
}

void MyCleanRules(char *myPathToClean)
{
	logThis("\nDeleting .tmp files ...");
	scanThisDir(myPathToClean, "\\*.tmp");
	logThis("\nDeleting temporary windows files ...");
	scanThisDir(myPathToClean, "\\~.*");
	logThis("\nDeleting temporary linux files ...");
	scanThisDir(myPathToClean, "\\*~");
}

void scanThisDir(LPCTSTR lpFolder, LPCTSTR lpFilePattern)
{
	char *szFullPattern, *myFile = NULL;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFindFile;

	szFullPattern = (char*)malloc(sizeof(MAX_PATH) * 10);
	szFullPattern = strcat_with_alloc((char*)lpFolder, "\\*");
	hFindFile = FindFirstFile(szFullPattern, &FindFileData);
	if (hFindFile != INVALID_HANDLE_VALUE) {
		do {
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if (FindFileData.cFileName[0] != '.')
					scanThisDir(strcat_with_alloc(strcat_with_alloc((char*)lpFolder, "\\"),FindFileData.cFileName) , lpFilePattern);
			}
		} while (FindNextFile(hFindFile, &FindFileData));
		FindClose(hFindFile);
	}
	hFindFile = FindFirstFile(strcat_with_alloc((char*)lpFolder, (char*)lpFilePattern) , &FindFileData);
	if (hFindFile != INVALID_HANDLE_VALUE) {
		logThis(strcat_with_alloc(strcat_with_alloc("Directory found : ", (char*)lpFolder), strcat_with_alloc("\nCleaning ", strcat_with_alloc((char*)lpFolder, " ..."))));
		do {
			if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				DeleteThisFile(strcat_with_alloc(strcat_with_alloc((char*)lpFolder, "\\"),FindFileData.cFileName));
		} while (FindNextFile(hFindFile, &FindFileData));
		FindClose(hFindFile);
	}
}

BOOL DeleteThisFile(char *myPath)
{
	if (!DeleteFile(myPath))
	{
		logThis(strcat_with_alloc("Failed to delete file : ", myPath));
		return FALSE;
	}
	logThis(strcat_with_alloc("File deleted : ", myPath));
	return TRUE;
}