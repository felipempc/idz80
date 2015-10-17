/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   24-09-2012 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Basic routines
 **************************************************************/


/*
 * Base for the modules
 *
 */
#include <wx/dynarray.h>

#ifndef _IDZ80BASE_H
#define _IDZ80BASE_H

#define MAX_OPCODE_SIZE     4   // Bytes

/* **** in idz80_basic_types.h
typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned int uint;

WX_DEFINE_SORTED_ARRAY_INT(int, SortedIntArray);
*/


//typedef unsigned int ProgramAddress; // rawdata
//typedef unsigned int LineNumber;
//typedef unsigned int FileOffset; //rawdata
//typedef unsigned int DisassembledIndex;
typedef unsigned int LabelIndex;
//typedef unsigned int DataIndex;


int CompareInt(int *item1, int *item2);
int CompareSortedInt(int item1, int item2);

#endif
