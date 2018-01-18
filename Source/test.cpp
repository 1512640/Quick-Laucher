// test.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "test.h"
#include <iostream>
#include <string>
#include <vector>
#include <shellapi.h>
#include <CommCtrl.h>
#include <windowsx.h>

#include <fstream>
#include <fcntl.h> //_O_WTEXT
#include <io.h>    //_setmode()
#include <locale>
#include <codecvt> //possible C++11?
#pragma comment(lib,"Comctl32.lib")
using namespace std;
#define MAX_LOADSTRING 100


#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")
using namespace Gdiplus;



// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK VeBieuDo(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);




	GdiplusStartupInput Input;
	ULONG_PTR	Token;
	GdiplusStartup(&Token, &Input, NULL);



	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEST));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

void ReadFile(WCHAR* chuoi);
void WriteFile(WCHAR* chuoi);
void WriteDataInFile(WCHAR* ten);
//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEST));
	wcex.hCursor = LoadCursor(NULL, IDC_HAND);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = 0;//MAKEINTRESOURCE(IDC_TEST);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
HWND hwnd;
HWND them;
struct DataLink
{
	wstring name;
	wstring link;
	int solansudung = 0;
};
vector<DataLink> list;
vector<DataLink> vectorlistbox;
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{


	hInst = hInstance; // Store instance handle in our global variable

	hwnd = CreateWindow(szWindowClass, szTitle,WS_POPUP /*WS_OVERLAPPEDWINDOW*/,
		400, 200, 500, 300, NULL, NULL, hInstance, NULL);

	if (!hwnd)
	{
		return FALSE;
	}

	ShowWindow(hwnd, 0);
	UpdateWindow(hwnd);
	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
void LoadNIcon(HWND hwnd)//load notify icon
{
	// 
	NOTIFYICONDATA nid = { 0 };
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	nid.uCallbackMessage = Load_Icon;
	wcscpy(nid.szTip, L"Quick Launch");
	nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL));
	Shell_NotifyIcon(NIM_ADD, &nid);
}
HHOOK hook;
HWND combobox;
HWND listbox;
LRESULT CALLBACK XuLyPhim(int message, WPARAM wParam, LPARAM lParam)
{
	if ((GetAsyncKeyState(VK_LCONTROL) & 0x8000) && (GetAsyncKeyState(VK_SPACE) & 1))
	{
		if (IsWindowVisible(hwnd) == false)
		{
			ShowWindow(hwnd, 1);
		}
		else
		{
			ShowWindow(hwnd, 0);
		}
	}
	if (GetAsyncKeyState(VK_RETURN))
	{
		HWND hfocus = GetFocus();
		if (hfocus == listbox)
		{ 
			int vtchon = ListBox_GetCurSel(listbox);
			if (vtchon != -1)
			{
				ShellExecute(0, 0, vectorlistbox[vtchon].link.c_str(), 0, 0, 1);
				for (int i = 0; i < list.size(); i++)
				{
					if (vectorlistbox[vtchon].name == list[i].name)
					{
						list[i].solansudung++;
						break;
					}
				}
			}
		}
		if (hfocus == combobox)
		{
			WCHAR ten[255];
			GetWindowText(combobox, ten, 255);
			int dem = 0;
			wstring tencantim = ten;
			for (int i = 0; i < vectorlistbox.size(); i++)
			{
				if (tencantim == vectorlistbox[i].name)
				{
					ShellExecute(0, 0, vectorlistbox[i].link.c_str(), 0, 0, 1);
					dem++;
					break;
				}
			}
			if (dem == 0)
			{
				MessageBox(hwnd, L"Không tìm thấy ", L"Thông báo", MB_OK | MB_ICONERROR);
			}
		}
	}
	if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_DOWN))
	{
		SetFocus(listbox);
	}
	return CallNextHookEx(hook, message, wParam, lParam);

}

void Duyet(WCHAR * link)
{
	WCHAR link_full[MAX_PATH];
	wsprintf(link_full, L"%s\\*.*", link);
	WIN32_FIND_DATA data;
	HANDLE hand = FindFirstFile(link_full, &data); // doc duong dan tìm file dau tien bo vao data
	if (hand != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if ((data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == false)
				{
					if (wcscmp(data.cFileName, L".") && wcscmp(data.cFileName, L".."))
					{
						WCHAR duongDan[MAX_PATH];
						wsprintf(duongDan, L"%s\\%s", link, data.cFileName);
						Duyet(duongDan);
					}
				}
			}
			else
			{
				wstring filename = data.cFileName;
				int vtexe = filename.find(L".exe");
				if (vtexe == filename.length() - 4)
				{
					DataLink temp;
					temp.name = data.cFileName;
					WCHAR *duongDan = new WCHAR[MAX_PATH];
					wsprintf(duongDan, L"%s\\%s", link, data.cFileName);
					temp.link = duongDan;
					list.push_back(temp);

				}
			}
		} while (FindNextFile(hand, &data));
		FindClose(hand);
	}
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	WCHAR* buffer1 = NULL;
	int size;
	PAINTSTRUCT ps;
	HDC hdc;
	
	switch (message)
	{
	case Load_Icon:
	{
				switch (lParam)
				{
				case WM_LBUTTONUP:
					//if (IsZoomed)
					//if (IsIconic)
					if (IsWindowVisible(hWnd) == false)
						ShowWindow(hWnd, 1);
					break;
				case WM_RBUTTONUP:
				{
									 HMENU NotifyMenu = CreatePopupMenu();
									 AppendMenu(NotifyMenu, MF_STRING, scan, L"scan");
									 AppendMenu(NotifyMenu, MF_STRING, view, L"View");
									 AppendMenu(NotifyMenu, MF_STRING, ID_exit, L"Exit");
									 POINT pos;
									 GetCursorPos(&pos);
									 TrackPopupMenu(NotifyMenu, TPM_RIGHTBUTTON, pos.x, pos.y, 0, hWnd, NULL);

					
				}
					break;
				}       
	}
		break;
	case WM_CREATE:
	{	// Lấy font hệ thống

		HFONT hFont = CreateFont(36, 14, 0, 0, FW_DONTCARE, FALSE, 0, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
			CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Times New Roman"));

		hook = SetWindowsHookEx(WH_KEYBOARD_LL, XuLyPhim, hInst, 0);
		LoadNIcon(hWnd);
		//combobox = CreateWindow(WC_COMBOBOX, L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWN , 0, 20, 400, 120, hWnd, (HMENU)ID_Combobox, 0, 0);
		combobox = CreateWindow(WC_EDIT, L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWN|ES_CENTER| ES_AUTOHSCROLL, 0, 40, 500, 80, hWnd, (HMENU)ID_Combobox, 0, 0);

		SendMessage(combobox, WM_SETFONT, WPARAM(hFont), TRUE);
		listbox = CreateWindow(WC_LISTBOX, L"", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | LBS_NOTIFY | LBS_COMBOBOX, 0, 80, 500,220, hWnd, (HMENU)ID_list, 0, 0);
		
		them = CreateWindowEx(0, WC_BUTTON, L"", BS_OWNERDRAW | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 400,0, 70, 40, hWnd, (HMENU)IDC_Them, hInst, NULL);


		ShowWindow(listbox, 0);
	}
		break;
	case WM_CTLCOLORBTN:
	{
		RECT crect;
		HDC hdc = (HDC)wParam;
		HWND button_handle = (HWND)lParam;
		GetClientRect(them, &crect);
		SetBkColor(hdc, RGB(0, 255, 0));
		SetTextColor(hdc, RGB(0, 0, 0));
		DrawText(hdc, L"Thêm link", -1, &crect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		return (LRESULT)CreateSolidBrush(RGB(0, 255, 0));
		break;
	}
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_Them:
			WriteFile(L"Link.txt");
			MessageBox(hWnd, L"Thêm link thành công", L"", MB_OK);
			break;
		case view:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_BIEUDO), NULL, VeBieuDo);
			break;
		case ID_list:
			if (wmEvent == LBN_DBLCLK)
			{
				int vtchon = ListBox_GetCurSel(listbox);
				if (vtchon == -1)
					break;
				ShellExecute(0, 0, vectorlistbox[vtchon].link.c_str(), 0, 0, 1);
				for (int i = 0; i < list.size(); i++)
				{
					if (vectorlistbox[vtchon].name == list[i].name)
					{
						list[i].solansudung++;
						break;
					}
				}
			}
			break;
		case ID_Combobox:
		{
			//if (wmEvent == /*CBN_EDITCHANGE|*/)
			{
				WCHAR ten[255];
				GetWindowText(combobox, ten, 255);
				for (int i = 0; i < vectorlistbox.size(); i++)
					ListBox_DeleteString(listbox, 0);
				vectorlistbox.clear();
				wstring tenkiemtra = ten;
				if (tenkiemtra == L"")
					ShowWindow(listbox, 0);
				else
				{
					for (int i = 0; i < list.size(); i++)
					{
						int kq = list[i].name.find(tenkiemtra);
						if (kq != -1)
						{
							vectorlistbox.push_back(list[i]);
						}
					}
					int len = vectorlistbox.size();
					for (int i = 0; i < len - 1; i++)
					{
						for (int j = i + 1; j < len; j++)
						{
							if (vectorlistbox[i].solansudung > vectorlistbox[j].solansudung)
							{
								DataLink tamp = vectorlistbox[i];
								vectorlistbox[i] = vectorlistbox[j];
								vectorlistbox[j] = tamp;
							}
						}
					}
					for (int i = 0; i < len; i++)
						ListBox_AddString(listbox, vectorlistbox[i].name.c_str());
					ShowWindow(listbox, 1);
				}
			}
		}
			break;
		case scan:
			Duyet(L"C:\\Program Files (x86)");
			MessageBox(hWnd, L"Quét hoàn tất", L"Thông Báo", MB_OK);
			WriteDataInFile(L"Data.txt");
			break;
		case ID_exit:
			DestroyWindow(hWnd);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_CLOSE:
		ShowWindow(hWnd, 0);
		return TRUE;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam)); 
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK VeBieuDo(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	switch (message)
	{
	
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case ID_OK:
			EndDialog(hWnd, 0);
		}
		break;
	case WM_PAINT:
	{
					 vector<DataLink> dscanve;
					 int tongsolansudung = 0;
					 for (int i = 0; i < list.size(); i++)
					 {
						 if (list[i].solansudung != 0)
						 {
							 dscanve.push_back(list[i]);
							 tongsolansudung += list[i].solansudung;
						 }
					 }

					 HDC hdc = GetDC(hWnd);
					 Graphics *graphic = new Graphics(hdc);

					 Brush* brush[5];
					 for (int i = 0; i < 5; i++)
					 {
						 int  j = i * 10;
						 brush[i] = new SolidBrush(Color(255, 255 - j * 40, j * 40, j * 40));

					 }
					 int vitri = 0;

					 Gdiplus::FontFamily  fontFamily(L"Arial");
					 Gdiplus::Font        font(&fontFamily, 15, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
					 Gdiplus::PointF      pointF(340.0f, 36.0f);
					 Gdiplus::SolidBrush  solidBrush(Gdiplus::Color(255, 0, 174, 239));

					
					 vector<float>phantran;
					 for (int i = 0; i < dscanve.size(); i++)
					 {
						 
						 phantran.push_back(((float)dscanve[i].solansudung / tongsolansudung) * 360);
						 graphic->FillPie(brush[i], Rect(250, vitri, 80, 80), -20, 40); // ve hinh chu thich
						 vitri += 50;
						 graphic->DrawString(dscanve[i].name.c_str(), -1, &font, pointF, &solidBrush); // viet chu chu thich
						 pointF = PointF(340.0f, 36.0f + vitri);
					 }
					 
					 int len = phantran.size();
					 int bd = 0, docanve = 0;
					 for (int i = 0; i < phantran.size(); i++)
					 {
						 docanve = phantran[i];
						 graphic->FillPie(brush[i], Rect(50, 50, 200, 200), bd, docanve);
						 bd += docanve;
					 }
					 
					 break;
	}
	}
	return (INT_PTR)FALSE;
}
void ReadFile(WCHAR* chuoi)
{
	locale loc(locale(), new codecvt_utf8<wchar_t>);
	wifstream readfile;
	readfile.open(chuoi, ios::in | ios::binary); // Mo file 
	readfile.imbue(loc);
	wchar_t *s;
	s = new wchar_t[1];
	if (readfile.is_open() == true)
	{
		while (!readfile.eof())
		{
			s[1] = 0;
			s[0] = readfile.get();
			SendMessage(combobox, EM_REPLACESEL, 0, (LPARAM)s);
		}
	}
	readfile.close();// Dong file 
}
void WriteFile(WCHAR* chuoi)
{
	int dodai = GetWindowTextLengthW(combobox);
	WCHAR *str = new WCHAR[dodai + 1];
	GetWindowText(combobox, str, dodai + 1);
	locale loc(locale(), new codecvt_utf8<wchar_t>);
	wfstream writeFile;
	writeFile.open(chuoi, ios::out | ios::binary | ios::app|ios::ate); // Mo file
	writeFile.imbue(loc);
	if (writeFile.is_open())
	{
		writeFile << str;
	}
	writeFile << "\n";
	writeFile.close();
	delete[]str;
}
void WriteDataInFile(WCHAR* ten)
{
	locale loc(locale(), new codecvt_utf8<wchar_t>);
	wfstream writeFile;
	writeFile.open(ten, ios::out | ios::binary | ios::app | ios::ate); // Mo file
	writeFile.imbue(loc);
	if (writeFile.is_open())
	{
		for (int i = 0; i < list.size(); i++)
		{
			writeFile << "Key: ";
			writeFile << list[i].name;
			writeFile << " || Value: ";
			writeFile << list[i].link;
			writeFile << "\n";
		}
	}
	writeFile.close();
}