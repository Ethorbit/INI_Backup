#include "INIBackup.h"
#include "Resource.h"
#include <iostream>
#include <ShlObj_core.h>
#include "backup.h"
#include "restore.h"

int wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	// Main window:
	WNDCLASS wndClass{ 0 };
	wndClass.hbrBackground = CreateSolidBrush(RGB(37, 49, 0));
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WindowProc;
	wndClass.lpszClassName = L"MainWin";
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	// Save confirmation window:
	WNDCLASS saveClass{ 0 };
	saveClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	saveClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	saveClass.hInstance = hInstance;
	saveClass.lpfnWndProc = SaveWindowProc;
	saveClass.lpszClassName = L"SaveWin";

	// Backup confirmation window:
	WNDCLASS backupClass{ 0 };
	backupClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	backupClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	backupClass.hInstance = hInstance;
	backupClass.lpfnWndProc = BackupWindowProc;
	backupClass.lpszClassName = L"BackupWin";

	// Restore confirmation window:
	WNDCLASS restoreClass{ 0 };
	restoreClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	restoreClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	restoreClass.hInstance = hInstance;
	restoreClass.lpfnWndProc = RestoreWindowProc;
	restoreClass.lpszClassName = L"RestoreWin";
	
	RegisterClassW(&wndClass);
	RegisterClassW(&saveClass);
	RegisterClassW(&backupClass);
	RegisterClassW(&restoreClass);

	POINT point;
	GetCursorPos(&point);

	// Load all BMPs so they can be displayed in future windows:
	bitmaps.push_back(LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1)));
	bitmaps.push_back(LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP2)));
	bitmaps.push_back(LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP3)));
	bitmaps.push_back(LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP4)));
	bitmaps.push_back(LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP5)));
	bitmaps.push_back(LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP7)));

	HWND mainWindow = CreateWindowW
	(
		L"MainWin",
		L"INI Saver",
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

void savePathConfirmation()
{
	GetWindowText(INIDir, savedDir, 1000);
	
	if (wcscmp(savedDir, iniPath.getPath()) != 0)
	{
		int answer = MessageBoxA(NULL, "Would you like to save the new path?", "Save path?", MB_YESNO);
		if (answer == IDYES)
		{
			SetWindowText(INIDir, savedDir);
			iniPath.savePath(savedDir);
		}
		else
		{
			SetWindowText(INIDir, iniPath.getPath());
		}
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

		INIDir = CreateWindowW
		(
			L"Edit",
			L"",
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

		if (wcslen(iniPath.getPath()) > 0)
		{
			SetWindowText(INIDir, iniPath.getPath());
		}
		else
		{
			SetWindowTextA(INIDir, "C:\\Program Files (x86)\\Steam\\steamapps\\common\\ARK");
			iniPath.savePath(L"C:\\Program Files (x86)\\Steam\\steamapps\\common\\ARK");
		}

		SendMessageW(INIDir, WM_SETFONT, (WPARAM)smallFont, NULL);

		HWND openDirBtn = CreateWindowW
		(
			L"Button",
			L"",
			WS_VISIBLE |
			WS_CHILD |
			BS_CENTER |
			BS_BITMAP |
			BS_FLAT,
			22, 18,
			27, 29,
			hWnd,
			(HMENU)OPENDIRECTORY, 
			NULL, NULL
		);

		SendMessageW(openDirBtn, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bitmaps[5]);

		HWND saveBtn = CreateWindowW
		(
			L"Button",
			L"",
			WS_CHILD | 
			WS_VISIBLE |
			BS_CENTER | 
			BS_BITMAP |
			BS_FLAT,
			290, 20,
			25, 25,
			hWnd,
			(HMENU)CONFIRMSAVEPATH, 
			NULL, NULL
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
			(HMENU)BACKUPFILES, 
			NULL, NULL
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
			161, 31,
			hWnd,
			(HMENU)RESTOREFILES, 
			NULL, NULL
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
		case OPENDIRECTORY:
		{
			IFileDialog* fileMenu;
			
			HRESULT createResult = CoCreateInstance(
				CLSID_FileOpenDialog,
				NULL,
				CLSCTX_INPROC_SERVER,
				IID_PPV_ARGS(&fileMenu)
			);

			if (createResult == S_OK)
			{
				fileMenu->SetTitle(L"Folder to backup .ini files from");
				fileMenu->SetOptions(FOS_PICKFOLDERS);
				fileMenu->Show(hWnd);

				IShellItem* isiPtr{ 0 };
				HRESULT getResult = fileMenu->GetResult(&isiPtr);
				if (getResult == S_OK)
				{
					wchar_t* folderPath;

					HRESULT dispName = isiPtr->GetDisplayName(SIGDN_FILESYSPATH, &folderPath);

					if (dispName == S_OK)
					{
						iniPath.savePath(folderPath);
						SetWindowText(INIDir, folderPath);
					}
				}
			}
			else
			{
				wchar_t error[2000] = L"";
				FormatMessage
				(
					FORMAT_MESSAGE_FROM_SYSTEM,
					NULL,
					GetLastError(),
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					error,
					sizeof(error) / sizeof(wchar_t),
					NULL
				);

				std::wstring fullError(error);
				fullError.insert(0, L"Failed to open folder dialog! - ");
				MessageBox(NULL, fullError.c_str(), L"Dialog Error!", MB_OK | MB_ICONERROR);
			}
			
			break;
		}

		case CONFIRMSAVEPATH:	
		{
			GetWindowText(INIDir, savedDir, 1000);

			if (SaveConf == NULL)
			{
				SaveConf = CreateWindowExW
				(
					WS_EX_TOPMOST,
					L"SaveWin",
					L"",
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
			savePathConfirmation();

			if (BackupConf == NULL)
			{
				BackupConf = CreateWindowExW
				(
					WS_EX_TOPMOST,
					L"BackupWin",
					L"",
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
				BackupConf,
				rect.left + 70,
				rect.top + 70,
				200,
				130,
				FALSE
			);

			break;
		}
		case RESTOREFILES:
		{
			savePathConfirmation();

			if (RestoreConf == NULL)
			{
				RestoreConf = CreateWindowExW
				(
					WS_EX_TOPMOST,
					L"RestoreWin",
					L"",
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
				RestoreConf,
				rect.left + 70,
				rect.top + 70,
				200,
				130,
				FALSE
			);
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

		MoveWindow
		(
			BackupConf,
			rect.left + 70,
			rect.top + 70,
			200,
			130,
			FALSE
		);

		MoveWindow
		(
			RestoreConf,
			rect.left + 70,
			rect.top + 70,
			200,
			130,
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
			L"Override Path?",
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
			iniPath.savePath(savedDir);
		}

		case CANCELSAVE:
		{
			SetWindowText(INIDir, iniPath.getPath());
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

LRESULT CALLBACK BackupWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		CreateWindowW
		(
			L"Static",
			L"Override backed up files?",
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
			(HMENU)CONFIRMBACKUP, NULL, NULL
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
			(HMENU)CANCELBACKUP, NULL, NULL
		);
	}

	if (uMsg == WM_GETMINMAXINFO)
	{
		MINMAXINFO* minMax = (MINMAXINFO*)lParam;
		POINT curSize{ 200, 130 };
		minMax->ptMaxTrackSize = curSize;
		minMax->ptMinTrackSize = curSize;
	}

	if (wParam == CONFIRMBACKUP)
	{
		backup Backup;
		DestroyWindow(BackupConf);
		BackupConf = NULL;
	}
	
	if (wParam == CANCELBACKUP)
	{
		DestroyWindow(BackupConf);
		BackupConf = NULL;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK RestoreWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		CreateWindowW
		(
			L"Static",
			L"REPLACE CONFIGS WITH BACKUPS?",
			WS_VISIBLE |
			WS_CHILD |
			SS_CENTER,
			0, 10,
			180, 50,
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
			10, 50,
			80, 50,
			hWnd,
			(HMENU)CONFIRMRESTORE, NULL, NULL
		);

		CreateWindowW
		(
			L"Button",
			L"No",
			WS_VISIBLE |
			WS_CHILD |
			WS_BORDER,
			95, 50,
			80, 50,
			hWnd,
			(HMENU)CANCELRESTORE, NULL, NULL
		);
	}

	if (uMsg == WM_GETMINMAXINFO)
	{
		MINMAXINFO* minMax = (MINMAXINFO*)lParam;
		POINT curSize{ 200, 130 };
		minMax->ptMaxTrackSize = curSize;
		minMax->ptMinTrackSize = curSize;
	}

	if (wParam == CONFIRMRESTORE)
	{
		restore Restore;
		DestroyWindow(RestoreConf);
		RestoreConf = NULL;
	}

	if (wParam == CANCELRESTORE)
	{
		DestroyWindow(RestoreConf);
		RestoreConf = NULL;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}