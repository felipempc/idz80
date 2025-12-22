/***************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   15-01-2014 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Base class for line code access
 **************************************************************/


#include "source_code_access.hpp"


SourceCodeAccess::SourceCodeAccess()
{
    m_current_item = 0;
    m_current_index = 0;
}


/// @brief Return the first object SourceCodeLine* from the source code
/// @return 
SourceCodeLine *SourceCodeAccess::firstLine()
{
    if (!m_sourcecode_list.empty())
    {
        m_current_index = 0;
        m_current_item = m_sourcecode_list[m_current_index];
    }
    else
    {
        m_current_item = 0;
        m_current_index = 0;
    }
    return m_current_item;
}



/// @brief Return the last object SourceCodeLine* from the source code
/// @return 
SourceCodeLine *SourceCodeAccess::lastLine()
{
    if (!m_sourcecode_list.empty())
    {
        m_current_index = m_sourcecode_list.size() - 1;
        //m_current_item = m_sourcecode_list[m_current_index];
        m_current_item = *m_sourcecode_list.end();
    }
    else
    {
        m_current_item = 0;
        m_current_index = 0;
    }

    return m_current_item;
}



/// @brief Go back one step and return the object SourceCodeLine* from the source code
/// @return 
SourceCodeLine *SourceCodeAccess::previousLine()
{
    if ((!m_sourcecode_list.empty())
        && (m_current_index > 0)
        && (m_current_index < m_sourcecode_list.size()))
    {
        --m_current_index;
        m_current_item = m_sourcecode_list[m_current_index];
    }
    else
    {
        m_current_item = 0;
        m_current_index = 0;
    }
    return m_current_item;
}



/// @brief Go ahead one step and return the object SourceCodeLine* from the source code
/// @return 
SourceCodeLine *SourceCodeAccess::nextLine()
{
    if ((!m_sourcecode_list.empty())
        && (m_current_index < (m_sourcecode_list.size() - 1)))
    {
        ++m_current_index;
        m_current_item = m_sourcecode_list[m_current_index];
    }
    else
    {
        m_current_item = 0;
        m_current_index = 0;
    }
    return m_current_item;
}



/// @brief Return the current pointed object SourceCodeLine*
/// @return 
SourceCodeLine *SourceCodeAccess::currentLine()
{
    return m_current_item;
}




/// @brief Return the current line number
/// @return 
LineNumber SourceCodeAccess::currentLineNumber()
{
    return m_current_index;
}



/// @brief Return the object SourceCodeLine* of t_linenumber from the source code
/// @param t_linenumber the line
/// @return Pointer to object SourceCodeLine
SourceCodeLine *SourceCodeAccess::line(LineNumber t_linenumber)
{
    if ((!m_sourcecode_list.empty()) && (t_linenumber < m_sourcecode_list.size()))
    {
        m_current_index = t_linenumber;
        m_current_item = m_sourcecode_list[m_current_index];
    }
    else
    {
        m_current_index = 0;
        m_current_item = 0;
    }
    return m_current_item;
}

