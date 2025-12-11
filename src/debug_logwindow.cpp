/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Log window
 **************************************************************/

#include "debug_logwindow.h"
#include <wx/font.h>
#include <wx/datetime.h>


DebugLogWindow::DebugLogWindow(wxWindow *parent, const wxString &title)
        :wxFrame(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE)
{
    TextLog = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(114,460), wxDefaultSize, wxTE_MULTILINE|wxTE_RICH);

    wxFontInfo fontInfo(8);
    fontInfo.Family(wxFONTFAMILY_SWISS)
            .Style(wxFONTSTYLE_NORMAL)
            .Weight(wxFONTWEIGHT_NORMAL)
            .FaceName("Courier New");
    wxFont PanelLogFont(fontInfo);

    TextLog->SetFont(PanelLogFont);
}


wxTextCtrl *DebugLogWindow::GetTextLog()
{
    return TextLog;
}


void DebugLogWindow::Print(const wxString &logstring)
{
    wxString str;
    wxDateTime now_time;

    if (TextLog != 0)
    {
        str = "[";
        now_time.SetToCurrent();
        str = str << now_time.FormatISOTime() << wxString::Format(".%d] ", now_time.GetMillisecond());
        TextLog->SetDefaultStyle(wxTextAttr(*wxLIGHT_GREY));
        TextLog->AppendText(str);

        str = logstring + "\n";
        TextLog->SetDefaultStyle(wxTextAttr(*wxBLACK));
        TextLog->AppendText(str);
    }
}



