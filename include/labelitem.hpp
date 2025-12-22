/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   04-07-2012 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Main module
 **************************************************************/


#ifndef _LABELITEM_H_
#define _LABELITEM_H_

#include <wx/string.h>
#include "rawdata.hpp"

struct LabelItem
{
    AbsoluteAddress address;
    IndexVector *labelUsers;
    wxString labelString;
};
#endif // _LABELITEM_H_
