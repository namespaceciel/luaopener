#include <luaopener/luaopener.h>

#include <iostream>
#include <functional>
#include <string>

int func1(int a, int b, int c) {
    return -a - b - c;
};

void func2(int a, int b) {
    std::cout << a + b << '\n';
}

std::function<int(int, int, int)> func3 = [](int a, int b, int c) {
    return a + b + c;
};

CIEL_RESISTER_LUA_FUNC(func1_binding, func1);
CIEL_RESISTER_LUA_FUNC(func2_binding, func2);
CIEL_RESISTER_LUA_FUNC(func3_binding, func3);

void test() {
    std::cout << "|-------------------------- LuaOpener Unit Test------------------------------------|" << '\n';

    ciel::LuaOpener opener;
    opener.loadFile("../test/test.lua");

    std::cout << opener["number"].asDouble() << '\n';

    std::cout << opener["table"]["name"].asString() << '\n';

    std::cout << opener["table"]["table2"]["name2"].asString() << "\n\n";

    std::cout << "测试 func1：（2, 3, \"随便打\", 4 ）" << '\n';
    opener["func1"].call(2, 3, "随便打", 4);
    std::cout << '\n';

    std::cout << "测试 func2：（2, 250, 9, 8）" << '\n';
    int result = opener["func2"].call(2, 250, 9, 8).asInt();
    std::cout << result << "\n\n";

    std::cout << "测试 cnumber1：2" << '\n';
    opener["cnumber1"] = 2;
    std::cout << opener["cnumber1"].asDouble() << "\n\n";

    std::cout << "测试 cfunc1：（1,3,5）" << '\n';
    opener["cfunc1"] = func1_binding;
    std::cout << opener["cfunc1"].call(1, 3, 5).asDouble() << "\n\n";

    std::cout << "测试 cfunc2：（2,4）" << '\n';
    opener["cfunc2"] = func2_binding;
    opener["cfunc2"].call(2, 4);
    std::cout << '\n';

    std::cout << "测试 cfunc3：（1,3,5）" << '\n';
    opener["cfunc3"] = func3_binding;
    std::cout << opener["cfunc3"].call(1, 3, 5).asDouble() << "\n\n";

    std::cout << "测试 func3：（）" << '\n';
    opener["func3"].call();

    opener.closeFile();
}

void test2() {
    std::function<void()> f = []() {
        ciel::LuaOpener opener;
        opener.loadFile("../test/test.lua");

        opener["ciel"].call();

        opener.closeFile();
    };

    f();
}

int main() {
    test2();
}