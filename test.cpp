#include "common.h"

int main(){
    char s[4096], d[4096];
    auto t1 = now();
    memcpy(d, s, 4096);
    auto t2 = now();
    std::cout << t2 - t1 << std::endl;
}