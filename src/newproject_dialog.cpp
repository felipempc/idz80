/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   20-12-2013 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Load programs to disassemble
 **************************************************************/

#include "newproject_dialog.h"
#include "wx/filedlg.h"
#include "file_settings_dialog.h"


// DEFinition for debug = IDZ80_NPRJD_DEBUG
//#define IDZ80_NPRJD_DEBUG


NewProjectDialog::NewProjectDialog(ProjectBase *parent)
{
    m_main_dialog = parent;
    ModuleName = "NPDialog";
    SetTextLog(m_main_dialog->GetTextLog());

    m_actualrow = -1;

    BuildDialog();
    Bind(wxEVT_SIZE, &NewProjectDialog::OnResize, this);
    Bind(wxEVT_BUTTON, &NewProjectDialog::OnAddButton, this, idADD_FILE_BUTTON);
    Bind(wxEVT_BUTTON, &NewProjectDialog::OnEditButton, this, wxID_EDIT);
    Bind(wxEVT_BUTTON, &NewProjectDialog::OnRemoveButton, this, idREMOVE_FILE_BUTTON);
    Bind(wxEVT_BUTTON, &NewProjectDialog::OnOkButtonPressed, this, wxID_OK);
    Bind(wxEVT_BUTTON, &NewProjectDialog::OnCancelButtonPressed, this, wxID_CANCEL);
    Bind(wxEVT_GRID_SELECT_CELL, &NewProjectDialog::OnGridSelectedRow, this);
    Bind(wxEVT_GRID_CELL_LEFT_DCLICK, &NewProjectDialog::OnGridLeftDoubleClick, this);

    if (m_main_dialog->Programs_->Count() != 0) {
        for(unsigned int j = 0; j < m_main_dialog->Programs_->Count(); j++){
            FillRow(m_main_dialog->Programs_->Index(j));
        }
        m_program_loaded = true;
    }
    else {
        m_OK_button->Disable();
        m_program_loaded = false;
    }

    m_edit_button->Disable();
    m_remove_button->Disable();
}




NewProjectDialog::~NewProjectDialog()
{
    delete m_filegrid;
}




void NewProjectDialog::BuildDialog()
{
    Create(0, wxID_ANY, "Project", wxPoint(100, 100), wxSize(500, 500), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER, "NewProject");
    wxPanel *main_panel = new wxPanel(this);
    m_main_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxPanel *buttons_panel = new wxPanel(main_panel);
    wxBoxSizer *buttons_sizer = new wxBoxSizer(wxVERTICAL);

    // Grid 
    m_filegrid = new wxGrid(main_panel, wxID_ANY, wxPoint(0, 0), wxSize(500, 300));
    m_filegrid->CreateGrid(0, 5);
    m_filegrid->SetSelectionMode(wxGrid::wxGridSelectRows);
    m_filegrid->SetColLabelValue(0, "Name");
    m_filegrid->SetColMinimalWidth(0, 90);
    m_filegrid->SetColLabelValue(1, "Type");
    m_filegrid->SetColMinimalWidth(1, 40);
    m_filegrid->SetColLabelValue(2, "Start");
    m_filegrid->SetColMinimalWidth(2, 55);
    m_filegrid->SetColLabelValue(3, "Execution");
    m_filegrid->SetColMinimalWidth(3, 65);
    m_filegrid->SetColLabelValue(4, "End");
    m_filegrid->SetColMinimalWidth(4, 55);
    m_main_sizer->Add(m_filegrid, wxSizerFlags(0).Border(wxALL, 10));
    m_filegrid->EnableEditing(false);

    // Buttons_panel
    m_OK_button = new wxButton(buttons_panel, wxID_OK, "OK", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "wxID_OK");
    buttons_sizer->Add(m_OK_button, wxSizerFlags(0).Border(wxALL, 10));
    m_edit_button = new wxButton(buttons_panel, wxID_EDIT, "Edit", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "wxID_EDIT");
    buttons_sizer->Add(m_edit_button, wxSizerFlags(0).Border(wxALL, 10));
    m_add_button = new wxButton(buttons_panel, idADD_FILE_BUTTON, "Add", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "AddButton");
    buttons_sizer->Add(m_add_button, wxSizerFlags(0).Border(wxALL, 10));
    m_remove_button = new wxButton(buttons_panel, idREMOVE_FILE_BUTTON, "Remove", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "RemoveButton");
    buttons_sizer->Add(m_remove_button, wxSizerFlags(0).Border(wxALL, 10));
    m_cancel_button = new wxButton(buttons_panel, wxID_CANCEL, "Cancel", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "wxID_CANCEL");
    buttons_sizer->Add(m_cancel_button, wxSizerFlags(0).Border(wxALL, 10));
    buttons_panel->SetSizer(buttons_sizer);
    m_main_sizer->Add(buttons_panel, wxSizerFlags(0).Border(wxALL, 10));

    main_panel->SetSizer(m_main_sizer);
    m_main_sizer->Fit(this);
}




/// @brief Open a file picker dialog and return an array of string with the full name of the selected file(s). Save current directory in main_.
/// @param file_list_str list with full name of the selected file(s)
/// @return true if success
bool NewProjectDialog::DialogGetFileToList(wxArrayString &file_list_str)
{
    wxString caption = "Choose a file";
    wxString wildcard = "Program files (*.ROM, *.COM, *.BIN)|*.rom;*.com;*.bin|All files (*.*)|*.*";
    wxFileDialog dialog(this, caption, m_main_dialog->fileopen_last_dir_, wxEmptyString, wildcard, wxFD_OPEN | wxFD_MULTIPLE);
    bool ret = false;

    if (dialog.ShowModal() == wxID_OK)
    {
        ret = true;
        dialog.GetPaths(file_list_str);
        m_main_dialog->fileopen_last_dir_ = dialog.GetDirectory();
    }

    return ret;
}



/// @brief Load the program and put on list (grid). Won't load pure BASIC programs.
/// @param filestr 
/// @return true if succeed.
bool NewProjectDialog::AddFileToGrid(wxString& filestr)
{
    RawData *program = 0;

    program = m_main_dialog->Programs_->AddFile(filestr);
    if (!program) {
        wxMessageBox(wxString::Format("Error while openning file [%s] !", filestr), "Error opening file !");
        return false;
    }
    if (program->PureBasic()) {
        wxMessageBox(wxString::Format("The file [%s] is a pure BASIC program. It's not supported yet!", filestr), "Error opening file !");
        return false;
    }
    FillRow(program);
    return true;
}



void NewProjectDialog::FillRow(RawData *program)
{
    m_filegrid->AppendRows();
    m_actualrow++;

    m_filegrid->SetCellValue(m_actualrow, 0, program->GetFileName());
    m_filegrid->SetCellValue(m_actualrow, 1, program->GetFileTypeStr());
    m_filegrid->SetCellAlignment(m_actualrow, 1, wxALIGN_CENTRE, wxALIGN_CENTRE);
    m_filegrid->SetCellValue(m_actualrow, 2, wxString::Format("0x%.4X", program->StartAddress));
    m_filegrid->SetCellAlignment(m_actualrow, 2, wxALIGN_CENTRE, wxALIGN_CENTRE);
    m_filegrid->SetCellValue(m_actualrow, 3, wxString::Format("0x%.4X", program->ExecAddress));
    m_filegrid->SetCellAlignment(m_actualrow, 3, wxALIGN_CENTRE, wxALIGN_CENTRE);
    m_filegrid->SetCellValue(m_actualrow, 4, wxString::Format("0x%.4X", program->EndAddress));
    m_filegrid->SetCellAlignment(m_actualrow, 4, wxALIGN_CENTRE, wxALIGN_CENTRE);
    m_filegrid->AutoSizeColumns();
}



void NewProjectDialog::UpdateGridRow(unsigned int line, RawData *program)
{
    m_filegrid->SetCellValue(line, 1, program->GetFileTypeStr());
    m_filegrid->SetCellValue(line, 2, wxString::Format("0x%.4X", program->StartAddress));
    m_filegrid->SetCellValue(line, 3, wxString::Format("0x%.4X", program->ExecAddress));
    m_filegrid->SetCellValue(line, 4, wxString::Format("0x%.4X", program->EndAddress));
    m_filegrid->AutoSizeColumns();
}



void NewProjectDialog::DialogEditRow(unsigned int line)
{
    RawData *program;

    if (line < m_main_dialog->Programs_->Count()) {
        program = m_main_dialog->Programs_->Index(line);
        if (program == 0)
            return;
        
        FileSettingsDialog config(m_main_dialog);
        if (config.ShowModal() == wxID_OK)
            UpdateGridRow(line, program);
    }
}



void NewProjectDialog::OnResize(wxSizeEvent &event)
{
    m_main_sizer->Fit(this);
}




void NewProjectDialog::OnAddButton(wxCommandEvent &event)
{
    wxArrayString file_list;
    bool success = false;

    if(!DialogGetFileToList(file_list))
        return;

    for(unsigned int idx = 0; idx < file_list.GetCount(); idx++)
        if (AddFileToGrid(file_list[idx]))
            success = true;
    
    if(success)
        m_OK_button->Enable();
    
    #ifdef IDZ80_NPRJD_DEBUG
    LogIt("Size of the Columns:");
    LogIt(wxString::Format("Name = %d", m_filegrid->GetColSize(0)));
    LogIt(wxString::Format("Type = %d", m_filegrid->GetColSize(1)));
    LogIt(wxString::Format("Start = %d", m_filegrid->GetColSize(2)));
    LogIt(wxString::Format("Execution = %d", m_filegrid->GetColSize(3)));
    LogIt(wxString::Format("End = %d", m_filegrid->GetColSize(4)));
    #endif

}



void NewProjectDialog::OnEditButton(wxCommandEvent &event)
{
    if(m_filegrid->GetSelectedRows().Count() != 1) {
        LogIt("Cannot edit multiple lines!");
        return;
    }
    int line = m_filegrid->GetSelectedRows().front();
    DialogEditRow(line);
}



void NewProjectDialog::OnRemoveButton(wxCommandEvent &event)
{
    wxArrayInt selected_rows = m_filegrid->GetSelectedRows();
    int row_delete = -1;
    int item_count = selected_rows.GetCount();
    int deleted_amount = 0;


    #ifdef IDZ80_NPRJD_DEBUG
    LogIt(wxString::Format("Remove %d row(s).", item_count));
    wxString str_debug = "[";
    for (int listitem = 0; listitem < item_count; listitem++) {
        str_debug << wxString::Format("%d ", selected_rows[listitem]);
    }
    str_debug.Trim();
    str_debug << "]";
    LogIt(str_debug);
    #endif

    if(item_count == 0)
        return;
    
    for(int kill_row = 0; kill_row < item_count; kill_row++)
    {
        row_delete = selected_rows[kill_row] - deleted_amount;
        if ((row_delete >= 0) || (row_delete < m_filegrid->GetNumberRows()))
        {
            m_filegrid->DeleteRows(row_delete, 1);
            m_main_dialog->Programs_->Remove(row_delete);
            deleted_amount++;
        }
        #ifdef IDZ80_NPRJD_DEBUG
        LogIt(wxString::Format("%d -> deleted = %d", row_delete, deleted_amount));
        #endif
    }
    m_actualrow = m_filegrid->GetNumberRows() - 1;

    if(m_filegrid->GetNumberRows() == 0) {
        m_OK_button->Disable();
        m_remove_button->Disable();
        m_edit_button->Disable();
    }

    m_filegrid->ForceRefresh();
}



void NewProjectDialog::OnOkButtonPressed(wxCommandEvent &event)
{
    EndModal(wxID_OK);
}



void NewProjectDialog::OnCancelButtonPressed(wxCommandEvent &event)
{
    if (!m_program_loaded)
        m_main_dialog->Programs_->Clear();

    EndModal(wxID_CANCEL);
}



void NewProjectDialog::OnGridSelectedRow(wxGridEvent &event)
{
    int line = event.GetRow();
    if(m_filegrid->GetGridCursorRow() >= 0) {
        m_edit_button->Enable();
        m_remove_button->Enable();
        m_main_dialog->Programs_->Index(line);
    }
}



void NewProjectDialog::OnGridLeftDoubleClick(wxGridEvent &event)
{
    int line = event.GetRow();
    DialogEditRow(line);
}

