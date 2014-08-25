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

struct Statistics
{
    uint numgroups;
    uint numinstructions;
};

class MnemonicContainer : public MnemonicAccess
{
    public:
        MnemonicContainer(wxTextCtrl *log);
        ~MnemonicContainer();

        void AddInstruction(MnemonicItem *mnemonicitem);
        void SetStatistics(Statistics &_statistics);
        void ShowStatistics();

    protected:
    private:
        uint num_groups_, num_instructions_;


        //DEBUG
        wxTextCtrl *log_;

};

#endif // MNEMONICCONTAINER_H
