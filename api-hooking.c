//
//      Ideas found at
//          https://www.ired.team/offensive-security/code-injection-process-injection/how-to-hook-windows-api-using-c++
//
#include <stdio.h>
#include <Windows.h>

#include "api-hooking.h"

void modifyFunctionEntry(
   hook_t                       hook,
// HANDLE                       process,
   function_entry_point_bytes_t bytes
)
{
   SIZE_T  bytesWritten = 0;

   WriteProcessMemory(
      hook.process, // GetCurrentProcess(),
      hook.fn_orig,
      bytes, // messageBoxOriginalBytes,
      sizeof(function_entry_point_bytes_t), // sizeof(messageBoxOriginalBytes),
     &bytesWritten
   );

   if (bytesWritten != sizeof(function_entry_point_bytes_t)) {

      printf("bytesWritten = %d!!!!\n", bytesWritten);

   }

}

void HookWinAPIFunction(
           void*       fn_orig,
           void*       fn_hook,
           HANDLE      process,
           hook_t*     hook       
     ) {

   SIZE_T bytesRead = 0;

   hook->fn_orig = fn_orig;

   hook->process = process; // GetCurrentProcess();

//
// Save hooked function's original bytes for unhooking
//
   ReadProcessMemory(
      hook->process,
      fn_orig,
      hook->orig_entry_bytes,
      sizeof(function_entry_point_bytes_t),
      &bytesRead
   );

   if (bytesRead != sizeof(function_entry_point_bytes_t)) {
      printf("bytes read = %d!!!!\n", bytesRead);
   }


   function_entry_point_bytes_t patch;

   void *address_of_hook = fn_hook;

   memcpy_s(patch    , 1, "\x68"          , 1);  // 68 = push (?)
   memcpy_s(patch + 1, 4, &address_of_hook, 4);  // address of address of hook!
   memcpy_s(patch + 5, 1, "\xC3"          , 1);  // C3 = ret

   modifyFunctionEntry(
     *hook,
      patch
   );

}

void UnHookWinAPIFunction(hook_t hook) {

   modifyFunctionEntry(
      hook,
      hook.orig_entry_bytes
   );

}
