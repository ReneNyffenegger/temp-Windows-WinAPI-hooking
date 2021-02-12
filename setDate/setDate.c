// #include <cstdio>
#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>
#include "Detours.h"

// void EnableDebugPriv() {
//     HANDLE hToken;
//     LUID luid;
//     TOKEN_PRIVILEGES tkp;
// 
//     OpenProcessToken( GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken );
// 
//     LookupPrivilegeValue( 0, SE_DEBUG_NAME, &luid );
// 
//     tkp.PrivilegeCount = 1;
//     tkp.Privileges[0].Luid = luid;
//     tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
// 
//     AdjustTokenPrivileges( hToken, 0, &tkp, sizeof( tkp ), 0, 0 );
// 
//     CloseHandle( hToken ); 
// }

int main() {

   STARTUPINFO         si; ZeroMemory(&si, sizeof(si));
   PROCESS_INFORMATION pi; ZeroMemory(&pi, sizeof(pi));

   si.cb = sizeof(si);
   si.dwFlags = STARTF_USESHOWWINDOW;
   si.wShowWindow = SW_SHOW;

 if (! DetourCreateProcessWithDllEx (
      0                , // _In_opt_    LPCTSTR lpApplicationName,
     "git commit a -m ""test"""     , // _Inout_opt_ LPTSTR lpCommandLine,
//   "..\\stackoverflow\\873658\\target.exe"     , // _Inout_opt_ LPTSTR lpCommandLine,
      0                , // _In_opt_    LPSECURITY_ATTRIBUTES lpProcessAttributes,
      0                , // _In_opt_    LPSECURITY_ATTRIBUTES lpThreadAttributes,
      1                , // _In_        BOOL bInheritHandles,
      CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED                , // _In_        DWORD dwCreationFlags,
      0                , // _In_opt_    LPVOID lpEnvironment,
      0                , // _In_opt_    LPCTSTR lpCurrentDirectory,
     &si              , // _In_        LPSTARTUPINFOW lpStartupInfo,
     &pi              , // _Out_       LPPROCESS_INFORMATION lpProcessInformation,
//   "hooks.dll"      , // _In_        LPCSTR lpDllName,
     "C:\\Users\\r.nyffenegger\\github\\temp\\Windows-WinAPI-hooking\\setDate\\hooks.dll"      , // _In_        LPCSTR lpDllName,
      0                 // _In_opt_    PDETOUR_CREATE_PROCESS_ROUTINEW pfCreateProcessW
  )) {

    printf("Failed.\n");
    exit(-1);


  }

    ResumeThread(pi.hThread);

		WaitForSingleObject(pi.hProcess, INFINITE);

		CloseHandle(&si);
		CloseHandle(&pi);

//    PROCESSENTRY32 entry;
//    entry.dwSize = sizeof( PROCESSENTRY32 );
//
//    HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
//
//    if ( Process32First( snapshot, &entry ) == TRUE ) {
//
//        while ( Process32Next( snapshot, &entry ) == TRUE ) {
//            if ( stricmp( entry.szExeFile, "target.exe" ) == 0 ) {
//                printf("target was found\n");
//                EnableDebugPriv();
//
//                char dirPath[MAX_PATH];
//                char fullPath[MAX_PATH];
//
//                GetCurrentDirectory( MAX_PATH, dirPath );
//
//                sprintf( fullPath, "%s\\hooks.dll", dirPath );
//                printf("Full path of dll: %s\n", fullPath);
//
//                HANDLE hProcess = OpenProcess( PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, entry.th32ProcessID );
//                printf("hProcess = %d\n", hProcess);
//
//                LPVOID libAddr = (LPVOID)GetProcAddress( GetModuleHandle( "kernel32.dll" ), "LoadLibraryA" );
//                printf("libAddr = %d\n", libAddr);
//
//                LPVOID llParam = (LPVOID)VirtualAllocEx( hProcess, 0, strlen( fullPath ), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE );
//                printf("llParam = %d\n", llParam);
//
//                if (! WriteProcessMemory( hProcess, llParam, fullPath, strlen( fullPath ), 0 )) {
//                   printf("WriteProcessMemory failed\n");
//                   exit(-1);
//                }
//                HANDLE th  = CreateRemoteThread( hProcess, 0, 0, (LPTHREAD_START_ROUTINE)libAddr, llParam, 0, 0 );
//                if (! th) {
//                   printf("CreateRemoteThread failed\n");
//                   exit(-1);
//                }
//                CloseHandle( hProcess );
//            }
//        }
//    }
//
//    CloseHandle( snapshot );

    return 0;
}
