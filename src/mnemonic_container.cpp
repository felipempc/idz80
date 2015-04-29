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

    if (log)
    {
        log_ = log;

    #ifdef _IDZ80_DEBUG_
        log_->AppendText("Welcome to Mnemonic Container !\n\n");
    #endif
    }
    else
        log_ = 0;
}




MnemonicContainer::~MnemonicContainer()
{
    MnemonicItem    *mnemonic;

    for(unsigned int i = 0; i < mnlist_.size(); i++)
    {
        mnemonic = mnlist_.back();
        delete mnemonic;
    }
    mnlist_.clear();
}




void MnemonicContainer::AddInstruction(MnemonicItem *mnemonicitem)
{
    if (!mnemonicitem)
        mnlist_.push_back(mnemonicitem);
}




void MnemonicContainer::SetStatistics(Statistics &_statistics)
{
    num_groups_ = _statistics.numgroups;
    num_instructions_ = _statistics.numinstructions;
}





void MnemonicContainer::ShowStatistics()
{
    if(log_)
    {
        log_->AppendText(wxString::Format("Number of groups found = %d\n", num_groups_));
        log_->AppendText(wxString::Format("Number of valid instructions found = %d\n", num_instructions_));
    }
}
