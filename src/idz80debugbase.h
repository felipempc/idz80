/****************************************************************
 * Name:      IDZ80
 * Purpose:   Disassembler for Z80 MSX
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2012-10-01
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 ***
 * This is a base to all objects
 **************************************************************/




#ifndef _IDZ80DEBUGBASE_H_
#define _IDZ80DEBUGBASE_H_

#include <wx/textctrl.h>
#include <wx/string.h>

class IDZ80LogBase
{
    public:
        IDZ80LogBase();

        void SetLog(wxTextCtrl *_lg);
        void LogIt(const wxString &_str);
    protected:
        wxString    m_modulename;
        wxTextCtrl  *m_log;
};


#endif
