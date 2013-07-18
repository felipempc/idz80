/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Store a program to be disassembled
 **************************************************************/


#include "rawdata.h"


// RawData implementation

const byte RawData::BIN_ID;
const uint RawData::BIN_HEADER_SIZE;
const uint RawData::CARTRIDGE_HEADER_SIZE;
const word RawData::ID_CARTRIDGE_ROM;
const word RawData::ID_CARTRIDGE_SUBROM;


RawData::RawData(LogWindow *logparent)
{
    m_buffer.SetDataLen(0);
    SetFileType(UNKNOWN);
    m_header_offset = 0;
    m_cartridge = 0;
    m_binheader = 0;
    m_iscartridge = false;
    m_isbasic = false;
    ModuleName = "RawData";
    SetTextLog(logparent);
}

RawData::~RawData(void)
{
    Clear();
}



bool RawData::isCartridge()
{
	return m_iscartridge;
}


bool RawData::Open(wxString filename)
{
    wxFile 	f;
    void 	*tempbuffer;
    uint 	bytesread = 0,
			buffer_size = 0;
    wxString ext;
    bool    ret;


    ret = false;


    if (f.Open(filename))
    {
        if (IsLoaded())
            Close();

        m_header_offset = 0;

        buffer_size = (uint)f.Length();
        tempbuffer = m_buffer.GetWriteBuf(buffer_size);
        if (tempbuffer != 0)
            bytesread = (uint)f.Read(tempbuffer, buffer_size);
        f.Close();

        if (bytesread == buffer_size)
        {
            m_buffer.UngetWriteBuf(buffer_size);
            rawdata_filename.Assign(filename);
            ext = filename.AfterLast('.');
            ext = ext.Lower();
            if (ext == "com")
                SetFileType(COM);
            else
                if (ext == "rom")
                {
                    SetFileType(ROM);
				}
                else
                    if (!SetFileType(BIN))
                        SetFileType(UNKNOWN);

            ret = true;
        }
        else
            m_buffer.SetDataLen(0);
    }

    #ifdef IDZ80DEBUG
    if (ret)
        LogIt(wxString::Format("File opened is %s.\n",filename.c_str()));
    else
        LogIt("File not found !\n");
    #endif

    return ret;
}


void RawData::Close()
{
    Clear();
}




void RawData::Clear()
{
    if (m_cartridge != 0)
    {
        delete m_cartridge;
        m_cartridge = 0;
    }

    if (m_binheader != 0)
    {
        delete m_binheader;
        m_binheader = 0;
    }

    memset(m_buffer.GetData(), '\0', m_buffer.GetDataLen());
    m_buffer.SetBufSize(0);

    rawdata_filename.Clear();

    SetFileType(UNKNOWN);
    m_isbasic = false;
}




bool RawData::IsLoaded()
{
    return (m_buffer.GetDataLen() > 0);
}


byte RawData::GetData(FileOffset offset)
{
    byte *buffertemp;
    uint sz;

    sz = (GetBufferSize() - 1);

    if (offset > sz)
        offset = sz;
    buffertemp = (byte *)m_buffer.GetData();
    buffertemp += offset;
    buffertemp += m_header_offset;
    return (*buffertemp);
}

uint RawData::GetBufferSize()
{
    uint sz;
    sz = (uint)m_buffer.GetDataLen();
    sz -= m_header_offset;
    return sz;
}


wxString RawData::GetFileName()
{
    return rawdata_filename.GetFullName();
}

wxString RawData::GetFileNameAndPath()
{
    return rawdata_filename.GetFullPath();
}

wxString RawData::GetFilePath()
{
    return rawdata_filename.GetPath();
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

bool RawData::SetFileType(FileType filetype)
{
    bool    ret = false;

    switch (filetype)
    {
        case COM:
                m_iscartridge = false;
                m_header_offset = 0;
                StartAddress = 0x100;
                EndAddress = 0x100 + GetBufferSize() - 1;
                ExecAddress = 0x100;
                m_filetype = filetype;
                ret = true;
                break;
        case BIN:
                m_iscartridge = false;
                if (ValidateBIN())
                {
                    StartAddress = m_binheader->start_address;
                    EndAddress = m_binheader->end_address;
                    ExecAddress = m_binheader->execution_address;
                    m_header_offset = BIN_HEADER_SIZE;
                    m_filetype = filetype;
                    ret = true;
                }
                else
                {
                    StartAddress = 0;
                    EndAddress = GetBufferSize() - 1;
                    ExecAddress = 0;
                    m_header_offset = 0;
                    m_filetype = UNKNOWN;
                }
                break;
        case ROM:
                m_filetype = filetype;
                ret = true;
				if (ValidateCartridge())
				{
					m_header_offset = CARTRIDGE_HEADER_SIZE;
					StartAddress = 0x4000 + m_header_offset;
					ExecAddress = StartAddress;
					EndAddress = 0x4000 + GetBufferSize() - 1;
					m_iscartridge = true;
				}
				else
				{
					m_header_offset = 0;
					StartAddress = 0;
					EndAddress = GetBufferSize() - 1;
					ExecAddress = 0;
					m_iscartridge = false;
				}
				break;
        default:
        case UNKNOWN:
                    m_iscartridge = false;
					m_header_offset = 0;
					StartAddress = 0;
					EndAddress = GetBufferSize() - 1;
					ExecAddress = 0;
                    m_filetype = UNKNOWN;
					ret = true;
					break;
    }
    return ret;
}



void RawData::SetStrFileType(const wxString &str_type)
{
    str_type.Upper();

    if (str_type == "COM")
        SetFileType(COM);
    else
        if (str_type == "ROM")
            SetFileType(ROM);
        else
            if (str_type == "BIN")
                SetFileType(BIN);
            else
                SetFileType(UNKNOWN);
}


bool RawData::ValidateBIN()
{
    bool	ret = false;
    byte    header_id;

    m_header_offset = 0;
    header_id = GetData(0);

    if (header_id == BIN_ID)
    {
        if (m_binheader == 0)
            m_binheader = new BinHeader;
        m_binheader->start_address = (GetData(1) + 0x100 * GetData(2)) & 0xFFFF;
        m_binheader->end_address = (GetData(3) + 0x100 * GetData(4)) & 0xFFFF;
        m_binheader->execution_address = (GetData(5) + 0x100 * GetData(6)) & 0xFFFF;
        if ((m_binheader->end_address > m_binheader->start_address) &&
            (m_binheader->end_address > m_binheader->execution_address) &&
            (m_binheader->execution_address >= m_binheader->start_address))
            ret = true;
        else
        {
            delete m_binheader;
            m_binheader = 0;
            ret = false;
        }
    }
    return ret;
}


bool RawData::ValidateCartridge()
{
	word    header_id, startaddr, endaddr;
	bool	ret = false;

    m_header_offset = 0;
	header_id = (GetData(1) + 0x100 * GetData(0)) & 0xFFFF;


	if ((header_id == ID_CARTRIDGE_ROM) || (header_id == ID_CARTRIDGE_SUBROM))
	{
	    if (m_cartridge == 0)
            m_cartridge = new CartHeader;
		m_cartridge->id = header_id;
		m_cartridge->init = (GetData(2) + 0x100 * GetData(3)) & 0xFFFF;
		m_cartridge->statement = (GetData(4) + 0x100 * GetData(5)) & 0xFFFF;
		m_cartridge->device = (GetData(6) + 0x100 * GetData(7)) & 0xFFFF;
		m_cartridge->text = (GetData(8) + 0x100 * GetData(9)) & 0xFFFF;

		startaddr = 0x4000;
		endaddr = startaddr + GetBufferSize() - 1;

		if (m_cartridge->init != 0)
		{
            if ((m_cartridge->init > startaddr) && (m_cartridge->init < endaddr))
                ret = true;
            else
                ret = false;
		}
		if (m_cartridge->statement != 0)
		{
            if ((m_cartridge->statement > startaddr) && (m_cartridge->statement < endaddr))
                ret = true;
            else
                ret = false;
		}
		if (m_cartridge->device != 0)
		{
            if ((m_cartridge->device > startaddr) && (m_cartridge->device < endaddr))
                ret = true;
            else
                ret = false;
		}
		if (m_cartridge->text != 0)  //We don't want basic programs
		{
            ret = false;
            m_isbasic = true;
		}

		if (!ret)
        {
            delete m_cartridge;
            m_cartridge = 0;
        }
        else
            m_header_offset = CARTRIDGE_HEADER_SIZE;
	}
	else
        if (m_cartridge != 0)
        {
            delete m_cartridge;
            m_cartridge = 0;
        }
	return ret;
}



bool RawData::GetEntries(SortedIntArray &entrylist)
{
    bool    ret = false;

    entrylist.Clear();

    if ((m_iscartridge) && (m_cartridge != 0))
    {
        if (m_cartridge->init != 0)
            entrylist.Add(m_cartridge->init);
        if (m_cartridge->statement != 0)
            entrylist.Add(m_cartridge->statement);
        if (m_cartridge->device != 0)
            entrylist.Add(m_cartridge->device);
    }
    else
        entrylist.Add(ExecAddress);

    //FIXME: It always will be true.
    if (entrylist.GetCount() > 0)
    {
        ret = true;
    }

    return ret;
}



bool RawData::isROM()
{
	return (GetFileType() == ROM);
}

bool RawData::isBIN()
{
	return (GetFileType() == BIN);
}

bool RawData::isCOM()
{
	return (GetFileType() == COM);
}


bool RawData::isBasicCartridge()
{
    return m_isbasic;
}

bool RawData::CheckCartridge()
{
    SetFileType(ROM);
    return m_iscartridge;
}

void RawData::ForceNoCartridge()
{
    StartAddress = 0;
    EndAddress = GetBufferSize();
    ExecAddress = StartAddress;
    m_header_offset = 0;
    m_iscartridge = false;
    if (m_cartridge != 0)
    {
        delete m_cartridge;
        m_cartridge = 0;
    }
}


const CartHeader *RawData::GetCartridgeHeader()
{
    return m_cartridge;
}

const BinHeader *RawData::GetBinHeader()
{
    return m_binheader;
}
