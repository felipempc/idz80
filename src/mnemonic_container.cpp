/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   27-06-2014 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Open and fill the mnemonics database
 **************************************************************/

#include "mnemonic_container.hpp"

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
    Clear();
}




void MnemonicContainer::Clear()
{
    MnemonicItem    *mnemonic = 0;
    unsigned int    to_delete = m_the_mnemonic_list.size();

    num_groups_ = 0;
    num_instructions_ = 0;

    while(to_delete--)
    {
        mnemonic = m_the_mnemonic_list.back();
        m_the_mnemonic_list.pop_back();
        delete mnemonic;
    }
    m_the_mnemonic_list.clear();
}




void MnemonicContainer::AddInstruction(MnemonicItem *mnemonicitem)
{
    if (mnemonicitem)
        m_the_mnemonic_list.push_back(mnemonicitem);
}



unsigned int MnemonicContainer::GetCount()
{
    return m_the_mnemonic_list.size();
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
        log_->AppendText(wxString::Format("Number of valid instructions found = %d (%d)\n", num_instructions_, GetCount()));
    }
}
