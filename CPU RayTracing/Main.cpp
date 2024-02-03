#include <iostream>
#include "Utility.h"

int main() {

    vec3 a = { 0,1,0 };
    vec3 b = { 0,1,0.1 };
    a.normalise();
    b.normalise();

    std::cout << a.relfect(b);

    return 0;
}