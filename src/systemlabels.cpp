/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   05-09-2012 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Shows and controls system labels
 **************************************************************/

 #include "systemlabels.h"

SystemLabelList::SystemLabelList(const wxString& section, LogBase *logparent)
{
	m_section = section;
	m_file = 0;
    ModuleName = "SystemLabel";
    SetTextLog(logparent->GetTextLog());
}


SystemLabelList::~SystemLabelList()
{
	Clear();
	if (m_file != 0)
		delete m_file;
}


void SystemLabelList::Clear()
{
	uint i;
	SystemLabelItem *sli;

	for(i = 0; i < m_data.GetCount(); i++)
	{
		sli = (SystemLabelItem *)m_data[i];
		if (sli != 0)
		{
			delete sli;
			sli = 0;
		}
	}
	m_data.Clear();
}




bool SystemLabelList::Open(const wxString& file)
{
	bool ret = false;

	m_file = new wxTextFile(file);
	if (m_file->Exists())
	{
/*
		m_file->Open();
		ret = readData();
		m_file->Close();
*/

		if (!m_file->Open())
			LogIt(m_section + "-> Cannot open file !");
		ret = readData();
		if (!ret)
			LogIt(m_section + "-> Error reading data !");
		if (!m_file->Close())
			LogIt(m_section + "-> Error closing file !");

	}
	else
	{
		#ifdef IDZ80DEBUG
		LogIt(m_section + "-> File not found !");
		#endif

		delete m_file;
		m_file = 0;
	}
	return ret;
}



bool SystemLabelList::readData()
{
    wxString		str;
    wxArrayString	arrstr;
    long			conv;
    bool			foundHeader;
    wxString		begin_section, end_section;
    SystemLabelItem	*sli;
    uint			line;


    begin_section << "[" << m_section << "]";
    end_section << "[/" << m_section << "]";

    str = m_file->GetFirstLine();
    line = 1;
    foundHeader = str.IsSameAs(begin_section);
    while ((!m_file->Eof()) && (!foundHeader))
    {
        str = m_file->GetNextLine();
        foundHeader = str.IsSameAs(begin_section);
        line++;
    }
    if (foundHeader)
    {
        str = m_file->GetNextLine();
        while ((!m_file->Eof()) && (str != end_section))
        {
			line++;
            str = str.BeforeFirst('#');
            str = str.Trim();

            ParseString(str, arrstr);
            if (arrstr.GetCount() == 2)
			{
				str = arrstr[0];
				if (str.ToLong(&conv, 16))
				{
					sli = new SystemLabelItem;
					sli->Address = (uint)conv;
					sli->Label = arrstr[1];
					m_data.Add(sli);
				}
				#ifdef IDZ80DEBUG
				else
					LogIt(wxString::Format("%s [%d] -> Cant convert number !", m_section, line));
				#endif
			}
			#ifdef IDZ80DEBUG
			else
			{
				str.Clear();
				str << m_section << wxString::Format("[%d] -> Cant find 2 columms ! (columms = %d)", line, arrstr.GetCount());
				LogIt(str);
			}
			#endif
            str = m_file->GetNextLine();
        } //endwhile
    }
    #ifdef IDZ80DEBUG
    if (!foundHeader)
		LogIt(m_section + "-> Header not found !");
	#endif
    return (m_data.GetCount() > 0);
}



SystemLabelItem *SystemLabelList::GetItem(const uint item)
{
	uint i = m_data.GetCount();

	if (i > 0)
	{
		if (item < i)
			return ((SystemLabelItem *)m_data[item]);
	}
	return 0;
}



wxString &SystemLabelList::Find(const uint addr)
{
	int		i, f;
	bool	endloop = false;
	static wxString	str;

	SystemLabelItem *sli;

	i = 0;
	f = m_data.GetCount();
	sli = 0;
	str = "";

	if (f == 0)
		return str;

	while (!endloop)
	{
		sli = GetItem(i);
		if ((sli != 0) && (sli->Address == addr))
		{
			str = sli->Label;
			endloop = true;
		}
		i++;
		if (i >= f)
			endloop = true;
	}

	return str;
}


uint SystemLabelList::GetCount()
{
	return (m_data.GetCount());
}




void SystemLabelList::ParseString(wxString& source_string, wxArrayString& string_list)
{
    int			str_len, str_index;
    wxString	stemp;
    bool		found_string = true;

    string_list.Clear();
    str_index = source_string.Find('"');
    found_string = (str_index > 0);
    if (found_string)
		stemp = source_string.Left(str_index);
	else
		stemp = source_string;
    str_len = stemp.Len();
    str_index = stemp.First(' ');
    while (str_len > 0)
    {
        string_list.Add(stemp.Left(str_index));
        stemp = stemp.AfterFirst(' ');
        str_len = stemp.Len();
        str_index = stemp.Find(' ');
        if ((str_index < 0) && (str_len > 0))
            str_index = str_len;
    }
    if (found_string)
	{
		stemp = source_string.AfterFirst('"');
		stemp = stemp.BeforeLast('"');
		string_list.Add(stemp);
	}
}



