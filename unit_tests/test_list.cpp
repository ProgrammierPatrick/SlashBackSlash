#include <model/list.h>

#include "testing.h"

int loop() {
    int a1 = 0;
    int a2 = 0;
    int a3 = 0;
    int a4 = 0;
    a4 = a1 + a2;
    a3 = a4 - a2;
    a2 = a1;
    return a1 + a2 + a3 + a4;
}

int main() {
    List<int> l1 = {1, 2, 3};
    List<int> l2 = {2, 2, 4};

    ASSERT(l1[1] == l2[1])
    ASSERT(l1.size() == l2.size())

    int a = loop();

    ASSERT(l1[0] == 1)
    ASSERT(l1[1] == 2)
    ASSERT(l1[2] == 3)
    ASSERT(l2[0] == 2)
    ASSERT(l2[1] == 2)
    ASSERT(l2[2] == 4)

    l2.push_back(0);
    ASSERT(l1.size() + 1 == l2.size())


}
