/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y) / Rewrite 30/06/2015
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * This module stores multiple disassembled containers (25/06/21)
 **************************************************************/


#include "disassembled_manager.h"


DisassembledManager::DisassembledManager(LogBase *parent)
{
    ModuleName = "DisassembledMGR";
    SetTextLog(parent->GetTextLog());
    current_item_index_ = -1;
    current_item_ = 0;
}


bool DisassembledManager::Add(DisassembledContainer *disassembled)
{
    if (disassembled != 0)
    {
        current_item_ = disassembled;
        current_item_index_++;
        m_disassembled_files.push_back(current_item_);
        return true;
    }
    return false;
}

void DisassembledManager::Delete(int index)
{
    try
    {
        m_disassembled_files.erase(m_disassembled_files.begin() + index);
        //todo: point current_item_xxx to valid item, if needed.
    }
    catch (const std::out_of_range& oor)
    {
        //TODO: do something
    }
}


DisassembledContainer *DisassembledManager::First()
{
    if (m_disassembled_files.empty()) {
        current_item_ = 0;
        current_item_index_ = -1;
    }
    else {
        current_item_ = m_disassembled_files.front();
        current_item_index_ = 0;
    }
    
    return current_item_;
}


DisassembledContainer *DisassembledManager::Last()
{
    if (m_disassembled_files.empty()) {
        current_item_ = 0;
        current_item_index_ = -1;
    }
    else {
        current_item_ = m_disassembled_files.back();
        current_item_index_ = m_disassembled_files.size() - 1;
    }

    return current_item_;
}

DisassembledContainer *DisassembledManager::Current()
{
    return current_item_;
}


DisassembledContainer *DisassembledManager::Previous()
{
    if (m_disassembled_files.empty()) {
        current_item_ = 0;
        current_item_index_ = -1;
    }
    else
        if (current_item_index_ > 0) {
            current_item_index_--;
            current_item_ = m_disassembled_files.at(current_item_index_);
        }

    return current_item_;
}

DisassembledContainer *DisassembledManager::Next()
{
    if (m_disassembled_files.empty()) {
        current_item_ = 0;
        current_item_index_ = -1;
    }
    else
        if (current_item_index_ < (m_disassembled_files.size() - 1)) {
            current_item_index_++;
            current_item_ = m_disassembled_files.at(current_item_index_);
        }

    return current_item_;
}


DisassembledContainer *DisassembledManager::Index(int index)
{

    try {
        current_item_index_ = index;
        current_item_ = m_disassembled_files.at(current_item_index_);
    }
    catch(const std::out_of_range& ex)
    {
        LogIt(wxString::Format(" Index -> %s", ex.what()));
        current_item_index_ = -1;
        current_item_ = 0;
    }

    return current_item_;

    /*
    if (m_disassembled_files.empty()) {
        current_item_ = 0;
        current_item_index_ = -1;
    }
    else
        if (current_item_index_ > 0) {
            current_item_index_--;
            current_item_ = m_disassembled_files.at(current_item_index_);
        }

    return current_item_;
    */
}


int DisassembledManager::CurrentIndex()
{
    return current_item_index_;
}


bool DisassembledManager::isEnd()
{
    return (current_item_index_ == (m_disassembled_files.size() - 1));
}
