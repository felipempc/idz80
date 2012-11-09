/***************************************************************
 * Name:      logwindow.h
 * Purpose:   Disassembler for Z80 processors
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 *
 **************************************************************/


#ifndef _LOGWINDOW_H_
#define _LOGWINDOW_H_

#include <wx/frame.h>
#include <wx/textctrl.h>



class LogWindow : public wxFrame
{
    public:
        LogWindow(wxWindow *parent, const wxString &title);

        wxTextCtrl *GetTextLog();
        void Print(const wxString &logstring);

    private:
        bool        SaveOnExit;
        wxTextCtrl  *TextLog;
};



#endif // _LOGWINDOW_H_
