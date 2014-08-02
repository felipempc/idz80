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
    xml_mnemonic = 0;
    if (log)
    {
        log_ = log;
        log_->AppendText("Welcome to Mnemonic Container !\n\n");
    }
}

MnemonicContainer::~MnemonicContainer()
{
    //dtor
}




bool MnemonicContainer::Open(const wxString &mnemonicfile)
{
    xml_mnemonic = new wxXmlDocument(mnemonicfile);
    bool ret;

    ret = xml_mnemonic->IsOk();
    if(ret)
    {
        ProcessFile();
        log_->AppendText("Open file completed !\n");
    }
    else
    {
        log_->AppendText("XML file failed to load !\n\n");
    }


    delete xml_mnemonic;
    xml_mnemonic = 0;
    return ret;
}




void MnemonicContainer::ProcessFile()
{
    NodeGroupList   group_list;
    wxXmlNode *group_item;
    uint index;

    if (!FindGroups(group_list))
        return;


    for(index = 0; index < group_list.size(); index++)
    {
        group_item = group_list[index];
        log_->AppendText(wxString::Format("%d: Processing group '%s'.\n", index, group_item->GetName()));
        ProcessGroup(group_item);
    }
}




void MnemonicContainer::ProcessGroup(const wxXmlNode *groupitem)
{
    MnemonicItem *mnemonic_item;
    wxXmlNode *nextitem;

    nextitem = groupitem->GetChildren();

    while (nextitem)
    {
        log_->AppendText(wxString::Format("Item '%s'\n", nextitem->GetAttribute("STRING", "Not Found")));
        nextitem = nextitem->GetNext();
    }

}




void MnemonicContainer::AddInstruction(MnemonicItem *mnemonicitem)
{

}




bool MnemonicContainer::FindGroups(NodeGroupList &grouplist)
{
    wxXmlNode *group_item;
    bool ret;

    num_groups_ = 0;
    grouplist.clear();

    group_item = xml_mnemonic->GetRoot()->GetChildren();
    group_item = group_item->GetChildren();

    while (group_item)
    {
        grouplist.push_back(group_item);
        num_groups_++;

        group_item = group_item->GetNext();
    }

    log_->AppendText(wxString::Format("Total groups = %d\n", num_groups_));

    return (num_groups_ > 0);
}




void MnemonicContainer::ReadInstruction(wxXmlNode *instruction_node)
{

}




