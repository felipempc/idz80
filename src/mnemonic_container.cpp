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

MnemonicContainer::MnemonicContainer()
{
    num_groups_ = 0;
    num_instructions_ = 0;
    xml_mnemonic = 0;
}

MnemonicContainer::~MnemonicContainer()
{
    //dtor
}




bool MnemonicContainer::Open(const wxString &mnemonicfile)
{
    xml_mnemonic = new wxXmlDocument(mnemonicfile);

    if(xml_mnemonic->IsOk())
    {
        ProcessInstructions();
    }

    delete xml_mnemonic;
    xml_mnemonic = 0;
}




void MnemonicContainer::ProcessFile()
{
    wxArrayString group_list;
    uint index;

    ReadGroups(group_list);

    for(index = 0; index < group_list.GetCount(); index++)
        ProcessGroup(group_list[index]);
}




void MnemonicContainer::ProcessGroup(const wxString &group_str)
{
    MnemonicItem *mnemonic_item;

}




void MnemonicContainer::AddInstruction(MnemonicItem *mnemonicitem)
{

}




void MnemonicContainer::ReadGroups(wxArrayString &grouplist)
{
    wxXmlNode *group_item;

    group_item = xml_mnemonic->GetRoot()->GetChildren();

    if(group_item && (group_item->GetName() == "GROUPS"))
        group_item = group_item->GetChildren();

    while (!group_item)
    {
        ReadInstruction(group_item);
        group_item = group_item->GetNext();
    }
}




void MnemonicContainer::ReadInstruction(wxXmlNode *instruction_node)
{

}




