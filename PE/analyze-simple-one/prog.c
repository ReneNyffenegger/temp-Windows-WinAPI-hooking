#include <windows.h>

int start(void* PEB) {

    MessageBoxA(0, "...World", "Hello...", 0);
    ExitProcess(0);
}
