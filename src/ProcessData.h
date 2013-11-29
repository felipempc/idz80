/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Process all data
 **************************************************************/

#ifndef _IDZ80_PROCESSDATA_H
#define _IDZ80_PROCESSDATA_H

#include "IDZ80Base.h"
#include "processbase.h"
#include "disassembled_item.h"
#include "labelslist.h"
#include "systemlabels.h"
#include "decoder.h"
#include "decoder_smart.h"
#include "logwindow.h"
#include "labelmanager.h"
#include "codeview_definitions.h"
#include "searchmanager.h"

#include <wx/gauge.h>
#include <wx/dynarray.h>



// Range of addresses
struct stRangeData
{
    uint First;
    uint Count;
};
typedef struct stRangeData RangeData;


class ProcessData : public LabelManager, public LogBase, public ProcessBase
{
    public:
        void DisassembleFirst(bool simulateexecution = false);
        void DisassembleItems(RangeItems &r);
        void TransformToData(SelectedItemInfo &selected);
        void DisassembleData(SelectedItemInfo &selected);
        void MakeData(RangeItems &r);
        void AutoLabel();
        void InitData();
        void InsertLineLabelsInSourceCode();

        bool SetupSystemLabels();
        void Clear();

        void RemoveFromLabelUserList(DisassembledItem *de, const uint dasmitem);
        bool RemoveLineAndVarLabels(const int index);
        bool RemoveLineAndProgLabels(const int index);

        void SearchInstructionArgument(word argument_value, uint search_config);
        bool SearchInstructionArgumentContinue(ProgramAddress &address);


        void SetGauge(wxGauge *g);

        ProcessData(wxWindow *parent, LogWindow *logparent);
        ~ProcessData();

    private:
        wxGauge     *gauge_;
        Decoder     *disassembler_;
        SmartDecoder    *smart_disassembler_;
        LogWindow   *window_log_;

        SearchManager   *search_status_;

        void RemoveLabelUsers(wxArrayInt *users);
        bool FilterInstructions(wxArrayInt &range, SelectedItemInfo &selected);
        void ProcessLabel(LabelListCtrl *label);

        bool FindInArgumentVariables(DisassembledItem *de, word argument);
        bool FindInArgumentLiteral(DisassembledItem *de, word argument);
        bool FindInArgumentJumpsCalls(DisassembledItem *de, word argument);

};


#endif

