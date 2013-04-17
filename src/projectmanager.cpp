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


const wxString ProjectManager::projectExtension = ".dap";

ProjectManager::ProjectManager(ProcessData *process, CodeView *codeview)
{
    m_saved = false;
    m_named = false;
    m_filename.Clear();
    Process = process;
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
    savefile.AddLine("[HEADER]");
    str = "File = ";
    str << Process->Program->GetFileName();
    savefile.AddLine(str);
    switch (Process->Program->GetFileType())
    {
        case COM:
                    str = "Type = COM";
                    break;
        case BIN:
                    str = "Type = BIN";
                    break;
        case ROM:
                    str = "Type = ROM";
                    break;
        default:
                    str = "Type = UNKNOWN";
                    break;
    }
    savefile.AddLine(str);
    str.Printf("StartAddress = %X", Process->Program->StartAddress);
    savefile.AddLine(str);
    str.Printf("ExecutionAddress = %X", Process->Program->ExecAddress);
    savefile.AddLine(str);
    str.Printf("EndAddress = %X", Process->Program->EndAddress);
    savefile.AddLine(str);
    str = "[\\HEADER]\n";
    savefile.AddLine(str);
}


void ProjectManager::writeProglabel(wxTextFile &savefile)
{
    wxString str;
    LabelItem *label;
    int i,j;
    if (!Process->prog_labels->IsEmpty())
    {
        savefile.AddLine("[PROGLABEL]");
        for (i = 0; i < Process->prog_labels->GetCount(); i++)
        {
            label = Process->prog_labels->GetLabelItem(i);
            if (label != 0)
            {
                str.Printf("%s = %X", label->LabelStr.c_str(), label->Address);
                if (label->LabelUsers != 0)
                    for (j = 0; j < label->LabelUsers->GetCount(); j++)
                        str << wxString::Format(" %d",label->LabelUsers->Item(j));
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
    if (!Process->var_labels->IsEmpty())
    {
        savefile.AddLine("[VARLABEL]");
        for (i = 0; i < Process->var_labels->GetCount(); i++)
        {
            label = Process->var_labels->GetLabelItem(i);
            if (label != 0)
            {
                str.Printf("%s = %X", label->LabelStr.c_str(), label->Address);
                if (label->LabelUsers != 0)
                    for (j = 0; j < label->LabelUsers->GetCount(); j++)
                        str << wxString::Format(" %d",label->LabelUsers->Item(j));
                savefile.AddLine(str);
            }
        }

        str = "[\\VARLABEL]\n";
        savefile.AddLine(str);
    }
}


void ProjectManager::writeIOlabel(wxTextFile &savefile)
{
    wxString str;
    LabelItem *label;
    int i,j;
    if (!Process->io_labels->IsEmpty())
    {
        savefile.AddLine("[IOLABEL]");
        for (i = 0; i < Process->io_labels->GetCount(); i++)
        {
            label = Process->io_labels->GetLabelItem(i);
            if (label !=0)
            {
                str.Printf("%s = %X", label->LabelStr.c_str(), label->Address);
                if (label->LabelUsers != 0)
                    for (j = 0; j < label->LabelUsers->GetCount(); j++)
                        str << wxString::Format(" %d",label->LabelUsers->Item(j));
                savefile.AddLine(str);
            }
        }

        str = "[\\IOLABEL]\n";
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
    if (Process->Disassembled->GetCount() != 0)
    {
        savefile.AddLine("[DASM]");
        for (i = 0; i < Process->Disassembled->GetCount(); i++)
        {
            de = Process->Disassembled->GetData(i);
            str.Printf("%.2X ", de->GetType());
            str << wxString::Format("%.2X ", de->GetLength());
            str << wxString::Format("%.4X ", de->GetOffset());
            for(j = 0; j < de->GetLength(); j++)
                str << wxString::Format("%.2X ", de->GetCodeItem(j));

			if (de->GetNumArgs() > 0)
			{
				str << wxString::Format("%.2X ", de->GetNumArgs());
				str << wxString::Format("%.2X ", de->GetArgSize());
				arg_aux = de->GetNumArgs() * de->GetArgSize();
				for(j = 0; j < arg_aux; j++)
				{
					c = de->GetArgItem(j);
					c = c & 0xFF;
					str << wxString::Format("%.2X ", c);
				}
			}
            str.Trim();
            str.Trim(false);
            savefile.AddLine(str);
        }
        savefile.AddLine("[\\DASM]\n");
    }
}


void ProjectManager::writeCodeLine(wxTextFile &savefile)
{
    int i;
    wxString str;
    CodeViewItem *cvi;
    if (m_codeview->m_CodeViewLine->GetCount() > 0)
    {
        savefile.AddLine("[CVLINE]");
        for (i = 0; i < m_codeview->m_CodeViewLine->GetCount(); i++)
        {
            cvi = m_codeview->m_CodeViewLine->getData(i);
            str.Printf("%d %d %d %d", cvi->Org, cvi->Dasmitem, cvi->LabelProgAddr, cvi->LabelVarAddr);
            if (cvi->Comment != 0)
            {
                str << wxString::Format(" \"%s\"",cvi->Comment->CommentStr.c_str());
            }
            savefile.AddLine(str);
        }
        savefile.AddLine("[\\CVLINE]");
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
        linkLabels();

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
    foundHeader = str.IsSameAs("[HEADER]");
    while ((!openfile.Eof()) && (!foundHeader))
    {
        str = openfile.GetNextLine();
        foundHeader = str.IsSameAs("[HEADER]");
    }

    if ((!openfile.Eof()) && foundHeader)
    {
        str = openfile.GetNextLine();
        while ((!openfile.Eof()) && (str != "[\\HEADER]"))
        {
            i = str.Find('=');
            if ((str.Left(4) == "File") && (i != wxNOT_FOUND))
            {
                i++;
                str_arg = str.Right(str.Len() - i - 1);
                ret = Process->Program->Open(str_arg);
                str_debug << str_arg << _(" ");
            }

            if ((str.Left(4) == "Type") && (i != wxNOT_FOUND))
            {
                i++;
                str_arg = str.Right(str.Len() - i - 1);

                if (str_arg == "COM")
                    Process->Program->SetFileType(COM);
                else
                    if (str_arg == "ROM")
                        Process->Program->SetFileType(ROM);
                    else
                        if (str_arg == "BIN")
                            Process->Program->SetFileType(BIN);
                        else
                            Process->Program->SetFileType(UNKNOWN);
                str_debug << wxString::Format("Type = %d ",Process->Program->GetFileType());

            }
            if ((str.Left(12) == "StartAddress") && (i != wxNOT_FOUND))
            {
                i++;
                str_arg = str.Right(str.Len() - i - 1);
                str_arg.Trim();
                str_arg.Trim(false);

                if (str_arg.ToLong(&conv,16))
                    Process->Program->StartAddress = (uint)conv;
                else
                    Process->Program->StartAddress = 0;
                str_debug << wxString::Format("Start = %X ",Process->Program->StartAddress);
            }

            if ((str.Left(16) == "ExecutionAddress") && (i != wxNOT_FOUND))
            {
                i++;
                str_arg = str.Right(str.Len() - i - 1);
                str_arg.Trim();
                str_arg.Trim(false);

                if (str_arg.ToLong(&conv, 16))
                    Process->Program->ExecAddress = (uint)conv;
                else
                    Process->Program->ExecAddress = 0;
                str_debug << wxString::Format("Exec = %X ", Process->Program->ExecAddress);
            }

            if ((str.Left(10) == "EndAddress") && (i != wxNOT_FOUND))
            {
                i++;
                str_arg = str.Right(str.Len() - i - 1);
                str_arg.Trim();
                str_arg.Trim(false);

                if (str_arg.ToLong(&conv, 16))
                    Process->Program->EndAddress = (uint)conv;
                else
                    Process->Program->EndAddress = 0;

                str_debug << wxString::Format("End = %X\n", Process->Program->EndAddress);
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
    foundHeader = str.IsSameAs("[PROGLABEL]");
    while ((!openfile.Eof()) && (!foundHeader))
    {
        str = openfile.GetNextLine();
        foundHeader = str.IsSameAs("[PROGLABEL]");
    }
    if (foundHeader)
    {
        str = openfile.GetNextLine();
        while ((!openfile.Eof()) && (str != "[\\PROGLABEL]"))
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
				Process->prog_labels->AddLabel(addr, str_name, conv);
				arrstr.RemoveAt(0);
				if (!arrstr.IsEmpty())
				{
					for (i = 0; i < arrstr.GetCount(); i++)
					{
						str = arrstr[i];
						if (str.ToLong(&conv))
							labelusers.Add((uint)conv);
					}
					Process->prog_labels->AddLabel(addr, str_name, labelusers);
				}
				labelusers.Clear();
			}
            str = openfile.GetNextLine();
        }
    }


    str = openfile.GetFirstLine();
    foundHeader = str.IsSameAs("[VARLABEL]");
    while ((!openfile.Eof()) && (!foundHeader))
    {
        str = openfile.GetNextLine();
        foundHeader = str.IsSameAs("[VARLABEL]");
    }
    if (foundHeader)
    {
        str = openfile.GetNextLine();
        while ((!openfile.Eof()) && (str != "[\\VARLABEL]"))
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
				Process->var_labels->AddLabel(addr, str_name, conv);
				arrstr.RemoveAt(0);
				if (!arrstr.IsEmpty())
				{
					for (i = 0; i < arrstr.GetCount(); i++)
					{
						str = arrstr[i];
						if (str.ToLong(&conv))
							labelusers.Add((uint)conv);
					}
					Process->var_labels->AddLabel(addr, str_name, labelusers);
				}
			}
			labelusers.Clear();
            str = openfile.GetNextLine();
        }
    }

    str = openfile.GetFirstLine();
    foundHeader = str.IsSameAs("[IOLABEL]");
    while ((!openfile.Eof()) && (!foundHeader))
    {
        str = openfile.GetNextLine();
        foundHeader = str.IsSameAs("[IOLABEL]");
    }
    if (foundHeader)
    {
        str = openfile.GetNextLine();
        while ((!openfile.Eof()) && (str != "[\\IOLABEL]"))
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
				Process->io_labels->AddLabel(addr, str_name, conv);
				arrstr.RemoveAt(0);
				if (!arrstr.IsEmpty())
				{
					for (i = 0; i < arrstr.GetCount(); i++)
					{
						str = arrstr[i];
						if (str.ToLong(&conv))
							labelusers.Add((uint)conv);
					}
					Process->io_labels->AddLabel(addr, str_name, labelusers);
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
    int			i,
				j,
				count;
    long		conv;
    bool		foundHeader,
				mnfailed;
    wxArrayString		arr_str;
    enum ElementType	dtype;
    uint		len,
				offs,
				arg_num,
				arg_size,
				arg_aux;
    ByteCode		bc;
    OpCodeArguments	args;
    DAsmElement		*de;
    MnemonicItem	*mi;

    count = 0;

    str = openfile.GetFirstLine();
    foundHeader = str.IsSameAs("[DASM]");
    while ((!openfile.Eof()) && (!foundHeader))
    {
        str = openfile.GetNextLine();
        foundHeader = str.IsSameAs("[DASM]");
    }

    if (foundHeader)
    {
        str = openfile.GetNextLine();

        mnfailed = false;

        while ((!openfile.Eof()) && (str != "[\\DASM]"))
        {
            ParseString(str, arr_str);
            de = new DAsmElement(Process->Program);
            j = 0;

            if (arr_str.GetCount() > 3)
            {
                str = arr_str[j++];
                str.ToLong(&conv, 16);
                dtype = (enum ElementType)conv;
                de->SetType(dtype);

                str = arr_str[j++];
                str.ToLong(&conv, 16);
                len = (uint)conv;
                de->SetLength(len);

                str = arr_str[j++];
                str.ToLong(&conv, 16);
                offs = (uint)conv;
                de->SetOffset(offs);

                memset(&bc, 0, MAX_OPCODE_SIZE);
                for (i = 0; i < len; i++)
                {
                    str = arr_str[(i + j)];
                    str.ToLong(&conv, 16);
                    if (i < sizeof(ByteCode))
                        bc[i] = (unsigned char)conv;
                }
                de->CopyOpCode(bc);
                //memcpy(de->Code, bc, MAX_OPCODE_SIZE);

                mi = Process->Mnemonics->FindItem(bc);
                if (mi == 0)
                    mnfailed = true;
				else
					de->SetMnemonic(mi);

                j += len;
            }

            if ((arr_str.GetCount() > (3 + len)) && !mnfailed)
            {
                str = arr_str[j++];
                str.ToLong(&conv, 16);
                arg_num = (uint)conv;

                str = arr_str[j++];
                str.ToLong(&conv, 16);
                arg_size = (uint)conv;

                arg_aux = arg_size * arg_num;

                for (i = 0; i < arg_aux; i++)
                {
                    str = arr_str[(i + j)];
                    str.ToLong(&conv, 16);
                    if (i < sizeof(OpCodeArguments))
                        args[i] = (unsigned char)conv;
                }
                de->CopyArguments(args, arg_aux);
                //memcpy(de->Args, args, arg_aux);
            }

            if (!mnfailed)
            {
                Process->Disassembled->AddDasm(de);
                count++;
            }

            str = openfile.GetNextLine();
        }
    }
    LogIt(wxString::Format("readDasmData: Dasm Lines %d.\n", count));
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
    foundHeader = str.IsSameAs("[CVLINE]");
    while ((!openfile.Eof()) && (!foundHeader))
    {
        str = openfile.GetNextLine();
        foundHeader = str.IsSameAs("[CVLINE]");
    }

    if (foundHeader)
    {
        lineOK = false;
        str = openfile.GetNextLine();
        while ((!openfile.Eof()) && (str != "[\\CVLINE]"))
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
    LogIt(wxString::Format("readCodeLine: Code lines %d.\n", count));
}



void ProjectManager::linkLabels()
{
    //CodeViewItem *cvi;
    DAsmElement *de;
    int i, x, f;
    uint item;
    wxArrayInt *users;

    i = 0;
    item = 0;
    f = Process->prog_labels->GetCount();
    if ((f > 0) && (!Process->CodeViewLines->IsEmpty()))
        for(i = 0; i < f; i++)
        {
            users = Process->prog_labels->GetLabelUsers(i);
            if (users > 0)
                for(x = 0; x < users->GetCount(); x++)
                {
                    item = users->Item(x);
                    de = Process->Disassembled->GetData(item);
                    if (de != 0)
                        de->SetArgLabel();
                }
        }

    // *** var labels
    i = 0;
    item = 0;
    f = Process->var_labels->GetCount();
    if ((f > 0) && (!Process->CodeViewLines->IsEmpty()))
        for(i = 0; i < f; i++)
        {
            users = Process->var_labels->GetLabelUsers(i);
            if (users > 0)
                for(x = 0; x < users->GetCount(); x++)
                {
                    item = users->Item(x);
                    de = Process->Disassembled->GetData(item);
                    if (de != 0)
                        de->SetArgLabel();
                }
        }

    // *** io labels
    i = 0;
    item = 0;
    f = Process->io_labels->GetCount();
    if ((f > 0) && (!Process->CodeViewLines->IsEmpty()))
        for(i = 0; i < f; i++)
        {
            users = Process->io_labels->GetLabelUsers(i);
            if (users > 0)
                for(x = 0; x < users->GetCount(); x++)
                {
                    item = users->Item(x);
                    de = Process->Disassembled->GetData(item);
                    if (de != 0)
                        de->SetArgLabel();
                }
        }
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
