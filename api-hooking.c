//
//      Ideas found at
//          https://www.ired.team/offensive-security/code-injection-process-injection/how-to-hook-windows-api-using-c++
//
#include <stdio.h>
#include <Windows.h>

#include "api-hooking.h"

// FARPROC messageBoxAddress = NULL;
// SIZE_T  bytesWritten = 0;
// char    messageBoxOriginalBytes[6] = {};



void modifyFunctionEntry(
   hook_t                       hook,
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
           void*                         fn_orig,
           void*                         fn_hook,
           hook_t*     hook       
     ) {

   SIZE_T bytesRead = 0;

   hook->fn_orig = fn_orig;

   hook->process = GetCurrentProcess();
// hook->fn_hook = fn_hook;

// save the first 6 bytes of the original MessageBoxA function - will need for unhooking
   ReadProcessMemory(
      hook->process, // GetCurrentProcess(),
      fn_orig, // messageBoxAddress,
      hook->function_entry_point_bytes, //  messageBoxOriginalBytes,
      sizeof(function_entry_point_bytes_t),
      &bytesRead
   );

   if (bytesRead != sizeof(function_entry_point_bytes_t)) {
      printf("bytes read = %d!!!!\n", bytesRead);
   }


   function_entry_point_bytes_t patch; //  = { 0 };

   void *address_of_hook = fn_hook;

   memcpy_s(patch    , 1, "\x68"                  , 1);
   memcpy_s(patch + 1, 4, &address_of_hook        , 4);  // address of address of hook!
   memcpy_s(patch + 5, 1, "\xC3"                  , 1);

   modifyFunctionEntry(
     *hook,
      patch
   );

/*
// patch the MessageBoxA
   WriteProcessMemory(
      GetCurrentProcess(),
      hook->fn_orig     , // messageBoxAddress,
      patch,
      sizeof(patch),
      &bytesWritten
   );
*/


}

void UnHookWinAPIFunction(hook_t hook) {

   modifyFunctionEntry(
      hook,
      hook.function_entry_point_bytes
   );

/*
   WriteProcessMemory(
      GetCurrentProcess(),
      hook.fn_orig, // messageBoxAddress,
      hook.function_entry_point_bytes, // messageBoxOriginalBytes,
      sizeof(function_entry_point_bytes_t),
     &bytesWritten
   );
   */

}

