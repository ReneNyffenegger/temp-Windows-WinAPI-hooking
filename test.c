#include <Windows.h>
#include <stdio.h>

#include "api-hooking.h"


int __stdcall HookedMessageBox(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) {
   
   printf("Caption: %s\nText:    %s\n", lpCaption, lpText);
   
// call the original MessageBoxA
// return MessageBoxA(NULL, lpText, lpCaption, uType);
   return 0;
}

int main() {

   MessageBoxA(NULL, "Not yet hooked", "Hello", MB_OK);

   HINSTANCE library = LoadLibraryA("user32.dll");
// SIZE_T bytesRead = 0;
   
// void* messageBoxAddress = GetProcAddress(library, "MessageBoxA");

/*

// save the first 6 bytes of the original MessageBoxA function - will need for unhooking
   ReadProcessMemory(
      GetCurrentProcess(),
      messageBoxAddress,
      messageBoxOriginalBytes,
      6,
      &bytesRead
   );

*/

// function_entry_point_bytes_t    orig;
   hook_t                          hook;

   HookWinAPIFunction(
       GetProcAddress(library, "MessageBoxA"), // messageBoxAddress,
       HookedMessageBox,
      &hook
   );
   

// create a patch "push <address of new MessageBoxA); ret"
// char patch[6] = { 0 };

/*
   function_entry_point_bytes_t patch; //  = { 0 };

   void *address_of_hook = HookedMessageBox;

   memcpy_s(patch    , 1, "\x68"                  , 1);
   memcpy_s(patch + 1, 4, &address_of_hook        , 4);  // address of address of hook!
   memcpy_s(patch + 5, 1, "\xC3"                  , 1);

// patch the MessageBoxA
   WriteProcessMemory(
      GetCurrentProcess(),
      hook.fn_orig, // messageBoxAddress,
      patch,
      sizeof(patch),
      &bytesWritten
   );
  */

// show messagebox after hooking
   MessageBoxA(NULL, "Hooked 1", "hi", MB_OK);
   MessageBoxA(NULL, "Hooked 2", "hi", MB_OK);

   UnHookWinAPIFunction(hook);

   MessageBoxA(NULL, "Unhooked agaain", "hi", MB_OK);

   return 0;
}
