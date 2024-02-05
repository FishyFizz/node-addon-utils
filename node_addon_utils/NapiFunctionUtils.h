#pragma once

// Function declaration macros & Function args retrieving macros -------------------------------------------------------

#define node_function(name) napi_value name(napi_env __env__, napi_callback_info __info__)

#define node_arg_start()                                                                        \
    size_t __argc__;                                                                            \
    std::vector<napi_value> __argv__;                                                           \
    napi_value __this__;                                                                        \
    napi_get_cb_info(__env__, __info__, &__argc__, nullptr, &__this__, nullptr);                \
    __argv__.resize(__argc__);                                                                  \
    napi_get_cb_info(__env__, __info__, &__argc__, &__argv__[0], &__this__, nullptr);           \
    size_t __curr_arg__ = 0;
#define node_arg(cpp_type, name) cpp_type name = (cpp_type)NapiValue(__env__, __argv__[__curr_arg__++])
#define node_env __env__
#define node_cbinfo __info__
#define node_arg_this(cpp_type, name)       \
    cpp_type *name;                         \
    napi_unwrap(node_env, __this__, (void**)&name)

#define node_ret_null                    \
    napi_value __null__;                 \
    napi_get_null(node_env, &__null__);   \
    return __null__;

#define node_value(x) NapiValue(node_env, (x))
#define node_array(itbegin, itend) NapiValue(node_env, (itbegin), (itend))
