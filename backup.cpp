#include "backup.h"
#include "inipath.h"
#include <filesystem>
#include <vector>
#include <Windows.h>

// Copy all INI files from the target directory to the program's file backup directory:
backup::backup()
{
	inipath iniPath;
	std::wstring targetPath = iniPath.getPath();
	if (!std::filesystem::exists(targetPath))
	{
		MessageBoxA(NULL, "The specified target path does not exist!", "PATH error!", MB_OK | MB_ICONERROR);
		return;
	}

	std::filesystem::create_directories(iniPath.getBackupDir().c_str());
	std::vector<std::wstring> parentDirs;
	std::vector<std::wstring> subDirPaths;

	auto canAddParentDir = [&subDirPaths](std::wstring thisPath)
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

	// Iterate all subdirectories and save parent folder if the folder contains .ini files
	for (auto& fs : std::filesystem::recursive_directory_iterator(targetPath))
	{
		if (fs.path().extension() == ".ini" || fs.path().extension() == ".INI")
		{
			parentDirs.push_back(fs.path().parent_path().wstring());	
		}
	}
		
	// Erase the target path from the parent path
	// (leaving something like: Engine\Config)
	for (int i = 0; i < parentDirs.size(); i++)
	{
		std::wstring filter = targetPath.c_str();
		filter += L"\\";
		parentDirs[i].erase(parentDirs[i].rfind(filter), filter.length());
		if (canAddParentDir(parentDirs[i]))
		{
			subDirPaths.push_back(parentDirs[i]);
		}	
	}

	// Clone the folder structure to the local backup directory
	for (int i = 0; i < subDirPaths.size(); i++)
	{
		std::wstring path(iniPath.getBackupDir());
		path += L"\\";
		path += subDirPaths[i];
		if (!std::filesystem::exists(path.c_str()))
		{
			std::filesystem::create_directories(path.c_str());
		}
	}

	// Copy all the INI files over to the local backup
	for (auto fs : std::filesystem::recursive_directory_iterator(targetPath))
	{
		if (fs.path().extension() == ".ini" || fs.path().extension() == ".INI")
		{
			std::wstring destination = fs.path().wstring();
			destination.erase(destination.rfind(targetPath.c_str()), targetPath.length());
			destination.insert(0, iniPath.getProgramPath() + L"\\Replace\\" + iniPath.getTargetPathID());
			std::wstring test = iniPath.getProgramPath() + L"\\Replace\\" + iniPath.getTargetPathID();
			std::filesystem::remove(destination);
			std::filesystem::copy(fs.path().wstring(), destination);
		}
	}
}