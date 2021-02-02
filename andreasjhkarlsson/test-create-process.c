#include <stdio.h>
#include <Windows.h>
#include "modifyImportTable.h"

int main() {



    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

       if( !CreateProcess( NULL,   // No module name (use command line)
        "msgBox.exe \"The message\" \"The title\"" ,   // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi )           // Pointer to PROCESS_INFORMATION structure
       )  {

         printf("Could not create msgBox process");

       }

}
