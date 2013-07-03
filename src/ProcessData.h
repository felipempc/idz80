/****************************************************************
 * Name:      IDZ80
 * Purpose:   Defines Application Frame
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 **************************************************************/



/*
 * Process data (disassemble & etc)
 */



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
        void processLabel();
        bool SetupSystemLabels();
        void Clear();

        void RemoveFromLabelUserList(DisassembledItem *de, const uint dasmitem);
        void RemoveLineAndVarLabels(const int index);
        void RemoveLineAndProgLabels(const int index);


        void SetGauge(wxGauge *g);

        ProcessData(wxWindow *parent, LogWindow *logparent);
        ~ProcessData();

    private:
        wxGauge     *gauge_;
        Decoder     *disassembler_;
        SmartDecoder    *smart_disassembler_;
        LogWindow   *window_log_;

        void RemoveLabelUsers(wxArrayInt *users);
        bool FilterInstructions(wxArrayInt &range, SelectedItemInfo &selected);
};


#endif

