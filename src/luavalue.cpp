#include <luaopener/luavalue.h>

namespace ciel {

LuaValue::LuaValue(lua_State* state, int index, const std::string& name)
    : name_(name), state_(state), index_(index) {}

LuaValue::~LuaValue() {
    clear();
}

int LuaValue::getIndex() const noexcept {
    return index_;
}

int LuaValue::asInt() noexcept {
    return static_cast<int>(lua_tointeger(state_, index_));
}

double LuaValue::asDouble() noexcept {
    return lua_tonumber(state_, index_);
}

std::string LuaValue::asString() {
    return lua_tostring(state_, index_);
}

LuaValue LuaValue::operator[](const std::string& key) {
    if (child_map_) {
        auto itr = child_map_->find(key);

        // 若找到，返还该值在虚拟栈的索引
        if (itr != child_map_->end() ) {
            return LuaValue(state_, itr->second.index_, key);
        }
    }

    // 若找不到，则在 lua 文件读取值放入虚拟栈，并获取索引
    int ret = lua_getfield(state_, index_ , key.c_str());

    if (ret != 0) {     // ret != 0时，lua文件中存在该值
        int index = lua_gettop(state_);
        createMap();
        child_map_->insert(std::make_pair(key, LuaValue(state_, index, key)));
        return LuaValue(state_, index, key);
    }
    return LuaValue(state_, 0, key);
}

void LuaValue::pushArg(int arg) {
    lua_pushnumber(state_, static_cast<double>(arg));
}

void LuaValue::pushArg(double arg) {
    lua_pushnumber(state_, arg);
}

void LuaValue::pushArg(const std::string& arg) {
    lua_pushstring(state_, arg.c_str());
}

void LuaValue::pushArg(const char* arg) {
    lua_pushstring(state_, arg);
}

void LuaValue::clear(){
    index_ = 0;
    clearMap();
}

void LuaValue::createMap() {
    if (child_map_ == nullptr) {
        child_map_ = std::make_shared<std::map<std::string, LuaValue>>();
    }
}

void LuaValue::clearMap() {
    child_map_ = nullptr;
}

}   // namespace ciel