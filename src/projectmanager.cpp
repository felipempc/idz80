/****************************************************************
 * Name:      IDZ80
 * Purpose:   Defines Application Frame
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 **************************************************************/



#include "projectmanager.h"
#include "mnemonicitem.h"
#include <wx/dynarray.h>
#include "mndb_tools.h"


const wxString ProjectManager::projectExtension = _(".dap");

ProjectManager::ProjectManager(ProcessData *process, CodeView *codeview)
{
    m_saved = false;
    m_named = false;
    m_filename.Clear();
    m_process = process;
    m_codeview = codeview;

    m_log = 0;
}

ProjectManager::~ProjectManager()
{
    //dtor
}



bool ProjectManager::IsSaved()
{
    return m_saved;
}

bool ProjectManager::HasName()
{
    return m_named;
}

void ProjectManager::New()
{
    m_saved = false;
    m_named = false;
    m_filename.Clear();

}


bool ProjectManager::Save(bool overwrite)
{
    bool ret = false;

    if (m_named)
    {
        wxTextFile savefile;

        ret = savefile.Create(m_filename);
        if ((!ret) && overwrite)
        {
            ret = savefile.Open(m_filename);
            if (ret)
                savefile.Clear();
        }

        if (ret)
        {
            writeHeader(savefile);
            writeProglabel(savefile);
            writeVarlabel(savefile);
            writeIOlabel(savefile);
            writeDasmData(savefile);
            writeCodeLine(savefile);
            savefile.Write(wxTextFileType_Dos);
            savefile.Close();
        }
    }
    else
        ret = false;

    return ret;
}

bool ProjectManager::Save(const wxString &filename, bool overwrite)
{
    SetFilename(filename);
    return Save(overwrite);
}

void ProjectManager::writeHeader(wxTextFile &savefile)
{
    wxString str;
    savefile.AddLine(_("[HEADER]"));
    str = _("File = ");
    str << m_process->Program->GetFileName();
    savefile.AddLine(str);
    switch (m_process->Program->GetFileType())
    {
        case COM:
                    str = _("Type = COM");
                    break;
        case BIN:
                    str = _("Type = BIN");
                    break;
        case ROM:
                    str = _("Type = ROM");
                    break;
        default:
                    str = _("Type = UNKNOWN");
                    break;
    }
    savefile.AddLine(str);
    str.Printf(_("StartAddress = %X"),m_process->Program->StartAddress);
    savefile.AddLine(str);
    str.Printf(_("ExecutionAddress = %X"),m_process->Program->ExecAddress);
    savefile.AddLine(str);
    str.Printf(_("EndAddress = %X"),m_process->Program->EndAddress);
    savefile.AddLine(str);
    str = _("[\\HEADER]\n");
    savefile.AddLine(str);
}


void ProjectManager::writeProglabel(wxTextFile &savefile)
{
    wxString str;
    LabelItem *label;
    int i,j;
    if (!m_process->prog_labels->IsEmpty())
    {
        savefile.AddLine(_("[PROGLABEL]"));
        for (i = 0; i < m_process->prog_labels->GetCount(); i++)
        {
            label = m_process->prog_labels->GetLabelItem(i);
            if (label != 0)
            {
                str.Printf(_("%s = %X"),label->LabelStr.c_str(),label->Address);
                for (j = 0; j < label->LabelUsers->GetCount(); j++)
                    str << wxString::Format(_(" %d"),label->LabelUsers->Item(j));
                savefile.AddLine(str);
            }
        }

        str = _("[\\PROGLABEL]\n");
        savefile.AddLine(str);
    }
}



void ProjectManager::writeVarlabel(wxTextFile &savefile)
{
    wxString str;
    LabelItem *label;
    int i,j;
    if (!m_process->var_labels->IsEmpty())
    {
        savefile.AddLine(_("[VARLABEL]"));
        for (i = 0; i < m_process->var_labels->GetCount(); i++)
        {
            label = m_process->var_labels->GetLabelItem(i);
            if (label != 0)
            {
                str.Printf(_("%s = %X"),label->LabelStr.c_str(),label->Address);
                for (j = 0; j < label->LabelUsers->GetCount(); j++)
                    str << wxString::Format(_(" %d"),label->LabelUsers->Item(j));
                savefile.AddLine(str);
            }
        }

        str = _("[\\VARLABEL]\n");
        savefile.AddLine(str);
    }
}


void ProjectManager::writeIOlabel(wxTextFile &savefile)
{
    wxString str;
    LabelItem *label;
    int i,j;
    if (!m_process->io_labels->IsEmpty())
    {
        savefile.AddLine(_("[IOLABEL]"));
        for (i = 0; i < m_process->io_labels->GetCount(); i++)
        {
            label = m_process->io_labels->GetLabelItem(i);
            if (label !=0)
            {
                str.Printf(_("%s = %X"),label->LabelStr.c_str(),label->Address);
                for (j = 0; j < label->LabelUsers->GetCount(); j++)
                    str << wxString::Format(_(" %d"),label->LabelUsers->Item(j));
                savefile.AddLine(str);
            }
        }

        str = _("[\\IOLABEL]\n");
        savefile.AddLine(str);
    }
}

/* Saves dasm structure into string lines
 *  Format: type[1] length[1] offset[2] opcode[1 to 4] << argnum[1] argsize[1]
 *		   argument[1 to 2] >>
 */
void ProjectManager::writeDasmData(wxTextFile &savefile)
{
    int		i, j, c;
    uint	arg_aux;
    wxString	str;
    DAsmElement *de;
    if (m_process->m_Dasm->GetCount() != 0)
    {
        savefile.AddLine(_("[DASM]"));
        for (i = 0; i < m_process->m_Dasm->GetCount(); i++)
        {
            de = m_process->m_Dasm->GetData(i);
            str.Printf(_("%.2X "), de->ElType);
            str << wxString::Format(_("%.2X "), de->Length);
            str << wxString::Format(_("%.4X "), de->Offset);
            for(j = 0; j < de->Length; j++)
                str << wxString::Format(_("%.2X "), de->Code[j]);

            if (de->MnItem != 0)
                if (de->MnItem->getArgNo() > 0)
                {
                    str << wxString::Format(_("%.2X "), de->MnItem->getArgNo());
                    str << wxString::Format(_("%.2X "), de->MnItem->getArgSize());
					arg_aux = de->MnItem->getArgNo() * de->MnItem->getArgSize();
					for(j = 0; j < arg_aux; j++)
					{
						c = de->Args[j] & 0xFF;
						str << wxString::Format(_("%.2X "), c);
					}
                }
            str.Trim();
            str.Trim(false);
            savefile.AddLine(str);
        }
        savefile.AddLine(_("[\\DASM]\n"));
    }
}


void ProjectManager::writeCodeLine(wxTextFile &savefile)
{
    int i;
    wxString str;
    CodeViewItem *cvi;
    if (m_codeview->m_CodeViewLine->GetCount() > 0)
    {
        savefile.AddLine(_("[CVLINE]"));
        for (i = 0; i < m_codeview->m_CodeViewLine->GetCount(); i++)
        {
            cvi = m_codeview->m_CodeViewLine->getData(i);
            str.Printf(_("%d %d %d %d"),cvi->Org, cvi->Dasmitem, cvi->LabelProgAddr, cvi->LabelVarAddr);
            if (cvi->Comment != 0)
            {
                str << wxString::Format(_(" \"%s\""),cvi->Comment->CommentStr.c_str());
            }
            savefile.AddLine(str);
        }
        savefile.AddLine(_("[\\CVLINE]"));
    }
}




bool ProjectManager::Open(const wxString &filename)
{
    wxTextFile openfile;
    bool ret;

    ret = openfile.Open(filename);
    if (ret)
    {
        readHeader(openfile);
        readLabels(openfile);
        readDasmData(openfile);
        readCodeLine(openfile);

        openfile.Close();
    }
    return ret;
}






bool ProjectManager::readHeader(wxTextFile &openfile)
{
    bool ret = false;
    wxString str, str_arg, str_debug;
    int i;
    long conv;
    bool foundHeader;

    str = openfile.GetFirstLine();
    foundHeader = str.IsSameAs(_("[HEADER]"));
    while ((!openfile.Eof()) && (!foundHeader))
    {
        str = openfile.GetNextLine();
        foundHeader = str.IsSameAs(_("[HEADER]"));
    }

    if ((!openfile.Eof()) && foundHeader)
    {
        str = openfile.GetNextLine();
        while ((!openfile.Eof()) && (str != _("[\\HEADER]")))
        {
            i = str.Find('=');
            if ((str.Left(4) == _("File")) && (i != wxNOT_FOUND))
            {
                i++;
                str_arg = str.Right(str.Len() - i - 1);
                ret = m_process->Program->Open(str_arg);
                str_debug << str_arg << _(" ");
            }

            if ((str.Left(4) == _("Type")) && (i != wxNOT_FOUND))
            {
                i++;
                str_arg = str.Right(str.Len() - i - 1);

                if (str_arg == _("COM"))
                    m_process->Program->SetFileType(COM);
                else
                    if (str_arg == _("ROM"))
                        m_process->Program->SetFileType(ROM);
                    else
                        if (str_arg == _("BIN"))
                            m_process->Program->SetFileType(BIN);
                        else
                            m_process->Program->SetFileType(UNKNOWN);
                str_debug << wxString::Format(_("Type = %d "),m_process->Program->GetFileType());

            }
            if ((str.Left(12) == _("StartAddress")) && (i != wxNOT_FOUND))
            {
                i++;
                str_arg = str.Right(str.Len() - i - 1);
                str_arg.Trim();
                str_arg.Trim(false);

                if (str_arg.ToLong(&conv,16))
                    m_process->Program->StartAddress = (uint)conv;
                else
                    m_process->Program->StartAddress = 0;
                str_debug << wxString::Format(_("Start = %X "),m_process->Program->StartAddress);
            }

            if ((str.Left(16) == _("ExecutionAddress")) && (i != wxNOT_FOUND))
            {
                i++;
                str_arg = str.Right(str.Len() - i - 1);
                str_arg.Trim();
                str_arg.Trim(false);

                if (str_arg.ToLong(&conv, 16))
                    m_process->Program->ExecAddress = (uint)conv;
                else
                    m_process->Program->ExecAddress = 0;
                str_debug << wxString::Format(_("Exec = %X "),m_process->Program->ExecAddress);
            }

            if ((str.Left(10) == _("EndAddress")) && (i != wxNOT_FOUND))
            {
                i++;
                str_arg = str.Right(str.Len() - i - 1);
                str_arg.Trim();
                str_arg.Trim(false);

                if (str_arg.ToLong(&conv, 16))
                    m_process->Program->EndAddress = (uint)conv;
                else
                    m_process->Program->EndAddress = 0;

                str_debug << wxString::Format(_("End = %X\n"),m_process->Program->EndAddress);
            }
            str = openfile.GetNextLine();
        }
        LogIt(str_debug);
    }
    return foundHeader;
}




void ProjectManager::readLabels(wxTextFile &openfile)
{
    wxString		str, str_name, str_addr, str_debug;
    wxArrayString	arrstr;
    wxArrayInt		labelusers;
    int				i;
    uint			addr;
    long			conv;
    bool			foundHeader;

    str = openfile.GetFirstLine();
    foundHeader = str.IsSameAs(_("[PROGLABEL]"));
    while ((!openfile.Eof()) && (!foundHeader))
    {
        str = openfile.GetNextLine();
        foundHeader = str.IsSameAs(_("[PROGLABEL]"));
    }
    if (foundHeader)
    {
        str = openfile.GetNextLine();
        while ((!openfile.Eof()) && (str != _("[\\PROGLABEL]")))
        {
            i = str.Find('=');
            str_name = str.Left(i);
            str_name.Trim(true);
            str_addr = str.Right(str.Len() - i - 1);
            str_addr.Trim(false);
            ParseString(str_addr,arrstr);
            if (arrstr.GetCount() > 0)
			{
				str_addr = arrstr[0];
				str_addr.ToLong(&conv,16);
				addr = (uint)conv;
				m_process->prog_labels->AddLabel(addr, str_name, conv);
				arrstr.RemoveAt(0);
				if (!arrstr.IsEmpty())
				{
					for (i = 0; i < arrstr.GetCount(); i++)
					{
						str = arrstr[i];
						if (str.ToLong(&conv))
							labelusers.Add((uint)conv);
					}
					m_process->prog_labels->AddLabel(addr, str_name, labelusers);
				}
				labelusers.Clear();
			}
            str = openfile.GetNextLine();
        }
    }


    str = openfile.GetFirstLine();
    foundHeader = str.IsSameAs(_("[VARLABEL]"));
    while ((!openfile.Eof()) && (!foundHeader))
    {
        str = openfile.GetNextLine();
        foundHeader = str.IsSameAs(_("[VARLABEL]"));
    }
    if (foundHeader)
    {
        str = openfile.GetNextLine();
        while ((!openfile.Eof()) && (str != _("[\\VARLABEL]")))
        {
            i = str.Find('=');
            str_name = str.Left(i);
            str_name.Trim(true);
            str_addr = str.Right(str.Len() - i - 1);
            str_addr.Trim(false);
            ParseString(str_addr,arrstr);
            if (arrstr.GetCount() > 0)
			{
				str_addr = arrstr[0];
				str_addr.ToLong(&conv,16);
				addr = (uint)conv;
				m_process->var_labels->AddLabel(addr, str_name, conv);
				arrstr.RemoveAt(0);
				if (!arrstr.IsEmpty())
				{
					for (i = 0; i < arrstr.GetCount(); i++)
					{
						str = arrstr[i];
						if (str.ToLong(&conv))
							labelusers.Add((uint)conv);
					}
					m_process->var_labels->AddLabel(addr, str_name, labelusers);
				}
			}
			labelusers.Clear();
            str = openfile.GetNextLine();
        }
    }

    str = openfile.GetFirstLine();
    foundHeader = str.IsSameAs(_("[IOLABEL]"));
    while ((!openfile.Eof()) && (!foundHeader))
    {
        str = openfile.GetNextLine();
        foundHeader = str.IsSameAs(_("[IOLABEL]"));
    }
    if (foundHeader)
    {
        str = openfile.GetNextLine();
        while ((!openfile.Eof()) && (str != _("[\\IOLABEL]")))
        {
            i = str.Find('=');
            str_name = str.Left(i);
            str_name.Trim(true);
            str_addr = str.Right(str.Len() - i - 1);
            str_addr.Trim(false);
            ParseString(str_addr,arrstr);
            if (arrstr.GetCount() > 0)
			{
				str_addr = arrstr[0];
				str_addr.ToLong(&conv,16);
				addr = (uint)conv;
				m_process->io_labels->AddLabel(addr, str_name, conv);
				arrstr.RemoveAt(0);
				if (!arrstr.IsEmpty())
				{
					for (i = 0; i < arrstr.GetCount(); i++)
					{
						str = arrstr[i];
						if (str.ToLong(&conv))
							labelusers.Add((uint)conv);
					}
					m_process->io_labels->AddLabel(addr, str_name, labelusers);
				}
			}
			labelusers.Clear();
            str = openfile.GetNextLine();
        }
    }

}


/* Loads dasm structure from string lines
 *  Format: type[1] length[1] offset[2] opcode[1 to 4] << argnum[1] argsize[1]
 *		   argument[1 to 2] >>
 */
void ProjectManager::readDasmData(wxTextFile &openfile)
{
    wxString	str;
    int			i, j, count;
    long		conv;
    bool		foundHeader, mnfailed;
    wxArrayString		arr_str;
    enum ElementType	dtype;
    uint		len, offs, arg_num, arg_size, arg_aux;
    ByteCode	bc;
    OpCodeArguments	args;
    DAsmElement	*de;

    count = 0;

    str = openfile.GetFirstLine();
    foundHeader = str.IsSameAs(_("[DASM]"));
    while ((!openfile.Eof()) && (!foundHeader))
    {
        str = openfile.GetNextLine();
        foundHeader = str.IsSameAs(_("[DASM]"));
    }

    if (foundHeader)
    {
        str = openfile.GetNextLine();

        mnfailed = false;

        while ((!openfile.Eof()) && (str != _("[\\DASM]")))
        {
            ParseString(str, arr_str);
            de = new DAsmElement(m_process->Program, &m_process->Program->StartAddress);
			de->Style.hasArgumentLabel = 0;
			de->Style.hasLabel = 0;
			de->Style.arg1 = ast_hex;
			de->Style.arg2 = ast_hex;
			de->Style.arg1styled = 0;
			de->Style.arg2styled = 0;

            j = 0;

            if (arr_str.GetCount() > 3)
            {
                str = arr_str[j++];
                str.ToLong(&conv,16);
                dtype = (enum ElementType)conv;
                de->ElType = dtype;

                str = arr_str[j++];
                str.ToLong(&conv,16);
                len = (uint)conv;
                de->Length = len;

                str = arr_str[j++];
                str.ToLong(&conv,16);
                offs = (uint)conv;
                de->Offset = offs;

                memset(&bc, 0, MAX_OPCODE_SIZE);
                for (i = 0; i < len; i++)
                {
                    str = arr_str[(i + j)];
                    str.ToLong(&conv,16);
                    if (i < sizeof(ByteCode))
                        bc[i] = (unsigned char)conv;
                }
                memcpy(de->Code, bc, MAX_OPCODE_SIZE);

                de->MnItem = m_process->Mnemonics->FindItem(de->Code);
                if (de->MnItem == 0)
                    mnfailed = true;


                j += len;
            }

            if ((arr_str.GetCount() > (3 + len)) && !mnfailed)
            {
                str = arr_str[j++];
                str.ToLong(&conv,16);
                arg_num = (uint)conv;

                str = arr_str[j++];
                str.ToLong(&conv,16);
                arg_size = (uint)conv;

                arg_aux = arg_size * arg_num;

                for (i = 0; i < arg_aux; i++)
                {
                    str = arr_str[(i + j)];
                    str.ToLong(&conv, 16);
                    if (i < sizeof(OpCodeArguments))
                        args[i] = (unsigned char)conv;
                }
                memcpy(de->Args, args, arg_aux);
            }

            if (!mnfailed)
            {
                m_process->m_Dasm->AddDasm(de);
                count++;
            }

            str = openfile.GetNextLine();
        }
    }
    LogIt(wxString::Format(_("readDasmData: Dasm Lines %d.\n"),count));
}




void ProjectManager::readCodeLine(wxTextFile &openfile)
{
    wxString	str;
    int			i,j,count;
    long		conv;
    bool		foundHeader, lineOK;
    wxArrayString	arr_str;
    int			dsm, la, lb, org, lastitem;

    count = 0;
    j = 0;

    str = openfile.GetFirstLine();
    foundHeader = str.IsSameAs(_("[CVLINE]"));
    while ((!openfile.Eof()) && (!foundHeader))
    {
        str = openfile.GetNextLine();
        foundHeader = str.IsSameAs(_("[CVLINE]"));
    }

    if (foundHeader)
    {
        lineOK = false;
        str = openfile.GetNextLine();
        while ((!openfile.Eof()) && (str != _("[\\CVLINE]")))
        {
            org = dsm = la = lb = -1;
            ParseString(str,arr_str);
            i = arr_str.GetCount();
            if (i > 3)
            {
                j = 0;
                str = arr_str[j++];
                lineOK = str.ToLong(&conv);
                org = (int)conv;

                str = arr_str[j++];
                lineOK = lineOK && str.ToLong(&conv);
                dsm = (int)conv;

                str = arr_str[j++];
                lineOK = lineOK && str.ToLong(&conv);
                la = (int)conv;

                str = arr_str[j++];
                lineOK = lineOK && str.ToLong(&conv);
                lb = (int)conv;

            }

            if (lineOK)
            {
                if (i > 4)
                    str = arr_str[j];
                else
                    str.Clear();

                if (dsm >= 0)
                {
                    lastitem = m_codeview->m_CodeViewLine->AddDasm(dsm, str);
                    count++;
                }
                else
                    if (la >= 0)
                    {
                        lastitem = m_codeview->m_CodeViewLine->AddProgLabel(la,str);
                        count++;
                    }
                    else
						if (lb >= 0)
						{
							lastitem = m_codeview->m_CodeViewLine->AddVarLabel(lb, str);
							count++;
						}
						else
							if (org >= 0)
							{
								lastitem = m_codeview->m_CodeViewLine->AddOrg(org,str);
								count++;
							}
							else
							{
								lastitem = m_codeview->m_CodeViewLine->Add(str);
								count++;
							}
            }
            str = openfile.GetNextLine();
        }
    }
    LogIt(wxString::Format(_("readCodeLine: Code lines %d.\n"),count));
}





void ProjectManager::SetFilename(const wxString &filename)
{
    int i;

    if (!filename.IsEmpty())
    {
        m_named = true;
        i = filename.Find('.');
        if (i > 0)
        {
            m_filename = filename.Left(i);
            m_filename << projectExtension;
        }
        else
            m_filename = filename + projectExtension;
    }
}


wxString ProjectManager::GetFilename()
{
    int i;
    wxString str;

    i = m_filename.Find('.');
    if (i > 0)
        str = m_filename.Left(i);
    else
        str = m_filename;
    return str;
}


void ProjectManager::SetLog(wxTextCtrl *log)
{
    m_log = log;
}


void ProjectManager::LogIt(const wxString &str)
{
    if (m_log != 0)
        m_log->AppendText(str);
}
