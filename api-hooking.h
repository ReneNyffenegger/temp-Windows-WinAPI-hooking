typedef char function_entry_point_bytes_t[6];

typedef struct {
   void*                          fn_orig;
// void*                          fn_hook;
   function_entry_point_bytes_t   function_entry_point_bytes;
} hook_t;

void HookWinAPIFunction(
           void*       fn_orig,
           void*       fn_hook,
           hook_t*     hook       
//         function_entry_point_bytes_t  orig
     );

void UnHookWinAPIFunction(hook_t hook);
