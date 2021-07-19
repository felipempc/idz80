/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y) / Rewrite 30/06/2015
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * This module stores multiple disassembled containers (25/06/21)
 **************************************************************/


#include "disassembled_list.h"


DisassembledList::DisassembledList()
{
    current_item_index_ = -1;
    current_item_ = 0;
}


boolean DisassembledList::Add(DisassembledContainer *dc)
{
    if (dc != 0)
    {
        current_item_ = dc;
        current_item_index_++;
        disassembled_file_.push_back(current_item_);
        return true;
    }
    return false;
}

void DisassembledList::Delete(int index)
{
    try
    {
        disassembled_file_.erase(disassembled_file_.begin() + index);
        //todo: point current_item_xxx to valid item, if needed.
    }
    catch (const std::out_of_range& oor)
    {
        // do something
    }
}


DisassembledContainer *DisassembledList::First()
{

}


DisassembledContainer *DisassembledList::Last()
{

}

DisassembledContainer *DisassembledList::Current()
{

}


DisassembledContainer *DisassembledList::Previous()
{

}

DisassembledContainer *DisassembledList::Next()
{

}


DisassembledContainer *DisassembledList::Index(int index)
{

}


int DisassembledList::CurrentIndex()
{

}


bool DisassembledList::isEnd()
{

}






