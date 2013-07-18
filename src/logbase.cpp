/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   01-10-2012 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Base class for logging
 **************************************************************/

#include "logbase.h"


LogBase::LogBase()
{
    ModuleName = "BASE:";
    TextLog = 0;
}

void LogBase::SetTextLog(LogWindow *textlog)
{
    TextLog = textlog;
}

void LogBase::LogIt(const wxString &logstring)
{
    if (TextLog != 0)
    {
        wxString str = ModuleName + ": " + logstring;
        TextLog->Print(str);
    }
}
