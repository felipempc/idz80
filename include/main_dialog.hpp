/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Main module
 **************************************************************/

#ifndef IDZ80_H
#define IDZ80_H


//#include <wx/menu.h>
//#include <wx/textctrl.h>
#include <wx/aui/aui.h>
#include <wx/statusbr.h>
#include <wx/config.h>
#include <wx/frame.h>

#include "project_base.hpp"
//#include "mnemonic_container.h"
//#include "rawdata.h"
//#include "process_data.h"
//#include "codeview.h"
//#include "labelslist.h"
//#include "projectmanager_xml.h"
//#include "codegenerator.h"
#include "debug_logwindow.hpp"




class IDZ80: public ProjectBase, public wxFrame
{
	public:
		const wxString ResourceDir = "resource";

		IDZ80(wxWindow* parent, wxArrayString &arraystr);
		virtual ~IDZ80();


	protected:
		static const long idMenuFileOpenProject;
		static const long idMenuFileOpenArchive;
		static const long idMenuFileOpen;
		static const long idMenuFileSave;
		static const long idMenuFileSaveAs;
		static const long idMenuFileClose;
		static const long idMenuFileInfo;
		static const long idMenuFileQuit;
		static const long idMenuViewDasmWindow;
		static const long idMenuViewProgLabels;
		static const long idMenuViewVarLabels;
		static const long idMenuViewIOLabels;
		static const long idMenuViewConstLabels;
		static const long idMenuViewLabels;
		static const long idMenuToolsDasmAll;
		static const long idMenuToolsAutoLabel;
		static const long idMenuToolsGenCode;
		static const long idMenuMnemLoad;
		static const long idMenuMnemInfo;
		static const long idMenuSettingsColors;
		static const long idMenuSettingsBlur;
		static const long idMenuHelpContents;
		static const long IdMenuHelpAbout;
		static const long IdMenuMnemonicXML;


	private:
		wxAuiManager    *m_aui_mgr;
		wxAuiNotebook	*m_notebook;
		wxTextCtrl      *m_panel_log;
		wxStatusBar     *m_status_bar;
        DebugLogWindow  *m_log_window;

//        ProcessData     *process_;
//        CodeView        *codeview_;

        wxConfig        *m_config;
//        ProjectManagerXML
//                        *project_;
        //wxArrayString	m_commandline;
		bool            m_maximize_main_window;

        bool LoadMnemonicsDB();
        bool SaveAs();
        void Clear_all();
        //bool OpenProgramFile(const wxString filename);
        void OpenProgramFile(wxString filename = "");
        //bool OpenProjectFile(const wxString filename);
        void OpenProjectFile();

		void PageOrganizer();

        void UpdateTitle(const wxString str);
        void SetupMenuItemStatus();
        void SetupAUIStoredConfiguration();
        void ReadStoredConfiguration();
        void StoreConfiguration();
        void SetupIcon();
        void SetupAUIPanes();
        void SetupMenuEvents();
        void SetupMenu();
        void SetupStatusBar();
        void SetupPanelLog();
        void SetupLabels();
		void SetupNotebook();

        //wxString DialogLoadProgramFile();
        wxString DialogLoadProjectFile();


		void OnMenuFileOpen(wxCommandEvent& event);
		void OnMenuFileQuit(wxCommandEvent& event);
		void OnMenuMnemonicsLoad(wxCommandEvent& event);
		void OnMenuMnemonicsInfo(wxCommandEvent& event);
		void OnMenuHelpAbout(wxCommandEvent& event);

		void OnAuiPaneClose(wxAuiManagerEvent& event);
		void OnAuiNotebookChanged(wxAuiNotebookEvent& event);
		void OnAuiNotebookClose(wxAuiNotebookEvent& event);

		void OnMenuViewDisassemblyWindow(wxCommandEvent& event);
		void OnMenuViewProgramLabels(wxCommandEvent& event);
		void OnMenuViewVarLabels(wxCommandEvent& event);
		void OnMenuViewIOLabels(wxCommandEvent& event);
		void OnMenuViewConstLabels(wxCommandEvent& event);

		void OnMenuFileSaveProject(wxCommandEvent& event);
		void OnMenuFileSaveAsProject(wxCommandEvent& event);
		void OnMenuFileInfo(wxCommandEvent& event);
		void OnMenuFileClose(wxCommandEvent& event);

		void OnMenuToolsDisAsm(wxCommandEvent& event);
		void OnMenuToolsGenCode(wxCommandEvent& event);
		void OnMenuToolAutoLabel(wxCommandEvent& event);
		void OnMenuToolMnemonicXML(wxCommandEvent& event);

		void OnMenuSettingsColor(wxCommandEvent& event);
		void OnMenuSettingsBlur(wxCommandEvent& event);

		void OnFirstIdle(wxIdleEvent &event);

};


#endif
