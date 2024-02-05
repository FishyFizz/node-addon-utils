//
// Created by Fizz on 2/5/2024.
//

#ifndef NODE_ADDON_UTILS_TESTCLASS_H
#define NODE_ADDON_UTILS_TESTCLASS_H

#include "NodeAddonUtils.h"

class TestClass {
public:
    node_destructor_callback(TestClass)
    //node_default_constructor_callback(TestClass)

    node_constructor_callback_start(TestClass)
    {
        node_arg(int, x);
        node_arg_this(TestClass, pthis);
        TestClass* instance = new TestClass;
        node_constructor_callback_end(TestClass, instance);
    }

    std::string str;
    node_member_default_getset(str);

    static std::string str_static;
    node_static_default_getset(str_static);

    node_member_func(SetStr) {
        node_arg_start();
        node_arg(std::string, val);
        str = val;
        return node_value(str);
    }

    node_static_member_func(SetStaticStr) {
        node_arg_start();
        node_arg(std::string, val);
        str_static = val;
        return node_value(str_static);
    }

    node_start_class_definition(TestClass);
        node_add_member("str"); node_member_use_default_getset(str);
        node_add_static_member("str_static"); node_static_member_use_default_getset(str_static);
        node_add_member_function("SetStr", SetStr);
        node_add_static_member_function("SetStaticStr", SetStaticStr);
    node_end_class_definition(TestClass);
};

#endif //NODE_ADDON_UTILS_TESTCLASS_H
