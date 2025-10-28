// #include <iostream>
// #include <unistd.h>
// #include <cstring>

// void my_print(const char* str) {
//     write(STDOUT_FILENO, str, strlen(str));
// }

// int main(int argc, char const *argv[])
// {
//     my_print("666\n");
//     printf("Hello!\n");

//     return 0;
// }

#include <chrono>
#include <iostream>
 #include <unistd.h>
#include <cstring>

void my_print(const char* str) {
    write(STDOUT_FILENO, str, strlen(str));
}

template<typename F>
auto timeit(F&& func) {
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100; i++)
    {
        func();
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "耗时: " 
              << std::chrono::duration<double, std::milli>(t2-t1).count()/100
              << " ms\n";
    return std::chrono::duration<double, std::milli>(t2-t1).count()/100;
}

// 用法
int main() {
    // auto time1 = timeit([]() {
    //     // 你的代码
    //     const char* str = "666";
    //     write(STDOUT_FILENO, str, strlen(str));
    //     return 42;
    // });
    // auto time2 = timeit([]() {
    //     // 你的代码
    //     std::cout << "666" << std::endl;
    //     return 42;
    // });

    // std::cout << time1 << "|" << time2;
    my_print("666");
    return 0;
}