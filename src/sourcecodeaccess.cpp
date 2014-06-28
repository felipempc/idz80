/***************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   15-01-2014 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Base class for line code access
 **************************************************************/


#include "sourcecodeaccess.h"



SourceCodeAccess::SourceCodeAccess()
{
    current_item_ = 0;
    current_index_ = 0;
}




CodeViewItem *SourceCodeAccess::First()
{
    if (!code_list_.empty())
    {
        current_index_ = 0;
        current_item_ = code_list_[current_index_];
    }
    else
    {
        current_item_ = 0;
        current_index_ = 0;
    }
    return current_item_;
}




CodeViewItem *SourceCodeAccess::Last()
{
    if (!code_list_.empty())
    {
        current_index_ = code_list_.size() - 1;
        current_item_ = code_list_[current_index_];
    }
    else
    {
        current_item_ = 0;
        current_index_ = 0;
    }

    return current_item_;
}




CodeViewItem *SourceCodeAccess::Previous()
{
    if ((!code_list_.empty()) && (current_index_ > 0) && (current_index_ < code_list_.size()))
    {
        current_index_--;
        current_item_ = code_list_[current_index_];
    }
    else
    {
        current_item_ = 0;
        current_index_ = 0;
    }
    return current_item_;
}




CodeViewItem *SourceCodeAccess::Next()
{
    if ((!code_list_.empty()) && (current_index_ < (code_list_.size() - 1)))
    {
        current_index_++;
        current_item_ = code_list_[current_index_];
    }
    else
    {
        current_item_ = 0;
        current_index_ = 0;
    }
    return current_item_;
}




CodeViewItem *SourceCodeAccess::CurrentItem()
{
    return current_item_;
}




LineNumber SourceCodeAccess::CurrentIndex()
{
    return current_index_;
}




CodeViewItem *SourceCodeAccess::Line(LineNumber linenumber)
{
    if ((!code_list_.empty()) && (linenumber < code_list_.size()))
    {
        current_index_ = linenumber;
        current_item_ = code_list_[current_index_];
    }
    else
    {
        current_index_ = 0;
        current_item_ = 0;
    }
    return current_item_;
}

