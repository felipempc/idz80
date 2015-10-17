/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Log window
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
