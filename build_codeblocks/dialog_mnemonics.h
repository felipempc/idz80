#ifndef DIALOG_MNEMONICS_H
#define DIALOG_MNEMONICS_H

#include "wx/dialog.h"
#include "wx/sizer.h"
#include "wx/stattext.h"
#include "wx/combobox.h"
#include "wx/checkbox.h"
#include "wx/statbox.h"

#include <mnemonic_container.h>


class Dialog_Mnemonics: public wxDialog
{
    public:
        Dialog_Mnemonics(wxWindow *parent, MnemonicContainer *mnemonics);
        virtual ~Dialog_Mnemonics();
    protected:
    private:
        enum {
            PREVIOUS_INST = 1000,
            NEXT_INST
        };
        wxStaticText    *field_bytecode_,
                        *field_bc_size_,
                        *field_arg_size_,
                        *field_arg_count_,
                        *field_arg_pos,
                        *field_arg_source,
                        *field_arg_dest;
        wxComboBox  *combo_strings_,
                    *combo_groups_;
        wxCheckBox      *chkbox_conditional_,
                        *chkbox_explicit_;

        MnemonicContainer *mnemonics_;

        void BuildDialog();
};

#endif // DIALOG_MNEMONICS_H
