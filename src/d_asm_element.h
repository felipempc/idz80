/****************************************************************
 * Name:      IDZ80
 * Purpose:   Defines Application Frame
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 **************************************************************/


/*
 * Hold one disassembled element
 *
 */

#ifndef _IDZ80_DASMELEMENT_H
#define _IDZ80_DASMELEMENT_H

#include "mndb.h"
#include "MnemonicItem.h"
#include "rawdata.h"
#include "labelslist.h"


typedef char OpCodeArguments [2];

enum ElementType
{
    et_None, et_Data, et_Instruction
};


class DAsmElement
{
    public:

        void Clear();
        wxString getMnemonic(LabelListCtrl *labels);
        wxString getCodeStr();
        wxString getAsciiStr();
        uint getArgument(uint arg);
        byte GetData(uint offset);

        MnemonicItem        *MnItem;
        ByteCode            Code;
        OpCodeArguments     Args;
        unsigned int        Offset,         // File address (0..EOF)
                            Length;         // How many BYTEs
        enum ElementType    ElType;
        bool                hasArgumentLabel,   // true if there is a label for its argument
                            hasLabel;           // true if there is a label for this item

        DAsmElement(RawData* rawdata, uint *baseaddress);
        ~DAsmElement();

    private:
        RawData     *FileData;
        uint        *BaseAddress;

        uint convertRelAbs(int reladdr);

};


#endif
