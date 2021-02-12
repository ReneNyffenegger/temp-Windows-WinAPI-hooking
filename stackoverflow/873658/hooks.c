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
          real_GetSystemTime = GetSystemTime;
          TQ84_DEBUG("Calling DetourAttach");
          LONG dar;
          if ( (dar = DetourAttach(  (PVOID*) &real_GetSystemTime, (PVOID) my_GetSystemTime )) != NO_ERROR ) {
             TQ84_DEBUG("DetourAttach failed");
             if      (dar == ERROR_INVALID_BLOCK      ) { TQ84_DEBUG("ERROR_INVALID_BLOCK"    ); }
             else if (dar == ERROR_INVALID_HANDLE     ) { TQ84_DEBUG("ERROR_INVALID_HANDLE"   ); }
             else if (dar == ERROR_INVALID_OPERATION  ) { TQ84_DEBUG("ERROR_INVALID_OPERATION"); }
             else if (dar == ERROR_NOT_ENOUGH_MEMORY  ) { TQ84_DEBUG("ERROR_NOT_ENOUGH_MEMORY"); }
             return FALSE; 
          }
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
            DetourTransactionBegin();
            DetourUpdateThread( GetCurrentThread() );
            
//          DetourDetach(  (PVOID* ) &real_MessageBox, (PVOID) my_MessageBox );
            DetourDetach(  (PVOID* ) &real_GetSystemTime, (PVOID) my_GetSystemTime );
//          DetourDetach( &(PVOID &)real_MessageBox, my_MessageBox );

//          DetourDetach( &(PVOID &)Real_Recv, Mine_Recv );
            DetourTransactionCommit(); 
      }
      break;
  }

    return TRUE;
}
