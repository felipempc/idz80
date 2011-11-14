/***************************************************************
 * Name:      IDZ80Main.h
 * Purpose:   Defines Application Frame
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 **************************************************************/

#ifndef IDZ80MAIN_H
#define IDZ80MAIN_H

//(*Headers(IDZ80Frame)
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/gauge.h>
#include <wx/statusbr.h>
//*)

#include "mndb.h"
#include "rawdata.h"
#include "dasmdata.h"
#include "processdata.h"
#include "codeview.h"
#include "DebugView.h"

class IDZ80Frame: public wxFrame
{
    public:

        IDZ80Frame(wxWindow* parent,wxWindowID id = -1);
        virtual ~IDZ80Frame();

    private:
        bool LoadMnemonicsDB();

        //(*Handlers(IDZ80Frame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnMudaIconeClick(wxCommandEvent& event);
        void OnMenuOpen(wxCommandEvent& event);
        void OnButtonDASMClick(wxCommandEvent& event);
        void OnButtonDrawClick(wxCommandEvent& event);
        void OnMneumLoadClick(wxCommandEvent& event);
        void OnMenuDebugView(wxCommandEvent& event);
        void OnMenuDebugLstMnem(wxCommandEvent& event);
        void OnResize(wxSizeEvent& event);
        void OnMenuInfoSelected(wxCommandEvent& event);
        //*)

        //(*Identifiers(IDZ80Frame)
        static const long ID_BUTTON1;
        static const long ID_GAUGE1;
        static const long ID_PANEL1;
        static const long IdMenuOpen;
        static const long idMenuQuit;
        static const long ID_DebugView;
        static const long ID_DebugListMneum;
        static const long ID_MneumLoad;
        static const long ID_MneumInfo;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        MnemonicDataBase *mnDb;
        RawData *program;
        DAsmData *dasmed;
        ProcessData *process;
        CodeView *codeview;
        DebugView *debugview;

        wxSize LastSize;

        //(*Declarations(IDZ80Frame)
        wxButton* B_DASM;
        wxMenuItem* MenuItem5;
        wxMenu* Menu3;
        wxGauge* Gauge1;
        wxMenuItem* MenuItem4;
        wxPanel* Panel1;
        wxMenuItem* Menu4;
        wxStatusBar* StatusBar1;
        wxMenuItem* MenuItem6;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // IDZ80MAIN_H
