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

void list_append_unique() {
    List<int> l1 = {1, 2, 3};
    List<int> l2 = {4, 2, 3};
    List<int> shared = {99, 99};
    l1.append_back(shared);
    l2.append_back(shared);

    ASSERT_EQ(l1.size(), 5)
    ASSERT_EQ(l2.size(), 5)

    List<int> res = List<int>::append_unique(l1, l2);

    ASSERT_EQ(res[0], 1)
    ASSERT_EQ(res[1], 2)
    ASSERT_EQ(res[2], 3)
    ASSERT_EQ(res[3], 99)
    ASSERT_EQ(res[4], 99)
    ASSERT_EQ(res[5], 4)
    ASSERT_EQ(res[6], 2)
    ASSERT_EQ(res[7], 3)
    ASSERT_EQ(res.size(), 8)
}

int main() {
    list_access();
    list_append_unique();

    return GET_ASSERT_RETURN;
}
