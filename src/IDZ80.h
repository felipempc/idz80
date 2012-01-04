#ifndef IDZ80_H
#define IDZ80_H

//(*Headers(IDZ80)
#include <wx/menu.h>
#include <wx/textctrl.h>
#include <wx/aui/aui.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
//*)


#include <wx/arrstr.h>

#include "mndb.h"
#include "rawdata.h"
#include "dasmdata.h"
#include "processdata.h"
#include "codeview.h"
#include "labelslist.h"
#include "projectmanager.h"
#include "codegenerator.h"

#include <wx/config.h>

enum
{
    id_CD_EVT=111
};

class IDZ80: public wxFrame
{
	public:

		IDZ80(wxWindow* parent, wxArrayString &arraystr, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
		virtual ~IDZ80();

		//(*Declarations(IDZ80)
		wxAuiManager* AuiManager1;
		wxMenuItem* MenuItem8;
		wxMenuItem* MenuItem7;
		wxMenuItem* MenuItem5;
		wxMenuItem* MenuItem2;
		wxMenu* Menu3;
		wxMenuItem* MenuItem4;
		wxMenuItem* MenuItem14;
		wxTextCtrl* PanelLog;
		wxMenuItem* MenuItem15;
		wxMenuItem* MenuItem17;
		wxMenuItem* MenuItem13;
		wxMenu* Menu1;
		wxMenuItem* MenuItem3;
		wxStatusBar* StatusBar1;
		wxMenuItem* MenuItem6;
		wxMenuBar* MenuBar1;
		wxMenuItem* MenuItem16;
		wxMenu* Menu2;
		wxMenu* MenuItem1;
		wxMenuItem* MenuItem18;
		wxMenu* Menu5;
		wxMenu* Menu4;
		//*)

	protected:

		//(*Identifiers(IDZ80)
		static const long ID_TEXTCTRL1;
		static const long idMenuFileOpenProject;
		static const long idMenuFileOpenArchive;
		static const long idMenuFileOpen;
		static const long idMenuFileSave;
		static const long idMenuFileClose;
		static const long idMenuFileInfo;
		static const long idMenuFileQuit;
		static const long idMenuViewDasmWindow;
		static const long idMenuViewProgLabels;
		static const long idMenuViewVarLabels;
		static const long idMenuViewIOLabels;
		static const long idMenuToolsDasmAll;
		static const long idMenuToolsAutoLabel;
		static const long idMenuToolsGenCode;
		static const long idMenuMnemLoad;
		static const long idMenuMnemInfo;
		static const long idMenuHelpContents;
		static const long IdMenuHelpAbout;
		static const long ID_STATUSBAR1;
		//*)

		static const long ID_VARLABELPANE;

	private:
        bool LoadMnemonicsDB();
        ProcessData     *process;
        CodeView        *codeview;
        wxIconBundle    *icons;
        wxConfig        *config;
        wxString        m_currentDir,
                        m_lastDir;
        ProjectManager  *m_project;
        wxArrayString	m_commandline;


        wxSize LastSize;

        void SaveAs();
        void Clear_all();
        bool OpenProgramFile(const wxString filename);
        bool OpenProjectFile(const wxString filename);
        void UpdateTitle(const wxString str);


		void OnMenuFileOpen(wxCommandEvent& event);
		void OnMenuFileQuit(wxCommandEvent& event);
		void OnMenuMnemonicsLoad(wxCommandEvent& event);
		void OnMenuMnemonicsInfo(wxCommandEvent& event);
		void OnMenuHelpAbout(wxCommandEvent& event);
		void OnMenuToolsDisAsm(wxCommandEvent& event);
		void OnMenuViewDisassemblyWindow(wxCommandEvent& event);
		void OnAuiPaneClose(wxAuiManagerEvent& event);
		void OnMenuViewProgramLabels(wxCommandEvent& event);
		void OnMenuViewVarLabels(wxCommandEvent& event);
		void OnMenuViewIOLabels(wxCommandEvent& event);
		void OnMenuToolAutoLabel(wxCommandEvent& event);
		void OnMenuFileSaveProject(wxCommandEvent& event);
		void OnMenuFileInfo(wxCommandEvent& event);
		void OnMenuFileClose(wxCommandEvent& event);
		void OnMenuToolsGenCode(wxCommandEvent& event);
		void OnFirstIdle(wxIdleEvent &event);


		DECLARE_EVENT_TABLE()
};

#endif
