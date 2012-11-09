/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2012-09-05
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 * This module holds system's labels
 **************************************************************/



 #include "systemlabels.h"
 #include "mndb_tools.h"

SystemLabelList::SystemLabelList(const wxString& section, LogWindow *logparent)
{
	m_section = section;
	m_file = 0;
    ModuleName = "SystemLabel";
    SetTextLog(logparent);
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
			LogIt(m_section + "-> Cannot open file !\n ");
		ret = readData();
		if (!ret)
			LogIt(m_section + "-> Error reading data !\n ");
		if (!m_file->Close())
			LogIt(m_section + "-> Error closing file !\n ");

	}
	else
	{
		#ifdef IDZ80DEBUG
		LogIt(m_section + "-> File not found !\n ");
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
			TrimComment(str);
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
					LogIt(wxString::Format("%s [%d] -> Cant convert number !\n", m_section, line));
				#endif
			}
			#ifdef IDZ80DEBUG
			else
			{
				str.Clear();
				str << m_section << wxString::Format("[%d] -> Cant find 2 columms ! (columms = %d)\n ", line, arrstr.GetCount());
				LogIt(str);
			}
			#endif
            str = m_file->GetNextLine();
        } //endwhile
    }
    #ifdef IDZ80DEBUG
    if (!foundHeader)
		LogIt(m_section + "-> Header not found !\n ");
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

