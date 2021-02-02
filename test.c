#include <Windows.h>
#include <stdio.h>

#include "api-hooking.h"

// int (__stdcall *fn_orig)(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);

int __stdcall HookedMessageBox(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) {
   
   printf("Caption: %s\nText:    %s\n", lpCaption, lpText);
   
// call the original MessageBoxA
// return fn_orig(NULL, lpText, lpCaption, uType);
   return 0;
}

int main() {

   MessageBoxA(NULL, "Not yet hooked", "Hello", MB_OK);

   HINSTANCE library = LoadLibraryA("user32.dll");
   hook_t    hook;


   HookWinAPIFunction(
       GetProcAddress(library, "MessageBoxA"),
       HookedMessageBox,
       GetCurrentProcess(),
      &hook
   );

   printf("Orig bytes:");
   for (int b=0; b<sizeof(function_entry_point_bytes_t); b++) {
      printf(" %x", (unsigned char) hook.orig_entry_bytes[b]);
   }
   printf("\n");
   

   MessageBoxA(NULL, "Hooked 1", "foo", MB_OK);
   MessageBoxA(NULL, "Hooked 2", "bar", MB_OK);

   UnHookWinAPIFunction(hook);

   MessageBoxA(NULL, "Unhooked agaain", "hi", MB_OK);

   return 0;
}
