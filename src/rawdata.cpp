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


RawData::RawData(void)
{
    m_buffer.SetDataLen(0);
    SetFileType(UNKNOWN);
    m_bin_header_offset=0;
}

RawData::~RawData(void)
{
    m_buffer.SetBufSize(0);
}


bool RawData::Open(wxString filename)
{
    wxFile 	f;
    void 	*tempbuffer;
    byte 	*binptr;
    uint 	bytesread = 0,
			buffer_size = 0;
    wxString ext;
    bool isbin, ret;


    ret = false;

    if (f.Open(filename))
    {
        if (IsLoaded())
            Close();

        m_bin_header_offset=0;

        buffer_size=(uint)f.Length();
        tempbuffer=m_buffer.GetWriteBuf(buffer_size);
        if (!(tempbuffer==NULL))
            bytesread=(uint)f.Read(tempbuffer,buffer_size);
        f.Close();
        if (bytesread==buffer_size)
        {
            m_buffer.UngetWriteBuf(buffer_size);
            binptr = (byte *)m_buffer.GetData();
            isbin = ValidateBIN(*binptr);
            m_filename = filename;
            ext=m_filename.AfterLast('.');
            ext=ext.Lower();
            if (ext == _T("com"))
                SetFileType(COM);
            else
                if (ext == _T("rom"))
                    SetFileType(ROM);
                else
                    if (/*(ext == _T("bin")) && */(isbin))
                        SetFileType(BIN);
                    else
                        SetFileType(UNKNOWN);

            ret = true;
        }
        else
            m_buffer.SetDataLen(0);
    }
    if (ret)
        LogIt(wxString::Format(_("rawdata.c: File opened is %s.\n"),filename.c_str()));
    else
        LogIt(_("rawdata.c: File not found !\n"));

    return ret;
}

void RawData::Close()
{
    memset(m_buffer.GetData(),'\0',m_buffer.GetDataLen());
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

    sz = (GetBufferSize()-1);

    if (offset>sz)
        offset=sz;
    buffertemp=(byte *)m_buffer.GetData();
    buffertemp +=offset;
    buffertemp +=m_bin_header_offset;
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
        case COM:   str = _("COM");
                    break;
        case ROM:   str = _("ROM");
                    break;
        case BIN:   str = _("BIN");
                    break;
        default:    str = _("UNKNOWN");
                    break;
    }
    return str;
}

void RawData::SetFileType(FileType filetype)
{
    m_filetype=filetype;
    switch (filetype)
    {
        case COM:
                m_bin_header_offset = 0;
                StartAddress=0x100;
                EndAddress=0x100 + GetBufferSize();
                ExecAddress=0x100;
                break;
        case BIN:
                m_bin_header_offset = 0;
                StartAddress=(GetData(1) + 0x100 * GetData(2)) & 0xFFFF;
                EndAddress=(GetData(3) + 0x100 * GetData(4)) & 0xFFFF;
                ExecAddress=(GetData(5) + 0x100 * GetData(6)) & 0xFFFF;
                m_bin_header_offset = BIN_HEADER_SIZE;
                break;
        default:
        case UNKNOWN:
        case ROM:
                m_bin_header_offset = 0;
                StartAddress=0;
                EndAddress=GetBufferSize();
                ExecAddress=0;
                break;
    }
}


bool RawData::ValidateBIN(unsigned char byte_id)
{
    bool ret=false;
    m_bin_header_offset = 0;
    if (byte_id == BIN_ID)
    {
        SetFileType(BIN);
        if ((EndAddress > StartAddress) && (EndAddress > ExecAddress) &&
            (ExecAddress >= StartAddress))
            ret=true;
    }

    return ret;
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

