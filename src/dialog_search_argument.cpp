/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   19-11-2013 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Dialog for searching in instructions's arguments
 **************************************************************/




#include "dialog_search_argument.h"
#include <wx/sizer.h>
#include <wx/panel.h>



const long SearchArgumentDialog::ID_SEARCH = wxNewId();


SearchArgumentDialog::SearchArgumentDialog(wxWindow *parent)
{
    wxPoint pt(200, 150);
    wxSize sz(300, 200);

    if (parent)
    {
        pt.x = (parent->GetSize().GetWidth() / 2) - (sz.GetWidth() / 2);
        pt.y = (parent->GetSize().GetHeight() /2) - (sz.GetHeight() / 2);
    }

    Create(0, wxID_ANY, "Search for argument", pt, sz, wxCAPTION);

    debug_value_str_ = "Value to search:";

    value_ = 0;
    check_jumps_calls_ = 0;
    check_literals_ = 0;
    check_variables_ = 0;

    BuildDialog();

    check_literals_->SetValue(true);
    check_variables_->SetValue(true);
    check_jumps_calls_->SetValue(true);

    search_button_->Enable(false);

    Bind(wxEVT_TEXT, &SearchArgumentDialog::OnTextUpdate, this, ID_SEARCH);
    Bind(wxEVT_TEXT_ENTER, &SearchArgumentDialog::OnKeyEnterPressed, this, ID_SEARCH);
}


SearchArgumentDialog::~SearchArgumentDialog()
{
    delete check_literals_;
    delete check_variables_;
    delete check_jumps_calls_;
    delete search_button_;
    delete cancel_button_;
    delete searchline_;
}




void SearchArgumentDialog::BuildDialog()
{
    wxPanel *main_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
    searchline_ = new wxTextCtrl(main_panel, ID_SEARCH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    search_button_ = new wxButton(main_panel, wxID_OK, "Search");
    cancel_button_ = new wxButton(main_panel, wxID_CANCEL, "Cancel");

    wxBoxSizer *main_sizer = new wxBoxSizer(wxVERTICAL);

    main_sizer->Add(main_panel, wxSizerFlags(1).Expand().Center());

    wxBoxSizer *search_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *buttons_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *check_sizer = new wxBoxSizer(wxVERTICAL);

    check_literals_ = new wxCheckBox(main_panel, wxID_ANY, "Literals");
    check_variables_ = new wxCheckBox(main_panel, wxID_ANY, "Variables");
    check_jumps_calls_ = new wxCheckBox(main_panel, wxID_ANY, "Jumps and Calls");
    check_sizer->Add(check_literals_);
    check_sizer->Add(check_variables_);
    check_sizer->Add(check_jumps_calls_);

    buttons_sizer->Add(search_button_, wxSizerFlags(0).Left().DoubleHorzBorder());
    buttons_sizer->AddSpacer(search_button_->GetSize().GetWidth());
    buttons_sizer->Add(cancel_button_, wxSizerFlags(0).Right().DoubleHorzBorder());

    search_sizer->Add(searchline_, wxSizerFlags(0).Top().DoubleBorder().Expand());
    converted_value_str_ = new wxStaticText(main_panel, wxID_ANY, debug_value_str_);
    search_sizer->Add(converted_value_str_, wxSizerFlags(0).DoubleHorzBorder());
    search_sizer->Add(check_sizer, wxSizerFlags(0).DoubleBorder());
    search_sizer->AddSpacer(10);
    search_sizer->Add(buttons_sizer, wxSizerFlags(0).Bottom());

    main_panel->SetSizer(search_sizer);
}




void SearchArgumentDialog::OnTextUpdate(wxCommandEvent &event)
{
    ValidateBaseNumber();
    ValidateSizeNumber();
}


void SearchArgumentDialog::OnKeyEnterPressed(wxCommandEvent &event)
{
    if (search_button_->IsEnabled())
        EndModal(wxID_OK);
}



void SearchArgumentDialog::ValidateBaseNumber()
{
    long number;
    wxString str;

    str = searchline_->GetValue();
    str.MakeLower();

    if (str.IsEmpty())
    {
        search_button_->Enable(false);
        return;
    }


    if (str.ToLong(&number))
    {
        search_button_->Enable(true);
        converted_value_str_->SetLabel(wxString::Format("%s %d (decimal)", debug_value_str_, number));
    }
    else
    {
        if (CheckHexadecimalValue(str, number))
        {
            search_button_->Enable(true);
            converted_value_str_->SetLabel(wxString::Format("%s %d (hexadecimal)", debug_value_str_, number));
        }
        else
        {
            converted_value_str_->SetLabel(debug_value_str_);
            wxMessageBox("Only decimal or hexadecimal (with 0x or &h prefix)\n numbers allowed !", "Error ...");
            searchline_->SetSelection(-1, -1);
            search_button_->Enable(false);
        }
    }
    value_ = static_cast<uint>(number);
}


void SearchArgumentDialog::ValidateSizeNumber()
{
    if (value_ > 0xFFFF)
    {
        converted_value_str_->SetLabel(debug_value_str_);
        wxMessageBox("Value limited to 16 bits (65535/0xFFFF) !", "Error ...");
        searchline_->SetSelection(-1, -1);
        search_button_->Enable(false);
    }
}



bool SearchArgumentDialog::CheckHexadecimalValue(const wxString &hexstr, long &converted_num)
{
    bool is_hex = false;

    if (hexstr.ToLong(&converted_num, 16))
        is_hex = true;
    else
    {
        if (hexstr.IsSameAs("0x"))
            is_hex = true;
        converted_num = 0;
    }

    return is_hex;
}



bool SearchArgumentDialog::hasJumpCallSearch()
{
    if (check_jumps_calls_)
        return check_jumps_calls_->IsChecked();
    else
        return false;
}


bool SearchArgumentDialog::hasLiteralSearch()
{
    if (check_literals_)
        return check_literals_->IsChecked();
    else
        return false;
}


bool SearchArgumentDialog::hasVariableSearch()
{
    if (check_variables_)
        return check_variables_->IsChecked();
    else
        return false;
}



word SearchArgumentDialog::getValue()
{
    return static_cast<word>(value_ & 0xFFFF);
}
