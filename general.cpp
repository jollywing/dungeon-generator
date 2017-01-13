#include "general.h"
#include <stdlib.h>


int rogue::Random(int n1, int n2)
{
    int big, small;
    if(n1 == n2)
        return n1;
    if(n1 > n2) {
        big = n1;
        small = n2;
    }
    else {
        big = n2;
        small = n1;
    }
    return small + rand() % (big - small + 1);
}

int rogue::Max(int n1, int n2)
{
    return n1 > n2 ? n1: n2;
}

int rogue::Min(int n1, int n2)
{
    return n1 < n2 ? n1: n2;
}
