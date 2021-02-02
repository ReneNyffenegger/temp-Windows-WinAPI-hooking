typedef char api_hook_t[6];

void HookWinAPIFunction(
           void*       fn_orig,
           void*       fn_hook,
           api_hook_t  orig
     );
