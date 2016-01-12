

#include "newproject_dialog.h"
#include "wx/filedlg.h"
#include "wx/bmpbuttn.h"
#include "wx/dynarray.h" // DEBUG




NewProjectDialog::NewProjectDialog(IDZ80MainBase *parent)
{
    main_ = parent;
    ModuleName = "NPDialog";


    add_icon_ = new wxBitmap(main_->app_resource_dir_ + "Plus icon.bmp", wxBITMAP_TYPE_BMP);
    remove_icon_ = new wxBitmap(main_->app_resource_dir_ + "Minus icon.bmp", wxBITMAP_TYPE_BMP);
    actualrow_ = -1;

    BuildDialog();
    Bind(wxEVT_SIZE, &NewProjectDialog::OnResize, this);
    Bind(wxEVT_BUTTON, &NewProjectDialog::OnAddButton, this, idADD_FILE_BUTTON);
    Bind(wxEVT_BUTTON, &NewProjectDialog::OnRemoveButton, this, idREMOVE_FILE_BUTTON);
    //Bind(wxEVT_BUTTON, &NewProjectDialog::OnUpdateButton, this, idUPDATE_DEBUG_STR);
    Bind(wxEVT_GRID_CELL_LEFT_CLICK, &NewProjectDialog::OnGridLeftClick, this);
    Bind(wxEVT_GRID_LABEL_LEFT_CLICK, &NewProjectDialog::OnGridLeftClick, this);
}




NewProjectDialog::~NewProjectDialog()
{
    delete add_icon_;
}




void NewProjectDialog::BuildDialog()
{
    Create(0, wxID_ANY, "Project", wxPoint(100, 100), wxSize(500, 500), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER, "NewProject");
    wxPanel *main_panel = new wxPanel(this);
    main_sizer_ = new wxBoxSizer(wxVERTICAL);

    wxPanel *file_panel = new wxPanel(main_panel);
    wxBoxSizer *file_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxBitmapButton *file_picker_button = new wxBitmapButton(file_panel, idADD_FILE_BUTTON, *add_icon_);
    file_sizer->Add(file_picker_button, wxSizerFlags(0).Border(wxALL, 0));
    wxBitmapButton *file_remove_button = new wxBitmapButton(file_panel, idREMOVE_FILE_BUTTON, *remove_icon_);
    file_sizer->Add(file_remove_button, wxSizerFlags(0).Border(wxLEFT | wxRIGHT, 10));

    /*DEBUG
    debugtext_ = new wxStaticText(file_panel, wxID_ANY, "[0 0 0 0]");
    file_sizer->Add(debugtext_, wxSizerFlags(0).Border(wxLEFT | wxRIGHT, 10));

    wxButton *UpdateButton = new wxButton(file_panel, idUPDATE_DEBUG_STR, "Update");
    file_sizer->Add(UpdateButton, wxSizerFlags(0).Border(wxLEFT | wxRIGHT, 10));
    */
    file_panel->SetSizer(file_sizer);

    main_sizer_->Add(file_panel, wxSizerFlags(0).Border(wxALL, 10));

    filegrid_ = new wxGrid(main_panel, wxID_ANY, wxPoint(0, 0), wxSize(500, 300));
    filegrid_->CreateGrid(0, 5);
    filegrid_->SetSelectionMode(wxGrid::wxGridSelectRows);
    filegrid_->SetColLabelValue(0, "Name");
    filegrid_->SetColLabelValue(1, "Type");
    filegrid_->SetColLabelValue(2, "Start");
    filegrid_->SetColLabelValue(3, "Execution");
    filegrid_->SetColLabelValue(4, "End");
    main_sizer_->Add(filegrid_, wxSizerFlags(0).Border(wxALL, 10));
    filegrid_->EnableEditing(false);

    main_panel->SetSizer(main_sizer_);
    main_sizer_->Fit(this);
}




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
}




void NewProjectDialog::OnRemoveButton(wxCommandEvent &event)
{
    wxArrayInt selected_rows = filegrid_->GetSelectedRows();
    int minor_item = 0xFFFF,
        item_count = selected_rows.GetCount();

    if(item_count == 0)
        return;

    for(int kill_row = 0; kill_row < item_count; kill_row++)
    {
        if(selected_rows[kill_row] < minor_item)
            minor_item = selected_rows[kill_row];
    }

    if(minor_item < item_count)
    {
        filegrid_->DeleteRows(minor_item, item_count);
        actualrow_ -= item_count;
    }
}



/*
void NewProjectDialog::UpdateDebugString()
{
    wxArrayInt selected_rows = filegrid_->GetSelectedRows();
    wxString str_text = "[";
    int selected_num = selected_rows.GetCount();

    if (selected_num > 0)
    {
        for(int f = 0; f < selected_num; f++)
            str_text << wxString::Format("%d ", selected_rows[f]);

        str_text.Trim();
    }
    else
        str_text << "none";

    str_text << "]";
    debugtext_->SetLabel(str_text);
}
*/



/*
void NewProjectDialog::OnUpdateButton(wxCommandEvent& event)
{
    UpdateDebugString();
}
*/



void NewProjectDialog::OnGridLeftClick(wxGridEvent& event)
{
    if(!event.ControlDown())
        event.Skip();
}
