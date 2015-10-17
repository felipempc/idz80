/****************************************************************
* Name:      IDZ80
* Purpose:   Interactive Disassembler for Z80 processors
* Author:    Felipe MPC (idz80a@gmail.com)
* Created:   09-11-2009 (D-M-Y) / Rewrited 30/06/2015
* License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
**************************************************************
* Represents one disassembled item
**************************************************************/


#ifndef DISASSEMBLEDITEM_H
#define DISASSEMBLEDITEM_H

#include <disassembled_item_data.h>


enum ArgumentStyleOptions
{
    STYLE_NONE,
    STYLE_LABELED,

    STYLE_BYTE_HEX,
    STYLE_BYTE_BIN,
    STYLE_BYTE_DEC,

    STYLE_WORD_HEX,
    STYLE_WORD_BIN,
    STYLE_WORD_DEC
};



struct ArgumentStyle
{
    ArgumentStyleOptions    first,     //Left to right
                            second;
};



enum ArgumentIndex
{
    FIRST_ARGUMENT = 0,
    SECOND_ARGUMENT
};



typedef signed int RelativeAddress;



class DisassembledItem : public DisassembledItemData
{
    public:
        DisassembledItem(RawData* program_file);
        ~DisassembledItem();

        void Clear();

        void CopyArguments(const ExplicitArguments &arguments);

        wxString GetOpcodeAsString();
        wxString GetAsciiCodeAsString();

        ArgumentStyle GetArgumentStyle();
        ArgumentStyleOptions GetArgumentStyle(ArgumentIndex index);

        FileOffset GetOffsetInFile();

        byte GetByteOpcode(unsigned int index);
        byte GetArgumentValue(ArgumentIndex index);
        byte GetArgumentValue(unsigned int index);

        MnemonicItem *GetMnemonic();

        RawData *GetProgram();

        void SetFileOffset(FileOffset _offset);
        void SetArgumentStyle(ArgumentIndex index, ArgumentStyleOptions style);

        void SetMnemonic(MnemonicItem *mnemonic);

    protected:
    private:
        ArgumentStyle   *style_;

        AbsoluteAddress ConvertRelativeToAbsolute(RelativeAddress relative, AbsoluteAddress baseaddress);
};

#endif // DISASSEMBLEDITEM_H
