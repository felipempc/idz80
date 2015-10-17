#include "dialog_mnemonics.h"

#include "wx/button.h"
#include <wx/msgdlg.h>
#include "mnemonic_item.h"
#include "mnemonicxmlfile.h"


const long Dialog_Mnemonics::idNAV_NEXT = wxNewId();
const long Dialog_Mnemonics::idNAV_PREVIOUS = wxNewId();
const long Dialog_Mnemonics::idSELECT_GROUP = wxNewId();
const long Dialog_Mnemonics::idSELECT_INSTRUCTION = wxNewId();


Dialog_Mnemonics::Dialog_Mnemonics(wxWindow *parent, MnemonicContainer *mnemonics)
{
    Create(parent,wxID_ANY, "Mnemonic Viewer", wxPoint(100, 100), wxSize(150, 150));

    mnemonics_ = mnemonics;

    BuildDialog();

    Bind(wxEVT_CLOSE_WINDOW, &Dialog_Mnemonics::OnCloseDialog, this, wxID_CLOSE);
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Dialog_Mnemonics::OnButtonNavNext, this, idNAV_NEXT);
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Dialog_Mnemonics::OnButtonNavPrevious, this, idNAV_PREVIOUS);
    Bind(wxEVT_CHOICE, &Dialog_Mnemonics::OnInstructionSelect, this, idSELECT_INSTRUCTION);
    Bind(wxEVT_CHOICE, &Dialog_Mnemonics::OnGroupSelect, this, idSELECT_GROUP);
    Bind(wxEVT_IDLE, &Dialog_Mnemonics::OnFirstIdle, this);

    combo_groups_->Enable(false);
    combo_strings_->Enable(false);
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
    combo_groups_ = new wxChoice(group_line, idSELECT_GROUP, wxPoint(45, 3), wxSize(150, 22));
    group_sizer->Add(group_line, wxSizerFlags(1).Border(wxALL, 2));
    group_panel->SetSizer(group_sizer);

    wxPanel *string_line = new wxPanel(group_panel);
    wxStaticText *text_string = new wxStaticText(string_line, wxID_ANY, "String",wxPoint(5, 5));
    combo_strings_ = new wxChoice(string_line, idSELECT_INSTRUCTION, wxPoint(45, 3), wxSize(150, 22));
    group_sizer->Add(string_line, wxSizerFlags(1).Border(wxALL, 2));

    panel_bytecode_ = new wxPanel(group_panel);
    wxStaticText *text_bytecode = new wxStaticText(panel_bytecode_, wxID_ANY, "Bytecode",wxPoint(5, 5));
    field_bytecode_ = new wxStaticText(panel_bytecode_, wxID_ANY, "Loading...please wait.",wxPoint(65, 5));
    group_sizer->Add(panel_bytecode_, wxSizerFlags(1).Border(wxALL, 2));

    wxPanel *size_line = new wxPanel(group_panel);
    wxStaticText *text_bc_size = new wxStaticText(size_line, wxID_ANY, "Size",wxPoint(5, 5));
    field_bc_size_ = new wxStaticText(size_line, wxID_ANY, "xx",wxPoint(65, 5));
    group_sizer->Add(size_line, wxSizerFlags(1).Border(wxALL, 2));

    chkbox_conditional_ = new wxCheckBox(group_panel, wxID_ANY, "Conditional ", wxPoint(5,5));
    chkbox_conditional_->SetWindowStyle(wxALIGN_RIGHT);
    group_sizer->Add(chkbox_conditional_, wxSizerFlags(1).Border(wxALL, 2));


    // ----- Arguments ----
    wxStaticBoxSizer *arg_box_sizer = new wxStaticBoxSizer(wxVERTICAL, main_panel, "Arguments");
    main_panel_sizer->Add(arg_box_sizer, wxSizerFlags(1).Border(wxALL, 2));

    wxPanel *arg_count_line = new wxPanel(arg_box_sizer->GetStaticBox());
    wxStaticText *text_count = new wxStaticText(arg_count_line, wxID_ANY, "Count", wxPoint(0, 0));
    field_arg_count_ = new wxStaticText(arg_count_line, wxID_ANY, "xx", wxPoint(65,0));
    arg_box_sizer->Add(arg_count_line, wxSizerFlags(1).Border(wxALL, 2).Align(wxALIGN_LEFT));

    wxPanel *arg_size_line = new wxPanel(arg_box_sizer->GetStaticBox());
    wxStaticText *text_arg_size = new wxStaticText(arg_size_line, wxID_ANY, "Size", wxPoint(0, 0));
    field_arg_size_ = new wxStaticText(arg_size_line, wxID_ANY, "xx", wxPoint(65,0));
    arg_box_sizer->Add(arg_size_line, wxSizerFlags(1).Border(wxALL, 2));

    wxPanel *arg_pos_line = new wxPanel(arg_box_sizer->GetStaticBox());
    wxStaticText *text_position = new wxStaticText(arg_pos_line, wxID_ANY, "Position", wxPoint(0, 0));
    field_arg_pos_ = new wxStaticText(arg_pos_line, wxID_ANY, "xx", wxPoint(65,0));
    arg_box_sizer->Add(arg_pos_line, wxSizerFlags(1).Border(wxALL, 2));

    chkbox_explicit_ = new wxCheckBox(arg_box_sizer->GetStaticBox(), wxID_ANY, "Explicit      ", wxPoint(0, 0));
    chkbox_explicit_->SetWindowStyle(wxALIGN_RIGHT);
    arg_box_sizer->Add(chkbox_explicit_, wxSizerFlags(1).Border(wxALL, 2));

    panel_arg_source_ = new wxPanel(arg_box_sizer->GetStaticBox());
    wxStaticText *text_source = new wxStaticText(panel_arg_source_, wxID_ANY, "Source", wxPoint(0, 0));
    field_arg_source_ = new wxStaticText(panel_arg_source_, wxID_ANY, "xx", wxPoint(65, 0));
    arg_box_sizer->Add(panel_arg_source_, wxSizerFlags(1).Border(wxALL, 2));

    panel_arg_srctype_ = new wxPanel(arg_box_sizer->GetStaticBox());
    wxStaticText *text_srctype = new wxStaticText(panel_arg_srctype_, wxID_ANY, "Type", wxPoint(0, 0));
    field_arg_srctype_ = new wxStaticText(panel_arg_srctype_, wxID_ANY, "xx", wxPoint(65, 0));
    arg_box_sizer->Add(panel_arg_srctype_, wxSizerFlags(1).Border(wxALL, 2));


    panel_arg_dest_ = new wxPanel(arg_box_sizer->GetStaticBox());
    wxStaticText *text_destination = new wxStaticText(panel_arg_dest_, wxID_ANY, "Destination", wxPoint(0, 0));
    field_arg_dest_ = new wxStaticText(panel_arg_dest_, wxID_ANY, "xx", wxPoint(65, 0));
    arg_box_sizer->Add(panel_arg_dest_, wxSizerFlags(1).Border(wxALL, 2));

    panel_arg_desttype_ = new wxPanel(arg_box_sizer->GetStaticBox());
    wxStaticText *text_desttype_ = new wxStaticText(panel_arg_desttype_, wxID_ANY, "Type", wxPoint(0, 0));
    field_arg_desttype_ = new wxStaticText(panel_arg_desttype_, wxID_ANY, "xx", wxPoint(65, 0));
    arg_box_sizer->Add(panel_arg_desttype_, wxSizerFlags(1).Border(wxALL, 2));


    // Navigation panel
    wxPanel *navigate_panel = new wxPanel(this);
    wxBoxSizer *navigate_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *previousbutton = new wxButton(navigate_panel, idNAV_PREVIOUS, "Previous");
    wxButton *nextbutton = new wxButton(navigate_panel, idNAV_NEXT, "Next");
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





void Dialog_Mnemonics::GatherGroupsAndStrings()
{
    wxString        mnemonic_str;
    MnemonicItem    *actual = 0;
    Groups  last_group = GRP_NONE;

    if ((!combo_strings_) || (!combo_strings_->IsEmpty()))
        return;

    for(unsigned int counter = 0; counter < mnemonics_->GetCount(); counter++)
    {
        actual = mnemonics_->Item(counter);
        mnemonic_str = MnemonicString(actual);
        combo_strings_->Append(mnemonic_str, actual);
        if(actual->GetGroup() != last_group)
        {
            last_group = actual->GetGroup();
            combo_groups_->Append(GetGroupAsString(last_group), reinterpret_cast<void *>(counter));
            GroupList.push_back(last_group);
        }
    }
}




wxString Dialog_Mnemonics::MnemonicString(MnemonicItem *mnemonic_item)
{
    wxString return_str;
    unsigned int arg_count = mnemonic_item->GetArgumentCount();

    for(unsigned int counter = 0; counter < mnemonic_item->GetMnemonicStrCount(); counter++)
    {
        return_str << mnemonic_item->GetMnemonicStr(counter);
        if (arg_count > 0)
        {
            return_str << "$";
            arg_count--;
        }
    }

    return return_str;
}




void Dialog_Mnemonics::UpdateFields()
{
    MnemonicItem *actual = mnemonics_->CurrentItem();
    Arguments actual_arguments = actual->GetDestinationArgument();

    field_bytecode_->SetLabel(FormatByteCode(actual->GetByteCode()));
    field_bc_size_->SetLabel(wxString::Format("%d", actual->GetByteCodeSize()));
    field_arg_count_->SetLabel(wxString::Format("%d", actual->GetArgumentCount()));
    field_arg_dest_->SetLabel(GetOperandAsStr(actual->GetDestinationArgument().operand));
    field_arg_desttype_->SetLabel(GetOperandTypeAsStr(actual->GetDestinationArgument().type));
    field_arg_source_->SetLabel(GetOperandAsStr(actual->GetSourceArgument().operand));
    field_arg_srctype_->SetLabel(GetOperandTypeAsStr(actual->GetSourceArgument().type));
    field_arg_size_->SetLabel(wxString::Format("%d", actual->GetArgumentSize()));
    field_arg_pos_->SetLabel(wxString::Format("%d", actual->GetArgumentPosition()));

    chkbox_conditional_->SetValue(actual->GetConditionalBranch());
    chkbox_explicit_->SetValue(actual->HasExplicitArguments());

    panel_bytecode_->Fit();
    panel_arg_source_->Fit();
    panel_arg_srctype_->Fit();
    panel_arg_dest_->Fit();
    panel_arg_desttype_->Fit();
}




void Dialog_Mnemonics::UpdateGroup()
{
    for(unsigned int counter = 0; counter < GroupList.size(); counter++)
        if (GroupList[counter] == mnemonics_->CurrentItem()->GetGroup())
        {
            combo_groups_->SetSelection(counter);
            break;
        }
}




wxString Dialog_Mnemonics::GetOperandAsStr(enum Operands operand)
{
    wxString returnstr;
    switch(operand)
    {
        case OP_A:
            returnstr = MnemonicXMLFile::ATTRIBUTE_REGISTER_A;
            break;
        case OP_B:
            returnstr = MnemonicXMLFile::ATTRIBUTE_REGISTER_B;
            break;
        case OP_C:
            returnstr = MnemonicXMLFile::ATTRIBUTE_REGISTER_C;
            break;
        case OP_D:
            returnstr = MnemonicXMLFile::ATTRIBUTE_REGISTER_D;
            break;
        case OP_E:
            returnstr = MnemonicXMLFile::ATTRIBUTE_REGISTER_E;
            break;
        case OP_F:
            returnstr = MnemonicXMLFile::ATTRIBUTE_REGISTER_F;
            break;
        case OP_H:
            returnstr = MnemonicXMLFile::ATTRIBUTE_REGISTER_H;
            break;
        case OP_I:
            returnstr = MnemonicXMLFile::ATTRIBUTE_REGISTER_I;
            break;
        case OP_L:
            returnstr = MnemonicXMLFile::ATTRIBUTE_REGISTER_L;
            break;
        case OP_R:
            returnstr = MnemonicXMLFile::ATTRIBUTE_REGISTER_R;
            break;
        case OP_BC:
            returnstr = MnemonicXMLFile::ATTRIBUTE_REGISTER_BC;
            break;
        case OP_DE:
            returnstr = MnemonicXMLFile::ATTRIBUTE_REGISTER_DE;
            break;
        case OP_HL:
            returnstr = MnemonicXMLFile::ATTRIBUTE_REGISTER_HL;
            break;
        case OP_IX:
            returnstr = MnemonicXMLFile::ATTRIBUTE_REGISTER_IX;
            break;
        case OP_IY:
            returnstr = MnemonicXMLFile::ATTRIBUTE_REGISTER_IY;
            break;
        case OP_SP:
            returnstr = MnemonicXMLFile::ATTRIBUTE_REGISTER_SP;
            break;
        case OP_LITERAL:
            returnstr = "xxh";
            break;

        // Uninplementend in mnemonicxmlfile (RST)
        case OP_00:
            returnstr = "00h";
            break;
        case OP_08:
            returnstr = "08h";
            break;
        case OP_10:
            returnstr = "10h";
            break;
        case OP_18:
            returnstr = "18h";
            break;
        case OP_20:
            returnstr = "20h";
            break;
        case OP_28:
            returnstr = "28h";
            break;
        case OP_30:
            returnstr = "30h";
            break;
        case OP_38:
            returnstr = "38h";
            break;
        case OP_NONE:
            returnstr = "None";
            break;
        default:
            returnstr = "error";
            break;
    }
    return returnstr;
}




wxString Dialog_Mnemonics::GetOperandTypeAsStr(enum OperandType operandtype)
{
    wxString returnstr;

    switch(operandtype)
    {
        case OT_ABSOLUTE_ADDRESS:
            returnstr = "Absolute address";
            break;
        case OT_IO_ADDRESS:
            returnstr = "IO address";
            break;
        case OT_REGISTER_ADDRESS:
            returnstr = "Register address";
            break;
        case OT_RELATIVE_ADDRESS:
            returnstr = "Relative address";
            break;
        case OT_DATA:
            returnstr = "Data";
            break;
        case OT_OFFSET:
            returnstr = "Offset";
            break;
        case OT_REGISTER:
            returnstr = "Register";
            break;
        case OT_REGISTER_PLUS_OFFSET:
            returnstr = "Register + offset";
            break;
        case OT_VARIABLE:
            returnstr = "Variable";
            break;
        case OT_NONE:
            returnstr = "None";
            break;
        default:
            returnstr = "error";
            break;
    }
    return returnstr;
}




wxString Dialog_Mnemonics::GetGroupAsString(Groups &group)
{
    wxString return_str;

    switch(group)
    {
    case GRP_LOAD_8BIT:
        return_str = "Load 8 bit";
        break;
    case GRP_LOAD_16BIT:
        return_str = "Load 16 bit";
        break;
    case GRP_EXCHANGE:
        return_str = "Exchange";
        break;
    case GRP_BLOCK_TRANSFER:
        return_str = "Block transfer";
        break;
    case GRP_BLOCK_SEARCH:
        return_str = "Block search";
        break;
    case GRP_ARITHMETIC_8BIT:
        return_str = "Arithmetic 8 bit";
        break;
    case GRP_ARITHMETIC_16BIT:
        return_str = "Arithmetic 16 bit";
        break;
    case GRP_GP_ARITHMETIC:
        return_str = "Arithmetic";
        break;
    case GRP_LOGICAL:
        return_str = "Logical";
        break;
    case GRP_CPU_CONTROL:
        return_str = "CPU Control";
        break;
    case GRP_ROTATE_SHIFT:
        return_str = "Rotate shift";
        break;
    case GRP_BIT_SET:
        return_str = "Bit set";
        break;
    case GRP_BIT_RESET:
        return_str = "Bit reset";
        break;
    case GRP_BIT_TEST:
        return_str = "Bit test";
        break;
    case GRP_JUMP:
        return_str = "Jump";
        break;
    case GRP_CALL:
        return_str = "Call";
        break;
    case GRP_RETURN:
        return_str = "Return";
        break;
    case GRP_INPUT:
        return_str = "Input";
        break;
    case GRP_OUTPUT:
        return_str = "Output";
        break;
    case GRP_RST:
        return_str = "RST";
        break;
    case GRP_STACK:
        return_str = "Stack";
        break;
    default:
        return_str = "Error";
        break;
    }

    return return_str;
}




wxString Dialog_Mnemonics::FormatByteCode(ByteCode bytecode)
{
    wxString return_str;
    MnemonicItem *actual = mnemonics_->CurrentItem();

    for(unsigned int codenum = 0; codenum < actual->GetByteCodeSize(); codenum++)
    {
        return_str << wxString::Format("%.2X ", actual->GetByteCode(codenum));
    }
    return_str.Trim();

    return return_str;
}




void Dialog_Mnemonics::OnFirstIdle(wxIdleEvent &event)
{
    Unbind(wxEVT_IDLE, &Dialog_Mnemonics::OnFirstIdle, this);
    GatherGroupsAndStrings();
    mnemonics_->First();
    combo_strings_->SetSelection(0);
    UpdateFields();
    UpdateGroup();

    combo_groups_->Enable(true);
    combo_strings_->Enable(true);
}




void Dialog_Mnemonics::OnCloseDialog(wxCloseEvent &event)
{
    SetReturnCode(wxID_ABORT);
}




void Dialog_Mnemonics::OnButtonNavNext(wxCommandEvent &event)
{
    mnemonics_->Next();
    combo_strings_->SetSelection(mnemonics_->CurrentIndex());
    UpdateFields();
    UpdateGroup();
}




void Dialog_Mnemonics::OnButtonNavPrevious(wxCommandEvent &event)
{
    mnemonics_->Previous();
    combo_strings_->SetSelection(mnemonics_->CurrentIndex());
    UpdateFields();
    UpdateGroup();
}




void Dialog_Mnemonics::OnInstructionSelect(wxCommandEvent &event)
{
    mnemonics_->Item(event.GetSelection());
    UpdateGroup();
    UpdateFields();
}




void Dialog_Mnemonics::OnGroupSelect(wxCommandEvent &event)
{
    MnemonicIndex item = reinterpret_cast<MnemonicIndex>(event.GetClientData());
    mnemonics_->Item(item);
    combo_strings_->SetSelection(item);
    UpdateFields();
}



