#ifndef LUAOPENER_INCLUDE_LUA_LUAOPENER_H_
#define LUAOPENER_INCLUDE_LUA_LUAOPENER_H_

#include <luaopener/luaregister.h>
#include <luaopener/luavalue.h>

#include <map>
#include <string>

namespace ciel {

class LuaOpener {
public:
    lua_State* state_;
    std::map<std::string, LuaValue> state_map_;

public:
    LuaOpener() noexcept;

    ~LuaOpener();

    bool loadFile(const std::string& filename) noexcept;

    void closeFile() noexcept;

    LuaValue operator[](const std::string& key) noexcept;

private:
    void clear() noexcept;

};  // class LuaOpener

}   // namespace ciel

#endif // LUAOPENER_INCLUDE_LUA_LUAOPENER_H_