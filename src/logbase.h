/****************************************************************
 * Name:      logbase
 * Purpose:   Disassembler for Z80 MSX
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2012-11-08
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 ***
 * Base for logging
 **************************************************************/



#ifndef _LOGBASE_H_
#define _LOGBASE_H_

#include <wx/string.h>
#include "logwindow.h"


class LogBase
{
    public:
        LogBase();

        void LogIt(const wxString &logstring);
        void SetTextLog(LogWindow *textlog);

        wxString    ModuleName;

    private:
        LogWindow   *TextLog;
};


#endif
