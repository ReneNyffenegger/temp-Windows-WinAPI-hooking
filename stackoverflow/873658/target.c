#include <stdio.h>
#include <windows.h>

int main() {

   printf("Press enter when dll is injected\n");
   int c = getc(stdin);

   SYSTEMTIME t;

   GetSystemTime(&t);

   char buf[100];
   sprintf(buf, "%d-%02d-%02d %02d:%02d:%02d.%d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);

   MessageBox(0, buf, "System time", 0);

   // printf("target is going to call message box\n");
   // MessageBox(0, "First time", "target", 0);

   // printf("target is again going to call message box\n");
   // MessageBox(0, "second time", "target", 0);

}
