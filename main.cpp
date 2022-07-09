#define WIN32_LEAN_AND_MEAN

//#pragma comment(linker,"/FILEALIGN:0x200 /MERGE:.data=.text /MERGE:.rdata=.text /SECTION:.text,EWR")

#include <windows.h>
#include "main.h"

LRESULT CALLBACK FilterProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	int keyChar;
	int keyScan;
	int i=0;
	int x=0;
	char mykeyname[12];

	// do not process these messages
	if (nCode < 0)  
		CallNextHookEx(g_hHook, nCode, wParam, lParam);

	EVENTMSG& m = *((EVENTMSG*)lParam);

	if (nCode == HC_ACTION)
	{
		switch(m.message)
		{
		case WM_KEYDOWN:
			keyChar = LOBYTE(m.paramL);
			keyScan = HIBYTE(m.paramL);

			keyChar <<= 16;

			if ((GetAsyncKeyState(VK_F12) & 0x80000000))
			{
				if (strcmp(buffer, "HIDE") == 0)
				{
					Lights(2);
					ShowWindow(g_hWnd, SW_HIDE);
					return 0;
				}
				else if (strcmp(buffer, "SHOW") == 0)
				{
					Lights(2);
					ShowWindow(g_hWnd, SW_SHOW);
					return 0;
				}
				else if (strcmp(buffer, "LITE") == 0)
				{
					Lights(30);
					return 0;
				}
				else if (strcmp(buffer, "EXIT") == 0)
				{
					Lights(2);
				    SetPrivileges(SE_SHUTDOWN_NAME);
					ExitWindowsEx(EWX_POWEROFF, 0);
					return 0;
				}
				else if (strcmp(buffer, "BOOT") == 0)
				{
					Lights(2);
				    SetPrivileges(SE_SHUTDOWN_NAME);
					ExitWindowsEx(EWX_REBOOT, 0);
					return 0;
				}
				else if (strcmp(buffer, "QUIT") == 0)
				{
					Lights(2);
					PostQuitMessage(0);
					return 0;
				}
				//MessageBox(0, buffer, "kvmswitch", 0);
			}
			else
			{
				GetKeyNameText(keyScan <<= 16, mykeyname, sizeof(mykeyname));
				for(i=0; i<strlen(mykeyname); i++)
				{
					if (strlen(buffer) <= sizeof(buffer)-1)
					{
						buffer[n] = mykeyname[i];
						n++;
					}
					else
					{
						for(x=0; x<sizeof(buffer)-1; x++)
							buffer[x] = buffer[x+1];
						buffer[x] = mykeyname[i];
						n = sizeof(buffer) - 1;
					}
				}
				SetWindowText(g_hWnd, buffer);
				return 0;
			}
			break;
		default:
		    break;
		}
	}

	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    HDC           hdc;
    PAINTSTRUCT   ps;
	RECT          rect;

	switch(Msg)
    {
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			Lights(2);
			PostQuitMessage(0);
			return 0;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(g_hWnd, &ps);
		GetClientRect(g_hWnd, &rect);
		DrawText(hdc, TEXT("[F12]\n\nHIDE - hide\nSHOW - show\nLITE - lights\nEXIT - shutdown\nBOOT - reboot\nQUIT (ESC) - close"), -1, &rect, DT_CENTER | DT_VCENTER);
		EndPaint(g_hWnd, &ps);
		break;
	case WM_DESTROY:
		UnhookWindowsHookEx(g_hHook);
		PostQuitMessage(0);
		return 0;
	default:
        break;
	}

    return DefWindowProc(hWnd, Msg, wParam, lParam);
}
//---------------------------------------------------------------------------
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX		WndCls;
    static char		szAppName[] = "KVMSwitch";
	static char		szAppTitle[]= "KVMSwitch";
    MSG				Msg;
//	HRESULT			hRes;

	n                    = 0;
	g_hInstance		     = hInstance;
	WndCls.cbSize        = sizeof(WndCls);
    WndCls.style         = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
    WndCls.lpfnWndProc   = WindowProc;
    WndCls.cbClsExtra    = 0;
    WndCls.cbWndExtra    = 0;
    WndCls.hInstance     = hInstance;
    WndCls.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    WndCls.hCursor       = LoadCursor(NULL, IDC_ARROW);
    WndCls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndCls.lpszMenuName  = NULL;
    WndCls.lpszClassName = szAppName;
    WndCls.hIconSm       = LoadIcon(hInstance, IDI_APPLICATION);
    RegisterClassEx(&WndCls);

    g_hWnd = CreateWindowEx(0,
					szAppName,
                    szAppTitle,
                    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                    0,
                    0,
					320,
					240,
                    NULL,
                    NULL,
                    hInstance,
                    NULL);

	ShowWindow(g_hWnd, SW_HIDE);
	g_hHook = SetWindowsHookEx(WH_JOURNALRECORD, FilterProc, GetModuleHandle(NULL), 0);

	if (g_hHook == NULL)
		MessageBox(0, "Cannot Set Hook.", "kvmswitch", 0);

    while( GetMessage(&Msg, NULL, 0, 0) )
    {
		if (Msg.message == WM_CANCELJOURNAL)
			g_hHook = SetWindowsHookEx(WH_JOURNALRECORD, FilterProc, GetModuleHandle(NULL), 0);
        TranslateMessage(&Msg);
        DispatchMessage( &Msg);
    }

    return Msg.wParam;
}
