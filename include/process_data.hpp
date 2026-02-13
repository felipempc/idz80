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

#include "project_base.hpp"
#include "disassembled_item.hpp"
#include "labelslist.hpp"
#include "systemlabels.hpp"
#include "decoder_smart.hpp"
#include "debug_logwindow.hpp"
#include "labelmanager.hpp"
#include "codeview_definitions.hpp"
#include "searchmanager.hpp"


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
        void disassembleFirst(const unsigned int t_index);
        void disassembleItems(const unsigned int t_index, RangeItems &t_range);
        void transformToData(const unsigned int t_index, SelectedItemInfo &t_selected);
        void disassembleData(const unsigned int t_index, SelectedItemInfo &t_selected);
        void makeData(const unsigned int t_index, RangeItems &t_range);
        //void AutoLabel();
        void initData(const unsigned int t_index);
        void insertLineLabelsInSourceCode(const unsigned int t_index);

        bool setupSystemLabels();
        void clear();

        void removeFromLabelUserList(DisassembledItem *t_de, const uint t_dasmindex);
        bool removeLineAndVarLabels(const int t_line);
        bool removeLineAndProgLabels(const int t_line);

        void searchInstructionArgument(word t_argument_value, uint t_search_config);
        bool searchInstructionArgumentContinue(AbsoluteAddress &t_address);

        ProcessData(ProjectBase *t_parent);
        ~ProcessData();

    private:
        SmartDecoder    *m_disassembler;
        SearchManager   *m_search_status;

        void removeLabelUsers(const unsigned int t_index, IntArray *t_users);
        bool filterInstructions(const unsigned int t_index, IntArray &t_range, SelectedItemInfo &t_selected);
        void processLabel(const unsigned int t_index, LabelListCtrl *t_label);

        bool findInArgumentVariables(DisassembledItem *t_de, word t_argument);
        bool findInArgumentLiteral(DisassembledItem *t_de, word t_argument);
        bool findInArgumentJumpsCalls(DisassembledItem *t_de, word t_argument);
};


#endif
