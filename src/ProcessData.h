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
#include "mndb.h"
#include "dasmdata.h"
#include "rawdata.h"
#include "d_asm_element.h"
#include "labelslist.h"
#include "codeviewline.h"
#include "systemlabels.h"
#include "decoder.h"
#include "IDZ80debugbase.h"
#include "labelmanager.h"

#include <wx/gauge.h>
#include <wx/dynarray.h>


//TODO:Remove it
#define OPCODE_NOT_MATCHED  0xFFFFFFFF


// Items of the instructions array
struct stRangeItems
{
    uint Index;
    uint Count;
};

typedef struct stRangeItems RangeItems;

// Range of addresses
struct stRangeData
{
    uint First;
    uint Count;
};
typedef struct stRangeData RangeData;


class ProcessData : public LabelManager, IDZ80LogBase
{
    public:
        CodeViewLine        *m_CodeViewLine;
        DAsmData            *m_Dasm;
        RawData             *Program;
        MnemonicDataBase    *Mnemonics;

        void DisassembleFirst();
        void DisassembleItems(RangeItems &r);
        void MakeData(RangeItems &r);
        void AutoLabel();
        void InitData();
        void processLabel();
        bool SetupSystemLabels();

        void SetGauge(wxGauge *g);
        void SetLog(wxTextCtrl *_lg);

        ProcessData(wxWindow *parent);
        ~ProcessData();

    private:
        wxGauge             *m_gauge;
        wxWindow            *m_main_frame;
        Decoder             *m_disassembler;

        uint MatchOpcode(const uint i, const uint max);
        void ConvertProgramAddress(RangeItems r, RangeData& d);
};


#endif

