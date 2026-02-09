/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Main module - Events handling routines
 **************************************************************/

#include <wx/filedlg.h>

#include "main_dialog.hpp"
#include "version.hpp"




/*
 *	Initialize after main window shows up
 */
void IDZ80::OnFirstIdle(wxIdleEvent &event)
{
	bool result;
	// this is needed to stop catching this event all the time
	result = Unbind(wxEVT_IDLE, &IDZ80::OnFirstIdle, this);
	#ifdef IDZ80DEBUG
	if (!result)
		m_panel_log->AppendText("*** First Idle Event failed to unbind !\n\n");
	#endif

    m_app_root_dir = wxGetCwd();
    m_app_resource_dir = m_app_root_dir + "\\" + ResourceDir;
    m_fileopen_last_dir = "";

    SetupMenuItemStatus();

    m_programs_mgr = new RawDataManager(this);
    m_disassembled_mgr = new DisassembledManager(this);
    m_sourcecode_mgr = new SourceCodeManager(this);
    m_labels = new LabelManager();
//    m_processdata = new ProcessData(this);
//    codeview_ = new CodeView(this, process_, m_log_window);
//    project_ = new ProjectManagerXML(this);

    SetupLabels();
    SetupNotebook();
    SetupAUIPanes();
    ReadStoredConfiguration();
    LoadMnemonicsDB();

    m_status_bar->SetStatusText(m_app_root_dir);

    /*
	if (m_commandline.GetCount() > 1)
	{
		OpenProgramFile(m_commandline[1]);
	}
    */

//    codeview_->Enable(false);

	m_log_window->Show();

	//TODO: Implement Log in project class
	//project_->SetLog(m_panel_log);

	SetupMenuEvents();

    if (m_maximize_main_window)
        Maximize();

/*
    if (m_programs_mgr->First()->IsLoaded())
    {
        wxMenuBar *mb;
        mb = GetMenuBar();
        mb->Enable(idMenuToolsDasmAll, true);
        mb->Enable(idMenuFileInfo, true);
        mb->Enable(idMenuFileClose, true);
        mb->Enable(idMenuToolsGenCode, false);
        mb->Enable(idMenuToolsAutoLabel, false);
    }
*/

    //  TEST AREA   --------------------->

    // <----------------------- TEST AREA
}


/// @brief Opens a message box to decide if the file will be closed or not.
/// @return true if it is to be closed.
bool IDZ80::closeFileDialog(const wxString t_title_str)
{
    wxString ask_str = wxString::Format("Are you sure you want to close %s?", t_title_str);
    int res = wxMessageBox(ask_str,
                       "wxAUI",
                       wxYES_NO,
                       this);
    
    return (res == wxYES);
}



/// @brief Disables items on menus when there is no file opened.
void IDZ80::disableMenuForNoFile()
{
    wxMenuBar *mb;
    mb = GetMenuBar();
    mb->Enable(idMenuToolsDasmAll, false);
    mb->Enable(idMenuFileInfo, false);
    mb->Enable(idMenuFileSave, false);
    mb->Enable(idMenuFileSaveAs, false);
    mb->Enable(idMenuFileClose, false);
    mb->Enable(idMenuToolsGenCode, false);
}



void IDZ80::OnMenuFileOpen(wxCommandEvent& event)
{
    Clear_all();
    if (event.GetId() == idMenuFileOpenArchive)
        OpenProgramFile();
    else
        OpenProjectFile();
}




void IDZ80::OnMenuFileQuit(wxCommandEvent& event)
{
    Close();
}



void IDZ80::OnMenuMnemonicsLoad(wxCommandEvent& event)
{
    wxString s;
    if (!LoadMnemonicsDB())
    {
        s = "Could not open the file !";
        wxMessageBox(s, "Error..");
    }
}




void IDZ80::OnMenuMnemonicsInfo(wxCommandEvent& event)
{
    wxString str = "Must fill in";
//    str.Printf("Num of Mnemonics: %d\nMemory allocated: %d bytes\nSyscalls = %d\nSysVars = %d\nSysIO = %d ",
//    process->Mnemonics->GetCount(), process->Mnemonics->GetAllocated(), process->sys_calls->GetCount(),
//    process->sys_vars->GetCount(), process->sys_io->GetCount());
    wxMessageBox(str, "Mnemonic Info");
}




void IDZ80::OnMenuHelpAbout(wxCommandEvent& event)
{
    using namespace AutoVersion;
    wxString msg,status(STATUS,wxConvUTF8);
    msg.Printf("Interactive Disassembler for\nZ80 processors.\n2009 by Felipe"
                 "\nVersion: %d.%d Build %d\nStatus: ",MAJOR,MINOR,BUILD,REVISION);
    msg << status;
    wxMessageBox(msg, "About");
}




void IDZ80::OnMenuToolsDisAsm(wxCommandEvent& event)
{
    /*
    wxSize  psize,ldsize;
    wxPoint ldpos;
    int     w,h,x,y;
    */
    //SortedIntArray  entries(CompareSortedInt);
//    codeview_->Enable(false);
    unsigned int file_selected = m_notebook->GetSelection();

//    m_processdata->disassembleFirst(file_selected);
//    m_processdata->initData(file_selected);
/*
    process_->insertLineLabelsInSourceCode();
    process_->prog_labels->SortAddress(true);
    process_->io_labels->SortAddress(true);
    process_->var_labels->SortAddress(true);
    process_->constant_labels->SortAddress(true);

    codeview_->Enable(true);
*/
    #ifdef IDZ80DEBUG
    wxString stemp;
    stemp.Printf("Used memory (dasmed)= %d bytes\n", process_->Disassembled->GetUsedMem());
    m_panel_log->AppendText(stemp);

    m_panel_log->AppendText(wxString::Format("Program Label = %d items, Var Label = %d items, IO Labels = %d items.\n",
                                          process_->prog_labels->GetCount(), process_->var_labels->GetCount(), process_->io_labels->GetCount()));
    #endif

//    codeview_->Plot();

    wxMenuBar *mb;
    mb = GetMenuBar();
    mb->Enable(idMenuToolsAutoLabel, true);
    mb->Enable(idMenuFileSave, true);
    mb->Enable(idMenuFileSaveAs, true);
    mb->Enable(idMenuToolsGenCode, true);

//    process_->Program->GetEntries(entries);

//    if (!entries.IsEmpty())
//        codeview_->CenterAddress(entries[0]);
}



void IDZ80::OnMenuViewDisassemblyWindow(wxCommandEvent& event)
{
    wxMenuBar *mb;
    bool test=false;
    mb = GetMenuBar();
    test = mb->IsChecked(idMenuViewDasmWindow);
    if (test)
    {
        mb->Check(idMenuViewDasmWindow,true);
        m_aui_mgr->GetPane("MainWindow").Show();
    }
    else
    {
        mb->Check(idMenuViewDasmWindow,false);
        m_aui_mgr->GetPane("MainWindow").Hide();
    }
    m_aui_mgr->Update();
}



void IDZ80::OnAuiPaneClose(wxAuiManagerEvent& event)
{
    wxMenuBar *mb;
    mb = GetMenuBar();
    if (event.pane->name == "MainWindow")
        mb->Check(idMenuViewDasmWindow, false);
    if (event.pane->name == "VarLabels")
        mb->Check(idMenuViewVarLabels, false);
    if (event.pane->name == "ProgLabels")
        mb->Check(idMenuViewProgLabels, false);
    if (event.pane->name == "IOLabels")
        mb->Check(idMenuViewIOLabels, false);
    if (event.pane->name == "ConstLabels")
        mb->Check(idMenuViewConstLabels, false);
}



void IDZ80::OnAuiNotebookChanged(wxAuiNotebookEvent& event)
{
    int index = event.GetSelection();
    wxString caption = m_notebook->GetPageText(index);
    LogIt(wxString::Format("Selected %d [%s].", index, caption));
}



void IDZ80::OnAuiNotebookClose(wxAuiNotebookEvent &event)
{
    int i = m_notebook->GetSelection();
    if (closeFileDialog(m_notebook->GetPageText(i))) {
        m_programs_mgr->Remove(event.GetSelection());
        if (m_programs_mgr->Count() == 0)
        {
            disableMenuForNoFile();
            UpdateTitle("");
            Clear_all();
        }
    }
    else    
        event.Veto();
}



void IDZ80::OnMenuViewProgramLabels(wxCommandEvent& event)
{
    wxMenuBar *mb;
    bool test=false;
    mb = GetMenuBar();
    test = mb->IsChecked(idMenuViewProgLabels);
    if (test)
    {
        mb->Check(idMenuViewProgLabels,true);
        m_aui_mgr->GetPane("ProgLabels").Show();
    }
    else
    {
        mb->Check(idMenuViewProgLabels, false);
        m_aui_mgr->GetPane("ProgLabels").Hide();
    }
    m_aui_mgr->Update();
}




void IDZ80::OnMenuViewVarLabels(wxCommandEvent& event)
{
    wxMenuBar *mb;
    bool test=false;
    mb = GetMenuBar();
    test = mb->IsChecked(idMenuViewVarLabels);
    if (test)
    {
        mb->Check(idMenuViewVarLabels,true);
        m_aui_mgr->GetPane("VarLabels").Show();
    }
    else
    {
        mb->Check(idMenuViewVarLabels,false);
        m_aui_mgr->GetPane("VarLabels").Hide();
    }
    m_aui_mgr->Update();
}




void IDZ80::OnMenuViewIOLabels(wxCommandEvent& event)
{
    wxMenuBar *mb;
    bool test = false;
    mb = GetMenuBar();
    test = mb->IsChecked(idMenuViewIOLabels);
    if (test)
    {
        mb->Check(idMenuViewIOLabels,true);
        m_aui_mgr->GetPane("IOLabels").Show();
    }
    else
    {
        mb->Check(idMenuViewIOLabels,false);
        m_aui_mgr->GetPane("IOLabels").Hide();
    }
    m_aui_mgr->Update();
}




void IDZ80::OnMenuViewConstLabels(wxCommandEvent& event)
{
    wxMenuBar *mb;
    bool test=false;
    mb = GetMenuBar();
    test = mb->IsChecked(idMenuViewConstLabels);
    if (test)
    {
        mb->Check(idMenuViewConstLabels,true);
        m_aui_mgr->GetPane("ConstLabels").Show();
    }
    else
    {
        mb->Check(idMenuViewConstLabels,false);
        m_aui_mgr->GetPane("ConstLabels").Hide();
    }
    m_aui_mgr->Update();
}




void IDZ80::OnMenuToolAutoLabel(wxCommandEvent& event)
{
    /*
    process_->AutoLabel();
    process_->insertLineLabelsInSourceCode();
    codeview_->ClearSelection();
    codeview_->Refresh();
    */
}



void IDZ80::OnMenuSettingsColor(wxCommandEvent& event)
{
    LogIt("Change colors !");
}


void IDZ80::OnMenuSettingsBlur(wxCommandEvent& event)
{
    LogIt("Event: Make Blur...");
//    codeview_->TestBlur();
}



void IDZ80::OnMenuFileSaveProject(wxCommandEvent& event)
{
/*
    if (project_->HasName())
    {
        if (!project_->Save())
            m_panel_log->AppendText("Project NOT saved !\n");
        else
            m_panel_log->AppendText("Project saved !\n");
    }
    else
        if (SaveAs())
        {
            UpdateTitle(project_->GetFilename());
        }
*/
}




void IDZ80::OnMenuFileSaveAsProject(wxCommandEvent& event)
{
/*
    if (SaveAs())
    {
        UpdateTitle(project_->GetFilename());
    }
*/
}




void IDZ80::OnMenuFileInfo(wxCommandEvent& event)
{
//    ShowFileInfo dialog(this);
//    dialog.SendInfo(process_);
//  dialog.ShowModal();
}





void IDZ80::OnMenuFileClose(wxCommandEvent& event)
{
    int i = m_notebook->GetSelection();
    if (closeFileDialog(m_notebook->GetPageText(i)))
    {
        m_notebook->DeletePage(i);
        m_programs_mgr->Remove(i);
        if(m_notebook->GetPageCount() == 0)
        {
            disableMenuForNoFile();
            UpdateTitle("");
            Clear_all();
        }
    }
}





void IDZ80::OnMenuToolsGenCode(wxCommandEvent& event)
{
    wxString fname, caption, wildcard,
            defaultFilename;

    wxDialog *dasmwin = new wxDialog(0, wxID_ANY, "Disassembled Code", wxDefaultPosition, wxDefaultSize, wxRESIZE_BORDER | wxCAPTION | wxCLOSE_BOX | wxSYSTEM_MENU, "TextCodeBox");
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);
    wxTextCtrl *textCode = new wxTextCtrl(dasmwin, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
//    codeGenerator *bakeCode;
    topsizer->Add(textCode, 1, wxEXPAND | wxALL, 10);

    dasmwin->SetSizer(topsizer);

//    defaultFilename = project_->GetFilename() << ".mac";

    caption = "Save source code as";
    wildcard = "Source code files (*.mac)|*.mac|All files (*.*)|*.*";
    wxFileDialog dialog(this, caption, m_fileopen_last_dir, defaultFilename,wildcard,
                         wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);
    if (dialog.ShowModal() == wxID_OK)
    {
        fname = dialog.GetPath();
/*        bakeCode = new codeGenerator(process_);
        //TODO: If viewing the final code is necessary. Replace with scintilla.
        //Since wxTextCtrl doesnt handle more than 32Kb, this is used only for debugging.
        textCode->AppendText(bakeCode->GenerateCode(fname, cfM80));
        dasmwin->ShowModal();

        delete bakeCode; */
    }
    dasmwin->Destroy();
}



void IDZ80::OnMenuToolMnemonicXML(wxCommandEvent& event)
{
    LogIt("Mnemonic XML!!");
}
