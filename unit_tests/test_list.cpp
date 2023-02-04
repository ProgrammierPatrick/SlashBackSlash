#include <model/list.h>

#include "testing.h"


void list_access() {
    List<int> l1 = {1, 2, 3};
    List<int> l2 = {2, 2, 4};

    ASSERT_EQ(l1[1], l2[1])
    ASSERT_EQ(l1.size(), l2.size())

    ASSERT_EQ(l1[0], 1)
    ASSERT_EQ(l1[1], 2)
    ASSERT_EQ(l1[2], 3)
    ASSERT_EQ(l2[0], 2)
    ASSERT_EQ(l2[1], 2)
    ASSERT_EQ(l2[2], 4)

    l2.push_back(0);
    ASSERT_EQ(l1.size() + 1, l2.size())
}

int main() {
    list_access();

    return GET_ASSERT_RETURN;
}
