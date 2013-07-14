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


#include <wx/menu.h>
#include <wx/textctrl.h>
#include <wx/aui/aui.h>
#include <wx/frame.h>
#include <wx/statusbr.h>


#include <wx/arrstr.h>

#include "mndb.h"
#include "rawdata.h"
#include "dasmdata.h"
#include "processdata.h"
#include "codeview.h"
#include "labelslist.h"
#include "projectmanager_xml.h"
#include "codegenerator.h"
#include "logwindow.h"

#include <wx/config.h>

enum
{
    id_CD_EVT = 111
};
static const wxString LocalPath = "X:/idz80/idz80";

class IDZ80: public wxFrame, LogBase
{
	public:

		IDZ80(wxWindow* parent, wxArrayString &arraystr);
		virtual ~IDZ80();

		wxAuiManager* AuiManager1;
		wxTextCtrl* PanelLog;
		wxStatusBar* StatusBar1;
        LogWindow      *Log;

	protected:

		static const long ID_TEXTCTRL1;
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
		static const long ID_STATUSBAR1;


		static const long ID_VARLABELPANE;

	private:
        ProcessData     *process;
        CodeView        *codeview;
        wxIconBundle    *icons;
        wxConfig        *config;
        wxString        m_currentDir,
                        m_lastDir;
        ProjectManagerXML  *m_project;
        wxArrayString	m_commandline;
        bool            MaximizeMainWindow;

        bool LoadMnemonicsDB();
        bool SaveAs();
        void Clear_all();
        //bool OpenProgramFile(const wxString filename);
        void OpenProgramFile(wxString filename = "");
        //bool OpenProjectFile(const wxString filename);
        void OpenProjectFile();

        void UpdateTitle(const wxString str);
        void SetupMenuItemStatus();
        void SetupAUIStoredConfiguration();
        void ReadStoredConfiguration();
        void StoreConfiguration();
        void SetupIcon();
        void SetupAUIPanes();
        void SetupMenuEvents();

        wxString DialogLoadProgramFile();
        wxString DialogLoadProjectFile();


		void OnMenuFileOpen(wxCommandEvent& event);
		void OnMenuFileQuit(wxCommandEvent& event);
		void OnMenuMnemonicsLoad(wxCommandEvent& event);
		void OnMenuMnemonicsInfo(wxCommandEvent& event);
		void OnMenuHelpAbout(wxCommandEvent& event);

		void OnAuiPaneClose(wxAuiManagerEvent& event);

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

		void OnMenuSettingsColor(wxCommandEvent& event);
		void OnMenuSettingsBlur(wxCommandEvent& event);

		void OnFirstIdle(wxIdleEvent &event);
};

#endif
