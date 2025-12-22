/***************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   17-01-2014 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Defines a container to store source code lines
 **************************************************************/

#include "source_code_container.hpp"


const unsigned int SourceCodeContainer::LINE_NOT_FOUND;


SourceCodeContainer::SourceCodeContainer()
{
    m_sourcecode_list.clear();
    m_sourcecode_list.reserve(10000);
}



SourceCodeContainer::~SourceCodeContainer()
{
    clear();
}



/// @brief Adds a new line to the source code
/// @param t_scline Pointer to new source code line
void SourceCodeContainer::addLine(SourceCodeLine *t_scline)
{
    m_sourcecode_list.push_back(t_scline);
}



void SourceCodeContainer::clear()
{
    for (auto scline : m_sourcecode_list)
    {
        if (scline)
        {
            if (scline->comment)
                delete scline->comment;
            if (scline->rectArg1)
                delete scline->rectArg1;
            if (scline->rectArg2)
                delete scline->rectArg2;

            delete scline;
        }
    }

    m_sourcecode_list.clear();
}



/// @brief Get total lines of the source code
/// @return Total lines of the source code
unsigned int SourceCodeContainer::getCount()
{
    return m_sourcecode_list.size();
}



/// @brief Insert a line of code before t_line
/// @param t_scline Pointer to SourceCodeLine
/// @param t_line 
void SourceCodeContainer::insertLine(SourceCodeLine *t_scline, LineNumber t_line)
{
    SourceCodeList::iterator it;

    if ((t_line > m_sourcecode_list.size()) || (t_scline == 0))
        return;

    if (t_line == m_sourcecode_list.size()) {
        m_sourcecode_list.push_back(t_scline);
        return;
    }

    it = m_sourcecode_list.begin() + t_line;
    m_sourcecode_list.insert(it, t_scline);
}



/// @brief Remove a line (t_line) of source code
/// @param t_line Line to be removed
void SourceCodeContainer::removeLine(const LineNumber t_line)
{
    SourceCodeList::iterator it;
    SourceCodeLine *scline;

    scline = m_sourcecode_list[t_line];
    if (scline)
    {
        if (scline->comment)
            delete scline->comment;
        if (scline->rectArg1)
            delete scline->rectArg1;
        if (scline->rectArg2)
            delete scline->rectArg2;
        delete scline;
    }

    it = m_sourcecode_list.begin() + (t_line - 1);
    m_sourcecode_list.erase(it);
}



/// @brief Remove a line (based on SourceCodeLine * info)
/// @param t_scline pointer to the source code line
void SourceCodeContainer::removeLine(const SourceCodeLine *t_scline)
{
    LineNumber item;
    SourceCodeList::iterator it;

    try
    {
        item = findLine(t_scline);
    }
    catch (uint e)
    {
        if (e == LINE_NOT_FOUND)
            return;
    }

    removeLine(item);
}




/// @brief Finds a line of code (based on SourceCodeLine*)
/// @param t_scline Pointer to the SourceCodeLine*
/// @return the line of source code found
LineNumber SourceCodeContainer::findLine(const SourceCodeLine *t_scline)
{
   LineNumber index = 0;

    for (auto found_item : m_sourcecode_list) {
        if (found_item == t_scline)
            return index;

        ++index;
    }

    throw LINE_NOT_FOUND;
    return 0;
}
