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
        wxString DebugCartHeader();
        void DebugLog(wxTextCtrl *log);
        void setCartridge(bool _cart);
        bool isCartridge();

        uint            StartAddress,
                        ExecAddress,
                        EndAddress;

        RawData(void);
        ~RawData(void);

    private:
		static const byte BIN_ID = 0xFE;
		static const uint BIN_HEADER_SIZE = 7;
		static const word ID_CARTRIDGE_ROM = 0x4142;
		static const word ID_CARTRIDGE_SUBROM = 0x4344;

		CartHeader		m_cartridge;
        wxMemoryBuffer  m_buffer;
        wxString        m_filename;
        FileType        m_filetype;
        uint            m_bin_header_offset;
        wxTextCtrl      *m_TC_Log;
        bool			m_iscartridge;

        bool ValidateBIN(unsigned char byte_id);
        bool ValidateCartridge();
        void LogIt(const wxString &str);
};

#endif
