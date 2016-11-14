#include "autoclean.h"

char *GetRegistredPath()
{
	HKEY hKey;
	BYTE content[MAX_PATH + 1];
	DWORD dwType;
	DWORD dwBufSize;
	char *output;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, MY_KEY_PATH,0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
	{
		dwBufSize = sizeof(content);
		output = (char*)malloc(sizeof(content));
		dwType = REG_SZ;
		if (RegQueryValueEx(hKey, MY_KEY_NAME, 0, &dwType, content, &dwBufSize) == ERROR_SUCCESS)
		{
			sprintf(output,"%s", content);
			return output;
		}
		else
			return "none";
		RegCloseKey(hKey);
	}
	else
		return "none";
}

BOOL SetRegistredPath(char *myPath)
{
	HKEY hKey;

	if (RegCreateKey(HKEY_LOCAL_MACHINE, MY_KEY_PATH, &hKey) != ERROR_SUCCESS)
	{
		fprintf(stderr, "RegCreateKey() failed : %d\n", GetLastError());
		return FALSE;
	}
	else
		printf("RegCreateKey() succeeded : %s\n", myPath);
	RegSetValueEx(hKey, MY_KEY_NAME, 0, REG_SZ, (BYTE*)myPath, 12);
	RegCloseKey(hKey);
	return TRUE;
}

BOOL DeleteRegistredPath()
{
	if (RegDeleteKey(HKEY_LOCAL_MACHINE, MY_KEY_PATH) != ERROR_SUCCESS)
	{
		fprintf(stderr, "RegDeleteKey() failed : %d\n", GetLastError());
		return FALSE;
	}
	else
		printf("RegDeleteKey() succeeded\n");
	return TRUE;
}