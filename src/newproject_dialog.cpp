

#include "newproject_dialog.h"
#include "wx/filedlg.h"
#include "wx/bmpbuttn.h"
#include "wx/dynarray.h" // DEBUG




NewProjectDialog::NewProjectDialog(IDZ80MainBase *parent)
{
    main_ = parent;
    ModuleName = "NPDialog";
    SetTextLog(main_->GetTextLog());

    actualrow_ = -1;

    BuildDialog();
    Bind(wxEVT_SIZE, &NewProjectDialog::OnResize, this);
    Bind(wxEVT_BUTTON, &NewProjectDialog::OnAddButton, this, idADD_FILE_BUTTON);
    Bind(wxEVT_BUTTON, &NewProjectDialog::OnRemoveButton, this, idREMOVE_FILE_BUTTON);
    Bind(wxEVT_BUTTON, &NewProjectDialog::OnOkButtonPressed, this, wxID_OK);
    Bind(wxEVT_BUTTON, &NewProjectDialog::OnCancelButtonPressed, this, wxID_CANCEL);
    Bind(wxEVT_GRID_CELL_LEFT_DCLICK, &NewProjectDialog::OnGridLeftDoubleClick, this, idGRID_LEFT_DCLICK);
}




NewProjectDialog::~NewProjectDialog()
{
    // Debug
    LogIt("Destroyed.");

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
    OK_button_->Disable();
    buttons_sizer->Add(OK_button_, wxSizerFlags(0).Border(wxALL, 10));
    Add_button_ = new wxButton(buttons_panel, idADD_FILE_BUTTON, "Add", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "AddButton");
    buttons_sizer->Add(Add_button_, wxSizerFlags(0).Border(wxALL, 10));
    Remove_button_ = new wxButton(buttons_panel, idREMOVE_FILE_BUTTON, "Remove", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "RemoveButton");
    Remove_button_->Disable();
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
    wxFileDialog dialog(this, caption, main_->fileopen_last_dir_, wxEmptyString, wildcard, wxFD_OPEN | wxFD_MULTIPLE);
    bool ret = false;

    if (dialog.ShowModal() == wxID_OK)
    {
        ret = true;
        dialog.GetPaths(file_list_str);
        main_->fileopen_last_dir_ = dialog.GetDirectory();
    }

    return ret;
}





bool NewProjectDialog::AddFileToGrid(wxString& filestr)
{
    bool ret = false;

    if(main_->Programs_->AddFile(filestr))
    {
        filegrid_->AppendRows();
        actualrow_++;

        filegrid_->SetCellValue(actualrow_, 0, main_->Programs_->Current()->GetFileName());
        filegrid_->SetCellValue(actualrow_, 1, main_->Programs_->Current()->GetFileTypeStr());
        filegrid_->SetCellAlignment(actualrow_, 1, wxALIGN_CENTRE, wxALIGN_CENTRE);
        filegrid_->SetCellValue(actualrow_, 2, wxString::Format("0x%.4X", main_->Programs_->Current()->StartAddress));
        filegrid_->SetCellAlignment(actualrow_, 2, wxALIGN_CENTRE, wxALIGN_CENTRE);
        filegrid_->SetCellValue(actualrow_, 3, wxString::Format("0x%.4X", main_->Programs_->Current()->ExecAddress));
        filegrid_->SetCellAlignment(actualrow_, 3, wxALIGN_CENTRE, wxALIGN_CENTRE);
        filegrid_->SetCellValue(actualrow_, 4, wxString::Format("0x%.4X", main_->Programs_->Current()->EndAddress));
        filegrid_->SetCellAlignment(actualrow_, 4, wxALIGN_CENTRE, wxALIGN_CENTRE);
        filegrid_->AutoSizeColumns();
        ret = true;
    }
    else
    {
        wxMessageBox(wxString::Format("Error while openning file [%s] !", filestr), "Error opening file !");
    }
    return ret;
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
    Remove_button_->Enable();
    
    // DEBUG
    /*
    LogIt("Size of the Columns:");
    LogIt(wxString::Format("Name = %d", filegrid_->GetColSize(0)));
    LogIt(wxString::Format("Type = %d", filegrid_->GetColSize(1)));
    LogIt(wxString::Format("Start = %d", filegrid_->GetColSize(2)));
    LogIt(wxString::Format("Execution = %d", filegrid_->GetColSize(3)));
    LogIt(wxString::Format("End = %d", filegrid_->GetColSize(4)));
    */
}




void NewProjectDialog::OnRemoveButton(wxCommandEvent &event)
{
    wxArrayInt selected_rows = filegrid_->GetSelectedRows();
    int row_delete = -1;
    int item_count = selected_rows.GetCount();
    int deleted_amount = 0;

    //DEBUG
    /*
    LogIt(wxString::Format("Remove %d row(s).", item_count));
    wxString str_debug = "[";
    for (int listitem = 0; listitem < item_count; listitem++) {
        str_debug << wxString::Format("%d ", selected_rows[listitem]);
    }
    str_debug.Trim();
    str_debug << "]";
    LogIt(str_debug);
    */
    // <- DEBUG

    if(item_count == 0)
        return;
    
    for(int kill_row = 0; kill_row < item_count; kill_row++)
    {
        row_delete = selected_rows[kill_row] - deleted_amount;
        if ((row_delete >= 0) || (row_delete < filegrid_->GetNumberRows()))
        {
            filegrid_->DeleteRows(row_delete, 1);
            main_->Programs_->Remove(row_delete);
            deleted_amount++;
        }
        //DEBUG
        //LogIt(wxString::Format("%d -> deleted = %d", row_delete, deleted_amount));
    }
    actualrow_ = filegrid_->GetNumberRows() - 1;

    if(filegrid_->GetNumberRows() == 0) {
        OK_button_->Disable();
        Remove_button_->Disable();
    }

    filegrid_->ForceRefresh();
}



void NewProjectDialog::OnOkButtonPressed(wxCommandEvent &event)
{
    EndModal(wxID_OK);
}

void NewProjectDialog::OnCancelButtonPressed(wxCommandEvent &event)
{
    main_->Programs_->Clear();
    EndModal(wxID_CANCEL);
}

void NewProjectDialog::OnGridLeftDoubleClick(wxGridEvent &event)
{
    int line = event.GetRow();
    LogIt(wxString::Format("Left double clicked on line %d", line));
}

