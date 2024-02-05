#pragma once

#include "node_api.h"
#include "js_native_api.h"

#include <cassert>
#include <cstdint>
#include <vector>
#include <string>

class NapiValue
{
protected:
    napi_value value;
    napi_env env;

public:

    //Implicit conversion to napi_value
    operator napi_value()
    {return value;}

    //Construction from napi_value
    NapiValue(napi_env _env, napi_value val)
    {
        env = _env;
        value = val;
    }

    //Construction from string
    NapiValue(napi_env _env, const std::string& str)
    {
        *this = NapiValue(_env, str.c_str());
    }
    NapiValue(napi_env _env, const char* str)
    {
        napi_create_string_utf8(_env, str, strlen(str), &value);
        env = _env;
    }

    //Construction from numerics
#define constr_conversion(cpp_type, napi_type)                      \
    NapiValue(napi_env _env, cpp_type val)                          \
    {napi_create_##napi_type(env, val, &value); env = _env;}

    constr_conversion(bool, int64)
    constr_conversion(uint8_t, int64)
    constr_conversion(int8_t, int64)
    constr_conversion(uint16_t, int64)
    constr_conversion(int16_t, int64)
    constr_conversion(uint32_t, int64)
    constr_conversion(int32_t, int64)
    constr_conversion(uint64_t, int64) //WARNING: This may cause overflow...
    constr_conversion(int64_t, int64)
    constr_conversion(float, double)
    constr_conversion(double, double)

    //Conversion to numerics
#define numeric_conv(cpp_type, napi_type, intermediate_type)    \
    operator cpp_type()                                         \
    {                                                           \
        intermediate_type result;                               \
        napi_get_value_##napi_type(env, value, &result);        \
        return result;                                          \
    }

    numeric_conv(bool, int64, int64_t)
    numeric_conv(uint8_t, int64, int64_t)
    numeric_conv(int8_t, int64, int64_t)
    numeric_conv(uint16_t, int64, int64_t)
    numeric_conv(int16_t, int64, int64_t)
    numeric_conv(uint32_t, int64, int64_t)
    numeric_conv(int32_t, int64, int64_t)
    numeric_conv(uint64_t, int64, int64_t)
    numeric_conv(int64_t, int64, int64_t)
    numeric_conv(float, double, double)
    numeric_conv(double, double, double)

    //Conversion to string
    operator std::string()
    {
        std::string result;
        size_t len;
        napi_get_value_string_utf8(env, value, nullptr, 1024, &len);
        result.resize(len);
        napi_get_value_string_utf8(env, value, &result[0], len, &len);
        return result;
    }

    //Implicit construction from containers
    template <class itTy>
    NapiValue(napi_env _env, itTy begin, itTy end)
    {
        assert(napi_create_array(env, &value) == napi_ok);
        size_t i = 0;
        for(itTy ite = begin; ite != end; ite ++, i++)
            napi_set_element(env, value, i, NapiValue(env, *ite));
        env = _env;
    }

    //Implicit conversion to vector
    template <class T>
    operator std::vector<T>()
    {
        std::vector<T> result;
        if(!IsArray())
            return result;

        uint32_t len;
        result.resize(napi_get_array_length(env, value, &len));

        napi_value element;
        for(size_t i=0; i<len ; i++)
        {
            napi_get_element(env, value, 0, &element);
            result[i] = (T)NapiValue(env, element);
        }
        return result;
    }


    //Utilities
    bool IsArray()
    {
        bool is_array;
        napi_is_array(env, value, &is_array);
        return is_array;
    }

    void test(napi_env env)
    {
        std::vector<std::string> vec = {"1","2","3","4","5"};
        auto arr = NapiValue(env, vec.begin(), vec.end());
        vec = arr;
    }

    template <class NativeClassType>
    NativeClassType* Unwrap()
    {
        NativeClassType* ptr;
        napi_unwrap(env, value, &ptr);
        return ptr;
    }
};
