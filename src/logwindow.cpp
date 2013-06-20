/***************************************************************
 * Name:      logwindow.cpp
 * Purpose:   Disassembler for Z80 processors
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 *
 **************************************************************/

#include "logwindow.h"
#include <wx/font.h>
#include <wx/datetime.h>


LogWindow::LogWindow(wxWindow *parent, const wxString &title)
        :wxFrame(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN)
{
    TextLog = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(114,460), wxDefaultSize, wxTE_MULTILINE|wxTE_RICH);
    wxFont PanelLogFont(8, wxSWISS, wxFONTSTYLE_NORMAL, wxNORMAL, false, "Courier New", wxFONTENCODING_DEFAULT);
    TextLog->SetFont(PanelLogFont);
}


wxTextCtrl *LogWindow::GetTextLog()
{
    return TextLog;
}


void LogWindow::Print(const wxString &logstring)
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


