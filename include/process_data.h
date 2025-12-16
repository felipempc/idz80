/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Process all data
 **************************************************************/


  // MUST BE COMPLETELY REWRITTEN !!!!


#ifndef _IDZ80_PROCESSDATA_H
#define _IDZ80_PROCESSDATA_H

#include "project_base.h"
#include "disassembled_item.h"
#include "labelslist.h"
#include "systemlabels.h"
#include "decoder_smart.h"
#include "debug_logwindow.h"
#include "labelmanager.h"
#include "codeview_definitions.h"
#include "searchmanager.h"


// Range of addresses
struct stRangeData
{
    uint First;
    uint Count;
};
typedef struct stRangeData RangeData;


class ProcessData : public ProjectBase
{
    public:
        void DisassembleFirst(const unsigned int t_index);
        void DisassembleItems(const unsigned int t_index, RangeItems &t_range);
        void TransformToData(const unsigned int t_index, SelectedItemInfo &t_selected);
        void DisassembleData(const unsigned int t_index, SelectedItemInfo &t_selected);
        void MakeData(const unsigned int t_index, RangeItems &t_range);
        //void AutoLabel();
        void InitData(const unsigned int t_index);
        void InsertLineLabelsInSourceCode();

        bool SetupSystemLabels();
        void Clear();

        void RemoveFromLabelUserList(DisassembledItem *t_de, const uint t_dasmitem);
        bool RemoveLineAndVarLabels(const int t_line);
        bool RemoveLineAndProgLabels(const int t_line);

        void SearchInstructionArgument(word t_argument_value, uint t_search_config);
        bool SearchInstructionArgumentContinue(AbsoluteAddress &t_address);

        ProcessData(ProjectBase *t_parent);
        ~ProcessData();

    private:
        SmartDecoder    *m_disassembler;
        SearchManager   *search_status_;

        void RemoveLabelUsers(const unsigned int t_index, IntArray *t_users);
        bool FilterInstructions(const unsigned int t_index, IntArray &t_range, SelectedItemInfo &t_selected);
        void ProcessLabel(LabelListCtrl *t_label);

        bool FindInArgumentVariables(DisassembledItem *t_de, word t_argument);
        bool FindInArgumentLiteral(DisassembledItem *t_de, word t_argument);
        bool FindInArgumentJumpsCalls(DisassembledItem *t_de, word t_argument);
};


#endif
