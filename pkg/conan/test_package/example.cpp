#include <sys_string/sys_string.h>

#include <iostream>

int main() {
    const sysstr::sys_string & str = S("Hello!");

    std::cout << str << '\n';
}
