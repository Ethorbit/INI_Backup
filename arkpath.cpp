#include "arkpath.h"
#include <Windows.h>
#include <fstream>

arkpath::arkpath()
{
	char* path;
	size_t pathLen;
	_dupenv_s(&path, &pathLen, "appdata");
	txtPath = path;
	txtPath += "\\ARKBackup";
}

void arkpath::savePath(const char* newPath)
{
	std::string fullpath = txtPath;
	fullpath += "\\ARKPath.txt";
	CreateDirectoryA(txtPath.c_str(), NULL);
	std::ofstream output(fullpath);
	output << newPath << std::endl;
	output.close();
}

void arkpath::getPath()
{

}