/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   19-11-2013 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Dialog for searching in instructions's arguments
 **************************************************************/



#ifndef SEARCHARGUMENTDIALOG_H
#define SEARCHARGUMENTDIALOG_H

#include <wx/dialog.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include <wx/button.h>

#include "IDZ80Base.h"
#include "dasmdata.h"

class SearchArgumentDialog : public wxDialog
{
    public:
        SearchArgumentDialog(wxWindow *parent);
        ~SearchArgumentDialog();

        bool hasLiteralSearch();
        bool hasVariableSearch();
        bool hasJumpCallSearch();

        word getValue();

    protected:
   		static const long ID_SEARCH;

    private:
        wxTextCtrl  *searchline_;
        wxCheckBox  *check_literals_,
                    *check_variables_,
                    *check_jumps_calls_;
        wxButton    *search_button_,
                    *cancel_button_;

        wxString    debug_value_str_;
        wxStaticText *converted_value_str_;

        uint        value_;

        void OnTextUpdate(wxCommandEvent &event);
        void OnKeyEnterPressed(wxCommandEvent &event);

        bool CheckHexadecimalValue(const wxString &hexstr, long &converted_num);
        void ValidateBaseNumber();
        void ValidateSizeNumber();
        void BuildDialog();
};

#endif // SEARCHARGUMENTDIALOG_H
