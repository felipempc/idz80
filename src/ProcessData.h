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
#include "d_asm_element.h"
#include "labelslist.h"
#include "systemlabels.h"
#include "decoder.h"
#include "logwindow.h"
#include "labelmanager.h"

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
        void MakeData(RangeItems &r);
        void AutoLabel();
        void InitData();
        void processLabel();
        bool SetupSystemLabels();
        void Clear();

        void SetGauge(wxGauge *g);

        ProcessData(wxWindow *parent, LogWindow *logparent);
        ~ProcessData();

    private:
        wxGauge     *m_gauge;
        Decoder     *m_disassembler;
        LogWindow   *WindowLog;
};


#endif

