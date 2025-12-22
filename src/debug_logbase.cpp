/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   01-10-2012 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Base class for logging
 **************************************************************/

#include "debug_logbase.hpp"


DebugLogBase::DebugLogBase()
{
    ModuleName = "BASE:";
    TextLog = 0;
}

void DebugLogBase::SetTextLog(DebugLogWindow *textlog)
{
    TextLog = textlog;
}

DebugLogWindow *DebugLogBase::GetTextLog()
{
    return TextLog;
}

void DebugLogBase::LogIt(const wxString &logstring)
{
    if (TextLog)
    {
        wxString str = ModuleName + ": " + logstring;
        TextLog->Print(str);
    }
}
