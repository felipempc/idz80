#include "dialog_mnemonics.h"

#include "wx/button.h"
#include <wx/msgdlg.h>

Dialog_Mnemonics::Dialog_Mnemonics(wxWindow *parent, MnemonicContainer *mnemonics)
{
    Create(parent,wxID_ANY, "Mnemonic Viewer", wxPoint(100, 100), wxSize(150, 150));

    mnemonics_ = mnemonics;

    unsigned int counter = mnemonics_->GetCount();

    if (counter < 100)
    {
        mnemonics_ = 0;
        wxMessageBox(wxString::Format("Mnemonics not properly loaded ! (%d)", counter), "Error");
        Close();
    }

    BuildDialog();
}

Dialog_Mnemonics::~Dialog_Mnemonics()
{
    //dtor
}





void Dialog_Mnemonics::BuildDialog()
{
    wxBoxSizer *main_sizer = new wxBoxSizer(wxVERTICAL);
    wxPanel *main_panel = new wxPanel(this);
    this->SetSizer(main_sizer);

    // Instruction data panel
    wxPanel *group_panel;
    group_panel = new wxPanel(main_panel);

    wxBoxSizer *main_panel_sizer = new wxBoxSizer(wxHORIZONTAL);
    main_panel_sizer->Add(group_panel, wxSizerFlags(1).Border(wxALL, 5));

    main_panel->SetSizer(main_panel_sizer);

    wxPanel *group_line = new wxPanel(group_panel);
    wxBoxSizer *group_sizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText *text_group = new wxStaticText(group_line, wxID_ANY, "Group",wxPoint(5, 5));
    combo_groups_ = new wxComboBox(group_line, wxID_ANY, "", wxPoint(45, 3), wxSize(150, 22));
    group_sizer->Add(group_line, wxSizerFlags(1).Border(wxALL, 2));
    group_panel->SetSizer(group_sizer);

    wxPanel *string_line = new wxPanel(group_panel);
    wxStaticText *text_string = new wxStaticText(string_line, wxID_ANY, "String",wxPoint(5, 5));
    combo_strings_ = new wxComboBox(string_line, wxID_ANY, "", wxPoint(45, 3), wxSize(150, 22));
    group_sizer->Add(string_line, wxSizerFlags(1).Border(wxALL, 2));

    wxPanel *bytecode_line = new wxPanel(group_panel);
    wxStaticText *text_bytecode = new wxStaticText(bytecode_line, wxID_ANY, "Bytecode",wxPoint(5, 5));
    field_bytecode_ = new wxStaticText(bytecode_line, wxID_ANY, "01 BC 34",wxPoint(65, 5));
    group_sizer->Add(bytecode_line, wxSizerFlags(1).Border(wxALL, 2));

    wxPanel *size_line = new wxPanel(group_panel);
    wxStaticText *text_bc_size = new wxStaticText(size_line, wxID_ANY, "Size",wxPoint(5, 5));
    field_bc_size_ = new wxStaticText(size_line, wxID_ANY, "3",wxPoint(65, 5));
    group_sizer->Add(size_line, wxSizerFlags(1).Border(wxALL, 2));

    chkbox_conditional_ = new wxCheckBox(group_panel, wxID_ANY, "Conditional ", wxPoint(5,5));
    chkbox_conditional_->SetWindowStyle(wxALIGN_RIGHT);
    group_sizer->Add(chkbox_conditional_, wxSizerFlags(1).Border(wxALL, 2));

    // ----- Arguments ----
    wxStaticBoxSizer *arg_box_sizer = new wxStaticBoxSizer(wxVERTICAL, main_panel, "Arguments");
    main_panel_sizer->Add(arg_box_sizer, wxSizerFlags(1).Border(wxALL, 2));

    wxPanel *arg_count_line = new wxPanel(arg_box_sizer->GetStaticBox());
    wxStaticText *text_count = new wxStaticText(arg_count_line, wxID_ANY, "Count", wxPoint(0, 0));
    field_arg_count_ = new wxStaticText(arg_count_line, wxID_ANY, "1", wxPoint(65,0));
    arg_box_sizer->Add(arg_count_line, wxSizerFlags(1).Border(wxALL, 2).Align(wxALIGN_LEFT));

    wxPanel *arg_size_line = new wxPanel(arg_box_sizer->GetStaticBox());
    wxStaticText *text_arg_size = new wxStaticText(arg_size_line, wxID_ANY, "Size", wxPoint(0, 0));
    field_arg_size_ = new wxStaticText(arg_size_line, wxID_ANY, "1", wxPoint(65,0));
    arg_box_sizer->Add(arg_size_line, wxSizerFlags(1).Border(wxALL, 2));

    wxPanel *arg_pos_line = new wxPanel(arg_box_sizer->GetStaticBox());
    wxStaticText *text_position = new wxStaticText(arg_pos_line, wxID_ANY, "Position", wxPoint(0, 0));
    field_arg_pos = new wxStaticText(arg_pos_line, wxID_ANY, "1", wxPoint(65,0));
    arg_box_sizer->Add(arg_pos_line, wxSizerFlags(1).Border(wxALL, 2));

    chkbox_explicit_ = new wxCheckBox(arg_box_sizer->GetStaticBox(), wxID_ANY, "Explicit      ", wxPoint(0, 0));
    chkbox_explicit_->SetWindowStyle(wxALIGN_RIGHT);
    arg_box_sizer->Add(chkbox_explicit_, wxSizerFlags(1).Border(wxALL, 2));

    wxPanel *arg_source_line = new wxPanel(arg_box_sizer->GetStaticBox());
    wxStaticText *text_source = new wxStaticText(arg_source_line, wxID_ANY, "Source", wxPoint(0, 0));
    field_arg_source = new wxStaticText(arg_source_line, wxID_ANY, "A", wxPoint(65, 0));
    arg_box_sizer->Add(arg_source_line, wxSizerFlags(1).Border(wxALL, 2));

    wxPanel *arg_dest_line = new wxPanel(arg_box_sizer->GetStaticBox());
    wxStaticText *text_destination = new wxStaticText(arg_dest_line, wxID_ANY, "Destination", wxPoint(0, 0));
    field_arg_dest = new wxStaticText(arg_dest_line, wxID_ANY, "B", wxPoint(65, 0));
    arg_box_sizer->Add(arg_dest_line, wxSizerFlags(1).Border(wxALL, 2));

    // Navigation panel
    wxPanel *navigate_panel = new wxPanel(this);
    wxBoxSizer *navigate_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *previousbutton = new wxButton(navigate_panel, PREVIOUS_INST, "Previous");
    wxButton *nextbutton = new wxButton(navigate_panel, NEXT_INST, "Next");
    navigate_panel->SetSizer(navigate_sizer);
    navigate_sizer->AddSpacer(10);
    navigate_sizer->Add(previousbutton, wxSizerFlags(1).Border(wxALL, 20));
    navigate_sizer->AddSpacer(10);
    navigate_sizer->Add(nextbutton, wxSizerFlags(1).Border(wxALL, 20));
    navigate_sizer->AddSpacer(10);

    main_sizer->Add(main_panel, wxSizerFlags(0).Border(wxALL, 5));
    main_sizer->Add(navigate_panel, wxSizerFlags(0).Border(wxALL, 5).Expand());
    main_sizer->Fit(this);

}
