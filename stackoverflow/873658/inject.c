// #include <cstdio>
#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>

void EnableDebugPriv() {
    HANDLE hToken;
    LUID luid;
    TOKEN_PRIVILEGES tkp;

    OpenProcessToken( GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken );

    LookupPrivilegeValue( 0, SE_DEBUG_NAME, &luid );

    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = luid;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    AdjustTokenPrivileges( hToken, 0, &tkp, sizeof( tkp ), 0, 0 );

    CloseHandle( hToken ); 
}

int main() {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof( PROCESSENTRY32 );

    HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );

    if ( Process32First( snapshot, &entry ) == TRUE ) {

        while ( Process32Next( snapshot, &entry ) == TRUE ) {
            if ( stricmp( entry.szExeFile, "target.exe" ) == 0 ) {
                printf("target was found\n");
                EnableDebugPriv();

                char dirPath[MAX_PATH];
                char fullPath[MAX_PATH];

                GetCurrentDirectory( MAX_PATH, dirPath );

                sprintf( fullPath, "%s\\hooks.dll", dirPath );
                printf("Full path of dll: %s\n", fullPath);

                HANDLE hProcess = OpenProcess( PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, entry.th32ProcessID );
                printf("hProcess = %d\n", hProcess);

                LPVOID libAddr = (LPVOID)GetProcAddress( GetModuleHandle( "kernel32.dll" ), "LoadLibraryA" );
                printf("libAddr = %d\n", libAddr);

                LPVOID llParam = (LPVOID)VirtualAllocEx( hProcess, 0, strlen( fullPath ), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE );
                printf("llParam = %d\n", llParam);

                if (! WriteProcessMemory( hProcess, llParam, fullPath, strlen( fullPath ), 0 )) {
                   printf("WriteProcessMemory failed\n");
                   exit(-1);
                }
                HANDLE th  = CreateRemoteThread( hProcess, 0, 0, (LPTHREAD_START_ROUTINE)libAddr, llParam, 0, 0 );
                if (! th) {
                   printf("CreateRemoteThread failed\n");
                   exit(-1);
                }
                CloseHandle( hProcess );
            }
        }
    }

    CloseHandle( snapshot );

    return 0;
}
