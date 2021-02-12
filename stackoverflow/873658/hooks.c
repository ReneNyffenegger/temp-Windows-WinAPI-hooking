#include <windows.h>
#include <stdio.h>
#include "detours.h"

#include "tq84_debug.h"

// #include "tq84_debug.h"

// #pragma comment( lib, "Ws2_32.lib" )
// #pragma comment( lib, "detours.lib" )
// #pragma comment( lib, "detoured.lib" )
//

// int (WINAPI *real_MessageBox)( HWND    hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT    uType);
void (WINAPI *real_GetSystemTime)(LPSYSTEMTIME  );
void WINAPI     my_GetSystemTime (LPSYSTEMTIME t) {

   real_GetSystemTime(t);
   t -> wYear   = 1970;
   t -> wMonth  =    8;
   t -> wDay    =   28;
   t -> wHour   =   22;
   t -> wMinute =   22;
   t -> wSecond =   23;

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

BOOL WINAPI DllMain(HINSTANCE i, DWORD dwReason, LPVOID l) {

  switch ( dwReason ) {

       case DLL_PROCESS_ATTACH: {
          TQ84_DEBUG_OPEN("hook.debug.out", "w");
//        tq84_debug_open("hook.debug.out", "w");
          TQ84_DEBUG("Calling DetourTransactionBegin");
          TQ84_DEBUG_INDENT_T("DLL_PROCESS_ATTACH");
          DetourTransactionBegin();
          TQ84_DEBUG("Calling DetourUpdateThread");
          DetourUpdateThread( GetCurrentThread() );

//        DetourAttach( &(PVOID &) real_MessageBox, my_MessageBox );
//        DetourAttach(  (PVOID*) &real_MessageBox, (PVOID) my_MessageBox );
//
          TQ84_DEBUG("Calling DetourAttach");
          DetourAttach(  (PVOID*) &real_GetSystemTime, (PVOID) my_GetSystemTime );
//
//        DetourAttach( &(PVOID &)Real_Recv, Mine_Recv );
          TQ84_DEBUG("Calling DetourTransactionCommit");
          DetourTransactionCommit();
//        TQ84_DEBUG_DEDENT();
         }
         break;
// 
      case DLL_PROCESS_DETACH:
            DetourTransactionBegin();
            DetourUpdateThread( GetCurrentThread() );
            
//          DetourDetach(  (PVOID* ) &real_MessageBox, (PVOID) my_MessageBox );
            DetourDetach(  (PVOID* ) &real_GetSystemTime, (PVOID) my_GetSystemTime );
//          DetourDetach( &(PVOID &)real_MessageBox, my_MessageBox );

//          DetourDetach( &(PVOID &)Real_Recv, Mine_Recv );
            DetourTransactionCommit(); 
      break;
  }

    return TRUE;
}
