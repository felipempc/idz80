

#include "newproject_dialog.h"
#include "wx/filedlg.h"
#include "wx/bmpbuttn.h"




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

    file_panel->SetSizer(file_sizer);

    main_sizer_->Add(file_panel, wxSizerFlags(0).Border(wxALL, 10));

    filegrid_ = new wxGrid(main_panel, wxID_ANY, wxPoint(0, 0), wxSize(500, 300));
    filegrid_->CreateGrid(0, 5);
    filegrid_->SetColLabelValue(0, "Name");
    filegrid_->SetColLabelValue(1, "Type");
    filegrid_->SetColLabelValue(2, "Start");
    filegrid_->SetColLabelValue(3, "Execution");
    filegrid_->SetColLabelValue(4, "End");
    main_sizer_->Add(filegrid_, wxSizerFlags(0).Border(wxALL, 10));

    main_panel->SetSizer(main_sizer_);
    main_sizer_->Fit(this);
}




void NewProjectDialog::OnResize(wxSizeEvent &event)
{

    main_sizer_->Fit(this);
}




void NewProjectDialog::OnAddButton(wxCommandEvent &event)
{
    wxString filename = DialogLoadProgramFile();

    if(filename.IsEmpty() || (!main_->Programs_->AddFile(filename)))
    {
            wxMessageBox(wxString::Format("Error while openning file [%s] !", filename), "Error opening file !");
            return;
    }

    filegrid_->AppendRows();
    actualrow_++;

    filegrid_->SetCellValue(actualrow_, 0, main_->Programs_->Current()->GetFileName());
    filegrid_->SetCellValue(actualrow_, 1, main_->Programs_->Current()->GetFileTypeStr());
}




wxString NewProjectDialog::DialogLoadProgramFile()
{
    wxString caption = "Choose a file";
    wxString wildcard = "Program files (*.ROM, *.COM, *.BIN)|*.rom;*.com;*.bin|All files (*.*)|*.*";
    wxFileDialog dialog(this, caption, main_->fileopen_last_dir_, wxEmptyString, wildcard, wxFD_OPEN);
    wxString ret = "";

    if (dialog.ShowModal() == wxID_OK)
    {
        ret = dialog.GetPath();
        main_->fileopen_last_dir_ = dialog.GetDirectory();
    }

    return ret;
}




void NewProjectDialog::OnRemoveButton(wxCommandEvent &event)
{
    if(filegrid_->GetNumberRows() != 0)
    {
        filegrid_->DeleteRows(filegrid_->GetNumberRows() - 1);
        if(actualrow_ > -1)
            actualrow_--;
    }
}



