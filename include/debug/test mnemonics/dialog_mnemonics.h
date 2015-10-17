#ifndef DIALOG_MNEMONICS_H
#define DIALOG_MNEMONICS_H

#include "wx/dialog.h"
#include "wx/sizer.h"
#include "wx/stattext.h"
#include "wx/choice.h"
#include "wx/checkbox.h"
#include "wx/statbox.h"

#include <vector>

#include <mnemonic_container.h>


typedef std::vector<Groups> ArrayGroups;

class Dialog_Mnemonics: public wxDialog
{
    public:
        Dialog_Mnemonics(wxWindow *parent, MnemonicContainer *mnemonics);
        virtual ~Dialog_Mnemonics();
    protected:
    private:
        static const long idNAV_PREVIOUS;
        static const long idNAV_NEXT;
        static const long idSELECT_GROUP;
        static const long idSELECT_INSTRUCTION;

        wxStaticText    *field_bytecode_,
                        *field_bc_size_,
                        *field_arg_size_,
                        *field_arg_count_,
                        *field_arg_pos_,
                        *field_arg_source_,
                        *field_arg_srctype_,
                        *field_arg_dest_,
                        *field_arg_desttype_;

        wxChoice        *combo_strings_,
                        *combo_groups_;

        wxCheckBox      *chkbox_conditional_,
                        *chkbox_explicit_;

        wxPanel         *panel_bytecode_,
                        *panel_arg_source_,
                        *panel_arg_srctype_,
                        *panel_arg_dest_,
                        *panel_arg_desttype_;

        MnemonicContainer *mnemonics_;
        ArrayGroups     GroupList;

        void BuildDialog();
        void UpdateFields();
        void UpdateGroup();

        wxString GetOperandTypeAsStr(enum OperandType operandtype);
        wxString GetOperandAsStr(enum Operands operand);
        wxString GetGroupAsString(Groups &group);
        wxString FormatByteCode(ByteCode bytecode);

        void GatherGroupsAndStrings();
        wxString MnemonicString(MnemonicItem *mnemonic_item);

        void OnCloseDialog(wxCloseEvent &event);
        void OnButtonNavNext(wxCommandEvent &event);
        void OnButtonNavPrevious(wxCommandEvent &event);
        void OnFirstIdle(wxIdleEvent &event);
        void OnInstructionSelect(wxCommandEvent &event);
        void OnGroupSelect(wxCommandEvent &event);

};

#endif // DIALOG_MNEMONICS_H
