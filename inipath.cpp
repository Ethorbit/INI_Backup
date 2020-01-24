#include "inipath.h"
#include <Windows.h>
#include <fstream>

inipath::inipath()
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

	targetPathID = getPath();
	if (targetPathID.rfind("\\") != targetPathID.npos && 
	targetPathID.rfind("\\") > targetPathID.rfind("/"))
	{
		targetPathID.erase(0, targetPathID.rfind("\\"));
	}
	else
	{
		targetPathID.erase(0, targetPathID.rfind("/"));
	}

	backupDir = programPath + "Replace\\" + targetPathID;
}

void inipath::savePath(const char* newPath)
{
	CreateDirectoryA(progTxtPath.c_str(), NULL);
	std::ofstream output(fullPath);
	output << newPath << std::endl;
	output.close();
}

const char* inipath::getPath()
{
	std::string value;
	char returnVal[1000];
	std::ifstream input(fullPath);
	input.getline(returnVal, 1000);
	return returnVal;
}