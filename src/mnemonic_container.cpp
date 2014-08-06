/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   27-06-2014 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Open and fill the mnemonics database
 **************************************************************/

#include "mnemonic_container.h"

MnemonicContainer::MnemonicContainer(wxTextCtrl *log)
{
    num_groups_ = 0;
    num_instructions_ = 0;

    #ifdef _IDZ80_DEBUG_
    if (log)
    {
        log_ = log;
        log_->AppendText("Welcome to Mnemonic Container !\n\n");
    }
    #endif
}

MnemonicContainer::~MnemonicContainer()
{
    //dtor
}




void MnemonicContainer::AddInstruction(MnemonicItem *mnemonicitem)
{

}


