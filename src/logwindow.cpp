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
#include "wx/textctrl.h"
#include "wx/font.h"


LogWindow::LogWindow(wxWindow *parent, const wxString &title)
        :wxFrame(parent, wxID_ANY, title)
{
    TextLog = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(114,460), wxDefaultSize, wxTE_MULTILINE|wxTE_RICH);
    SetupFont();
}

void LogWindow::LogIt(const wxString &logstring)
{
    if (TextLog != 0)
        TextLog->AppendText(logstring);
}


void LogWindow::SetupFont()
{
    if (TextLog != 0)
    {
        wxFont PanelLogFont(8, wxSWISS, wxFONTSTYLE_NORMAL, wxNORMAL, false, "Courier New", wxFONTENCODING_DEFAULT);
        TextLog->SetFont(PanelLogFont);
    }
}

