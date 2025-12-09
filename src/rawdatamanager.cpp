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

// DEFinition for debug = IDZ80_RAWDATAMGR_DEBUG
//#define IDZ80_RAWDATAMGR_DEBUG

RawDataManager::RawDataManager(LogBase *logparent)
{
    SetTextLog(logparent->GetTextLog());
    ModuleName = "RAWDATAMGR";
    m_current_file_index = -1;
    m_current_file = 0;
}




RawData *RawDataManager::AddFile(const wxString name)
{
    try
    {
        m_current_file = new RawData(this);
    }
    catch (...)
    {
        m_current_file = 0;
        m_current_file_index = -1;
        LogIt("Error creating rawdata !");
        return 0;
    }

    if (m_current_file->Open(name))
    {
        m_data_list.push_back(m_current_file);

        #ifdef IDZ80_RAWDATAMGR_DEBUG
        LogIt(m_current_file->GetFileName() + "   " + m_current_file->GetFileTypeStr());
        #endif
    }
    else
    {
        delete m_current_file;
        m_current_file = 0;
        LogIt(wxString::Format("Error opening file: %s", name));
    }
    m_current_file_index = m_data_list.size() - 1;

    return m_current_file;
}



void RawDataManager::Clear()
{
    RawData *program_file;
    int size;

    size = m_data_list.size();

    for(int i = 0; i < size; i++)
    {
        program_file = m_data_list.back();
        if (program_file)
            delete program_file;
        m_data_list.pop_back();
    }
    m_current_file = 0;
    m_current_file_index = -1;    
}



RawDataManager::~RawDataManager()
{
    Clear();
}




RawData *RawDataManager::Index(const uint index)
{
    if (index >= m_data_list.size())
        return 0;
    m_current_file = m_data_list[index];
    m_current_file_index = static_cast<int>(index);
    return m_current_file;
}



void RawDataManager::Remove(const uint index)
{
    uint last_index = m_data_list.size() - 1;

    if (index >= m_data_list.size())
        return;
    
    if (Index(index) != 0)
        delete m_current_file;

    m_data_list.erase(m_data_list.begin() + index);

    if (index == last_index)
        Last();
    else
        m_current_file = m_data_list[index];
}



void RawDataManager::RemoveCurrent()
{
    if ((m_current_file_index >= 0) && (m_current_file)) {
        delete m_current_file;
        m_data_list.erase(m_data_list.begin() + m_current_file_index);
        m_current_file_index--;
        if (m_current_file_index < 0)
            m_current_file = 0;
        else
            Index(m_current_file_index);
    }
}



uint RawDataManager::Count()
{
    return m_data_list.size();
}



bool RawDataManager::isLoaded()
{
    return (m_data_list.size() > 0);
}



/// @brief Checks if a file has been opened and in the program list.
/// @param fullpath 
/// @return true if it's in the program list
bool RawDataManager::FileLoaded(const wxString fullpath)
{
    for (RawData* program: m_data_list) {
        if (program->GetFileNameAndPath().IsSameAs(fullpath, false)) {
            return true;
        }
    }
    return false;
}



RawData *RawDataManager::First()
{
    if(m_data_list.size() > 0)
    {
        m_current_file = m_data_list[0];
        m_current_file_index = 0;
    }
    else
    {
        m_current_file = 0;
        m_current_file_index = -1;
    }
    return m_current_file;
}




RawData *RawDataManager::Current()
{
    if (m_data_list.size() == 0)
    {
        m_current_file = 0;
        m_current_file_index = -1;
    }
    return m_current_file;
}




RawData *RawDataManager::Last()
{
    unsigned int size = m_data_list.size();

    if (size > 0)
    {
        m_current_file = m_data_list[size - 1];
        m_current_file_index = size - 1;
    }
    else
    {
        m_current_file = 0;
        m_current_file_index = -1;
    }
    return m_current_file;
}


RawData *RawDataManager::Next()
{
    int size = m_data_list.size();

    m_current_file_index++;

    if (m_current_file_index < size)
    {
        m_current_file = m_data_list[m_current_file_index];
    }
    else
    {
        m_current_file_index--;
    }
    return m_current_file;

}


RawData *RawDataManager::Previous()
{
    m_current_file_index--;

    if (m_current_file_index > 0)
    {
        m_current_file = m_data_list[m_current_file_index];
    }
    else
        m_current_file = First();

    return m_current_file;

}


