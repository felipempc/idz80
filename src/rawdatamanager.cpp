/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   20-12-2013 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Manages multiple file program
 **************************************************************/


#include "rawdatamanager.h"

RawDataManager::RawDataManager(LogWindow *logparent)
{
    logwindow = logparent;
    SetTextLog(logparent);
    ModuleName = "RAWDATAMGR";
    current_file_index_ = -1;
    current_file_ = 0;
}




RawData *RawDataManager::AddFile(wxString name)
{
    try
    {
        current_file_ = new RawData(logwindow);
    }
    catch (...)
    {
        current_file_ = 0;
        current_file_index_ = -1;
        LogIt("Error creating rawdata !");
        return 0;
    }

    if (current_file_->Open(name))
    {
        data_list_.push_back(current_file_);
        LogIt(current_file_->GetFileName() + "   " + current_file_->GetFileTypeStr());
    }
    else
    {
        delete current_file_;
        current_file_ = 0;
        LogIt(wxString::Format("Error opening file: %s", name));
    }
    current_file_index_ = data_list_.size() - 1;

    return current_file_;
}



void RawDataManager::Clear()
{
    RawData *program_file;
    int size;

    size = data_list_.size();

    for(int i = 0; i < size; i++)
    {
        program_file = data_list_.back();
        if (program_file)
            delete program_file;
        data_list_.pop_back();
    }
    current_file_ = 0;
    current_file_index_ = -1;    
}



RawDataManager::~RawDataManager()
{
    Clear();
}




RawData *RawDataManager::Index(uint index)
{
    if (index >= data_list_.size())
        return 0;
    current_file_ = data_list_[index];
    current_file_index_ = static_cast<int>(index);
    return current_file_;
}



void RawDataManager::Remove(uint index)
{
    if (index >= data_list_.size())
        return;
    
    if (Index(index) != 0) {
        delete current_file_;
        current_file_ = 0;
    }
    
    data_list_.erase(data_list_.begin() + index);

    First();    // Don't know where to go, go to First element then.
}



RawData *RawDataManager::First()
{
    if(data_list_.size() > 0)
    {
        current_file_ = data_list_[0];
        current_file_index_ = 0;
    }
    else
    {
        current_file_ = 0;
        current_file_index_ = -1;
    }
    return current_file_;
}




RawData *RawDataManager::Current()
{
    if (data_list_.size() == 0)
    {
        current_file_ = 0;
        current_file_index_ = -1;
    }
    return current_file_;
}




RawData *RawDataManager::Last()
{
    unsigned int size = data_list_.size();

    if (size > 0)
    {
        current_file_ = data_list_[size - 1];
        current_file_index_ = size - 1;
    }
    else
    {
        current_file_ = 0;
        current_file_index_ = -1;
    }
    return current_file_;
}


RawData *RawDataManager::Next()
{
    unsigned int size = data_list_.size();

    current_file_index_++;

    if (current_file_index_ < size)
    {
        current_file_ = data_list_[current_file_index_];
    }
    else
    {
        current_file_index_--;
    }
    return current_file_;

}


RawData *RawDataManager::Previous()
{
    unsigned int size = data_list_.size();

    current_file_index_--;

    if (current_file_index_ > 0)
    {
        current_file_ = data_list_[current_file_index_];
    }
    else
        current_file_ = First();

    return current_file_;

}


