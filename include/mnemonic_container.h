/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   27-06-2014 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Open and fill the mnemonics database
 **************************************************************/


#ifndef MNEMONICCONTAINER_H
#define MNEMONICCONTAINER_H

#include <wx/xml/xml.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <vector>
#include "mnemonic_access.h"


typedef std::vector<wxXmlNode *> NodeGroupList;


class MnemonicContainer : public MnemonicAccess
{
    public:
        MnemonicContainer(wxTextCtrl *log);
        ~MnemonicContainer();

        bool Open(const wxString &mnemonicfile);
    protected:
    private:
        uint num_groups_, num_instructions_;
        wxXmlDocument *xml_mnemonic;

        //DEBUG
        wxTextCtrl *log_;

        void AddInstruction(MnemonicItem *mnemonicitem);
        void ProcessFile();
        void ProcessGroup(const wxXmlNode *groupitem);
        bool FindGroups(NodeGroupList &grouplist);
        void ReadInstruction(wxXmlNode *instruction_node);
};

#endif // MNEMONICCONTAINER_H
