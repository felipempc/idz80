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
#include "rawdata.h"
#include "logbase.h"

typedef std::vector<RawData *> RawDataList;

class RawDataManager: public LogBase
{
    public:
        RawDataManager(LogWindow *logparent);
        virtual ~RawDataManager();

        RawData *AddFile(wxString name);

        RawData *First();
        RawData *Last();
        RawData *Current();
        RawData *Previous();
        RawData *Next();

        RawData *Index(uint index);


    protected:
    private:
        RawDataList data_list_;
        RawData     *current_file_;
        int         current_file_index_;

        LogWindow *logwindow;
};

#endif // RAWDATAMANAGER_H
