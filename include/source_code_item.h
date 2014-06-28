/***************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   15-01-2014 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Code line representation
 **************************************************************/


#ifndef _SOURCE_CODE_ITEM_H_
#define _SOURCE_CODE_ITEM_H_

#include <wx/gdicmn.h>
#include <wx/string.h>
#include "labelitem.h"

struct CodeViewItem
{
    wxString *Comment;
    LabelItem *LabelProgAddr;
    LabelItem *LabelVarAddr;
    int Dasmitem;
    int Org;
    wxRect *RectArg1, *RectArg2;
};

#endif // _SOURCE_CODE_ITEM_H_
