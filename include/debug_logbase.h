/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   01-10-2012 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Base class for logging
 **************************************************************/



#ifndef _DEBUGLOGBASE_H_
#define _DEBUGLOGBASE_H_

#include <wx/string.h>
#include "debug_logwindow.h"


class DebugLogBase
{
    public:
        DebugLogBase();

        void LogIt(const wxString &logstring);
        void SetTextLog(DebugLogWindow *textlog);
        DebugLogWindow *GetTextLog();

        wxString    ModuleName;

    private:
        DebugLogWindow   *TextLog;
};


#endif
