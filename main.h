#include <windows.h>

HHOOK			g_hHook;
HINSTANCE		g_hInstance;
HWND			g_hWnd;
int				n;
char			buffer[4];

void SetPrivileges(LPCTSTR lpName)
{
    DWORD ret;
    HANDLE hProcess;
    HANDLE hToken;
    LUID    Luid;
    TOKEN_PRIVILEGES tokenNew, tokenPre;

    hProcess = GetCurrentProcess();
    OpenProcessToken(hProcess, TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken);
    LookupPrivilegeValue(NULL, lpName, &Luid);

    tokenNew.PrivilegeCount = 1;
    tokenNew.Privileges[0].Luid = Luid;
    tokenNew.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    AdjustTokenPrivileges(hToken, FALSE, &tokenNew, sizeof(tokenPre), &tokenPre, &ret);
}

void Lights (int Cycle)
{
	for(int i=0; i<Cycle; i++)
	{
			// Simulate a key press
			Sleep(75);
			keybd_event( VK_NUMLOCK,
                      0x45,
                      KEYEVENTF_EXTENDEDKEY | 0,
                      0 );
			// Simulate a key release
			Sleep(75);
			keybd_event( VK_NUMLOCK,
                      0x45,
                      KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
                      0);
			// Simulate a key press
			Sleep(75);
			keybd_event( VK_CAPITAL,
                      0x45,
                      KEYEVENTF_EXTENDEDKEY | 0,
                      0 );
			// Simulate a key release
			Sleep(75);
			keybd_event( VK_CAPITAL,
                      0x45,
                      KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
                      0);
			// Simulate a key press
			Sleep(75);
			keybd_event( VK_SCROLL,
                      0x45,
                      KEYEVENTF_EXTENDEDKEY | 0,
                      0 );
			// Simulate a key release
			Sleep(75);
			keybd_event( VK_SCROLL,
                      0x45,
                      KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
                      0);
	}
}