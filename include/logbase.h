/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   01-10-2012 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Base class for logging
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
