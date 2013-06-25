/****************************************************************
 * Name:      IDZ80
 * Purpose:   Defines Application Frame
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 **************************************************************/


/*
 * Base for the modules
 *
 */
#include <wx/dynarray.h>

#ifndef _IDZ80BASE_H
#define _IDZ80BASE_H

#define MAX_OPCODE_SIZE     4   // Bytes

typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned int uint;
typedef unsigned int ProgramAddress;
typedef unsigned int LineNumber;
typedef unsigned int FileOffset;
typedef unsigned int DisassembledItem;
//typedef unsigned int DataIndex;

WX_DEFINE_SORTED_ARRAY_INT(int, SortedIntArray);

int CompareInt(int *item1, int *item2);
int CompareSortedInt(int item1, int item2);

#endif
