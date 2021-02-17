#include <Windows.h>
#include <winternl.h>

typedef NTSTATUS (WINAPI *PFN_NtQueryInformationProcess)(
    IN HANDLE               ProcessHandle,
//  IN DWORD                ProcessHandle,
    IN PROCESSINFOCLASS       ProcessInformationClass,
    IN PVOID                ProcessInformation,
    IN ULONG                ProcessInformationLength,
    _Out_opt_  PULONG ReturnLength );

int main()
{
   // Get PEB block address

   PROCESS_BASIC_INFORMATION pbi;
   ULONG result;

   PFN_NtQueryInformationProcess pfnQueryProcess = (PFN_NtQueryInformationProcess) GetProcAddress(LoadLibrary("ntdll"), "NtQueryInformationProcess");

   if (pfnQueryProcess) {
     MessageBox(0, "ok", 0, 0);
   }
   else {
     MessageBox(0, "not ok", 0, 0);
     return 1;
   }

  pfnQueryProcess(GetCurrentProcess(), ProcessBasicInformation, &pbi, sizeof(pbi), &result);

  // Modify ProcessParameters->CommandLine

  MessageBoxW(0, pbi.PebBaseAddress->ProcessParameters->CommandLine.Buffer, 0, 0);

  // This works
  pbi.PebBaseAddress->ProcessParameters->CommandLine.Buffer[0] = L'a';
  pbi.PebBaseAddress->ProcessParameters->CommandLine.Buffer[1] = L' ';
  pbi.PebBaseAddress->ProcessParameters->CommandLine.Buffer[2] = L'b';
  pbi.PebBaseAddress->ProcessParameters->CommandLine.Buffer[3] = L'\0';
  pbi.PebBaseAddress->ProcessParameters->CommandLine.Length = 6;

  // This does not work  

  UNICODE_STRING cmdLine;

  wchar_t wszNewCmdLine[] = L"x y\0";

  cmdLine.Buffer = (wchar_t*)GlobalAlloc(GMEM_FIXED, sizeof(wchar_t)*pbi.PebBaseAddress->ProcessParameters->CommandLine.MaximumLength);
  cmdLine.MaximumLength = pbi.PebBaseAddress->ProcessParameters->CommandLine.MaximumLength;
  cmdLine.Length = sizeof(wszNewCmdLine) - sizeof(L'\0');

  //Copy buffer 
  for(int i=0; i<cmdLine.Length; ++i)
      cmdLine.Buffer[i] = wszNewCmdLine[i];

  pbi.PebBaseAddress->ProcessParameters->CommandLine.Buffer = cmdLine.Buffer;
  pbi.PebBaseAddress->ProcessParameters->CommandLine.Length = cmdLine.Length;
  pbi.PebBaseAddress->ProcessParameters->CommandLine.MaximumLength = cmdLine.MaximumLength;

  // Now testing, pCmdLine returned is "a b", not "x y".
  wchar_t *pCmdLine = GetCommandLineW();
  MessageBoxW(0, pCmdLine, 0, 0);

  return 0;
}
