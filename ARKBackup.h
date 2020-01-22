#pragma once
#include <Windows.h>
#include <vector>
#include "arkpath.h"

#define CANCELSAVE 1400
#define CONFIRMSAVEPATH 4998
#define SAVEFILEPATH 4999
#define BACKUPFILES 5000
#define RESTOREFILES 5001

arkpath arkPath;
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK SaveWindowProc(HWND, UINT, WPARAM, LPARAM);
HWND ARKDir, SaveConf;
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

char savedDir[1000];