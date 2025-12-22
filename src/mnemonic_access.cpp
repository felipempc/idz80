/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   26-06-2014 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Access mnemonics in the container
 **************************************************************/

#include "mnemonic_access.hpp"

MnemonicAccess::MnemonicAccess()
{
    current_item_ = 0;
    current_index_ = -1;
}


void MnemonicAccess::Find(wxArrayInt& mnemonics_found_list, byte opcode, uint scanoffset)
{
    uint			item_count, remaining_items, index;
    wxArrayInt		temporary_found;
    bool			arg_detected;
    byte			opcodetest;

    if (scanoffset == 0)
    {
        mnemonics_found_list.Clear();
        remaining_items = mnlist_.size();
    }
    else
        remaining_items = mnemonics_found_list.GetCount();

    arg_detected = true;
    if (remaining_items > 1)
    {
        temporary_found.Clear();
        for (item_count = 0; item_count < remaining_items; item_count++)
        {

            if (scanoffset == 0)
                index = item_count;
            else
                index = mnemonics_found_list[item_count];

            Item(index);

            /* In four-byte instructions of Z80, there are opcodes that
               have its argument in the middle of it (DD CB's and FD CB's series)
               so we must shift right the index to take the code
               The test below detects this condition
             */
            if ((!(CurrentItem()->GetArgumentPosition() == scanoffset)) || (CurrentItem()->GetArgumentPosition() == 0))
            {
                arg_detected = false;
                opcodetest = CurrentItem()->GetByteCode(scanoffset);
                if (opcode == opcodetest)
                {
                    if (scanoffset == 0)
                        mnemonics_found_list.Add(item_count);
                    else
                        temporary_found.Add(index);
                }
            }
        }
        if ((scanoffset > 0) && (!arg_detected))
        {
            mnemonics_found_list.Clear();
            mnemonics_found_list = temporary_found;
        }
    }
}




MnemonicItem *MnemonicAccess::FindByOpCode(const ByteCode& code)
{
    if (First() != 0)
        while (!isEnd()) {
            if (memcmp(CurrentItem()->GetByteCode(), &code, MAX_OPCODE_SIZE))
                return CurrentItem();

            if (Next() == 0)
                break;
        };

    return static_cast<MnemonicItem *>(0);
}




MnemonicItem *MnemonicAccess::FindBySignature(const unsigned int &signature)
{
    if(First() != 0)
        while(!isEnd())
        {
            if(CurrentItem()->GetMnemonicSignature() == signature)
                return CurrentItem();

            if(Next() == 0)
                break;
        }
    return static_cast<MnemonicItem *>(0);
}




MnemonicItem *MnemonicAccess::First()
{
    if(mnlist_.size() > 0)
    {
        current_index_ = 0;
        current_item_ = mnlist_[current_index_];
        return current_item_;
    }
    return static_cast<MnemonicItem *>(0);
}




MnemonicItem *MnemonicAccess::Last()
{
    uint last = mnlist_.size();
    if(last > 0)
    {
        current_index_ = last - 1;
        current_item_ = mnlist_[current_index_];
        return current_item_;
    }
    return static_cast<MnemonicItem *>(0);
}




MnemonicItem *MnemonicAccess::CurrentItem()
{
    return current_item_;
}




MnemonicItem *MnemonicAccess::Previous()
{
    if(current_index_ > 0)
    {
        current_index_--;
        current_item_ = mnlist_[current_index_];
        return current_item_;
    }
    return static_cast<MnemonicItem *>(0);
}




MnemonicItem *MnemonicAccess::Next()
{
    int lastitem = mnlist_.size() - 1;
    if((current_index_ >= 0) && (current_index_ < lastitem))
    {
        current_index_++;
        current_item_ = mnlist_[current_index_];
        return current_item_;
    }
    return static_cast<MnemonicItem *>(0);
}




MnemonicItem *MnemonicAccess::Item(MnemonicIndex index)
{
    int listsize = mnlist_.size();
    if((index >= 0) && (index < listsize))
    {
        current_index_ = index;
        current_item_ = mnlist_[index];
        return current_item_;
    }
    return static_cast<MnemonicItem *>(0);
}



MnemonicIndex MnemonicAccess::CurrentIndex()
{
    return current_index_;
}



bool MnemonicAccess::isEnd()
{
    if(mnlist_.size() > 0) {
        int lastitem = mnlist_.size() - 1;
        return (current_index_ == lastitem);
    }
    return true;
}
