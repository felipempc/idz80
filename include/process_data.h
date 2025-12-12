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
        void DisassembleFirst(const unsigned int index);
        void DisassembleItems(const unsigned int index, RangeItems &r);
        void TransformToData(SelectedItemInfo &selected);
        void DisassembleData(SelectedItemInfo &selected);
        void MakeData(RangeItems &r);
        //void AutoLabel();
        void InitData();
        void InsertLineLabelsInSourceCode();

        bool SetupSystemLabels();
        void Clear();

        void RemoveFromLabelUserList(DisassembledItem *de, const uint dasmitem);
        bool RemoveLineAndVarLabels(const int index);
        bool RemoveLineAndProgLabels(const int index);

        void SearchInstructionArgument(word argument_value, uint search_config);
        bool SearchInstructionArgumentContinue(AbsoluteAddress &address);

        ProcessData(ProjectBase *parent);
        ~ProcessData();

    private:
        SmartDecoder    *m_disassembler;
        SearchManager   *search_status_;

        void RemoveLabelUsers(wxArrayInt *users);
        bool FilterInstructions(wxArrayInt &range, SelectedItemInfo &selected);
        void ProcessLabel(LabelListCtrl *label);

        bool FindInArgumentVariables(DisassembledItem *de, word argument);
        bool FindInArgumentLiteral(DisassembledItem *de, word argument);
        bool FindInArgumentJumpsCalls(DisassembledItem *de, word argument);
};


#endif

