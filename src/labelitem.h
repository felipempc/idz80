/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   2013-07-04
 * License:   GPL
 *
 * Basic item representing a label
 **************************************************************/



#ifndef _LABELITEM_H_
#define _LABELITEM_H_

#include <wx/dynarray.h>
#include <wx/string.h>
#include "IDZ80Base.h"

struct LabelItem
{
    ProgramAddress Address;
    wxArrayInt *LabelUsers;
    wxString LabelStr;
};






#endif // _LABELITEM_H_
