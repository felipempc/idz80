/****************************************************************
 * Name:      logbase
 * Purpose:   Disassembler for Z80 MSX
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2012-10-01
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 ***
 * Base for logging
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
