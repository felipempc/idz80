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

#include <wx/dynarray.h>

#include "mnemonic_container_base.h"
#include "idz80_basic_types.h"



class MnemonicAccess : public MnemonicContainerBase
{
    public:
        MnemonicAccess();

        void Find(wxArrayInt& mnemonics_found_list, byte opcode, uint scanoffset);
        MnemonicItem *FindByOpCode(const ByteCode& code);

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
        MnemonicItem    *current_item_;
        MnemonicIndex  current_index_;
};

#endif // MNEMONICACCESS_H
