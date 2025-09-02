/****************************************************************
* Name:      IDZ80
* Purpose:   Interactive Disassembler for Z80 processors
* Author:    Felipe MPC (idz80a@gmail.com)
* Created:   09-11-2009 (D-M-Y) / Rewrited 30/06/2015
* License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
**************************************************************
* Represents one disassembled item
**************************************************************/



#include "disassembled_item.h"



DisassembledItem::DisassembledItem(RawData* program_file)
                :DisassembledItemData(program_file)
{
    binary_data_ = program_file;
    style_ = 0;
    is_data_ = false;
    Clear();
}




DisassembledItem::~DisassembledItem()
{
    if (style_)
        delete style_;
}




void DisassembledItem::Clear()
{
    mnemonic_ = 0;
    memset(&arguments_, 0, sizeof(ExplicitArguments));
    offset_in_file_ = 0;
    if(style_)
    {
        delete style_;
        style_ = 0;
    }
}




void DisassembledItem::CopyArguments(const ExplicitArguments &arguments)
{
    memcpy(&arguments_, &arguments, sizeof(ExplicitArguments));
}




/// @brief Return a string containing the opcode in hex format. Example: 3C BD FF
/// @param hex_style Specifies the hex representation symbol: 00H, $00, 0x00
/// @return The string
wxString DisassembledItem::GetOpcodeAsStringHex(const HexadecimalStrStyle hex_style, const DataSeparation separation)
{
    wxString as_string, fmt_string("%.2X");
    byte bytecode;
    unsigned int counter;

    if(mnemonic_)
    {
        as_string.Clear();
        switch (hex_style) {
            case HEX_STYLE_0X:
                fmt_string = "0x%.2X";
                break;
            case HEX_STYLE_$:
                fmt_string = "$%.2X";
                break;
            case HEX_STYLE_H:
                fmt_string = "%.2XH";
                break;
        }

        if  (separation == COMMA_SEPARATION)
            fmt_string << ",";
        else
            fmt_string << " ";

        for (counter = 0; counter < mnemonic_->GetByteCodeSize(); counter++)
        {
            bytecode = binary_data_->GetData(offset_in_file_ + counter);
            as_string << wxString::Format(fmt_string, bytecode);
        }
        counter = as_string.Len() - 1;
        as_string.Left(counter);
    }

    return as_string;
}




wxString DisassembledItem::GetAsciiCodeAsString()
{
    wxString as_string;
    byte bytecode;

    if(mnemonic_)
    {
        as_string.Clear();
        for (unsigned int counter = 0; counter < mnemonic_->GetByteCodeSize(); counter++)
        {
            bytecode = binary_data_->GetData(offset_in_file_ + counter);
            if ((bytecode < 32) || (bytecode > 126))
                bytecode = '.';
            as_string << wxString::Format("%c", bytecode);
        }
    }

    return as_string;
}




ArgumentStyle DisassembledItem::GetArgumentStyle()
{
    ArgumentStyle style;
    style.first = STYLE_NONE;
    style.second = STYLE_NONE;

    if(style_)
    {
        style.first = style_->first;
        style.second = style_->second;
    }

    return style;
}




ArgumentStyleOptions DisassembledItem::GetArgumentStyle(ArgumentIndex index)
{
    if(style_)
    {
        if(index == FIRST_ARGUMENT)
            return style_->first;

        if(index == SECOND_ARGUMENT)
            return style_->second;
    }

    return STYLE_NONE;
}




FileOffset DisassembledItem::GetOffsetInFile()
{
    return offset_in_file_;
}




byte DisassembledItem::GetByteOpcode(unsigned int index)
{
    if(binary_data_)
        return binary_data_->GetData(offset_in_file_ + index);

    return 0;
}



byte DisassembledItem::GetArgumentValue(ArgumentIndex index)
{
    return arguments_[index];
}




byte DisassembledItem::GetArgumentValue(unsigned int index)
{
    if(index > 1)
        index = 1;
    return arguments_[index];
}




MnemonicItem *DisassembledItem::GetMnemonic()
{
    return mnemonic_;
}




RawData *DisassembledItem::GetProgram()
{
    return binary_data_;
}




void DisassembledItem::SetFileOffset(FileOffset _offset)
{
    if(_offset < binary_data_->GetSize())
        offset_in_file_ = _offset;
}




void DisassembledItem::SetArgumentStyle(ArgumentIndex index, ArgumentStyleOptions style)
{
    if(!style_)
        style_ = new ArgumentStyle;

    if(index == FIRST_ARGUMENT)
        style_->first = style;
    if(index == SECOND_ARGUMENT)
        style_->second = style;
}




void DisassembledItem::SetMnemonic(MnemonicItem *mnemonic)
{
    mnemonic_ = mnemonic;
}




AbsoluteAddress DisassembledItem::ConvertRelativeToAbsolute(RelativeAddress relative, AbsoluteAddress baseaddress)
{
    return static_cast<AbsoluteAddress>(baseaddress + mnemonic_->GetByteCodeSize() + offset_in_file_ + relative);
}



