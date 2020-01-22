#include "arkpath.h"
#include <Windows.h>
#include <fstream>

arkpath::arkpath()
{
	char progPath[400] = "";
	DWORD getProgPath = GetModuleFileNameA
	(
		NULL,
		progPath,
		sizeof(progPath) / sizeof(char)
	);

	programPath = progPath;
	programPath.erase(programPath.rfind('\\') + 1, programPath.length()); // Trim executable's name from the program path

	char* path;
	size_t pathLen;
	_dupenv_s(&path, &pathLen, "appdata");
	if (path != NULL)
	{
		progTxtPath = path;
		progTxtPath += "\\ARKBackup";
		fullPath = progTxtPath;
		fullPath += "\\ARKPath.txt";
	}
}

void arkpath::savePath(const char* newPath)
{
	CreateDirectoryA(progTxtPath.c_str(), NULL);
	std::ofstream output(fullPath);
	output << newPath << std::endl;
	output.close();
}

const char* arkpath::getPath()
{
	std::string value;
	char returnVal[1000];
	std::ifstream input(fullPath);
	input.getline(returnVal, 1000);
	return returnVal;
}