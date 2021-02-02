#include <Windows.h>
#include <Dbghelp.h>

#include "modifyImportTable.h"


BOOL ModifyImportTable(IMAGE_IMPORT_DESCRIPTOR* iid, void* target,void* replacement) {

  IMAGE_THUNK_DATA* itd = (IMAGE_THUNK_DATA*)(((char*)GetModuleHandle(NULL)) + iid->FirstThunk);

  while (itd->u1.Function) {

    if (((void*)itd->u1.Function) == target) {

      // Temporary change access to memory area to READWRITE
      MEMORY_BASIC_INFORMATION mbi;
      VirtualQuery(itd, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
      VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_READWRITE, &mbi.Protect);

      // Replace entry!!
      *((void**)itd) = replacement;

      // Restore memory permissions
      VirtualProtect(mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &mbi.Protect);

      return TRUE;
    }

    itd += 1;
  }
  return FALSE;
}

BOOL InstallHook(LPCSTR module, LPCSTR function, void* hook, void** original) {

    HMODULE process = GetModuleHandle(NULL);

//  Save original address to function
   *original = (void*)GetProcAddress(GetModuleHandleA(module), function);
  
    ULONG entrySize;

    IMAGE_IMPORT_DESCRIPTOR* iid = (IMAGE_IMPORT_DESCRIPTOR*)ImageDirectoryEntryToData(process, 1, IMAGE_DIRECTORY_ENTRY_IMPORT, &entrySize);

//  Search for module
    while (iid->Name) {
       const char* name = ((char*)process) + iid->Name;

       if (stricmp(name, module) == 0) {
           return ModifyImportTable(iid, *original, hook);
    }

    iid += 1;
  }

  return FALSE;
}

