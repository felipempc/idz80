#include "dialog_mnemonics.h"
#include "wx/stattext.h"
#include "wx/combobox.h"
#include "wx/checkbox.h"

Dialog_Mnemonics::Dialog_Mnemonics(wxWindow *parent)
{
    Create(parent,wxID_ANY, "Mnemonic Viewer");
    wxBoxSizer *main_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxPanel *main_panel = new wxPanel(this);
    main_sizer->Add(main_panel, wxSizerFlags(1).Border(wxALL, 5));
    this->SetSizer(main_sizer);

    wxPanel *group_panel, *arg_panel;
    group_panel = new wxPanel(main_panel);
    arg_panel = new wxPanel(main_panel);

    wxBoxSizer *main_panel_sizer = new wxBoxSizer(wxHORIZONTAL);
    main_panel_sizer->Add(group_panel, wxSizerFlags(1).Border(wxALL, 5));
    main_panel_sizer->Add(arg_panel, wxSizerFlags(1).Border(wxALL, 5));
    main_panel->SetSizer(main_panel_sizer);

    wxPanel *group_line = new wxPanel(group_panel);
    wxBoxSizer *group_sizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText *text_group = new wxStaticText(group_line, wxID_ANY, "Group",wxPoint(5, 5));
    wxComboBox *combo_group = new wxComboBox(group_line, wxID_ANY, "", wxPoint(45, 3), wxSize(150, 22));
    group_sizer->Add(group_line, wxSizerFlags(1).Border(wxALL, 2));
    group_panel->SetSizer(group_sizer);

    wxPanel *string_line = new wxPanel(group_panel);
    wxStaticText *text_string = new wxStaticText(string_line, wxID_ANY, "String",wxPoint(5, 5));
    wxComboBox *combo_string = new wxComboBox(string_line, wxID_ANY, "", wxPoint(45, 3), wxSize(150, 22));
    group_sizer->Add(string_line, wxSizerFlags(1).Border(wxALL, 2));

    wxPanel *bytecode_line = new wxPanel(group_panel);
    wxStaticText *text_bytecode = new wxStaticText(bytecode_line, wxID_ANY, "Bytecode",wxPoint(5, 5));
    wxStaticText *text_bc_value = new wxStaticText(bytecode_line, wxID_ANY, "01 BC 34",wxPoint(65, 5));
    group_sizer->Add(bytecode_line, wxSizerFlags(1).Border(wxALL, 2));

    wxPanel *size_line = new wxPanel(group_panel);
    wxStaticText *text_bc_size = new wxStaticText(size_line, wxID_ANY, "Size",wxPoint(5, 5));
    wxStaticText *text_bcsz_value = new wxStaticText(size_line, wxID_ANY, "3",wxPoint(65, 5));
    group_sizer->Add(size_line, wxSizerFlags(1).Border(wxALL, 2));

    //wxPanel *conditional_line = new wxPanel(group_panel);
    //wxStaticText *text_conditional = new wxStaticText(conditional_line, wxID_ANY, "Conditional",wxPoint(5, 5));
    wxCheckBox *check_conditional = new wxCheckBox(group_panel, wxID_ANY, "Conditional", wxPoint(5,5));
    check_conditional->SetWindowStyle(wxALIGN_RIGHT);
    group_sizer->Add(check_conditional, wxSizerFlags(1).Border(wxALL, 2));



    wxStaticText *text_arguments = new wxStaticText(arg_panel, wxID_ANY, "Arguments");
}

Dialog_Mnemonics::~Dialog_Mnemonics()
{
    //dtor
}
