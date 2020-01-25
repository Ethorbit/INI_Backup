#pragma once
#include <iostream>

class inipath
{
	public:
		inipath();
		void savePath(const wchar_t* newPath);
		const wchar_t* getPath();
		bool validatePath();
		std::wstring getProgramPath()
		{
			return programPath;
		}

		std::wstring getBackupDir()
		{
			return backupDir;
		}

		std::wstring getTargetPathID()
		{
			return targetPathID;
		}
	private:
		std::wstring progTxtPath;
		std::wstring fullPath;
		std::wstring programPath;
		std::wstring targetPathID;
		std::wstring backupDir;
};

