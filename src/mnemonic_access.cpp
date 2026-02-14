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
    m_current_item = 0;
    m_current_index = -1;
}


/// @brief Looks for an opcode within a bytecode at scanoffset position.\
/// @brief 00 | 11 | 22 | 33 (bytecode of 4 bytes (opcodes) in this example)\
/// @brief ww | xx | yy | zz\
/// @brief ^- scanoffset
/// @param mnemonics_found_list list of bytecode found based on opcode and scanoffset
/// @param opcode a byte part of a bytecode.
/// @param scanoffset position part of the bytecode that is being searched.
void MnemonicAccess::Find(UintArray& mnemonics_found_list, byte opcode, uint scanoffset)
{
    uint			item_count, remaining_items, index;
    UintArray		temporary_found;
    bool			arg_detected;
    byte			opcodetest;

    if (scanoffset == 0)
    {
        mnemonics_found_list.clear();
        remaining_items = m_the_mnemonic_list.size();
    }
    else
        remaining_items = mnemonics_found_list.size();

    arg_detected = true;
    if (remaining_items > 1)
    {
        temporary_found.clear();
        for (item_count = 0; item_count < remaining_items; ++item_count)
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
                        mnemonics_found_list.push_back(item_count);
                    else
                        temporary_found.push_back(index);
                }
            }
        }
        if ((scanoffset > 0) && (!arg_detected))
        {
            mnemonics_found_list.clear();
            mnemonics_found_list = temporary_found;
        }
    }
}



/// @brief Searches for item of the given ByteCode
/// @param code to be searched in the list 
/// @return pointer to MnemonicItem found or 0, if not found.
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



/// @brief Searches for item of the given signature
/// @param signature to be searched in the list 
/// @return pointer to MnemonicItem found or 0, if not found.
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



/// @brief Selects first item from MnemonicList and makes it current item
/// @return pointer to the selected MnemonicItem
MnemonicItem *MnemonicAccess::First()
{
    if(m_the_mnemonic_list.size() > 0)
    {
        m_current_index = 0;
        m_current_item = m_the_mnemonic_list[m_current_index];
        return m_current_item;
    }
    return static_cast<MnemonicItem *>(0);
}



/// @brief Selects last item from MnemonicList and makes it current item
/// @return pointer to the selected MnemonicItem
MnemonicItem *MnemonicAccess::Last()
{
    uint last = m_the_mnemonic_list.size();
    if(last > 0)
    {
        m_current_index = last - 1;
        m_current_item = m_the_mnemonic_list[m_current_index];
        return m_current_item;
    }
    return static_cast<MnemonicItem *>(0);
}



/// @brief Current pointer of MnemonicItem
/// @return Current pointer of MnemonicItem
MnemonicItem *MnemonicAccess::CurrentItem()
{
    return m_current_item;
}



/// @brief Selects previous item from MnemonicList and makes it current item
/// @return pointer to the selected MnemonicItem
MnemonicItem *MnemonicAccess::Previous()
{
    if(m_current_index > 0)
    {
        --m_current_index;
        m_current_item = m_the_mnemonic_list[m_current_index];
        return m_current_item;
    }
    return static_cast<MnemonicItem *>(0);
}



/// @brief Selects next item from MnemonicList and makes it current item
/// @return pointer to the selected MnemonicItem
MnemonicItem *MnemonicAccess::Next()
{
    int lastitem = m_the_mnemonic_list.size() - 1;
    if((m_current_index >= 0) && (m_current_index < lastitem))
    {
        ++m_current_index;
        m_current_item = m_the_mnemonic_list[m_current_index];
        return m_current_item;
    }
    return static_cast<MnemonicItem *>(0);
}



/// @brief Selects an item from the mnemonic list
/// @param index to the mnemonic list
/// @return pointer to MnemonicItem
MnemonicItem *MnemonicAccess::Item(MnemonicIndex index)
{
    int listsize = m_the_mnemonic_list.size();
    if((index >= 0) && (index < listsize))
    {
        m_current_index = index;
        m_current_item = m_the_mnemonic_list[index];
        return m_current_item;
    }
    return static_cast<MnemonicItem *>(0);
}



/// @brief Current index of the list
/// @return Current index of the list
MnemonicIndex MnemonicAccess::CurrentIndex()
{
    return m_current_index;
}



/// @brief Verify if m_current_index points to last item of the list
/// @return true if m_current_index points to last item.
bool MnemonicAccess::isEnd()
{
    if(m_the_mnemonic_list.size() > 0) {
        int lastitem = m_the_mnemonic_list.size() - 1;
        return (m_current_index == lastitem);
    }
    return true;
}

