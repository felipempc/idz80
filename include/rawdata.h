/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Store a program to be disassembled
 **************************************************************/



#ifndef _IDZ80_RAWDATA_H
#define _IDZ80_RAWDATA_H

#include <wx/file.h>
#include <wx/string.h>
#include <wx/buffer.h>
#include <wx/filename.h>

#include "logbase.h"

#include "binfile.h"
#include "cartridge_rom_file.h"


enum eFileType
{
    UNKNOWN,
    COM,
    ROM,
    BIN
};
typedef enum eFileType FileType;

typedef unsigned int FileOffset;
typedef unsigned int AbsoluteAddress;


class RawData : public CartridgeRomFile, public BinFile, public LogBase
{
    public:
        bool Open(wxString filename);
        void Close();
        void Clear();

        byte GetData(FileOffset offset);

        unsigned int GetSize();
        unsigned int GetFileSize();

        bool IsLoaded();

        wxString GetFileName();
        wxString GetFileNameAndPath();
        wxString GetFilePath();

        FileType GetFileType();
        wxString GetFileTypeStr();

        void SetFileType(FileType filetype);
        void SetStrFileType(const wxString &str_type);
        void ForceNoCartridge();

        void GuessFileType();

        bool isROM();
        bool isBIN();
        bool isCOM();

        AbsoluteAddress StartAddress,
                        ExecAddress,
                        EndAddress;

        RawData(LogWindow *logparent);
        ~RawData(void);

    private:
        wxMemoryBuffer  buffer_;
        wxFileName      filename_;
        FileType        filetype_;
        FileOffset      header_offset_;

        bool LoadToBuffer(const wxString filename);
        void SetupFileType();
};

#endif
