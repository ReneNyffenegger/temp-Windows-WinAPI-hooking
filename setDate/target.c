#include <stdio.h>
#include <windows.h>

int main() {

// printf("Press enter when dll is injected\n");
// int c = getc(stdin);

   SYSTEMTIME t;
   printf("Calling GetSystemTime\n");
   GetSystemTime(&t);
   printf("returned\n");

// char buf[100];
   printf("Time is: %d-%02d-%02d %02d:%02d:%02d.%d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
// MessageBox(0, buf, "System time", 0);

}
