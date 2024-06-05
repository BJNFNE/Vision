#include <windows.h>
#include <cstdlib>

// Define global variables
DWORD g_dwPlatformId;
DWORD g_dwMajorVersion;
DWORD g_dwMinorVersion;
DWORD g_dwBuildNumber;

// Function prototypes
int __heap_init();
int __mtinit();
void __RTC_Initialize();
int __ioinit();
int __setargv();
int FUN_004ac617();
int AfxWinMain(HMODULE hModule, DWORD dwReserved, LPSTR lpCmdLine, int nCmdShow);

int entry(void) {
    HMODULE hModule;
    int nCmdShow;
    int nReturnValue;
    BOOL bConsoleWindow;
    DWORD dwExitCode;

    // Initialize local variables
    OSVERSIONINFOA osVersionInfo;
    STARTUPINFOA startupInfo;

    osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
    GetVersionExA(&osVersionInfo);

    g_dwPlatformId = osVersionInfo.dwPlatformId;
    g_dwMajorVersion = osVersionInfo.dwMajorVersion;
    g_dwMinorVersion = osVersionInfo.dwMinorVersion;
    g_dwBuildNumber = osVersionInfo.dwBuildNumber & 0x7fff;

    if (g_dwPlatformId != 2) {
        g_dwBuildNumber |= 0x8000;
    }

    // Perform other initializations
    if (__heap_init() == 0) {
        exit(0x1c);
    }

    if (__mtinit() == 0) {
        exit(0x10);
    }

    __RTC_Initialize();

    if (__ioinit() < 0) {
        exit(0x1b);
    }

    // Set command line arguments
    LPSTR lpCmdLine = GetCommandLineA();
    DWORD dwReserved = FUN_004ac8ec();
    if (__setargv() < 0 || FUN_004ac617() < 0) {
        exit(9);
    }

    if (__cinit(1) != 0) {
        exit(__cinit(1));
    }

    // Get startup info
    GetStartupInfoA(&startupInfo);
    bConsoleWindow = !(startupInfo.dwFlags & STARTF_USESHOWWINDOW);

    // Determine the command line show state
    if (bConsoleWindow) {
        nCmdShow = SW_SHOWNORMAL;
    } else {
        nCmdShow = (int)startupInfo.wShowWindow;
    }

    // Get module handle
    hModule = GetModuleHandleA(NULL);

    // Call the main function
    nReturnValue = AfxWinMain(hModule, dwReserved, lpCmdLine, nCmdShow);

    // Cleanup and exit
    dwExitCode = (g_dwBuildNumber != 0) ? nReturnValue : 0;
    __cexit();
    ExitProcess(dwExitCode);
}
