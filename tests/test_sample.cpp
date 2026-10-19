#include <iostream>
#include <cassert>

void testSample() {
    assert(1 + 1 == 2);
    std::cout << "Test passed!" << std::endl;
}

int main() {
    testSample();
    return 0;
}
