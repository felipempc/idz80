/****************************************************************
* Name:      IDZ80
* Purpose:   Interactive Disassembler for Z80 processors
* Author:    Felipe MPC (idz80a@gmail.com)
* Created:   09-11-2009 (D-M-Y) / Rewrited 30/06/2015
* License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
**************************************************************
* Represents one disassembled item
**************************************************************/



#include "disassembled_item.hpp"



DisassembledItem::DisassembledItem(RawData* program_file)
{
    m_program = program_file;
    clear();
}



DisassembledItem::~DisassembledItem()
{
    if (m_arg_style)
        delete m_arg_style;
}



/// @brief Init all data
void DisassembledItem::clear()
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
void DisassembledItem::destroy()
{
    if(m_arg_style)
        delete m_arg_style;
    clear();
}



/// @brief Return a string containing the opcode in hex format. Example: 3C BD FF
/// @param hex_style Specifies the hex representation symbol: 00H, $00, 0x00
/// @return The string
wxString DisassembledItem::getOpcodeAsStringHex(const HexadecimalStrStyle hex_style, const DataSeparation separation)
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

        for (counter = 0; counter < m_length; ++counter)
        {
            bytecode = getByteOpcode(counter);
            as_string << wxString::Format(fmt_string, bytecode);
        }
        counter = as_string.Len() - 1;
        as_string.Left(counter);
    }

    return as_string;
}



/// @brief Converts the bytecode into a string, substitutes "invisible" chars by a dot.
/// @return The string
wxString DisassembledItem::getAsciiCodeAsString()
{
    wxString as_string;
    byte bytecode;

    if(m_mnemonic) {
        as_string.Clear();
        for (unsigned int counter = 0; counter < m_length; ++counter) {
            bytecode = getByteOpcode(counter);
            if ((bytecode < 32) || (bytecode > 126)) {
                bytecode = '.';
            }
            as_string << wxString::Format("%c", bytecode);
        }
    }

    return as_string;
}



/// @brief Gets the style of arguments
/// @return A struct with the arguments' style
ArgumentStyle DisassembledItem::getArgumentStyle()
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



/// @brief Gets the style of the selected argument
/// @param index Selects the argument: 0 to the first, 1 to the second.
/// @return The argument style
ArgumentStyleOptions DisassembledItem::getArgumentStyle(unsigned int index)
{
    if(m_arg_style)
    {
        if(index == 0) {
            return m_arg_style->first;
        }

        if(index == 1) {
            return m_arg_style->second;
        }
    }

    return STYLE_NONE;
}



/// @brief Gets the offset position of this item in the file.
/// @return 
FileOffset DisassembledItem::getOffsetInFile()
{
    return m_file_offset;
}



/// @brief Gets the opcode' size of this item.
/// @return The size
unsigned int DisassembledItem::getOpcodeSize()
{
    return m_length;
}



/// @brief Sets the size of this item
/// @param len The new size
void DisassembledItem::setSize(unsigned int len)
{
    m_length = len;
}



/// @brief Verifies if this item is data.
/// @return true if it is data.
bool DisassembledItem::isData()
{
    return m_is_data;
}



/// @brief Marks this item as data
/// @param isdata 
void DisassembledItem::markAsData(const bool isdata)
{
    m_is_data = isdata;
}



/// @brief Gets the selected byte in the opcode, limited by MAX_OPCODE_SIZE
/// @param t_opcode_index Selects the byte, 0 to MAX_OPCODE_SIZE
/// @return The byte
unsigned int DisassembledItem::getByteOpcode(unsigned int t_opcode_index)
{
    unsigned int result = 0;

    if (t_opcode_index >= MAX_OPCODE_SIZE) {
        return 0;
    }
    result = m_real_bytecode[t_opcode_index];
    return (result & 0xFF);
}



/// @brief Gets a byte data directly from file, limited by m_length
/// @param t_offset The offset from 0 to m_length.
/// @return The byte
unsigned int DisassembledItem::getByteFromFile(unsigned int t_offset)
{
    unsigned int    result = 0,
                    offset = m_file_offset + t_offset,
                    limit = m_file_offset + m_length;

    if (offset >= limit) {
        return 0;
    }
    result = m_program->GetData(offset);
    return (result & 0xFF);
}



/// @brief Gets an word from opcode. Little endian result.
/// @param t_opcode_index An even index.
/// @return The word
unsigned int DisassembledItem::getWordOpcode(unsigned int t_opcode_index)
{
    unsigned int    result = 0,
                    high_byte_index = t_opcode_index + 1,
                    low_byte = 0,
                    high_byte = 0;

    if ((t_opcode_index % 2 > 0)
        || (high_byte_index >= MAX_OPCODE_SIZE)) {
        return 0;
    }
    low_byte = m_real_bytecode[t_opcode_index] & 0xFF;
    high_byte = m_real_bytecode[high_byte_index] & 0xFF;
    result = high_byte << 8 + low_byte;
    return (result & 0xFFFF);
}



/// @brief Gets an word directly from file, limited by m_length. Little endian result.
/// @param t_index The offset from 0 to m_length.
/// @return The word from file
unsigned int DisassembledItem::getWordFromFile(unsigned int t_index)
{
    unsigned int    result = 0;
    unsigned int    offset = m_file_offset + t_index,
                    limit = m_file_offset + m_length,
                    high_byte_offset = offset + 1,
                    low_byte = 0,
                    high_byte = 0;

    if (high_byte_offset >= limit) {
        return 0;
    }
    low_byte = m_program->GetData(offset) & 0xFF;
    high_byte = m_program->GetData(high_byte_offset) & 0xFF;
    result = high_byte << 8 + low_byte;
    return (result && 0xFFFF);
}



/// @brief Gets the value of a selected argument.
/// @param index Selects the argument: 0 to the first, 1 to the second, if exists.
/// @param base_address The base address where the program is loaded in the original machine
/// @return The value of the argument
int DisassembledItem::getArgumentValue(unsigned int index, unsigned int base_address)
{
    if (m_mnemonic->GetArgumentSize() == 2) {
        return static_cast<int>(m_arguments.unsigned_16bit);
    }

    if ((m_mnemonic->GetArgumentCount() == 1) && (m_mnemonic->GetArgumentSize() == 1)) {
        if (m_mnemonic->GetArgument(index).type == OT_RELATIVE_ADDRESS) {
            return convertRelativeToAbsolute(m_arguments.signed_value, base_address);
        }
    }

    if ((m_mnemonic->GetArgumentCount() == 2) && (m_mnemonic->GetArgumentSize() == 1)) {
        if (index == 0) {
            return static_cast<int>(m_arguments.unsigned_8bit_low);
        }
        if (index == 1) {
            return static_cast<int>(m_arguments.unsigned_8bit_high);
        }
    }

    return 0;
}



/// @brief Gets the pointer to the mnemonics list.
/// @return The pointer
MnemonicItem *DisassembledItem::getMnemonic() const
{
    return m_mnemonic;
}



/// @brief Gets the pointer to the object representing the program file
/// @return the pointer
RawData *DisassembledItem::getProgram()
{
    return m_program;
}



/// @brief Setups the argument style
/// @param index Selects the first(0) or the second(1) argument
/// @param style 
void DisassembledItem::SetArgumentStyle(unsigned int index, ArgumentStyleOptions style)
{
    if(!m_arg_style)
        m_arg_style = new ArgumentStyle;

    if(index == 0)
        m_arg_style->first = style;

    if(index == 1)
        m_arg_style->second = style;
}



/// @brief Initializes the DisassembledItem as an instruction code.
/// @param mnemonic The mnemonic of this instruction
/// @param offset in the file
void DisassembledItem::setupInstructionItem(MnemonicItem *mnemonic, const FileOffset offset)
{
    m_mnemonic = mnemonic;
    m_file_offset = offset;
    m_is_data = false;
    m_length = mnemonic->GetByteCodeSize();
    m_mnemonic_signature = mnemonic->GetMnemonicSignature();
    fillArgument();
    copyRealBytecode();
}



/// @brief Initializes the DisassembledItem as data.
/// @param offset in the file
void DisassembledItem::setupDataItem(const FileOffset offset)
{
    clear();
    m_file_offset = offset;
    m_is_data = true;
    m_length = 1;
}



/// @brief Copy the bytecode from program file \
/// @brief 0  1  2  3   <- Position          \
/// @brief aa bb cc dd
void DisassembledItem::copyRealBytecode()
{
    if (m_program && m_mnemonic) {
        for(int offset = 0; offset < m_length; ++offset)
            m_real_bytecode[offset] = m_program->GetData(m_file_offset + offset);
    }
}



/// @brief Calculate and fill arguments fields
void DisassembledItem::fillArgument()
{
    if ((m_mnemonic->GetArgumentCount() == 1) && (m_mnemonic->GetArgumentSize() == 1)) {  // One 8bit argument
        m_arguments.unsigned_8bit_low = getByteOpcode(m_mnemonic->GetArgumentPosition()) & 0xFF;
        m_arguments.unsigned_8bit_high = 0;
        m_arguments.unsigned_16bit = m_arguments.unsigned_8bit_low;
        m_arguments.signed_value = static_cast<int>(getByteOpcode(m_mnemonic->GetArgumentPosition()));
    }
    else {  //Two of 8bit or 1 of 16bit
        m_arguments.unsigned_8bit_low = getByteOpcode(m_mnemonic->GetArgumentPosition()) & 0xFF;
        m_arguments.unsigned_8bit_high = getByteOpcode(m_mnemonic->GetArgumentPosition() + 1) & 0xFF;
        m_arguments.unsigned_16bit = m_arguments.unsigned_8bit_low + m_arguments.unsigned_8bit_high * 0x100;
        m_arguments.signed_value = static_cast<int>(m_arguments.unsigned_16bit);
    }
}



/// @brief Converts a relative address (may be a signed value) to an absolute address
/// @param relative The relative address
/// @param baseaddress The base address where the program is load in the original machine
/// @return The absolute address
AbsoluteAddress DisassembledItem::convertRelativeToAbsolute(RelativeAddress relative, AbsoluteAddress baseaddress)
{
    return static_cast<AbsoluteAddress>(baseaddress + m_mnemonic->GetByteCodeSize() + m_file_offset + relative);
}

