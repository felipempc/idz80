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

#include "disassembled_item_base.hpp"


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


/// @brief Text of a value hexadecimal representation: 5F, 5FH, 0x5F, $5F
enum HexadecimalStrStyle
{
    HEX_STYLE_NONE,     // 5F
    HEX_STYLE_H,        // 5FH
    HEX_STYLE_0X,       // 0x5F
    HEX_STYLE_$         // $5F
};

enum DataSeparation
{
    SPACE_SEPARATED,
    COMMA_SEPARATED
};

struct ArgumentStyle
{
    ArgumentStyleOptions    first,     //Left to right
                            second;
};


/*
enum ArgumentIndex
{
    FIRST_ARGUMENT = 0,
    SECOND_ARGUMENT
};
*/


typedef signed int RelativeAddress;

// Inheritance: DisassembledItem > DisassembledItemBase
// TODO: Think about why we need all this inheritance.
class DisassembledItem : public DisassembledItemBase
{
    public:
        DisassembledItem(RawData* program_file);
        ~DisassembledItem();

        void Clear();
        void Destroy();
        bool SetupInstructionItem(MnemonicItem *mnemonic, const FileOffset offset);
        void SetupDataItem(const FileOffset offset);

        wxString GetOpcodeAsStringHex(const HexadecimalStrStyle hex_style = HEX_STYLE_NONE, const DataSeparation separation = SPACE_SEPARATED);
        wxString GetAsciiCodeAsString();

        FileOffset GetOffsetInFile();

        unsigned int GetLength();
        void SetLength(unsigned int len);

        bool isData();
        void MarkAsData(const bool isdata = true);

        byte GetByteOpcode(unsigned int index);

        // Arguments
        ArgumentStyle GetArgumentStyle();
        ArgumentStyleOptions GetArgumentStyle(unsigned int index);
        //byte GetArgumentValue(ArgumentIndex index);
        int GetArgumentValue(unsigned int index, unsigned int base_address = 0);
        void SetArgumentStyle(unsigned int index, ArgumentStyleOptions style);

        MnemonicItem *GetMnemonic();
        RawData *GetProgram();

    protected:
    private:
        ArgumentStyle   *m_arg_style;

        void CopyRealBytecode();
        void FillArgument();

        AbsoluteAddress ConvertRelativeToAbsolute(RelativeAddress relative, AbsoluteAddress baseaddress);
};

#endif // DISASSEMBLEDITEM_H
