#include "backup.h"
#include "arkpath.h"
#include <filesystem>

backup::backup()
{
	arkpath arkPath;
	std::string gamePath = arkPath.getPath();
	std::string backupDir = arkPath.getProgramPath() + "Replace";
	std::filesystem::create_directory(backupDir.c_str());

	// Copy all INI files from the game directory to this path
	for (auto fs : std::filesystem::recursive_directory_iterator(gamePath))
	{

	}
}