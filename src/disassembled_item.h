/****************************************************************
* Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Represents one disassembled item
 **************************************************************/


#ifndef _IDZ80_DASMELEMENT_H
#define _IDZ80_DASMELEMENT_H

#include "mndb.h"
#include "MnemonicItem.h"
#include "rawdata.h"
#include "labelslist.h"
#include "logbase.h"


typedef char OpCodeArguments [2];

enum ElementType
{
    et_None, et_Data, et_Instruction
};

enum StyleType
{
	ast_bytehex,
	ast_bytebin,
	ast_bytedec,
	ast_wordhex,
	ast_wordbin,
	ast_worddec
};


struct ArgStyle
{
	StyleType arg1;
	StyleType arg2;
	unsigned char reserved;
	unsigned char arg1styled:1;
	unsigned char arg2styled:1;
	unsigned char hasArgumentLabel:1;	// true if there is a label for its argument
	unsigned char hasLabel:1;			// true if there is a label for this item
};


class DisassembledItem: public LogBase
{
    public:

        void Clear();
        wxString GetCodeStr();
        wxString GetAsciiStr();
        word GetArgument(uint argument_index, uint baseaddress);
        byte GetData(uint offset);
        ArgStyle GetStyle();
        StyleType GetStyleArgument(uint argument_index) ;
        uint GetOffset();
        uint GetLength();
        ElementType GetType();
        ArgumentTypes GetArgumentType(uint argument_index);
        BranchType GetBranchType();
        int GetArgumentCount();
        int GetArgumentSize();
        wxString &GetMnemonicStr(uint index);
        uint GetMnemonicStrCount();
        byte GetByteOpCode(uint index);
        byte GetByteArgument(uint argument_index);
        InstructionTypes GetInstructionType();
        InstructionDetails GetInstructionDetail();
        bool HasArgumentLabel();

        void SetArgLabel(bool hal = true);
        void SetOffset(uint _offset);
        void SetLength(uint _length);
        void SetLength();
        void SetType(ElementType _etype);
        void SetStyle(ArgStyle &_style);
        void SetStyleArgument(uint argument_index, StyleType ast);
        void SetMnemonic(MnemonicItem *mnemonic);
        bool CopyArguments();
        void CopyArguments(OpCodeArguments &arg, uint size);
        void CopyOpCode(ByteCode &bytecode);


        bool IsData();
        bool IsInstruction();
        bool IsArgumentProgramAddress();
        bool IsArgumentVariableAddress();

        MnemonicItem        *mnemonic_;

        DisassembledItem(RawData* rawdata);
        ~DisassembledItem();

    private:
        RawData				*data_file_;

        ByteCode            opcode_;
        OpCodeArguments     opcode_arguments_;
        unsigned int        offset_in_file_,
                            opcode_size_;
        ElementType         item_type_;
        ArgStyle			arguments_style_;

		void CopyOpcode();
        ProgramAddress ConvertRelativeToAbsoluteAddress(int reladdr, ProgramAddress baseaddress);
        void SetStyleDefault();

};


#endif
