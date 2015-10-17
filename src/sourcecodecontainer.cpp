/***************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   17-01-2014 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Defines a container to store source code lines
 **************************************************************/

#include "source_code_container.h"


const uint SourceCodeContainer::LINE_NOT_FOUND;


SourceCodeContainer::SourceCodeContainer()
{
    code_list_.clear();
    code_list_.reserve(10000);
}




SourceCodeContainer::~SourceCodeContainer()
{
    Clear();
}




void SourceCodeContainer::Add(CodeViewItem *cvi)
{
    code_list_.push_back(cvi);
}




void SourceCodeContainer::Clear()
{
    CodeViewItem    *cvi;
    SourceCodeList::iterator    it;

    for(it = code_list_.begin(); it != code_list_.end(); it++)
    {
        cvi = *it;
        if (cvi)
        {
            if (cvi->Comment)
                delete cvi->Comment;
            if (cvi->RectArg1)
                delete cvi->RectArg1;
            if (cvi->RectArg2)
                delete cvi->RectArg2;
            delete cvi;
        }
    }
    code_list_.clear();
}




uint SourceCodeContainer::GetCount()
{
    return code_list_.size();
}




void SourceCodeContainer::Insert(CodeViewItem *cvi, LineNumber line)
{
    SourceCodeList::iterator it;

    if ((line > code_list_.size()) || (cvi == 0))
        return;

    if (line == code_list_.size())
    {
        code_list_.push_back(cvi);
        return;
    }

    it = code_list_.begin() + line;
    code_list_.insert(it, cvi);

}




void SourceCodeContainer::RemoveLine(const LineNumber line)
{
    SourceCodeList::iterator it;
    CodeViewItem *cvi;

    cvi = code_list_[line];
    if (cvi)
    {
        if (cvi->Comment)
            delete cvi->Comment;
        if (cvi->RectArg1)
            delete cvi->RectArg1;
        if (cvi->RectArg2)
            delete cvi->RectArg2;
        delete cvi;
    }

    it = code_list_.begin() + (line - 1);
    code_list_.erase(it);
}




void SourceCodeContainer::Remove(CodeViewItem *cvi)
{
    LineNumber item;
    SourceCodeList::iterator it;

    try
    {
        item = Find(cvi);
    }
    catch (uint e)
    {
        if (e == LINE_NOT_FOUND)
            return;
    }

    RemoveLine(item);
}




LineNumber SourceCodeContainer::Find(CodeViewItem *cvi)
{
   LineNumber index = 0;
   CodeViewItem *found_cvi = 0;


    for (SourceCodeList::iterator it = code_list_.begin(); it != code_list_.end(); it++)
    {
        found_cvi = *it;
        if (found_cvi == cvi)
        {
            return index;
        }
        index++;
    }

    throw LINE_NOT_FOUND;
    return 0;
}



