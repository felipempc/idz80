/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   24-09-2012 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Basic routines
 **************************************************************/

#include "idz80_base.h"


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


