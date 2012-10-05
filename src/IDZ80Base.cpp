/****************************************************************
 * Name:      IDZ80
 * Purpose:   Shared Tools
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   September 24 of 2012
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 **************************************************************/



#include "IDZ80Base.h"


int CompareInt(int *item1, int *item2)
{
    if (*item1 < *item2)
        return -1;

    if (*item1 > *item2)
        return 1;
    return 0;
}



int CompareSortedInt(int item1, int item2)
{
    if (item1 < item2)
        return -1;

    if (item1 > item2)
        return 1;
    return 0;
}


