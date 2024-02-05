#pragma once

#include "NapiValue.h"
#include "NapiFunctionUtils.h"

#define node_class_statics(class_name) \
    const char* class_name::__export_name = "";     \
    napi_value class_name::__node_constructor_func;

#define node_constructor_callback_start(class_name)                     \
    static napi_value __node_constructor_func;                          \
    static node_function(__construct_callback_##class_name)                   \
    {                                                                         \
        node_arg_start();

#define node_constructor_callback_end(class_name, ptr_new_instance)\
        napi_wrap(node_env, __this__, ptr_new_instance, __finalize_callback_##class_name, nullptr, nullptr);\
        return __this__;\
    }

#define node_default_constructor_callback(class_name)            \
    node_constructor_callback_start(class_name)                  \
    {                                                            \
        auto instance = new class_name;                          \
        node_constructor_callback_end(class_name, instance);     \
    }

#define node_destructor_callback(class_name) \
    static void __finalize_callback_##class_name(napi_env env, void* finalize_data, void* finalize_hint)\
    {delete ((class_name*)finalize_data);}

#define node_member_func(funcname) \
    static node_function(_static_##funcname)                \
    {                                                       \
        node_arg_start();                                   \
        node_arg_this(TestClass, pthis);                    \
        return pthis->funcname(node_env, node_cbinfo);      \
    }                                                       \
    node_function(funcname)

#define node_static_member_func(funcname)                   \
    static node_function(funcname)

#define node_start_class_definition(class_name) \
    static const char* __export_name;                       \
    static void __register_class(napi_env __env__)          \
    {                                                       \
        napi_property_descriptor __pd;                      \
        std::vector<napi_property_descriptor> __pds;

#define node_end_class_definition(class_name) \
        napi_define_class(node_env, __export_name, strlen(__export_name), \
        __construct_callback_##class_name, nullptr,                      \
        __pds.size(), &__pds[0], &__node_constructor_func);\
        }

#define node_attr_member ((napi_property_attributes)((int)napi_writable | (int)napi_enumerable | (int)napi_configurable))
#define node_attr_static_member ((napi_property_attributes)((int)napi_writable |(int) napi_static| (int)napi_enumerable | (int)napi_configurable))
#define node_attr_member_func ((napi_property_attributes)((int)napi_writable | (int)napi_configurable))
#define node_attr_static_member_func ((napi_property_attributes)((int)napi_writable |(int) napi_static | (int)napi_configurable))

#define node_empty_property                      \
    (napi_property_descriptor {                  \
        nullptr,nullptr,nullptr,                \
        nullptr, nullptr, nullptr,              \
        (napi_property_attributes)0, nullptr    \
    })

#define node_add_member(export_name) \
    __pd = node_empty_property;         \
    __pd.utf8name = export_name;        \
    __pd.attributes = node_attr_member; \
    __pds.push_back(__pd);

#define node_add_static_member(export_name)    \
    __pd = node_empty_property;         \
    __pd.utf8name = export_name;              \
    __pd.attributes = node_attr_static_member; \
    __pds.push_back(__pd);

#define node_add_member_function(export_name, func_name) \
    __pd = node_empty_property;         \
    __pd.utf8name = export_name;                 \
    __pd.attributes = node_attr_member_func;   \
    __pd.method = _static_##func_name;            \
    __pds.push_back(__pd);

#define node_add_static_member_function(export_name, func_name) \
    __pd = node_empty_property;         \
    __pd.utf8name = export_name;                 \
    __pd.attributes = node_attr_static_member_func;   \
    __pd.method = func_name;            \
    __pds.push_back(__pd);

#define node_member_default_get(varname) \
    node_member_func(_get_##varname)            \
    {                                           \
        return node_value(varname);    \
    }
#define node_member_default_set(varname)        \
    node_member_func(_set_##varname)            \
    {                                           \
        node_arg_start();                       \
        node_arg(decltype(varname), __value__); \
        varname = __value__;                    \
        node_ret_null;                          \
    }
#define node_member_default_getset(varname)     \
    node_member_default_get(varname)            \
    node_member_default_set(varname)

#define node_static_default_get(varname)        \
    node_static_member_func(_get_##varname)     \
    {                                           \
        return node_value(varname);    \
    }
#define node_static_default_set(varname)        \
    node_static_member_func(_set_##varname)     \
    {                                           \
        node_arg_start();                       \
        node_arg(decltype(varname), __value__); \
        varname = __value__;                    \
        node_ret_null;                          \
    }
#define node_static_default_getset(varname) \
    node_static_default_get(varname)        \
    node_static_default_set(varname)

#define node_member_use_default_get(varname) \
    __pds[__pds.size()-1].getter = _static__get_##varname;
#define node_member_use_default_set(varname) \
    __pds[__pds.size()-1].setter = _static__set_##varname;
#define node_member_use_default_getset(varname) \
    node_member_use_default_set(varname)\
    node_member_use_default_get(varname)

#define node_static_member_use_default_get(varname) \
    __pds[__pds.size()-1].getter = _get_##varname;
#define node_static_member_use_default_set(varname) \
    __pds[__pds.size()-1].setter = _set_##varname;
#define node_static_member_use_default_getset(varname) \
    node_static_member_use_default_set(varname)\
    node_static_member_use_default_get(varname)

