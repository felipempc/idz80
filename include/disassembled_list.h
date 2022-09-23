/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y) / Rewrite 30/06/2015
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * This module stores multiple disassembled containers (25/06/21)
 **************************************************************/

//TODO: Remove
#ifndef DISASSEMBLED_LIST_H_INCLUDED
#error "This file must be deleted"
#endif // DISASSEMBLED_LIST_H_INCLUDED

#ifdef DISASSEMBLED_LIST_H_INCLUDED
#define DISASSEMBLED_LIST_H_INCLUDED


#include <vector>
#include "disassembled_container.h"



class DisassembledList
{
    public:
    DisassembledList();

    boolean Add(*DisassembledContainer);
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


#endif // DISASSEMBLED_LIST_H_INCLUDED