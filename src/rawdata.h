/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 * This module stores and manages the program to be disassembled
 **************************************************************/



/*
 * Holds the program in memory
 *
 */

 #ifndef _IDZ80_RAWDATA_H
 #define _IDZ80_RAWDATA_H

#include <wx/file.h>
#include <wx/string.h>
#include <wx/buffer.h>
#include <wx/textctrl.h>
#include "IDZ80Base.h"

#define BIN_ID          0xFE
#define BIN_HEADER_SIZE 7


enum eFileType
{
    UNKNOWN,
    COM,
    ROM,
    BIN
};
typedef enum eFileType FileType;

class RawData
{
    public:

        bool Open(wxString filename);
        void Close();
        byte GetData(uint offset);
        uint GetBufferSize();
        bool IsLoaded();
        wxString GetFileName();
        uint GetFileSize();
        FileType GetFileType();
        wxString GetFileTypeStr();
        void SetFileType(FileType filetype);
        void DebugLog(wxTextCtrl *log);

        uint            StartAddress,
                        ExecAddress,
                        EndAddress;

        RawData(void);
        ~RawData(void);

    private:
        wxMemoryBuffer  m_buffer;
        wxString        m_filename;
        FileType        m_filetype;
        uint            m_bin_header_offset;
        wxTextCtrl      *m_TC_Log;

        bool ValidateBIN(unsigned char byte_id);
        void LogIt(const wxString &str);

};

#endif
