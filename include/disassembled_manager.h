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
#include "disassembled_container.h"
#include "logbase.h"


class DisassembledManager: public LogBase
{
    public:
    DisassembledManager(LogBase *parent);

    bool Add(DisassembledContainer *disassembled);
    void Delete(int index);

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

    DisassembledContainer *current_item_;
    int current_item_index_;

};


#endif // DISASSEMBLED_MANAGER_H
