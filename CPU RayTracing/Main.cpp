#include <iostream>
#include "Utility.h"

int main() {

    vec3 a = { 1,1,1 };
    vec3 b = { 1,1,1 };

    std::cout << a.dot(b);
    std::cout << a.length();

    return 0;
}