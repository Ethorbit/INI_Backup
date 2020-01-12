#include "ARKBackup.h"
#include "Resource.h"
#include <iostream>
#include <fstream>
#include <bitset>

int wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	// Main window:
	WNDCLASS wndClass{ 0 };
	wndClass.hbrBackground = CreateSolidBrush(RGB(37, 49, 0));
	wndClass.hCursor = LoadCursorW(hInstance, IDC_ARROW);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WindowProc;
	wndClass.lpszClassName = L"MainWin";
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	// Save confirmation window:
	WNDCLASS saveClass{ 0 };
	saveClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	saveClass.hCursor = LoadCursorW(hInstance, IDC_ARROW);
	saveClass.hInstance = hInstance;
	saveClass.lpfnWndProc = SaveWindowProc;
	saveClass.lpszClassName = L"SaveWin";
	
	RegisterClassW(&wndClass);
	RegisterClassW(&saveClass);

	POINT point;
	GetCursorPos(&point);

	// Load all BMPs so they can be displayed in future windows:
	bitmaps.push_back(LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1)));
	bitmaps.push_back(LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP2)));
	bitmaps.push_back(LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP3)));
	bitmaps.push_back(LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP4)));
	bitmaps.push_back(LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP5)));

	HWND mainWindow = CreateWindowW
	(
		L"MainWin",
		L"ARK INI Saver",
		WS_VISIBLE |
		WS_BORDER |
		WS_OVERLAPPEDWINDOW,
		point.x - 150, point.y - 150,
		350, 270,
		NULL, NULL, NULL, NULL
	);

	DWORD winStyle = GetWindowLong(mainWindow, GWL_STYLE);
	winStyle &= ~WS_MAXIMIZEBOX;
	SetWindowLong(mainWindow, GWL_STYLE, winStyle);

	MSG msg;
	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		HWND pathTitle = CreateWindowW
		(
			L"Static",
			L"",
			WS_CHILD |
			WS_VISIBLE |
			SS_BITMAP,
			80, 10,
			163, 31,
			hWnd,
			NULL, NULL, NULL
		);

		SendMessageW(pathTitle, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bitmaps[0]);

		ARKDir = CreateWindowW
		(
			L"Edit",
			L"C:\\Program Files (x86)\\Steam\\steamapps\\common\\ARK",
			WS_VISIBLE |
			WS_CHILD |
			WS_BORDER |
			SS_CENTER |
			ES_AUTOHSCROLL,
			10, 50,
			315, 20,
			hWnd,
			NULL, NULL, NULL
		);

		SendMessageW(ARKDir, WM_SETFONT, (WPARAM)smallFont, NULL);

		HWND saveBtn = CreateWindowW
		(
			L"Button",
			L"",
			WS_CHILD | 
			WS_VISIBLE |
			BS_CENTER | 
			BS_BITMAP |
			BS_FLAT,
			290, 15,
			25, 25,
			hWnd,
			(HMENU)CONFIRMSAVEPATH, NULL, NULL
		);

		SendMessageW(saveBtn, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bitmaps[3]);

		HWND backupBtn = CreateWindowW
		(
			L"Button",
			L"",
			WS_VISIBLE | 
			WS_CHILD |
			BS_BITMAP,
			80, 110,
			163, 31,
			hWnd,
			(HMENU)BACKUPFILES, NULL, NULL
		);

		SendMessageW(backupBtn, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bitmaps[1]);

		HWND restoreBtn = CreateWindowW
		(
			L"Button",
			L"",
			WS_VISIBLE |
			WS_CHILD |
			BS_BITMAP,
			80, 170,
			163, 31,
			hWnd,
			(HMENU)RESTOREFILES, NULL, NULL
		);

		SendMessageW(restoreBtn, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bitmaps[2]);

		HWND copyright = CreateWindowW
		(
			L"Static",
			L"",
			WS_VISIBLE | 
			WS_CHILD |
			SS_BITMAP,
			240, 212,
			100, 15, 
			hWnd,
			NULL, NULL, NULL
		);

		SendMessageW(copyright, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bitmaps[4]);
	}

	switch (wParam)
	{
		case CONFIRMSAVEPATH:
		{
			GetWindowTextA(ARKDir, savedDir, 1000);

			if (SaveConf == NULL)
			{
				SaveConf = CreateWindowExW
				(
					WS_EX_TOPMOST,
					L"SaveWin",
					L"Save Confirmation",
					WS_CLIPSIBLINGS |
					WS_VISIBLE |
					WS_CHILD |
					WS_SIZEBOX |
					CW_USEDEFAULT,
					0, 0,
					200, 200,
					hWnd,
					NULL, NULL, NULL
				);
			}

			RECT rect;
			GetWindowRect(hWnd, &rect);

			MoveWindow
			(
				SaveConf,
				rect.left + 70,
				rect.top + 70,
				200,
				200,
				FALSE
			);

			break;
		}
		case BACKUPFILES:
		{


			break;
		}
		case RESTOREFILES:
		{
		
		}
	}

	if (uMsg == WM_MOVE)
	{
		RECT rect;
		GetWindowRect(hWnd, &rect);

		MoveWindow
		(
			SaveConf,
			rect.left + 70,
			rect.top + 70,
			200,
			200,
			FALSE
		);
	}

	if (uMsg == WM_DESTROY)
	{
		PostQuitMessage(0);
	}

	if (uMsg == WM_GETMINMAXINFO)
	{
		MINMAXINFO* minmax = (MINMAXINFO*)lParam;
		POINT curSize { 350, 270 };
		minmax->ptMaxTrackSize = curSize;
		minmax->ptMinTrackSize = curSize;
	}

	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK SaveWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		CreateWindowW
		(
			L"Static",
			L"Override ARK Path?",
			WS_VISIBLE |
			WS_CHILD |
			SS_CENTER,
			-5, 10,
			200, 50,
			hWnd,
			NULL, NULL, NULL
		);

		CreateWindowW
		(
			L"Button",
			L"Yes",
			WS_VISIBLE |
			WS_CHILD |
			WS_BORDER,
			10, 40,
			80, 50,
			hWnd,
			(HMENU)SAVEFILEPATH, NULL, NULL
		);

		CreateWindowW
		(
			L"Button",
			L"No",
			WS_VISIBLE |
			WS_CHILD |
			WS_BORDER,
			95, 40,
			80, 50,
			hWnd,
			(HMENU)CANCELSAVE, NULL, NULL
		);
	}
	
	switch (wParam)
	{
		case SAVEFILEPATH: 
		{
			char* path;
			size_t pathLen;
			_dupenv_s(&path, &pathLen, "appdata");
			if (path != NULL)
			{
				std::string path2 = path;
				path2 += "\\ARKBackup";
				std::string fullpath = path2;
				fullpath += "\\ARKPath.txt";
				CreateDirectoryA(path2.c_str(), NULL);
				std::ofstream output(fullpath);
				output << savedDir << std::endl;
				output.close();
			}
		}

		case CANCELSAVE:
		{
			DestroyWindow(SaveConf);
			SaveConf = NULL;
		}
	}

	if (uMsg == WM_GETMINMAXINFO)
	{
		MINMAXINFO* minMax = (MINMAXINFO*)lParam;
		POINT curSize{ 200, 120 };
		minMax->ptMaxTrackSize = curSize;
		minMax->ptMinTrackSize = curSize;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}