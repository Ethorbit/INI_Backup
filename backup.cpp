#include "backup.h"
#include "inipath.h"
#include <filesystem>

backup::backup()
{
	inipath iniPath;
	std::string gamePath = iniPath.getPath();
	std::string backupDir = iniPath.getProgramPath() + "Replace";
	std::filesystem::create_directory(backupDir.c_str());

	// Copy all INI files from the target directory to the program's file backup directory
	for (auto fs : std::filesystem::recursive_directory_iterator(gamePath))
	{
		if (fs.path().extension() == ".ini" || fs.path().extension() == ".INI")
		{
			
		}
	}
}