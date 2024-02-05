#pragma once

#define node_start_module_definition() \
    NAPI_MODULE_INIT(){
#define node_end_module_definition()\
    return exports;}
#define node_export_class(class_name, export_name) \
    class_name::__export_name = export_name;\
    class_name::__register_class(env);\
    napi_set_named_property(env, exports, export_name, class_name::__node_constructor_func);

#define node_export_function(function_name, export_name)\
    napi_value __export_##function_name;\
    napi_create_function(env, export_name, NAPI_AUTO_LENGTH, function_name, nullptr, &__export_##function_name);\
    napi_set_named_property(env, exports, export_name, __export_##function_name);