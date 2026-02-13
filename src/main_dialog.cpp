/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Main module
 **************************************************************/

#include <wx/filedlg.h>
//#include <wx/dir.h>
//#include <wx/filefn.h>
//#include <wx/filename.h>
//#include <wx/font.h>
//#include <wx/intl.h>
//#include <wx/icon.h>
//#include <wx/string.h>
//#include <wx/msgdlg.h>

#include "main_dialog.hpp"
//#include "fileinfo_dialog.h"
//#include "systemlabels.h"
//#include "file_settings_dialog.h"
#include "newproject_dialog.hpp"

const long IDZ80::idMenuFileOpenProject = wxNewId();
const long IDZ80::idMenuFileOpenArchive = wxNewId();
const long IDZ80::idMenuFileOpen = wxNewId();
const long IDZ80::idMenuFileSave = wxNewId();
const long IDZ80::idMenuFileSaveAs = wxNewId();
const long IDZ80::idMenuFileClose = wxNewId();
const long IDZ80::idMenuFileInfo = wxNewId();
const long IDZ80::idMenuFileQuit = wxNewId();
const long IDZ80::idMenuViewDasmWindow = wxNewId();
const long IDZ80::idMenuViewProgLabels = wxNewId();
const long IDZ80::idMenuViewVarLabels = wxNewId();
const long IDZ80::idMenuViewIOLabels = wxNewId();
const long IDZ80::idMenuViewConstLabels = wxNewId();
const long IDZ80::idMenuViewLabels = wxNewId();
const long IDZ80::idMenuToolsDasmAll = wxNewId();
const long IDZ80::idMenuToolsAutoLabel = wxNewId();
const long IDZ80::idMenuToolsGenCode = wxNewId();
const long IDZ80::idMenuMnemLoad = wxNewId();
const long IDZ80::idMenuMnemInfo = wxNewId();
const long IDZ80::idMenuSettingsColors = wxNewId();
const long IDZ80::idMenuSettingsBlur = wxNewId();
const long IDZ80::idMenuHelpContents = wxNewId();
const long IDZ80::IdMenuHelpAbout = wxNewId();
const long IDZ80::IdMenuMnemonicXML = wxNewId();



IDZ80::IDZ80(wxWindow* parent, wxArrayString &arraystr)
{
    m_aui_mgr = 0;
    m_panel_log = 0;
    m_status_bar = 0;
    m_log_window = 0;
//    m_processdata = 0;
//    codeview_ = 0;
    m_config = 0;
//    project_ = 0;
	m_maximize_main_window = true;
//	m_source_code = 0;
    m_disassembled_mgr = 0;
    m_programs_mgr = 0;
    m_mnemonics = 0;
    m_labels = 0;

	Create(parent, wxID_ANY, "Interactive Disassembler Z80", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, "id");

    m_log_window = new DebugLogWindow(this, "IDZ80 - Log Window");
    SetTextLog(m_log_window);
    ModuleName = "Main";

    SetupIcon();

	SetClientSize(wxSize(600,465));
	Move(wxDefaultPosition);

	SetupPanelLog();
	m_aui_mgr = new wxAuiManager(this, wxAUI_MGR_DEFAULT);
	m_aui_mgr->AddPane(m_panel_log, wxAuiPaneInfo().Name("MessageLog").Caption("Message Log").CaptionVisible().Bottom());
	m_aui_mgr->Update();

    SetupMenu();
    SetupStatusBar();

	Bind(wxEVT_IDLE, &IDZ80::OnFirstIdle, this);

}




IDZ80::~IDZ80()
{
    StoreConfiguration();
    m_aui_mgr->UnInit();
//    delete project_;
    delete m_config;
    delete m_labels;
//    delete m_processdata;
    delete m_sourcecode_mgr;
    delete m_disassembled_mgr;

    delete m_programs_mgr;
    delete m_notebook;
//    delete codeview_;
//    delete process_;
    delete m_aui_mgr;
    delete m_status_bar;
    delete m_panel_log;
    delete m_log_window;
}



void IDZ80::OpenProgramFile()
{
    NewProjectDialog dialog(this/*, m_notebook*/);
    bool wasLoaded = m_programs_mgr->isLoaded();
    
    if (dialog.ShowModal() == wxID_OK) {
        if(!m_programs_mgr->isLoaded()) {
            m_notebook->DeleteAllPages();
            SetupMenuItemStatus();
            return;
        }


        wxMenuBar *mb;
        mb = GetMenuBar();
        mb->Enable(idMenuToolsDasmAll, true);
        mb->Enable(idMenuFileInfo, true);
        mb->Enable(idMenuFileClose, true);
        mb->Enable(idMenuToolsGenCode, false);
        mb->Enable(idMenuToolsAutoLabel, false);

        if (dialog.WasModified())
            updateNotebookPages();
        
        if (m_labels)
        {
            wxString labelspath = m_app_resource_dir + "\\Labels.txt";
            if (!m_labels->loadSystemLabels(labelspath))
                LogIt(wxString::Format("System labels failed to load. [%s]", labelspath));
            else
                LogIt("System labels loaded sucessfully.");
        }
    }
    else {
        m_status_bar->SetStatusText("Cancelled by user !");
    }


    /* ----    OLD     ---------
            UpdateTitle(caption);

            if (process->setupSystemLabels())
            {
                process->loadSystemLabels(m_currentDir + "/resource/Labels.txt");
            }

            process->Program->StartAddress = config.GetStartAddress();
            process->Program->ExecAddress = config.GetExecAddress();
            process->Program->EndAddress = config.GetEndAddress();

            process->Disassembled->AddOrgAddress(0, process->Program->StartAddress);

            m_project->New();
            if (config.WantsAutoDisassembly())
            {
                simulateexecution = config.WantsSimulateExecution();

                wxCommandEvent ev_dasm(wxEVT_MENU, idMenuToolsDasmAll);
                ev_dasm.SetClientData(&simulateexecution);
                AddPendingEvent(ev_dasm);
            }
        }

	*/

}


/// @brief Updates pages in the notebook.\
/// @brief Creates DisassembledContainer and SourceCode.
void IDZ80::updateNotebookPages()
{
    wxString filename, namepage;
    unsigned int    num_files = m_programs_mgr->Count(),
                    num_pages = m_notebook->GetPageCount();

    m_notebook->Freeze();

    for (unsigned int i = 0; i < num_files; ++i) {
        filename = m_programs_mgr->Index(i)->GetFileName();
        namepage = m_notebook->GetPageText(i);
        if (!filename.IsSameAs(namepage))
        {
            DisassembledContainer *disassembled = new DisassembledContainer(this);
            m_disassembled_mgr->Add(disassembled);
            m_sourcecode_mgr->addSourceCode(new SourceCode(disassembled, m_labels));
            m_notebook->AddPage(new wxPanel(this), filename, true);
        }
    }

    m_notebook->Thaw();
}



void IDZ80::OpenProjectFile()
{
/*
	wxString filename;

	filename = DialogLoadProjectFile();
	if ((!filename.IsEmpty()) && project_->Open(filename, m_app_resource_dir + "Labels.txt"))
    {
//		codeview_->Enable(true);
//		codeview_->Plot();
		wxMenuBar *mb;
		mb = GetMenuBar();
		mb->Enable(idMenuToolsDasmAll, true);
		mb->Enable(idMenuFileInfo, true);
		mb->Enable(idMenuFileSave, true);
		mb->Enable(idMenuFileSaveAs, true);
		mb->Enable(idMenuFileClose, true);
		mb->Enable(idMenuToolsGenCode, true);
		mb->Enable(idMenuToolsAutoLabel, true);
	}
*/
}



wxString IDZ80::DialogLoadProjectFile()
{
    wxString caption = "Choose a project";
    wxString wildcard = "Project files (*.dap)|*.dap|All files (*.*)|*.*";
//    wxFileDialog dialog(this, caption, m_fileopen_last_dir, wxEmptyString, wildcard, wxFD_OPEN);
    wxString ret = "";

/*    if (dialog.ShowModal() == wxID_OK)
    {
        ret = dialog.GetPath();
        m_fileopen_last_dir = dialog.GetDirectory();
    } */
    return ret;
}



bool IDZ80::SaveAs()
{
    wxString fname, caption, wildcard,
            defaultFilename = wxEmptyString;
    bool    ret;

    ret = false;
    caption = "Save project as";
    wildcard = "Project files (*.dap)|*.dap|All files (*.*)|*.*";
    wxFileDialog dialog(this, caption, m_fileopen_last_dir, defaultFilename,wildcard,
                         wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);
    if (dialog.ShowModal() == wxID_OK)
    {
        fname = dialog.GetPath();
//        if (project_->Save(fname))
//            ret = true;
    }
    return ret;
}



void IDZ80::Clear_all()
{
//    codeview_->Clear();
//    codeview_->Enable(false);
//    process_->Clear();
}



void IDZ80::UpdateTitle(const wxString str)
{
	wxString info = GetTitle();
	info = info.BeforeFirst('-');
	info.Trim();
	if (!str.IsEmpty())
		info << " - " << str;
	SetTitle(info);
}
