/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   20-04-2014 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * (BASE) Stores the Z80 mnemonic's instruction
 **************************************************************/

#ifndef MNEMONICCONTAINERBASE_H
#define MNEMONICCONTAINERBASE_H

#include <vector>

#include "mnemonic_item.hpp"


typedef std::vector<MnemonicItem *> MnemonicList;
typedef int MnemonicIndex;


class MnemonicContainerBase
{
    public:
    protected:
        MnemonicList m_the_mnemonic_list;
    private:
};

#endif // MNEMONICCONTAINERBASE_H
