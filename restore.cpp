#include "restore.h"
#include <filesystem>
#include <Windows.h>
#include "inipath.h"

// Replace all target path ini files with the existing ones in the backup directory:
restore::restore()
{
	inipath iniPath;
	std::wstring targetPath = iniPath.getPath();
	if (!std::filesystem::exists(iniPath.getBackupDir()))
	{
		MessageBoxA(NULL, "No backups found for the specified path.", "Backup failed!", MB_OK | MB_ICONERROR);
		return;
	}

	for (auto fs : std::filesystem::recursive_directory_iterator(iniPath.getBackupDir()))
	{
		if (fs.path().extension() == ".ini" || fs.path().extension() == ".INI")
		{
			// Modify the file paths as if they reside in the target path for easy file replacement
			std::wstring destination = fs.path().wstring();
			destination.erase(0, iniPath.getBackupDir().size());
			destination.insert(0, iniPath.getPath());
			if (std::filesystem::exists(destination))
			{
				std::filesystem::remove(destination);
				std::filesystem::copy(fs.path().wstring(), destination);
			}
			else
			{
				std::wstring parentFolder = destination;
				parentFolder.erase(parentFolder.rfind(L"\\"), parentFolder.length());
				std::filesystem::create_directory(parentFolder);
				std::filesystem::copy(fs.path().wstring(), destination);
			}
		}
	}
}