#include "disassembled_item_data.h"

DisassembledItemData::DisassembledItemData(RawData* program_file)
{
    is_data_ = true;
    binary_data_ = program_file;
    length_ = 0;
}




unsigned int DisassembledItemData::GetLength()
{
    return length_;
}




void DisassembledItemData::SetLength(unsigned int len)
{
    length_ = len;
}




bool DisassembledItemData::isData()
{
    return is_data_;
}
