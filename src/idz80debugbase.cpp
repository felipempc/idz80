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

#include "idz80debugbase.h"


IDZ80LogBase::IDZ80LogBase()
{
    m_log = 0;
    m_modulename = "BASE:";
}


void IDZ80LogBase::SetLog(wxTextCtrl *_lg)
{
    m_log = _lg;
}


void IDZ80LogBase::LogIt(const wxString &_str)
{
    if (m_log != 0)
        m_log->AppendText(m_modulename + _str);
}
