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
#include "IDZ80debugbase.h"


typedef char OpCodeArguments [2];

enum ElementType
{
    et_None, et_Data, et_Instruction
};

enum aStyleType //: byte
{
	ast_hex,
	ast_bin,
	ast_dec
};


struct ArgStyle
{
	aStyleType arg1;
	aStyleType arg2;
	unsigned char reserved;
	unsigned char arg1styled:1;
	unsigned char arg2styled:1;
	unsigned char hasArgumentLabel:1;	// true if there is a label for its argument
	unsigned char hasLabel:1;			// true if there is a label for this item
};


class DAsmElement: public IDZ80LogBase
{
    public:

        void Clear();
        wxString getMnemonic(LabelListCtrl *labels);
        wxString getCodeStr();
        wxString getAsciiStr();
        uint getArgument(uint arg, uint _baseaddress);
        byte GetData(uint offset);
        ArgStyle GetStyle();
        aStyleType GetStyleArgument(uint arg_idx) ;
        uint GetOffset();
        uint GetLength();
        enum ElementType GetType();
        enum ArgumentTypes GetArgumentType(uint arg_num);
        enum BranchType GetBranchType();
        int GetNumArgs();
        int GetArgSize();
        wxString &GetMnemonicStr(uint index);
        uint GetMnemonicStrNum();
        byte GetCodeItem(uint index);
        byte GetArgItem(uint index);
        enum InstructionTypes GetInstructionType();
        enum InstructionDetails GetInstructionDetail();
        bool HasArgumentLabel();

        void SetArgLabel(bool hal = true);
        void SetOffset(uint _offset);
        void SetLength(uint _length);
        void SetLength();
        void SetType(enum ElementType _etype);
        void SetStyle(ArgStyle &_style);
        void SetStyleArgument(uint arg_idx, aStyleType ast);
        void SetMnemonic(MnemonicItem *mnemonic);
        bool CopyArguments();
        void CopyArguments(OpCodeArguments &arg, uint size);
        void CopyOpCode(ByteCode &bytecode);


        bool isData();
        bool isInstruction();

        MnemonicItem        *MnemonicObject;

        DAsmElement(RawData* rawdata);
        ~DAsmElement();

    private:
        RawData				*FileData;

        ByteCode            Code;
        OpCodeArguments     Args;
        unsigned int        Offset,         // File address (0..EOF)
                            Length;         // How many BYTEs
        enum ElementType    ElType;
        ArgStyle			Style;

		void CopyOpcode();
        uint convertRelAbs(int reladdr, uint _baseaddress);
        void SetStyleDefault();

};


#endif
