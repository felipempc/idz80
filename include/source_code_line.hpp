/***************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   15-01-2014 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Code line representation
 **************************************************************/


#ifndef _SOURCE_CODE_LINE_HPP_
#define _SOURCE_CODE_LINE_HPP_

#include <wx/gdicmn.h>
#include <wx/string.h>
#include "labelitem.hpp"

/// @brief Represents a line of source code
struct SourceCodeLine
{
    wxString *comment;
    LabelItem *labelProgAddress;
    LabelItem *labelVarAddress;
    DisassembledIndex dasmedItem;
    int originAddress;
    wxRect *rectArg1, *rectArg2;
};

#endif // _SOURCE_CODE_LINE_HPP_
