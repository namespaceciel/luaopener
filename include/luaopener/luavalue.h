#ifndef LUAOPENER_INCLUDE_LUA_LUAVALUE_H_
#define LUAOPENER_INCLUDE_LUA_LUAVALUE_H_

#include <iostream>
#include <map>
#include <memory>
#include <string>

extern "C" {

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

}   // extern "C"

namespace ciel {

class LuaValue {
private:
    std::string name_;
    std::shared_ptr<std::map<std::string, LuaValue>> child_map_;
    lua_State* state_;
    int index_;

public:
    LuaValue(lua_State* r_state, int index, const std::string& name);

    LuaValue(const LuaValue&) = default;

    LuaValue(LuaValue&&) noexcept = default;

    ~LuaValue();

    int getIndex() const noexcept;

    int asInt() noexcept;

    double asDouble() noexcept;

    std::string asString();

    // 函数调用
    template<class... Args>
    LuaValue call(Args... args) {
        // 先压入函数
        lua_pushvalue(state_, index_);

        // 再压入参数
        int arr[] = {(0),(pushArg(args),0)...};

        if (lua_pcall(state_, sizeof...(args), 1, 0) != 0) {
            std::cerr << ("lua_pcall failed: %s\n", lua_tostring(state_, -1));
            return LuaValue(state_, 0, "_Error");
        }

        int index = lua_gettop(state_);
        this->createMap();
        return child_map_->emplace("_FuncCall", LuaValue(state_, index, "_FuncCall")).first->second;
    }

    // 获取 Lua 值
    LuaValue operator[](const std::string& key);

    // 注册 C 函数
    template<class T, class ...Args>
    LuaValue& operator=(T func(Args... args)) {
        lua_register(state_, name_.c_str(), func);
        index_ = lua_getglobal(state_, name_.c_str());
        return *this;
    }

    // 注册 C 变量
    template<class T>
    LuaValue operator=(T&& value) {
        pushArg(value);
        lua_setglobal(state_, name_.c_str());
        return *this;
    }

    void clear();

private:
    void createMap();

    void clearMap();

    void pushArg(int arg);

    void pushArg(double arg);

    void pushArg(const std::string& arg);

    void pushArg(const char* arg);

};  // class LuaValue

}   // namespace ciel

#endif // LUAOPENER_INCLUDE_LUA_LUAVALUE_H_