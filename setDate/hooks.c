#include <windows.h>
#include <winternl.h>

#include <stdio.h>
#include "detours.h"

#include "tq84_debug.h"

// #include "tq84_debug.h"

// #pragma comment( lib, "Ws2_32.lib" )
// #pragma comment( lib, "detours.lib" )
// #pragma comment( lib, "detoured.lib" )
//

// int (WINAPI *real_MessageBox)( HWND    hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT    uType);
//
void     (WINAPI *GetSystemTimePreciseAsFileTime_real)(LPFILETIME );
void     (WINAPI *GetSystemTimeAsFileTime_real       )(LPFILETIME    );
void     (WINAPI *GetSystemTime_real                 )(LPSYSTEMTIME  );
void     (WINAPI *GetLocalTime_real                  )(LPSYSTEMTIME  );
NTSTATUS (WINAPI *NtQuerySystemTime_real             )(PLARGE_INTEGER);

int      (WINAPI *GetTimeFormatA_real                )(LCID             Locale, DWORD            dwFlags, const SYSTEMTIME *lpTime, LPCSTR           lpFormat, LPSTR            lpTimeStr, int              cchTime);
int      (WINAPI *GetTimeFormatW_real                )(LCID             Locale, DWORD            dwFlags, const SYSTEMTIME *lpTime, LPCSTR           lpFormat, LPSTR            lpTimeStr, int              cchTime);


int      WINAPI GetTimeFormatA_hook(LCID             Locale, DWORD            dwFlags, const SYSTEMTIME *lpTime, LPCSTR           lpFormat, LPSTR            lpTimeStr, int              cchTime) {

  TQ84_DEBUG_INDENT();
   MessageBox(0, "GetTimeFormatA_hook", 0, 0);
  return GetTimeFormatA_real(Locale, dwFlags, lpTime, lpFormat, lpTimeStr, cchTime);
}

int      WINAPI GetTimeFormatW_hook(LCID             Locale, DWORD            dwFlags, const SYSTEMTIME *lpTime, LPCSTR           lpFormat, LPSTR            lpTimeStr, int              cchTime) {
  TQ84_DEBUG_INDENT();
   MessageBox(0, "GetTimeFormatW_hook", 0, 0);
  return GetTimeFormatW_real(Locale, dwFlags, lpTime, lpFormat, lpTimeStr, cchTime);
}



void WINAPI GetSystemTimeAsFileTime_hook(LPFILETIME t) {
   TQ84_DEBUG_INDENT();
   char buf[200];
   GetSystemTimeAsFileTime_real(t);
   sprintf(buf, "GetSystemTimeAsFileTime_hook %u %u", t->dwLowDateTime, t->dwHighDateTime);
   MessageBox(0, buf, "TQ84", 0);

   t->dwLowDateTime  = 0;
   t->dwHighDateTime = 0;
}

void WINAPI GetSystemTimePreciseAsFileTime_hook(LPFILETIME t) {
   TQ84_DEBUG_INDENT();
   MessageBox(0, "GetSystemTimePreciseAsFileTime_real", 0, 0);

   GetSystemTimePreciseAsFileTime_real(t);
}

NTSTATUS WINAPI NtQuerySystemTime_hook(PLARGE_INTEGER t) {
   TQ84_DEBUG_INDENT();
   MessageBox(0, "NtQuerySystemTime_real", 0, 0);
   return NtQuerySystemTime_real(t);
}


void WINAPI GetLocalTime_hook (LPSYSTEMTIME t) {
   TQ84_DEBUG_INDENT();

   MessageBox(0, "GetLocalTime", "hook", 0);
   GetLocalTime_real(t);

// Change year, month and day …
   t -> wYear   = 1970;
   t -> wMonth  =    8;
   t -> wDay    =   28;

// … but leave hour, minute and secnod
// t -> wHour   =   22;
// t -> wMinute =   22;
// t -> wSecond =   23;

}

void WINAPI   GetSystemTime_hook (LPSYSTEMTIME t) {

   TQ84_DEBUG_INDENT();
   MessageBox(0, "GetSystemTime", "hook", 0);

   GetSystemTime_real(t);

// Change year, month and day …
   t -> wYear   = 1970;
   t -> wMonth  =    8;
   t -> wDay    =   28;

// … but leave hour, minute and secnod
// t -> wHour   =   22;
// t -> wMinute =   22;
// t -> wSecond =   23;

}

// int WINAPI my_MessageBox (HWND    hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT    uType) {
//    printf("message %s was intercepted\n", lpText);
// 
//    int ret = real_MessageBox(hWnd, lpText, lpCaption, uType);
//    printf("return value is: %d\n", ret);
//    return ret;
// }

// int ( WINAPI *Real_Send )( SOCKET s, const char *buf, int len, int flags ) = send;
// int ( WINAPI *Real_Recv )( SOCKET s, char *buf, int len, int flags ) = recv;  
// int WINAPI Mine_Send( SOCKET s, const char* buf, int len, int flags );
// int WINAPI Mine_Recv( SOCKET s, char *buf, int len, int flags );
// 
// int WINAPI Mine_Send( SOCKET s, const char *buf, int len, int flags ) {
//     // .. do stuff ..
// 
//     return Real_Send( s, buf, len, flags );
// }
// 
// int WINAPI Mine_Recv( SOCKET s, char *buf, int len, int flags ) {
//     // .. do stuff ..
// 
//     return Real_Recv( s, buf, len, flags );
// }


int attach(PVOID func, PVOID* real, PVOID hook) {

  *real = func;
  
   TQ84_DEBUG("Calling DetourAttach");
   LONG dar;


// if ( (dar = DetourAttach(  (PVOID*) &real_GetSystemTime, (PVOID) my_GetSystemTime )) != NO_ERROR )
   if ( (dar = DetourAttach(            real              ,            hook          )) != NO_ERROR )
   {
      TQ84_DEBUG("DetourAttach failed");
      if      (dar == ERROR_INVALID_BLOCK      ) { TQ84_DEBUG("ERROR_INVALID_BLOCK"    ); }
      else if (dar == ERROR_INVALID_HANDLE     ) { TQ84_DEBUG("ERROR_INVALID_HANDLE"   ); }
      else if (dar == ERROR_INVALID_OPERATION  ) { TQ84_DEBUG("ERROR_INVALID_OPERATION"); }
      else if (dar == ERROR_NOT_ENOUGH_MEMORY  ) { TQ84_DEBUG("ERROR_NOT_ENOUGH_MEMORY"); }

      return 0; 
   }

   return 1;


}
BOOL WINAPI DllMain(HINSTANCE i, DWORD dwReason, LPVOID l) {

  switch ( dwReason ) {

       case DLL_PROCESS_ATTACH: {
          TQ84_DEBUG_OPEN("hook.debug.out", "w");
//        tq84_debug_open("hook.debug.out", "w");
          TQ84_DEBUG("Calling DetourTransactionBegin");
          TQ84_DEBUG_INDENT_T("DLL_PROCESS_ATTACH");

          if (DetourTransactionBegin() != NO_ERROR) {
             TQ84_DEBUG("DetourTransactionBegin failed");
             return FALSE; 
          }
          TQ84_DEBUG("Calling DetourUpdateThread");

          if (  DetourUpdateThread( GetCurrentThread()) != NO_ERROR ) {
             TQ84_DEBUG("DetourUpdateThread failed");
             return FALSE; 
          }

//        DetourAttach( &(PVOID &) real_MessageBox, my_MessageBox );
//        DetourAttach(  (PVOID*) &real_MessageBox, (PVOID) my_MessageBox );
//
//
//        GetSystemTime_real                  = GetSystemTime;
//        GetLocalTime_real                   = GetLocalTime;
//        GetSystemTimePreciseAsFileTime_real = GetSystemTimePreciseAsFileTime_hook;
//        NtQuerySystemTime_real              = NtQuerySystemTime_hook;
//        GetSystemTimePreciseAsFileTime_real = GetSystemTimePreciseAsFileTime_hook;

          TQ84_DEBUG_INDENT_T("attachs");
          attach((PVOID) GetSystemTime                 , (PVOID*) &GetSystemTime_real                 , (PVOID) GetSystemTime_hook                 );
          attach((PVOID) GetLocalTime                  , (PVOID*) &GetLocalTime_real                  , (PVOID) GetLocalTime_hook                  );
          attach((PVOID) GetSystemTimeAsFileTime       , (PVOID*) &GetSystemTimeAsFileTime_real       , (PVOID) GetSystemTimeAsFileTime_hook       );
          attach((PVOID) GetSystemTimePreciseAsFileTime, (PVOID*) &GetSystemTimePreciseAsFileTime_real, (PVOID) GetSystemTimePreciseAsFileTime_hook);
          attach((PVOID) NtQuerySystemTime             , (PVOID*) &NtQuerySystemTime_real             , (PVOID) NtQuerySystemTime_hook             );
          attach((PVOID) GetTimeFormatW                , (PVOID*) &GetTimeFormatW_real                , (PVOID) GetTimeFormatW_hook                );
          attach((PVOID) GetTimeFormatA                , (PVOID*) &GetTimeFormatA_real                , (PVOID) GetTimeFormatA_hook                );
          TQ84_DEBUG_DEDENT();


//
//
//        DetourAttach( &(PVOID &)Real_Recv, Mine_Recv );
//
          TQ84_DEBUG("Calling DetourTransactionCommit");
          if ( DetourTransactionCommit() != NO_ERROR) {
             TQ84_DEBUG("DetourTransactionCommit failed");
             return FALSE; 
          }
//        TQ84_DEBUG_DEDENT();
         }
         break;
// 
      case DLL_PROCESS_DETACH: {
            TQ84_DEBUG_INDENT_T("DLL_PROCESS_DETACH");

            DetourTransactionBegin();
            DetourUpdateThread( GetCurrentThread() );

            
//          DetourDetach(  (PVOID* ) &real_MessageBox, (PVOID) my_MessageBox );
//
  // ***    DetourDetach(  (PVOID* ) &real_GetSystemTime, (PVOID) my_GetSystemTime );

//          DetourDetach( &(PVOID &)real_MessageBox, my_MessageBox );

//          DetourDetach( &(PVOID &)Real_Recv, Mine_Recv );
            DetourTransactionCommit(); 
      }
      break;
  }

    return TRUE;
}
