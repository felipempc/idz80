/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   20-12-2013 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Manages multiple file program
 **************************************************************/

#ifndef RAWDATAMANAGER_H
#define RAWDATAMANAGER_H

#include <vector>
#include "rawdata.hpp"
#include "debug_logbase.hpp"

typedef std::vector<RawData *> RawDataList;

class RawDataManager: public DebugLogBase
{
    public:
        RawDataManager(DebugLogBase *logparent);
        virtual ~RawDataManager();

        RawData *AddFile(const wxString name);

        void Clear();

        RawData *First();
        RawData *Last();
        RawData *Current();
        RawData *Previous();
        RawData *Next();

        RawData *Index(const uint index);

        void Remove(const uint index);
        void RemoveCurrent();
        uint Count();

        bool isLoaded();
        bool FileLoaded(const wxString fullpath);

    protected:
    private:
        RawDataList m_data_list;
        RawData     *m_current_file;
        int         m_current_file_index;
};

#endif // RAWDATAMANAGER_H
