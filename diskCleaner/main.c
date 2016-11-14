#include "autoclean.h"

int main(int argc, char* argv[])
{
	int path = 0;
	s_args args = {0, 0, 0};

	parseArgs(argv, argc, &path, &args);
	if (args.u)
		DeleteRegistredPath();
	if (args.p)
		SetRegistredPath(argv[path]);
	if (args.i)
		InstallMyService();
	if (args.d)
		DeleteMyService();
	if (!args.p && !args.i && !args.d && !args.u)
	{
		SERVICE_TABLE_ENTRY DispatchTable[] = {{MY_SERVICE_NAME, ServiceMain}, {NULL, NULL}};
		StartServiceCtrlDispatcher(DispatchTable);
	}
	printf("Key value is %s", GetRegistredPath());
	if (!path)
		CleanThisPath("C:\\");
	else
		CleanThisPath(argv[path]);

	return EXIT_SUCCESS;
}