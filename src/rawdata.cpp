/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Store a program to be disassembled
 **************************************************************/


#include "rawdata.hpp"


// RawData implementation

RawData::RawData(DebugLogBase *logparent)
{
    Clear();
    SetFileType(UNKNOWN);

    ModuleName = "RawData";
    SetTextLog(logparent->GetTextLog());
}



RawData::~RawData(void)
{
    Clear();
}



bool RawData::LoadToBuffer(const wxString filename)
{
    wxFile 	program_file;
    void    *tempbuffer;
    uint    buffer_size;
    int     bytesread;
    bool    success = false;

    tempbuffer = 0;

    if (program_file.Open(filename))
    {
        buffer_size = static_cast<uint>(program_file.Length());
        tempbuffer = m_memory_buffer.GetWriteBuf(buffer_size * 2);
        if (tempbuffer)
        {
            bytesread = program_file.Read(tempbuffer, buffer_size);
            if (bytesread == wxInvalidOffset)
            {
                LogIt("Error reading the file.");
                m_memory_buffer.Clear();
                tempbuffer = 0;
            }
            else
            {
                m_memory_buffer.UngetWriteBuf(buffer_size);
                m_filename.Assign(filename, wxPATH_NATIVE);
                success = true;
            }
        }
        else
        {
            LogIt("Couldn't allocate buffer to load the file.");
        }
        program_file.Close();
    }

    return success;
}



void RawData::SetupFileType()
{
    wxString ext;

    ext = m_filename.GetExt();
    ext = ext.Lower();
    if (ext == "com")
        SetFileType(COM);
    else
        if (ext == "rom")
        {
            SetFileType(ROM);
        }
        else
            if (ext == "bin")
            {
                SetFileType(BIN);
            }
            else
                GuessFileType();
}



bool RawData::Open(wxString filename)
{
    bool    success = false;

    Clear();
    if (LoadToBuffer(filename))
    {
        SetupFileType();
        success = true;
    }

    #ifdef IDZ80DEBUG
    if (success)
        LogIt(wxString::Format("File opened is %s.\n",filename.c_str()));
    else
        LogIt("File not found !\n");
    #endif

    return success;
}



void RawData::Close()
{
    Clear();
}



void RawData::Clear()
{
    ClearBin();
    ClearCartridgeInfo();

    m_memory_buffer.Clear();
    m_filename.Clear();

    SetFileType(UNKNOWN);

    m_header_offset = 0;
}



bool RawData::IsLoaded()
{
    return (m_memory_buffer.GetDataLen() > 0);
}



byte RawData::GetData(FileOffset offset)
{
    byte *buffertemp;
    uint sz;

    sz = (GetSize() - 1);

    if (offset > sz)
        offset = sz;
    buffertemp = static_cast<byte *>(m_memory_buffer.GetData());
    buffertemp += offset;
    buffertemp += m_header_offset;
    return (*buffertemp);
}



uint RawData::GetSize()
{
    uint sz;
    sz = static_cast<uint>(m_memory_buffer.GetDataLen());
    sz -= m_header_offset;
    return sz;
}



wxString RawData::GetFileName()
{
    return m_filename.GetFullName();
}



wxString RawData::GetFileNameAndPath()
{
    return m_filename.GetFullPath();
}



wxString RawData::GetFilePath()
{
    return m_filename.GetPath();
}



uint RawData::GetFileSize()
{
    return static_cast<uint>(m_memory_buffer.GetDataLen());
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
    switch (filetype)
    {
        case COM:
                m_header_offset = 0;
                StartAddress = 0x100;
                EndAddress = 0x100 + GetSize() - 1;
                ExecAddress = 0x100;
                m_filetype = filetype;
                break;
        case BIN:
                if (FillAndValidateBIN(m_memory_buffer.GetData()))
                {
                    const BinHeader *binheader = GetBinHeader();
                    StartAddress = binheader->start_address;
                    EndAddress = binheader->end_address;
                    ExecAddress = binheader->execution_address;
                    m_header_offset = BIN_HEADER_SIZE;
                    m_filetype = filetype;
                }
                else
                {
					m_header_offset = 0;
					StartAddress = 0;
					EndAddress = GetSize() - 1;
					ExecAddress = 0;
                    m_filetype = UNKNOWN;
                }
                break;
        case ROM:
                m_filetype = filetype;
				if (ValidateCartridge(m_memory_buffer.GetData()))
				{
					m_header_offset = CARTRIDGE_HEADER_SIZE;
					StartAddress = CARTRIDGE_PAGE_ADDRESS + m_header_offset;
					ExecAddress = StartAddress;
					EndAddress = StartAddress + GetSize() - 1;
				}
				else
				{
					m_header_offset = 0;
					StartAddress = 0;
					EndAddress = GetSize() - 1;
					ExecAddress = 0;
				}
				break;
        default:
        case UNKNOWN:
					m_header_offset = 0;
					StartAddress = 0;
					EndAddress = GetSize() - 1;
					ExecAddress = 0;
                    m_filetype = UNKNOWN;
					break;
    }
}



void RawData::SetNewAddresses(AbsoluteAddress start_addr, AbsoluteAddress exec_addr, AbsoluteAddress end_addr)
{
    AbsoluteAddress difference = end_addr - start_addr;
    if ((difference <= GetSize()) && (start_addr >= exec_addr) && (start_addr < end_addr) && (exec_addr < end_addr))
    {
        EndAddress = end_addr;
        StartAddress = start_addr;
        ExecAddress = exec_addr;
    }
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



void RawData::ForceNoCartridge()
{
    StartAddress = 0;
    EndAddress = GetSize() - 1;
    ExecAddress = StartAddress;
    m_header_offset = 0;
    ClearCartridgeInfo();
}



void RawData::GuessFileType()
{
    if (FillAndValidateBIN(m_memory_buffer.GetData()))
    {
        const BinHeader *binheader = GetBinHeader();
        StartAddress = binheader->start_address;
        EndAddress = binheader->end_address;
        ExecAddress = binheader->execution_address;
        m_header_offset = BIN_HEADER_SIZE;
        m_filetype = BIN;
        return;
    }

    if (ValidateCartridge(m_memory_buffer.GetData()))
    {
        m_header_offset = CARTRIDGE_HEADER_SIZE;
        StartAddress = CARTRIDGE_PAGE_ADDRESS + m_header_offset;
        ExecAddress = StartAddress;
        EndAddress = StartAddress + GetSize() - 1;
        m_filetype = ROM;
        return;
    }

    SetFileType(UNKNOWN);
}
