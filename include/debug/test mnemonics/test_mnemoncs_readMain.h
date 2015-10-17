/***************************************************************
 * Name:      test_mnemoncs_readMain.h
 * Purpose:   Defines Application Frame
 * Author:    felipempc (idz80a@gmail.com)
 * Created:   2014-07-04
 * Copyright: felipempc (#######)
 * License:
 **************************************************************/

#ifndef TEST_MNEMONCS_READMAIN_H
#define TEST_MNEMONCS_READMAIN_H

#include <wx/frame.h>
#include <wx/toplevel.h>
#include "mnemonic_container.h"
//include "test_mnemoncs_readApp.h"

class test_mnemoncs_readFrame: public wxFrame
{
    public:
        test_mnemoncs_readFrame(wxFrame *frame, const wxString& title);
        ~test_mnemoncs_readFrame();


    private:
        enum
        {
            idMenuQuit = 1000,
            idMenuAbout,
            idMenuOpen,
            idMenuViewMnemonics
        };

        wxTextCtrl  *logwindow_;
        MnemonicContainer *mnemonics_;
        wxMenuBar *mainwindow_menubar;

        void OnOpen(wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnViewMnemonics(wxCommandEvent& event);
        DECLARE_EVENT_TABLE()
};


#endif // TEST_MNEMONCS_READMAIN_H
