/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   26-06-2014 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Basic Types
 **************************************************************/

#ifndef IDZ80BASICTYPES_H
#define IDZ80BASICTYPES_H


#include <wx/dynarray.h>

typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned int uint;

WX_DEFINE_SORTED_ARRAY_INT(int, SortedIntArray);




#endif // IDZ80BASICTYPES_H


