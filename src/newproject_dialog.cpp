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


NewProjectDialog::NewProjectDialog(IDZ80MainBase *parent)
{
    main_dialog_ = parent;
    ModuleName = "NPDialog";
    SetTextLog(main_dialog_->GetTextLog());

    actualrow_ = -1;

    BuildDialog();
    Bind(wxEVT_SIZE, &NewProjectDialog::OnResize, this);
    Bind(wxEVT_BUTTON, &NewProjectDialog::OnAddButton, this, idADD_FILE_BUTTON);
    Bind(wxEVT_BUTTON, &NewProjectDialog::OnEditButton, this, wxID_EDIT);
    Bind(wxEVT_BUTTON, &NewProjectDialog::OnRemoveButton, this, idREMOVE_FILE_BUTTON);
    Bind(wxEVT_BUTTON, &NewProjectDialog::OnOkButtonPressed, this, wxID_OK);
    Bind(wxEVT_BUTTON, &NewProjectDialog::OnCancelButtonPressed, this, wxID_CANCEL);
    Bind(wxEVT_GRID_SELECT_CELL, &NewProjectDialog::OnGridSelectedRow, this);
    Bind(wxEVT_GRID_CELL_LEFT_DCLICK, &NewProjectDialog::OnGridLeftDoubleClick, this);

    if (main_dialog_->Programs_->Count() != 0) {
        for(int j = 0; j < main_dialog_->Programs_->Count(); j++){
            FillRow(main_dialog_->Programs_->Index(j));
        }
        program_loaded = true;
    }
    else {
        OK_button_->Disable();
        program_loaded = false;
    }

    Edit_button_->Disable();
    Remove_button_->Disable();
}




NewProjectDialog::~NewProjectDialog()
{
    #ifdef IDZ80_NPRJD_DEBUG
    LogIt("Destroyed.");
    #endif

    delete filegrid_;
}




void NewProjectDialog::BuildDialog()
{
    Create(0, wxID_ANY, "Project", wxPoint(100, 100), wxSize(500, 500), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER, "NewProject");
    wxPanel *main_panel = new wxPanel(this);
    main_sizer_ = new wxBoxSizer(wxHORIZONTAL);

    wxPanel *buttons_panel = new wxPanel(main_panel);
    wxBoxSizer *buttons_sizer = new wxBoxSizer(wxVERTICAL);

    // Grid 
    filegrid_ = new wxGrid(main_panel, wxID_ANY, wxPoint(0, 0), wxSize(500, 300));
    filegrid_->CreateGrid(0, 5);
    filegrid_->SetSelectionMode(wxGrid::wxGridSelectRows);
    filegrid_->SetColLabelValue(0, "Name");
    filegrid_->SetColMinimalWidth(0, 90);
    filegrid_->SetColLabelValue(1, "Type");
    filegrid_->SetColMinimalWidth(1, 40);
    filegrid_->SetColLabelValue(2, "Start");
    filegrid_->SetColMinimalWidth(2, 55);
    filegrid_->SetColLabelValue(3, "Execution");
    filegrid_->SetColMinimalWidth(3, 65);
    filegrid_->SetColLabelValue(4, "End");
    filegrid_->SetColMinimalWidth(4, 55);
    main_sizer_->Add(filegrid_, wxSizerFlags(0).Border(wxALL, 10));
    filegrid_->EnableEditing(false);

    // Buttons_panel
    OK_button_ = new wxButton(buttons_panel, wxID_OK, "OK", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "wxID_OK");
    buttons_sizer->Add(OK_button_, wxSizerFlags(0).Border(wxALL, 10));
    Edit_button_ = new wxButton(buttons_panel, wxID_EDIT, "Edit", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "wxID_EDIT");
    buttons_sizer->Add(Edit_button_, wxSizerFlags(0).Border(wxALL, 10));
    Add_button_ = new wxButton(buttons_panel, idADD_FILE_BUTTON, "Add", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "AddButton");
    buttons_sizer->Add(Add_button_, wxSizerFlags(0).Border(wxALL, 10));
    Remove_button_ = new wxButton(buttons_panel, idREMOVE_FILE_BUTTON, "Remove", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "RemoveButton");
    buttons_sizer->Add(Remove_button_, wxSizerFlags(0).Border(wxALL, 10));
    Cancel_button_ = new wxButton(buttons_panel, wxID_CANCEL, "Cancel", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "wxID_CANCEL");
    buttons_sizer->Add(Cancel_button_, wxSizerFlags(0).Border(wxALL, 10));
    buttons_panel->SetSizer(buttons_sizer);
    main_sizer_->Add(buttons_panel, wxSizerFlags(0).Border(wxALL, 10));

    main_panel->SetSizer(main_sizer_);
    main_sizer_->Fit(this);
}




/// @brief Open a file picker dialog and return an array of string with the full name of the selected file(s). Save current directory in main_.
/// @param file_list_str list with full name of the selected file(s)
/// @return true if success
bool NewProjectDialog::DialogLoadProgramFile(wxArrayString &file_list_str)
{
    wxString caption = "Choose a file";
    wxString wildcard = "Program files (*.ROM, *.COM, *.BIN)|*.rom;*.com;*.bin|All files (*.*)|*.*";
    wxFileDialog dialog(this, caption, main_dialog_->fileopen_last_dir_, wxEmptyString, wildcard, wxFD_OPEN | wxFD_MULTIPLE);
    bool ret = false;

    if (dialog.ShowModal() == wxID_OK)
    {
        ret = true;
        dialog.GetPaths(file_list_str);
        main_dialog_->fileopen_last_dir_ = dialog.GetDirectory();
    }

    return ret;
}



bool NewProjectDialog::AddFileToGrid(wxString& filestr)
{
    RawData *program = 0;

    program = main_dialog_->Programs_->AddFile(filestr);
    if (!program) {
        wxMessageBox(wxString::Format("Error while openning file [%s] !", filestr), "Error opening file !");
        return false;
    }
    FillRow(program);
    return true;
}



void NewProjectDialog::FillRow(RawData *program)
{
    filegrid_->AppendRows();
    actualrow_++;

    filegrid_->SetCellValue(actualrow_, 0, program->GetFileName());
    filegrid_->SetCellValue(actualrow_, 1, program->GetFileTypeStr());
    filegrid_->SetCellAlignment(actualrow_, 1, wxALIGN_CENTRE, wxALIGN_CENTRE);
    filegrid_->SetCellValue(actualrow_, 2, wxString::Format("0x%.4X", program->StartAddress));
    filegrid_->SetCellAlignment(actualrow_, 2, wxALIGN_CENTRE, wxALIGN_CENTRE);
    filegrid_->SetCellValue(actualrow_, 3, wxString::Format("0x%.4X", program->ExecAddress));
    filegrid_->SetCellAlignment(actualrow_, 3, wxALIGN_CENTRE, wxALIGN_CENTRE);
    filegrid_->SetCellValue(actualrow_, 4, wxString::Format("0x%.4X", program->EndAddress));
    filegrid_->SetCellAlignment(actualrow_, 4, wxALIGN_CENTRE, wxALIGN_CENTRE);
    filegrid_->AutoSizeColumns();
}


void NewProjectDialog::DialogEditRow(int line)
{
    RawData *program;

    if ((line >= 0) || (line < main_dialog_->Programs_->Count())) {
        program = main_dialog_->Programs_->Index(line);
        if (program == 0)
            return;
        
        FileSettingsDialog config(main_dialog_);
        config.ShowModal();
    }
}



void NewProjectDialog::OnResize(wxSizeEvent &event)
{
    main_sizer_->Fit(this);
}




void NewProjectDialog::OnAddButton(wxCommandEvent &event)
{
    wxArrayString file_list;

    if(!DialogLoadProgramFile(file_list))
        return;

    for(int idx = 0; idx < file_list.GetCount(); idx++)
        AddFileToGrid(file_list[idx]);
    
    OK_button_->Enable();
    
    #ifdef IDZ80_NPRJD_DEBUG
    LogIt("Size of the Columns:");
    LogIt(wxString::Format("Name = %d", filegrid_->GetColSize(0)));
    LogIt(wxString::Format("Type = %d", filegrid_->GetColSize(1)));
    LogIt(wxString::Format("Start = %d", filegrid_->GetColSize(2)));
    LogIt(wxString::Format("Execution = %d", filegrid_->GetColSize(3)));
    LogIt(wxString::Format("End = %d", filegrid_->GetColSize(4)));
    #endif

}



void NewProjectDialog::OnEditButton(wxCommandEvent &event)
{
    if(filegrid_->GetSelectedRows().Count() != 1) {
        LogIt("Cannot edit multiple lines!");
        return;
    }
    int line = filegrid_->GetSelectedRows().front();
    DialogEditRow(line);
}



void NewProjectDialog::OnRemoveButton(wxCommandEvent &event)
{
    wxArrayInt selected_rows = filegrid_->GetSelectedRows();
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
        if ((row_delete >= 0) || (row_delete < filegrid_->GetNumberRows()))
        {
            filegrid_->DeleteRows(row_delete, 1);
            main_dialog_->Programs_->Remove(row_delete);
            deleted_amount++;
        }
        #ifdef IDZ80_NPRJD_DEBUG
        LogIt(wxString::Format("%d -> deleted = %d", row_delete, deleted_amount));
        #endif
    }
    actualrow_ = filegrid_->GetNumberRows() - 1;

    if(filegrid_->GetNumberRows() == 0) {
        OK_button_->Disable();
        Remove_button_->Disable();
        Edit_button_->Disable();
    }

    filegrid_->ForceRefresh();
}



void NewProjectDialog::OnOkButtonPressed(wxCommandEvent &event)
{
    EndModal(wxID_OK);
}



void NewProjectDialog::OnCancelButtonPressed(wxCommandEvent &event)
{
    if (!program_loaded)
        main_dialog_->Programs_->Clear();

    EndModal(wxID_CANCEL);
}



void NewProjectDialog::OnGridSelectedRow(wxGridEvent &event)
{
    int line = event.GetRow();
    if(filegrid_->GetGridCursorRow() >= 0) {
        Edit_button_->Enable();
        Remove_button_->Enable();
        main_dialog_->Programs_->Index(line);
    }
}



void NewProjectDialog::OnGridLeftDoubleClick(wxGridEvent &event)
{
    int line = event.GetRow();
    DialogEditRow(line);
}

