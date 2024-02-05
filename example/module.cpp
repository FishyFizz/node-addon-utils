//
// Created by Fizz on 2/4/2024.
//

#include "NodeAddonUtils.h"
#include "TestClass.h"

std::string global_str;
node_function(set_global_str)
{
    node_arg_start();
    node_arg(std::string, val);
    global_str = val;
    return node_value(global_str);
}

node_start_module_definition()
    node_export_class(TestClass, "TestClass");
    node_export_function(set_global_str, "SetGlobalStr");
node_end_module_definition()

