#include <iostream>

extern void day1();
extern void day2();
extern void day3();
extern void day4();

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    day1();
    day2();
    day3();
    day4();

    return 0;
}
