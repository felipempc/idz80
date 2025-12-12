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
{
    m_program = program_file;
    Clear();
}




DisassembledItem::~DisassembledItem()
{
    if (m_arg_style)
        delete m_arg_style;
}




/// @brief Init all data
void DisassembledItem::Clear()
{
    m_mnemonic = 0;
    m_arg_style = 0;
    m_is_data = false;
    m_file_offset = 0;
    m_length = 0;
    m_mnemonic_signature = 0;
    m_arguments.signed_value = 0;
    m_arguments.unsigned_16bit = 0;
    m_arguments.unsigned_8bit_high = 0;
    m_arguments.unsigned_8bit_low = 0;
    memset(&m_real_bytecode, 0, sizeof(ByteCode));
}




/// @brief Release allocated memory and init all data
void DisassembledItem::Destroy()
{
    if(m_arg_style)
        delete m_arg_style;
    Clear();
}



/// @brief Return a string containing the opcode in hex format. Example: 3C BD FF
/// @param hex_style Specifies the hex representation symbol: 00H, $00, 0x00
/// @return The string
wxString DisassembledItem::GetOpcodeAsStringHex(const HexadecimalStrStyle hex_style, const DataSeparation separation)
{
    wxString as_string, fmt_string("%.2X");
    byte bytecode;
    unsigned int counter;

    if(m_mnemonic)
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

        if  (separation == COMMA_SEPARATED)
            fmt_string << ",";
        else
            fmt_string << " ";

        for (counter = 0; counter < m_mnemonic->GetByteCodeSize(); counter++)
        {
            bytecode = m_program->GetData(m_file_offset + counter);
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

    if(m_mnemonic)
    {
        as_string.Clear();
        for (unsigned int counter = 0; counter < m_mnemonic->GetByteCodeSize(); ++counter)
        {
            bytecode = m_program->GetData(m_file_offset + counter);
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

    if(m_arg_style)
    {
        style.first = m_arg_style->first;
        style.second = m_arg_style->second;
    }

    return style;
}




ArgumentStyleOptions DisassembledItem::GetArgumentStyle(ArgumentIndex index)
{
    if(m_arg_style)
    {
        if(index == FIRST_ARGUMENT)
            return m_arg_style->first;

        if(index == SECOND_ARGUMENT)
            return m_arg_style->second;
    }

    return STYLE_NONE;
}




FileOffset DisassembledItem::GetOffsetInFile()
{
    return m_file_offset;
}



unsigned int DisassembledItem::GetLength()
{
    return m_length;
}



void DisassembledItem::SetLength(unsigned int len)
{
    m_length = len;
}



bool DisassembledItem::isData()
{
    return m_is_data;
}



void DisassembledItem::MarkAsData(const bool isdata)
{
    m_is_data = isdata;
}



byte DisassembledItem::GetByteOpcode(unsigned int index)
{
    if(m_program)
        return m_program->GetData(m_file_offset + index);

    return 0;
}



byte DisassembledItem::GetArgumentValue(ArgumentIndex index)
{
    return m_arguments[index];
}




byte DisassembledItem::GetArgumentValue(unsigned int index)
{
    if(index > 1)
        index = 1;
    return m_arguments[index];
}




MnemonicItem *DisassembledItem::GetMnemonic()
{
    return m_mnemonic;
}




RawData *DisassembledItem::GetProgram()
{
    return m_program;
}



/* DEPRECATED
void DisassembledItem::SetFileOffset(FileOffset _offset)
{
    if(_offset < m_program->GetSize())
        m_file_offset = _offset;
}
*/



void DisassembledItem::SetArgumentStyle(ArgumentIndex index, ArgumentStyleOptions style)
{
    if(!m_arg_style)
        m_arg_style = new ArgumentStyle;

    if(index == FIRST_ARGUMENT)
        m_arg_style->first = style;
    if(index == SECOND_ARGUMENT)
        m_arg_style->second = style;
}



/* DEPRECATED
void DisassembledItem::SetMnemonic(MnemonicItem *mnemonic)
{
    m_mnemonic = mnemonic;
}
*/


bool DisassembledItem::SetupDisassembledItem(MnemonicItem *mnemonic, const FileOffset offset)
{
    m_mnemonic = mnemonic;
    m_file_offset = offset;
    m_is_data = false;
    m_length = mnemonic->GetByteCodeSize();
    m_mnemonic_signature = mnemonic->GetMnemonicSignature();

    if ((m_mnemonic->GetArgumentCount() == 1) && (m_mnemonic->GetArgumentSize() == 1)) {  // One 8bit argument
            m_arguments.unsigned_16bit = m_program->GetData();
    }
    if ((m_mnemonic->GetArgumentCount() == 1) && (m_mnemonic->GetArgumentSize() == 2)) {  // One 16bit argument
            
    }
    if ((m_mnemonic->GetArgumentCount() == 2) && (m_mnemonic->GetArgumentSize() == 1)) {  // Two 8bit arguments
            
    }



    return false;
}



/// @brief Copy the bytecode from program file
/// @brief Position -> 0  1  2  3
/// @brief             aa bb cc dd
void DisassembledItem::CopyRealBytecode()
{
    if (m_program && m_mnemonic) {
        for(int offset = 0; offset < m_length; ++offset)
            m_real_bytecode[offset] = m_program->GetData(m_file_offset + offset);
    }
}



AbsoluteAddress DisassembledItem::ConvertRelativeToAbsolute(RelativeAddress relative, AbsoluteAddress baseaddress)
{
    return static_cast<AbsoluteAddress>(baseaddress + m_mnemonic->GetByteCodeSize() + m_file_offset + relative);
}



