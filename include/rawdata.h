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
#include <wx/textctrl.h>
#include <wx/dynarray.h>
#include <wx/filename.h>
#include "idz80_base.h"
#include "logbase.h"


enum eFileType
{
    UNKNOWN,
    COM,
    ROM,
    BIN
};
typedef enum eFileType FileType;



struct stCartHeader
{
	word	id;
	word	init;
	word	statement;
	word	device;
	word	text;
};
typedef struct stCartHeader CartHeader;


struct stBinHeader
{
    byte    id;
    word    start_address;
    word    end_address;
    word    execution_address;
};
typedef struct stBinHeader BinHeader;

class RawData : public LogBase
{
    public:
		static const uint BIN_HEADER_SIZE = 7;
		static const uint CARTRIDGE_HEADER_SIZE = 16;

        bool Open(wxString filename);
        void Close();
        void Clear();
        byte GetData(FileOffset offset);
        uint GetBufferSize();
        bool IsLoaded();
        wxString GetFileName();
        wxString GetFileNameAndPath();
        wxString GetFilePath();
        uint GetFileSize();
        FileType GetFileType();
        wxString GetFileTypeStr();
        bool SetFileType(FileType filetype);
        void SetStrFileType(const wxString &str_type);
        void DebugLog(wxTextCtrl *log);
        bool isCartridge();
        bool isROM();
        bool isBIN();
        bool isCOM();
        bool isBasicCartridge();
        const CartHeader *GetCartridgeHeader();
        const BinHeader *GetBinHeader();
        bool CheckCartridge();
        void ForceNoCartridge();
        bool GetEntries(SortedIntArray &entrylist);

        uint            StartAddress,
                        ExecAddress,
                        EndAddress;

        RawData(LogWindow *logparent);
        ~RawData(void);

    private:
		static const byte BIN_ID = 0xFE;
		static const word ID_CARTRIDGE_ROM = 0x4142;
		static const word ID_CARTRIDGE_SUBROM = 0x4344;

		CartHeader		*m_cartridge;
		BinHeader       *m_binheader;
        wxMemoryBuffer  m_buffer;
        wxFileName      rawdata_filename;
        FileType        m_filetype;
        uint            m_header_offset;
        bool			m_iscartridge, m_isbasic;

        bool ValidateBIN();
        bool ValidateCartridge();
};

#endif
