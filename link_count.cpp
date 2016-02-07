//
// Created by Rutvik Choudhary on 12/9/15.
//

#include "link_count.h"

int& LINK_COUNT()
{
    static int count = 0;
    return count;
}
