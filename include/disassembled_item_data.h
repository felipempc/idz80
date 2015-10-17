#ifndef DISASSEMBLEDITEMDATA_H
#define DISASSEMBLEDITEMDATA_H

#include <disassembled_item_base.h>


class DisassembledItemData : public DisassembledItemBase
{
    public:
        DisassembledItemData(RawData* program_file);
        unsigned int GetLength();
        void SetLength(unsigned int len);
        bool isData();

    protected:
        bool is_data_;
        unsigned int length_;
    private:
};

#endif // DISASSEMBLEDITEMDATA_H
