#include "backup.h"
#include "inipath.h"
#include <filesystem>
#include <Windows.h>
#include <vector>

// Copy all INI files from the target directory to the program's file backup directory:
backup::backup()
{
	inipath iniPath;
	std::string targetPath = iniPath.getPath();
	std::string backupDir = iniPath.getProgramPath() + "Replace";
	std::filesystem::create_directory(backupDir.c_str());
	std::vector<std::string> parentDirs;
	std::vector<std::string> subDirPaths;

	auto canAddParentDir = [&subDirPaths](std::string thisPath)
	{
		bool result = true;

		for (int i = 0; i < subDirPaths.size(); i++)
		{
			if (subDirPaths[i] == thisPath)
			{
				result = false;
			}
		}

		return result;
	};

	// Iterate all subdirectories and save parent folder if folder contains .ini files
	for (auto& fs : std::filesystem::recursive_directory_iterator(targetPath))
	{
		if (fs.path().extension() == ".ini" || fs.path().extension() == ".INI")
		{
			parentDirs.push_back(fs.path().parent_path().string());	
		}
	}
		
	// Erase the target path from the parent path
	// (leaving something like: Engine\Config)
	for (int i = 0; i < parentDirs.size(); i++)
	{
		std::string filter = targetPath.c_str();
		filter += "\\";
		parentDirs[i].erase(parentDirs[i].rfind(filter), filter.length());
		if (canAddParentDir(parentDirs[i]))
		{
			subDirPaths.push_back(parentDirs[i]);
		}	
	}

	// Clone the folder structure to the local backup directory
	for (int i = 0; i < subDirPaths.size(); i++)
	{
		std::string path = iniPath.getProgramPath() += "\\Replace";
		path += "\\";
		path += subDirPaths[i];
		if (!std::filesystem::exists(path.c_str()))
		{
			std::filesystem::create_directories(path.c_str());
		}
	}
}