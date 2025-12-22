/***************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   22-12-2025 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Manager of multiple source code files
 **************************************************************/


#include "source_code_manager.hpp"

SourceCodeManager::SourceCodeManager(DebugLogBase *t_parent)
{
    m_current_item = 0;
    m_current_item_index = -1;
    ModuleName = "SRC_MGR";
    SetTextLog(t_parent->GetTextLog());
}



SourceCodeManager::~SourceCodeManager()
{
    clear();
}



/// @brief Adds a source code "file" to the list
/// @param t_src_code 
/// @return true if succeed
bool SourceCodeManager::addSourceCode(SourceCode *t_src_code)
{
    if (t_src_code) {
        m_current_item = t_src_code;
        ++m_current_item_index;
        m_source_code.push_back(t_src_code);
        return true;
    }
    return false;
}



/// @brief Delete t_index Source Code
/// @param t_index 
void SourceCodeManager::deleteSourceCode(int t_index)
{
    try
    {
        index(t_index);
        if (m_current_item_index < 0)
            return;
        delete m_current_item;
        m_source_code.erase(m_source_code.begin() + m_current_item_index);
        previous();
    }
    catch (const std::out_of_range& oor)
    {
        m_current_item = 0;
        m_current_item_index = -1;
    }
}



/// @brief Deletes objects and clear the Source Code list
void SourceCodeManager::clear()
{
    for (SourceCode *src : m_source_code)
        delete src;
    
    m_source_code.clear();
}



/// @brief Return the the number of Source Code "files" in the list
/// @return 
unsigned int SourceCodeManager::size()
{
    return m_source_code.size();
}



/// @brief The first object source code in the list
/// @return 
SourceCode *SourceCodeManager::first()
{
    if (m_source_code.empty()) {
        m_current_item = 0;
        m_current_item_index = -1;
    }
    else {
        m_current_item = m_source_code.front();
        m_current_item_index = 0;
    }
    
    return m_current_item;
}



/// @brief The last object source code in the list
/// @return 
SourceCode *SourceCodeManager::last()
{
    if (m_source_code.empty()) {
        m_current_item = 0;
        m_current_item_index = -1;
    }
    else {
        m_current_item = m_source_code.back();
        m_current_item_index = m_source_code.size() - 1;
    }

    return m_current_item;
}



/// @brief Current selected object Source Code in the list
/// @return 
SourceCode *SourceCodeManager::current()
{
    return m_current_item;
}



/// @brief Selects and return the previous object Source Code from the actual selected
/// @return 
SourceCode *SourceCodeManager::previous()
{
    if (m_source_code.empty()) {
        m_current_item = 0;
        m_current_item_index = -1;
    }
    else
        if (m_current_item_index > 0) {
            m_current_item_index--;
            m_current_item = m_source_code.at(m_current_item_index);
        }

    return m_current_item;
}



/// @brief Selects and return the next object Source Code from the actual selected
/// @return 
SourceCode *SourceCodeManager::next()
{
    int lastindex = m_source_code.size() - 1;
    if (m_source_code.empty()) {
        m_current_item = 0;
        m_current_item_index = -1;
    }
    else
        if (m_current_item_index < lastindex) {
            ++m_current_item_index;
            m_current_item = m_source_code.at(m_current_item_index);
        }

    return m_current_item;
}



/// @brief Selects and return the object Source Code at position t_index from the list
/// @param t_index 
/// @return 
SourceCode *SourceCodeManager::index(int t_index)
{
    try {
        m_current_item_index = t_index;
        m_current_item = m_source_code.at(m_current_item_index);
    }
    catch(const std::out_of_range& ex)
    {
#ifdef IDZ80_SRC_DEBUG
        LogIt(wxString::Format(" Index -> %s", ex.what()));
#endif
        m_current_item_index = -1;
        m_current_item = 0;
    }

    return m_current_item;
}



/// @brief Current index of selected Source Code
/// @return 
int SourceCodeManager::currentIndex()
{
    return m_current_item_index;
}



bool SourceCodeManager::isEnd()
{
    int lastindex = m_source_code.size() - 1;
    return (m_current_item_index == lastindex);
}


