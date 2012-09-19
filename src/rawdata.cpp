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

#include "rawdata.h"


// RawData implementation

const byte RawData::BIN_ID;
const uint RawData::BIN_HEADER_SIZE;
const word RawData::ID_CARTRIDGE_ROM;
const word RawData::ID_CARTRIDGE_SUBROM;

RawData::RawData(void)
{
    m_buffer.SetDataLen(0);
    SetFileType(UNKNOWN);
    m_bin_header_offset = 0;
    m_cartridge.id = 0;
    m_cartridge.init = 0;
    m_cartridge.statement = 0;
    m_cartridge.device = 0;
    m_cartridge.text = 0;
    
    m_iscartridge = false;
}

RawData::~RawData(void)
{
    m_buffer.SetBufSize(0);
}



void RawData::setCartridge(bool _cart)
{
	m_iscartridge = _cart;
}

bool RawData::isCartridge()
{
	return m_iscartridge;
}


bool RawData::Open(wxString filename)
{
    wxFile 	f;
    void 	*tempbuffer;
    byte 	*binptr;
    uint 	bytesread = 0,
			buffer_size = 0;
    wxString ext;
    bool	isbin, ret;


    ret = false;

    if (f.Open(filename))
    {
        if (IsLoaded())
            Close();

        m_bin_header_offset = 0;

        buffer_size = (uint)f.Length();
        tempbuffer = m_buffer.GetWriteBuf(buffer_size);
        if (!(tempbuffer == NULL))
            bytesread = (uint)f.Read(tempbuffer, buffer_size);
        f.Close();
        if (bytesread == buffer_size)
        {
            m_buffer.UngetWriteBuf(buffer_size);
            binptr = (byte *)m_buffer.GetData();
            isbin = ValidateBIN(*binptr);
            m_filename = filename;
            ext = m_filename.AfterLast('.');
            ext = ext.Lower();
            if (ext == "com")
                SetFileType(COM);
            else
                if (ext == "rom")
                {
                    SetFileType(ROM);
                    if (ValidateCartridge())
						LogIt(DebugCartHeader());
				}
                else
                    if (isbin)
                        SetFileType(BIN);
                    else
                        SetFileType(UNKNOWN);

            ret = true;
        }
        else
            m_buffer.SetDataLen(0);
    }

    #ifdef IDZ80DEBUG
    if (ret)
        LogIt(wxString::Format("rawdata.c: File opened is %s.\n",filename.c_str()));
    else
        LogIt("rawdata.c: File not found !\n");
    #endif

    return ret;
}


void RawData::Close()
{
    memset(m_buffer.GetData(), '\0', m_buffer.GetDataLen());
    m_buffer.SetDataLen(0);
    m_filename.Clear();
}

bool RawData::IsLoaded()
{
    return (m_buffer.GetDataLen() > 0);
}


byte RawData::GetData(uint offset)
{
    byte *buffertemp;
    uint sz;

    sz = (GetBufferSize() - 1);

    if (offset > sz)
        offset = sz;
    buffertemp = (byte *)m_buffer.GetData();
    buffertemp += offset;
    buffertemp += m_bin_header_offset;
    return (*buffertemp);
}

uint RawData::GetBufferSize()
{
    uint sz;
    sz = (uint)m_buffer.GetDataLen();
    sz -= m_bin_header_offset;
    return sz;
}


wxString RawData::GetFileName()
{
    return m_filename;
}

uint RawData::GetFileSize()
{
    return GetBufferSize();
}


FileType RawData::GetFileType()
{
    return m_filetype;
}

wxString RawData::GetFileTypeStr()
{
    wxString str;
    switch (m_filetype)
    {
        case COM:   str = "COM";
                    break;
        case ROM:   str = "ROM";
                    break;
        case BIN:   str = "BIN";
                    break;
        default:    str = "UNKNOWN";
                    break;
    }
    return str;
}

void RawData::SetFileType(FileType filetype)
{
    m_filetype = filetype;
    switch (filetype)
    {
        case COM:
                m_bin_header_offset = 0;
                StartAddress = 0x100;
                EndAddress = 0x100 + GetBufferSize();
                ExecAddress = 0x100;
                break;
        case BIN:
                //FIXME: remove m_bin_header_offset = 0;
                StartAddress = (GetData(1) + 0x100 * GetData(2)) & 0xFFFF;
                EndAddress = (GetData(3) + 0x100 * GetData(4)) & 0xFFFF;
                ExecAddress = (GetData(5) + 0x100 * GetData(6)) & 0xFFFF;
                m_bin_header_offset = BIN_HEADER_SIZE;
                break;
        case ROM:
				if ((m_iscartridge) && (ValidateCartridge()))
				{
					m_bin_header_offset = 0;
					StartAddress = 0x4000;
					EndAddress = StartAddress + GetBufferSize();
					if (m_cartridge.init != 0)
						ExecAddress = m_cartridge.init;
					else
						if (m_cartridge.statement != 0)
							ExecAddress = m_cartridge.statement;
						else
							if (m_cartridge.device != 0)
								ExecAddress = m_cartridge.device;
							else //text != 0 means Basic program
							{
								ExecAddress = m_cartridge.text;
							}
				}
				else
				{
					m_bin_header_offset = 0;
					StartAddress = 0;
					EndAddress = GetBufferSize();
					ExecAddress = 0;
				}
				break;
        default:
        case UNKNOWN:
					m_bin_header_offset = 0;
					StartAddress = 0;
					EndAddress = GetBufferSize();
					ExecAddress = 0;
					break;
    }
}


bool RawData::ValidateBIN(unsigned char byte_id)
{
    bool	ret = false;

    m_bin_header_offset = 0;
    if (byte_id == BIN_ID)
    {
        SetFileType(BIN);
        if ((EndAddress > StartAddress) && (EndAddress > ExecAddress) &&
            (ExecAddress >= StartAddress))
            ret = true;
    }

    return ret;
}

bool RawData::ValidateCartridge()
{
	word header;
	bool	ret = false;

	header = (GetData(1) + 0x100 * GetData(0)) & 0xFFFF;


	if ((header == ID_CARTRIDGE_ROM) || (header == ID_CARTRIDGE_SUBROM))
	{
		ret = true;
		m_cartridge.id = header;
		m_cartridge.init = (GetData(2) + 0x100 * GetData(3)) & 0xFFFF;
		m_cartridge.statement = (GetData(4) + 0x100 * GetData(5)) & 0xFFFF;
		m_cartridge.device = (GetData(6) + 0x100 * GetData(7)) & 0xFFFF;
		m_cartridge.text = (GetData(8) + 0x100 * GetData(9)) & 0xFFFF;
	}
	else
	{
		m_cartridge.id = 0;
		m_cartridge.init = 0;
		m_cartridge.statement = 0;
		m_cartridge.device = 0;
		m_cartridge.text = 0;
	}
	return ret;
}


wxString RawData::DebugCartHeader()
{
	return wxString::Format("ID = 0x%.4X, INIT = 0x%.4X, STATEMENT = 0x%.4X, DEVICE = 0x%.4X, TEXT = 0x%.4X\n", m_cartridge.id,
							 m_cartridge.init, m_cartridge.statement, m_cartridge.device, m_cartridge.text);
}


// TODO: Remove debug log it function
void RawData::DebugLog(wxTextCtrl *log)
{
    m_TC_Log = log;
}

void RawData::LogIt(const wxString &str)
{
    if (m_TC_Log!=0)
        m_TC_Log->AppendText(str);
}

