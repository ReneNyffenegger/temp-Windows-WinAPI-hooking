//
//   Found at
//      https://github.com/andreasjhkarlsson/winapi-hooking-demo/blob/master/src/main.c
//
//   Compile with
//      gcc modifyImportTable.c test.c -lDbgHelp
//

#include <stdio.h>
#include <Windows.h>
#include "modifyImportTable.h"


int (__stdcall *RealMessageBoxA)(HWND, LPCSTR, LPCSTR, UINT);

int __stdcall HookedMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) {

   static int nof_calls = 0;

   printf("Hooked Message Box: title '%s' and text '%s'.\n",lpCaption,lpText);
   return RealMessageBoxA(hWnd, lpText, lpCaption, uType);

}

int main() {

  if (InstallHook(
       "User32.dll",
       "MessageBoxA",
       (void*)HookedMessageBoxA,
       (void**)(&RealMessageBoxA))) {

    printf("Hook installed!\n\n");
    MessageBoxA(NULL, "Message one", "Title one", 0);
    MessageBoxA(NULL, "Message two", "Title two", 0);
  }
  else
  {
    printf("Failed to install hook!\n");
  }

  return 0;
}
