typedef char function_entry_point_bytes_t[6];

void HookWinAPIFunction(
           void*       fn_orig,
           void*       fn_hook,
           function_entry_point_bytes_t  orig
     );
