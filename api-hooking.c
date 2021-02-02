//
//      Ideas found at
//          https://www.ired.team/offensive-security/code-injection-process-injection/how-to-hook-windows-api-using-c++
//
#include <stdio.h>
#include <Windows.h>

FARPROC messageBoxAddress = NULL;
SIZE_T  bytesWritten = 0;
char    messageBoxOriginalBytes[6] = {};

int __stdcall HookedMessageBox(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) {
   
//
// 
   printf("Caption: %s\nText:    %s\n", lpCaption, lpText);
   
// unpatch MessageBoxA
   WriteProcessMemory(
      GetCurrentProcess(),
      (LPVOID) messageBoxAddress,
      messageBoxOriginalBytes,
      sizeof(messageBoxOriginalBytes),
     &bytesWritten
   );
   
// call the original MessageBoxA
   return MessageBoxA(NULL, lpText, lpCaption, uType);
}

int main() {

   MessageBoxA(NULL, "Not yet hooked", "Hello", MB_OK);

   HINSTANCE library = LoadLibraryA("user32.dll");
   SIZE_T bytesRead = 0;
   
   messageBoxAddress = GetProcAddress(library, "MessageBoxA");

// save the first 6 bytes of the original MessageBoxA function - will need for unhooking
   ReadProcessMemory(
      GetCurrentProcess(),
      messageBoxAddress,
      messageBoxOriginalBytes,
      6,
      &bytesRead
   );
   
// create a patch "push <address of new MessageBoxA); ret"
   void *hookedMessageBoxAddress = &HookedMessageBox;
   char patch[6] = { 0 };
   memcpy_s(patch, 1, "\x68", 1);
   memcpy_s(patch + 1, 4, &hookedMessageBoxAddress, 4);
   memcpy_s(patch + 5, 1, "\xC3", 1);

// patch the MessageBoxA
   WriteProcessMemory(GetCurrentProcess(), (LPVOID)messageBoxAddress, patch, sizeof(patch), &bytesWritten);

// show messagebox after hooking
   MessageBoxA(NULL, "Hooked 1", "hi", MB_OK);
   MessageBoxA(NULL, "Hooked 2", "hi", MB_OK);

   return 0;
}
