#include <cstdio>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

int main() {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    int retLoad = luaL_loadfile(L, "../test.lua");

    if (retLoad == 0) {
        printf("load file success retLoad:%d\n", retLoad);
    }

    if (retLoad || lua_pcall(L, 0, 0, 0)) {
        printf("error %s\n", lua_tostring(L, -1));
        return -1;
    }

    //调用函数,依次压入参数
    lua_getglobal(L, "add");
    lua_pushnumber(L, 10);
    lua_pushnumber(L, 20);
    //查看压入栈的元素
    for (int i=1;i<3;i++)
    {
        printf("number:%f\n",lua_tonumber(L, -i));
    }
    //lua_pcall(L,2,1,0):传入两个参数 期望得到一个返回值，0表示错误处理函数在栈中的索引值，压入结果前会弹出函数和参数
    int pcallRet = lua_pcall(L, 2, 1, 0); //lua_pcall将计算好的值压入栈顶,并返回状态值
    if (pcallRet != 0)
    {
        printf("error %s\n", lua_tostring(L, -1));
        return -1;
    }

    printf("pcallRet:%d\n", pcallRet);
    int val = lua_tonumber(L, -1); //在栈顶取出数据
    printf("val:%d\n", val);
    lua_pop(L, -1); //弹出栈顶
    //再次查看栈内元素，发现什么都没有，因为lua在返回函数计算值后会清空栈,只保留返回值
    for (int i=1;i<3;i++)
    {
        printf("number:%f\n",lua_tonumber(L, -i));
    }
    lua_close(L);
    return 0;
}
