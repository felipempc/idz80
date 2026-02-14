/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   26-06-2014 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Access mnemonics in the container
 **************************************************************/


#ifndef MNEMONICACCESS_H
#define MNEMONICACCESS_H


#include "mnemonic_container_base.hpp"
#include "idz80_basic_types.hpp"



class MnemonicAccess : public MnemonicContainerBase
{
    public:
        MnemonicAccess();

        void Find(UintArray& mnemonics_found_list, byte opcode, uint scanoffset);
        MnemonicItem *FindByOpCode(const ByteCode& code);
        MnemonicItem *FindBySignature(const unsigned int &signature);

        MnemonicItem *First();
        MnemonicItem *Last();
        MnemonicItem *CurrentItem();
        MnemonicItem *Previous();
        MnemonicItem *Next();
        MnemonicItem *Item(MnemonicIndex index);

        MnemonicIndex CurrentIndex();

        bool isEnd();


    protected:
    private:
        MnemonicItem    *m_current_item;
        MnemonicIndex   m_current_index;
};

#endif // MNEMONICACCESS_H
