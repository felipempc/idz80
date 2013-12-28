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

RawData::RawData(LogWindow *logparent)
{
    Clear();
    SetFileType(UNKNOWN);

    ModuleName = "RawData";
    SetTextLog(logparent);
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
        tempbuffer = buffer_.GetWriteBuf(buffer_size * 2);
        if (tempbuffer)
        {
            bytesread = program_file.Read(tempbuffer, buffer_size);
            if (bytesread == wxInvalidOffset)
            {
                LogIt("Error reading the file.");
                buffer_.Clear();
                tempbuffer = 0;
            }
            else
            {
                buffer_.UngetWriteBuf(buffer_size);
                filename_.Assign(filename);
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

    ext = filename_.GetExt();
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
                SetFileType(UNKNOWN);
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

    buffer_.Clear();
    filename_.Clear();

    SetFileType(UNKNOWN);

    header_offset_ = 0;
}




bool RawData::IsLoaded()
{
    return (buffer_.GetDataLen() > 0);
}




byte RawData::GetData(FileOffset offset)
{
    byte *buffertemp;
    uint sz;

    sz = (GetSize() - 1);

    if (offset > sz)
        offset = sz;
    buffertemp = static_cast<byte *>(buffer_.GetData());
    buffertemp += offset;
    buffertemp += header_offset_;
    return (*buffertemp);
}




uint RawData::GetSize()
{
    uint sz;
    sz = static_cast<uint>(buffer_.GetDataLen());
    sz -= header_offset_;
    return sz;
}




wxString RawData::GetFileName()
{
    return filename_.GetFullName();
}




wxString RawData::GetFileNameAndPath()
{
    return filename_.GetFullPath();
}




wxString RawData::GetFilePath()
{
    return filename_.GetPath();
}




uint RawData::GetFileSize()
{
    return static_cast<uint>(buffer_.GetDataLen());
}




FileType RawData::GetFileType()
{
    return filetype_;
}




wxString RawData::GetFileTypeStr()
{
    wxString str;
    switch (filetype_)
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
                header_offset_ = 0;
                StartAddress = 0x100;
                EndAddress = 0x100 + GetSize() - 1;
                ExecAddress = 0x100;
                filetype_ = filetype;
                break;
        case BIN:
                if (FillAndValidateBIN(buffer_.GetData()))
                {
                    const BinHeader *binheader = GetBinHeader();
                    StartAddress = binheader->start_address;
                    EndAddress = binheader->end_address;
                    ExecAddress = binheader->execution_address;
                    header_offset_ = BIN_HEADER_SIZE;
                    filetype_ = filetype;
                }
                else
                {
					header_offset_ = 0;
					StartAddress = 0;
					EndAddress = GetSize() - 1;
					ExecAddress = 0;
                    filetype_ = UNKNOWN;
                }
                break;
        case ROM:
                filetype_ = filetype;
				if (ValidateCartridge(buffer_.GetData()))
				{
					header_offset_ = CARTRIDGE_HEADER_SIZE;
					StartAddress = CARTRIDGE_HEADER_ADDRESS + header_offset_;
					ExecAddress = StartAddress;
					EndAddress = StartAddress + GetSize() - 1;
				}
				else
				{
					header_offset_ = 0;
					StartAddress = 0;
					EndAddress = GetSize() - 1;
					ExecAddress = 0;
				}
				break;
        default:
        case UNKNOWN:
					header_offset_ = 0;
					StartAddress = 0;
					EndAddress = GetSize() - 1;
					ExecAddress = 0;
                    filetype_ = UNKNOWN;
					break;
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
    header_offset_ = 0;
    ClearCartridgeInfo();
}


