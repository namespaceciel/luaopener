#include <luaopener/luaopener.h>

#include <iostream>

namespace ciel {

LuaOpener::LuaOpener() noexcept : state_(nullptr){}

LuaOpener::~LuaOpener(){
    closeFile();
}

bool LuaOpener::loadFile(const std::string& filename) noexcept {
    // 先确保关闭状态
    closeFile();

    // 开启新状态
    state_ = luaL_newstate();
    luaL_openlibs(state_);

    // 加载脚本并运行
    if (luaL_loadfile(state_, filename.c_str()) || lua_pcall(state_, 0, 0, 0)) {
        std::clog << "load Lua script failed: " << lua_tostring(state_, -1) ;
        return false;
    }

    return true;
}

void LuaOpener::closeFile() noexcept {
    if (state_) {
        clear();
        lua_close(state_);
        state_ = nullptr;
    }
}

LuaValue LuaOpener::operator[](const std::string& key) noexcept {
    auto itr = state_map_.find(key);

    if (itr != state_map_.end()) {     // 若找到，返还该值在虚拟栈的索引
        return LuaValue(state_, itr->second.getIndex(), key);

    } else {    // 若找不到，则在lua文件读取值放入虚拟栈，并获取索引
        int ret = lua_getglobal(state_, key.c_str());

        if (ret != 0) {      // ret != 0时，lua文件中存在该值
            int index = lua_gettop(state_);
            state_map_.insert(std::make_pair(key, LuaValue(state_, index, key)));
            return LuaValue(state_, index, key);
        }
    }

    return LuaValue(state_, 0, key);
}

void LuaOpener::clear() noexcept {
    state_map_.clear();
}

}   // namespace ciel