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

#include "mnemonicxmlfile.h"
#include "dialog_mnemonics.h"


BEGIN_EVENT_TABLE(test_mnemoncs_readFrame, wxFrame)
    EVT_CLOSE(test_mnemoncs_readFrame::OnClose)
    EVT_MENU(idMenuOpen, test_mnemoncs_readFrame::OnOpen)
    EVT_MENU(idMenuQuit, test_mnemoncs_readFrame::OnQuit)
    EVT_MENU(idMenuAbout, test_mnemoncs_readFrame::OnAbout)
    EVT_MENU(idMenuViewMnemonics, test_mnemoncs_readFrame::OnViewMnemonics)
END_EVENT_TABLE()




test_mnemoncs_readFrame::test_mnemoncs_readFrame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title)
{
    // create a menu bar
    mainwindow_menubar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu("");
    fileMenu->Append(idMenuOpen, "&Open\tAlt-F3", "Open a Mnemonic file");
    fileMenu->Append(idMenuQuit, "&Quit\tAlt-F4", "Quit the application");
    mainwindow_menubar->Append(fileMenu, "&File");

    wxMenu* viewmenu = new wxMenu("");
    viewmenu->Append(idMenuViewMnemonics, "&Mnemonics\tAlt-m", "View mnemonics");
    mainwindow_menubar->Append(viewmenu, "&View");

    wxMenu* helpMenu = new wxMenu("");
    helpMenu->Append(idMenuAbout, "&About\tF1", "Show info about this application");
    mainwindow_menubar->Append(helpMenu, "&Help");

    SetMenuBar(mainwindow_menubar);

    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(2);
    SetStatusText(_("Hello Code::Blocks user!"),0);
    SetStatusText("Nothing to say", 1);

    wxBoxSizer *logwindowsizer = new wxBoxSizer(wxVERTICAL);
    logwindow_ = new wxTextCtrl(this,wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(800, 600), wxTE_MULTILINE|wxTE_RICH);
    logwindowsizer->Add(logwindow_);
    mnemonics_ = new MnemonicContainer(logwindow_);

    mainwindow_menubar->Enable(idMenuViewMnemonics, false);
}


test_mnemoncs_readFrame::~test_mnemoncs_readFrame()
{
    delete mnemonics_;
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

    if (dialog.ShowModal() != wxID_OK)
        return;

    filechoosen = dialog.GetPath();

    mnemonics_->Clear();

    caption.Printf("Reading mnemonic file %s...\n", filechoosen);
    logwindow_->AppendText(caption);
    logwindow_->Enable(false);

    mnemonics_->ShowStatistics();
    MnemonicXMLFile mnemonic_loader(mnemonics_, logwindow_);
    if (mnemonic_loader.Open(filechoosen))
    {
        mainwindow_menubar->Enable(idMenuViewMnemonics, true);
    }
    logwindow_->Enable(true);
    mnemonics_->ShowStatistics();
}




void test_mnemoncs_readFrame::OnViewMnemonics(wxCommandEvent& event)
{
    unsigned int counter = mnemonics_->GetCount();

    if (counter < 100)
    {
        wxMessageBox(wxString::Format("Mnemonics not properly loaded ! (%d)", counter), "Error");
    }
    else
    {
        Dialog_Mnemonics view_mnemonics(this, mnemonics_);
        view_mnemonics.ShowModal();
    }
}




