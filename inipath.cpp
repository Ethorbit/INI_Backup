#include "inipath.h"
#include <Windows.h>
#include <fstream>

inipath::inipath()
{
	wchar_t progPath[800] = L"";
	DWORD getProgPath = GetModuleFileName
	(
		NULL,
		progPath,
		sizeof(progPath) / sizeof(wchar_t)
	);

	programPath = progPath;
	programPath.erase(programPath.rfind(L'\\') + 1, programPath.length()); // Trim executable's name from the program path

	wchar_t* path;
	size_t pathLen;
	
	_wdupenv_s(&path, &pathLen, L"appdata");
	if (path != NULL)
	{
		progTxtPath = path;
		progTxtPath += L"\\Backup";
		fullPath = progTxtPath;
		fullPath += L"\\BackupPath.txt";
	}

	targetPathID = getPath();
	targetPathID.erase(0, targetPathID.rfind(L"\\"));
	backupDir = programPath + L"Replace\\" + targetPathID;
}

void inipath::savePath(const wchar_t* newPath)
{
	DWORD openType = 0;
	HKEY openKey = NULL;
	
	// Replace all forward slashes with backslashes:
	std::wstring fixStr(newPath);
	std::wstring newStr;
	for (int i = 0; i < fixStr.length(); i++)
	{
		if (fixStr[i] == L'/')
		{
			newStr += L"\\";
		}
		else
		{
			newStr += fixStr[i];
		}
	}

	const wchar_t* fixedPath = newStr.c_str();

	RegCreateKeyExW
	(
		HKEY_CURRENT_USER,
		L"SOFTWARE\\INIBackup",
		NULL,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		NULL,
		&openKey,
		&openType
	);

	if (openKey != NULL)
	{
		RegSetValueW(openKey, 0, REG_SZ, fixedPath, NULL);
	}
	else
	{
		MessageBoxA(NULL, "Error saving to registry!", "Error!", MB_OK | MB_ICONERROR);
	}

	RegCloseKey(openKey);
}

const wchar_t* inipath::getPath()
{
	wchar_t output[2000] = L"";
	DWORD outputSize = sizeof(output) / sizeof(wchar_t);
	RegGetValueW
	(
		HKEY_CURRENT_USER,
		L"SOFTWARE\\INIBackup",
		0,
		RRF_RT_REG_SZ,
		NULL,
		&output,
		&outputSize
	);

	return output;
}