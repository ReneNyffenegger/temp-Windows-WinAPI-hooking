typedef char function_entry_point_bytes_t[6];

typedef struct {
    void*                          fn_orig;
    HANDLE                         process;
    function_entry_point_bytes_t   orig_entry_bytes;
} hook_t;

void HookWinAPIFunction(
           void*       fn_orig,
           void*       fn_hook,
           HANDLE      process,
           hook_t*     hook       
     );

void UnHookWinAPIFunction(
           hook_t hook
    );
