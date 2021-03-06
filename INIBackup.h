#pragma once
#include <Windows.h>
#include <vector>
#include "inipath.h"

#define initialWidth 350
#define initialHeight 270

#define OPENDIRECTORY 4996
#define CANCELSAVE 4997
#define CONFIRMSAVEPATH 4998
#define SAVEFILEPATH 4999
#define BACKUPFILES 5000
#define RESTOREFILES 5001
#define CONFIRMBACKUP 5002
#define CANCELBACKUP 5003
#define CONFIRMRESTORE 5004
#define CANCELRESTORE 5005

inipath iniPath;
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK SaveWindowProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK BackupWindowProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK RestoreWindowProc(HWND, UINT, WPARAM, LPARAM);
HWND INIDir, SaveConf, BackupConf, RestoreConf;
std::vector<HBITMAP>bitmaps;

HFONT smallFont = CreateFontW (
	15, 0,
	NULL,
	NULL,
	FW_EXTRALIGHT,
	NULL,
	NULL,
	NULL,
	OEM_CHARSET,
	OUT_DEFAULT_PRECIS,
	CLIP_DEFAULT_PRECIS,
	DEFAULT_QUALITY,
	DEFAULT_PITCH,
	L"Arial"
);

wchar_t savedDir[2000] = L"";