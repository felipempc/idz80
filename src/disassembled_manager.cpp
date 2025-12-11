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


DisassembledManager::DisassembledManager(DebugLogBase *parent)
{
    ModuleName = "DisassembledMGR";
    SetTextLog(parent->GetTextLog());
    m_current_item_index = -1;
    m_current_item = 0;
}



DisassembledManager::~DisassembledManager()
{
    Clear();
}



bool DisassembledManager::Add(DisassembledContainer *disassembled)
{
    if (disassembled != 0)
    {
        m_current_item = disassembled;
        m_current_item_index++;
        m_disassembled_files.push_back(m_current_item);
        return true;
    }
    return false;
}



void DisassembledManager::Delete(int index)
{
    try
    {
        Index(index);
        if (m_current_item_index < 0)
            return;
        delete m_current_item;
        m_disassembled_files.erase(m_disassembled_files.begin() + m_current_item_index);
        Previous();
    }
    catch (const std::out_of_range& oor)
    {
        m_current_item = 0;
        m_current_item_index = -1;
    }
}



/// @brief Deallocate data and clear the vector
void DisassembledManager::Clear()
{
    for(DisassembledContainer *dasmctr : m_disassembled_files)
        delete dasmctr;
    m_disassembled_files.clear();
}



unsigned int DisassembledManager::Size()
{
    return m_disassembled_files.size();
}



DisassembledContainer *DisassembledManager::First()
{
    if (m_disassembled_files.empty()) {
        m_current_item = 0;
        m_current_item_index = -1;
    }
    else {
        m_current_item = m_disassembled_files.front();
        m_current_item_index = 0;
    }
    
    return m_current_item;
}



DisassembledContainer *DisassembledManager::Last()
{
    if (m_disassembled_files.empty()) {
        m_current_item = 0;
        m_current_item_index = -1;
    }
    else {
        m_current_item = m_disassembled_files.back();
        m_current_item_index = m_disassembled_files.size() - 1;
    }

    return m_current_item;
}



DisassembledContainer *DisassembledManager::Current()
{
    return m_current_item;
}



DisassembledContainer *DisassembledManager::Previous()
{
    if (m_disassembled_files.empty()) {
        m_current_item = 0;
        m_current_item_index = -1;
    }
    else
        if (m_current_item_index > 0) {
            m_current_item_index--;
            m_current_item = m_disassembled_files.at(m_current_item_index);
        }

    return m_current_item;
}



DisassembledContainer *DisassembledManager::Next()
{
    int lastindex = m_disassembled_files.size() - 1;
    if (m_disassembled_files.empty()) {
        m_current_item = 0;
        m_current_item_index = -1;
    }
    else
        if (m_current_item_index < lastindex) {
            m_current_item_index++;
            m_current_item = m_disassembled_files.at(m_current_item_index);
        }

    return m_current_item;
}



DisassembledContainer *DisassembledManager::Index(int index)
{

    try {
        m_current_item_index = index;
        m_current_item = m_disassembled_files.at(m_current_item_index);
    }
    catch(const std::out_of_range& ex)
    {
        LogIt(wxString::Format(" Index -> %s", ex.what()));
        m_current_item_index = -1;
        m_current_item = 0;
    }

    return m_current_item;
}



int DisassembledManager::CurrentIndex()
{
    return m_current_item_index;
}



bool DisassembledManager::isEnd()
{
    int lastindex = m_disassembled_files.size() - 1;
    return (m_current_item_index == lastindex);
}
