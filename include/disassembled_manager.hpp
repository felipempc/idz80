/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y) / Rewrite 30/06/2015
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * This module stores multiple disassembled containers (25/06/21)
 **************************************************************/


#ifndef DISASSEMBLED_MANAGER_H
#define DISASSEMBLED_MANAGER_H


#include <vector>
#include "disassembled_container.hpp"
#include "debug_logbase.hpp"


class DisassembledManager: public DebugLogBase
{
    public:
        DisassembledManager(DebugLogBase *parent);
        ~DisassembledManager();

        bool Add(DisassembledContainer *disassembled);
        void Delete(int index);
        void Clear();
        unsigned int Size();

        DisassembledContainer *First();
        DisassembledContainer *Last();
        DisassembledContainer *Current();
        DisassembledContainer *Previous();
        DisassembledContainer *Next();
        DisassembledContainer *Index(int index);

        int CurrentIndex();

        bool isEnd();

    private:
    std::vector<DisassembledContainer *> m_disassembled_files;

    DisassembledContainer *m_current_item;
    int m_current_item_index;

};


#endif // DISASSEMBLED_MANAGER_H
