/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y) / Rewrite 30/06/2015
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * This module stores multiple disassembled containers (25/06/21)
 **************************************************************/


#ifndef DISASSEMBLED_CONTAINER_LIST_H_INCLUDED
#define DISASSEMBLED_CONTAINER_LIST_H_INCLUDED


#include <vector>
#include "disassembled_container.h"



class DisassembledContainerList
{
    public:
    DisassembledContainerList();

    bool Add(DisassembledContainer *dc);
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
    std::vector<DisassembledContainer *> disassembled_file_;

    DisassembledContainer *current_item_;
    int current_item_index_;

};


#endif // DISASSEMBLED_CONTAINER_LIST_H_INCLUDED
