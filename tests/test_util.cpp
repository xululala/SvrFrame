#include "../primo/include.h"

void test_assert()
{
    P_ASSERT2(0 == 1, "test util");
}

int main(int argc, char** argv)
{
    test_assert();
    return 0;
}