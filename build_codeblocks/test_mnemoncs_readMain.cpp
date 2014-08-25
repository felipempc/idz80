/***************************************************************
 * Name:      test_mnemoncs_readMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    felipempc (idz80a@gmail.com)
 * Created:   2014-07-04
 * Copyright: felipempc (#######)
 * License:
 **************************************************************/

#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>

#include "test_mnemoncs_readMain.h"
#include "mnemonic_container.h"
#include "mnemonicxmlfile.h"


BEGIN_EVENT_TABLE(test_mnemoncs_readFrame, wxFrame)
    EVT_CLOSE(test_mnemoncs_readFrame::OnClose)
    EVT_MENU(idMenuOpen, test_mnemoncs_readFrame::OnOpen)
    EVT_MENU(idMenuQuit, test_mnemoncs_readFrame::OnQuit)
    EVT_MENU(idMenuAbout, test_mnemoncs_readFrame::OnAbout)
END_EVENT_TABLE()

test_mnemoncs_readFrame::test_mnemoncs_readFrame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title)
{
    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(_T(""));
    fileMenu->Append(idMenuOpen, "&Open\tAlt-F3", "Open a Mnemonic file");
    fileMenu->Append(idMenuQuit, "&Quit\tAlt-F4", "Quit the application");
    mbar->Append(fileMenu, "&File");

    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(idMenuAbout, _("&About\tF1"), _("Show info about this application"));
    mbar->Append(helpMenu, _("&Help"));

    SetMenuBar(mbar);

    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(2);
    SetStatusText(_("Hello Code::Blocks user!"),0);
    SetStatusText("Nothing to say", 1);

    wxBoxSizer *logwindowsizer = new wxBoxSizer(wxVERTICAL);
    logwindow_ = new wxTextCtrl(this,wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(800, 600), wxTE_MULTILINE|wxTE_RICH);
    logwindowsizer->Add(logwindow_);

}


test_mnemoncs_readFrame::~test_mnemoncs_readFrame()
{
}

void test_mnemoncs_readFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void test_mnemoncs_readFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void test_mnemoncs_readFrame::OnAbout(wxCommandEvent &event)
{
    wxString msg = "Mnemonics XML test";
    wxMessageBox(msg, _("Welcome to..."));
}



void test_mnemoncs_readFrame::OnOpen(wxCommandEvent& event)
{
    wxString filechoosen;
    wxString caption = "Choose a file";
    wxString wildcard = "XML files (*.xml)|*.xml|All files (*.*)|*.*";
    wxFileDialog dialog(this, caption, "x:", wxEmptyString, wildcard, wxFD_OPEN);

    if (dialog.ShowModal() == wxID_OK)
    {
        filechoosen = dialog.GetPath();
    }


    caption.Printf("Open file %s requested !\n", filechoosen);
    logwindow_->AppendText(caption);

    MnemonicContainer mnemonics(logwindow_);
    mnemonics.ShowStatistics();
    MnemonicXMLFile mnemonic_loader(&mnemonics, logwindow_);
    mnemonic_loader.Open(filechoosen);
    mnemonics.ShowStatistics();

}



