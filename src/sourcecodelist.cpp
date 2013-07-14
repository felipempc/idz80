/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   04-07-2013 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * This module simplifies interface of std::list
 **************************************************************/



 #include "sourcecodelist.h"


SourceCodeLines::SourceCodeLines()
{
    code_list_.clear();
    code_list_.reserve(10000);
}


SourceCodeLines::~SourceCodeLines()
{
    Clear();
}


void SourceCodeLines::Add(CodeViewItem *cvi)
{
    code_list_.push_back(cvi);
}


void SourceCodeLines::Clear()
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


uint SourceCodeLines::GetCount()
{
    return code_list_.size();
}


CodeViewItem *SourceCodeLines::GetLine(LineNumber line)
{
    if (line >= GetCount())
		return static_cast<CodeViewItem *>(0);
    return code_list_[line];
}



void SourceCodeLines::Insert(CodeViewItem *cvi, LineNumber line)
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


void SourceCodeLines::Remove(CodeViewItem *cvi)
{
    int item = Find(cvi);
    SourceCodeList::iterator it;

    if (item >= 0)
    {
        it = code_list_.begin() + item;
        code_list_.erase(it);
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
}



int SourceCodeLines::Find(CodeViewItem *cvi)
{
   uint counter = 0;
   CodeViewItem *found_cvi = 0;


    for (SourceCodeList::iterator it = code_list_.begin(); it != code_list_.end(); it++)
    {
        found_cvi = *it;
        if (found_cvi == cvi)
        {
            return counter;
        }
        counter++;
    }

    return (-1);
}


