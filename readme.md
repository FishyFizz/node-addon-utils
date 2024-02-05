# node-addon-utils Documentation

---

node-addon-utils is used to enable quick rewrite of native C++ code to export them into node.js modules, by following 
relatively simple rules, without the need to understand much of underlying node api.


## 0. Module Definition

In your main source file, add following code to define a module.
```c++
    #include "NodeAddonUtils.h"
    node_start_module_definition()
        //exports goes here...
    node_end_module_definition()
```
All exported class/function needs to be registered with `node_export_class` or `node_export_function` macro in the region.


## 1. NapiValue

When interfacing with Node.js, all data should be wrapped in `napi_value` type. `NapiValue` class is provided as a wrapper.

- NapiValue` implicitly converts to C++ numeric types and `std::string`. 
- To convert C++ data type to NapiValue, `napi_env` must be passed to the constructor. 
> Code within **functions declared by macros from node-addon-utils** can use `node_value(x)` and `node_array(itbegin, itend)` 
> to convert native data types to `NapiValue`.
> 
> Unless stated, all example code are assumed to be in such context. 
- `NapiValue` object can be constructed from a container range of previously stated types, which corresponds to a javascript array.

### Examples:
```c++
    NapiValue napi_int = node_value(42);
    int i = napi_int;
    
    std::vector<float> fv = {1.1, 2.2, 3.3, 4.4};
    NapiValue napi_fv = node_array(fv.begin(), fv.end());
```

## 2. Functions

### Converting a C++ Function to an Exported Function

C++ Equivalent:
```c++
    int Function(std::string s, float f){
        //Do something with s and f ...
        
        return 0;
    }

```

To export with node-addon-utils:
```c++
    node_function(Function){
        node_arg_start();
        node_arg(std::string, s);
        node_arg(float, f);
        
        //Do something with s and f ...
        
        return node_value(0);
    }
```

> Note: `node_arg(type, var_name)` macro requires that `NapiValue` is convertible to `type`. If you have a non-basic type 
> argument passed in, use `node_arg(NapiValue, var_name)`, then convert manually later.

>Note: use `node_ret_null` for exported function without a return value.

### Exporting a Function
C++:
```c++
    node_start_module_definition()
        node_export_function(MyFunc, "ExportedMyFunc")
    node_end_module_definition()
```

Calling it in javascript:
```javascript
    module = require("your_module")
    module.ExportedMyFunc()
```

## 3. Classes

### Converting a C++ Class to an Exported Class

C++ equivalent:
```c++
    class MyClass
    {};
```

To export with node-addon-utils:
```C++
    class MyClass{
    public:
        //constructor and destructor callback
        node_destructor_callback(MyClass)
        node_default_constructor_callback(MyClass)
        
        //class export definition
        node_start_class_definition(MyClass);
            //member export definitions goes here 
        node_end_class_definition(MyClass);
    };
```

### Exported Member Function:

C++ equivalent:
```c++
    class MyClass{
        int MemberFunction(int x) {return x*2;}
    };
```

To export with node-addon-utils:
```C++
    class MyClass{
    public:
        //...
        node_member_func(MemberFunction){
            node_arg_start();
            node_arg(int, x);
            return node_value(x*2);
        }
        //...
        
        node_start_class_definition(MyClass);
            node_add_member_function(MemberFuncion, "ExportedMemberFunction")
        node_end_class_definition(MyClass);
    };
```

> To export a static member function, use `node_static_member_func` and `node_add_static_member_function` instead.

### Exported Member Variable with Default Getter and Setter

C++ equivalent:
```c++
    class MyClass{
        std::string str;
    };
```

To export with node-addon-utils:
```C++
    class MyClass{
    public:
        std::string str;
        node_member_default_getset(str);
        
        node_start_class_definition(MyClass);
            node_add_member("ExportedStr"); node_member_use_default_getset(str);
        node_end_class_definition(MyClass);
    };
```

### Custom Class Constructor

C++ equivalent:
```C++
    class MyClass{
        MyClass(int x){
            //do something
        }
    };
```

To export with node-addon-utils:
```C++
    class MyClass{
    public:
        MyClass(int x){
            //do something
        }
        
        node_constructor_callback_start(MyClass){
            node_arg(int, x);
            auto instance = new MyClass(x);
            node_constructor_callback_end(MyClass, instance);   
        }
    };
```

> Note: `node_constructor_callback_start` implies `node_arg_start`, so don't call it before `node_arg` macro.

### Custom Property with Getter and Setter

To export with node-addon-utils:
```C++
    class MyClass{
    public:
        
        node_member_func(PropertyGetter){
            //do something...
            //return property value wrapped in NapiValue
        }

        node_member_func(PropertySetter){
            node_arg_start();
            node_arg(NapiValue, value)
            //do something...
            //return property value wrapped in NapiValue
        }
    
        node_start_class_definition(MyClass);
            node_add_member("property");
                node_custom_getter(PropertyGetter);
                node_custom_setter(PropertySetter);
        node_end_class_definition(MyClass);
    };
```

> Note: For static properties, use `node_add_static_member`, `node_custom_static_getter/setter`, and `node_static_member_func`

### Class Destructor
Macro `node_destructor_callback` makes the destructor of your class get called when the corresponding javascript 
object is garbage collected. Implement a custom class destructor as in normal code and it will work.

### Exporting a Class
C++:
```c++
    node_start_module_definition()
        node_export_class(MyClass, "ExportedClass")
    node_end_module_definition()
```

Calling it in javascript:
```javascript
    module = require("your_module")
    obj = new module.ExportedClass()
```


