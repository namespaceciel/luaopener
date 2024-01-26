#ifndef LUAOPENER_INCLUDE_LUA_LUAREGISTER_H_
#define LUAOPENER_INCLUDE_LUA_LUAREGISTER_H_

#include <luaopener/luavalue.h>

#include <functional>
#include <string>

namespace ciel {

template<class T>
inline T getArgu(lua_State* l, int index) {
    static_assert(false, "Invalid Arg Type");
}

template<>
inline int getArgu<int>(lua_State* l, int index) {
    int num = static_cast<int>(lua_tonumber(l, index));
    return num;
}

template<>
inline double getArgu<double>(lua_State* l, int index) {
    double num = lua_tonumber(l, index);
    return num;
}

template<>
inline std::string getArgu<std::string>(lua_State* l, int index) {
    auto str = lua_tostring(l, index);
    return str;
}

template<class T, class... Args>
std::function<int(lua_State*)> packAsLuaFunc(T f(Args...)) {
    return [f](lua_State* l) {
        int index = -1;
        T v = f(getArgu<Args>(l, index--)...);
        lua_pushnumber(l, v);
        return 1;
    };
}

template<class... Args>
std::function<int(lua_State*)> packAsLuaFunc(void f(Args...)) {
    return [f](lua_State* l) {
        int index = -1;
        f(getArgu<Args>(l, index--)...);
        return 0;
    };
}

template<class T, class... Args>
std::function<int(lua_State*)> packAsLuaFunc(std::function<T(Args...)>& f) {
    return [&f](lua_State* l) {
        int index = -1;
        T v = f(getArgu<Args>(l, index--)...);
        lua_pushnumber(l, v);
        return 1;
    };
}

template<class... Args>
std::function<int(lua_State*)> packAsLuaFunc(std::function<void(Args...)>& f) {
    return [&f](lua_State* l) {
        int index = -1;
        f(getArgu<Args>(l, index--)...);
        return 0;
    };
}

}   // namespace ciel

// 通过宏定义先包装成静态 C 函数。
#define CIEL_RESISTER_LUA_FUNC(str,func)  \
static int str(lua_State* s) {      \
	return ciel::packAsLuaFunc(func)(s); \
}

#endif // LUAOPENER_INCLUDE_LUA_LUAREGISTER_H_