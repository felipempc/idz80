/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   05-09-2012 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Shows and controls system labels
 **************************************************************/

 #include "systemlabels.hpp"



SystemLabelList::SystemLabelList(const wxString& t_section, DebugLogBase *t_logparent)
{
	m_section = t_section;
	m_file = 0;
    ModuleName = "SystemLabel";
    SetTextLog(t_logparent->GetTextLog());
}



SystemLabelList::~SystemLabelList()
{
	clear();
	if (m_file != 0)
		delete m_file;
}



/// @brief Delete item objects and clear the list
void SystemLabelList::clear()
{
	for (SystemLabelItem *sli : m_data)
		if (sli) {
			delete sli;
			sli = 0;
		}

	m_data.clear();
}



/// @brief Opens a text file containing System Labels
/// @param t_file Name of text file
/// @return true if succeed
bool SystemLabelList::open(const wxString& t_file)
{
	bool ret = false;

	m_file = new wxTextFile(t_file);
	if (m_file->Exists())
	{
#ifdef IDZ80_SYSLABELS_DEBUG
		if (!m_file->Open())
			LogIt(m_section + "-> Cannot open file !");
		ret = readData();
		if (!ret)
			LogIt(m_section + "-> Error reading data !");
		if (!m_file->Close())
			LogIt(m_section + "-> Error closing file !");
#else
		m_file->Open();
		ret = readData();
		m_file->Close();
#endif
	}
	else
	{
#ifdef IDZ80_SYSLABELS_DEBUG
		LogIt(m_section + "-> File not found !");
#endif

		delete m_file;
		m_file = 0;
	}
	return ret;
}



/// @brief Reads the text file containing system labels
/// @return 
bool SystemLabelList::readData()
{
    wxString		str;
    wxArrayString	arrstr;
    long			conv;
    bool			foundHeader;
    wxString		begin_section, end_section;
    SystemLabelItem	*sli;
    unsigned int	line;


    begin_section << "[" << m_section << "]";
    end_section << "[/" << m_section << "]";

    str = m_file->GetFirstLine();
    line = 1;
    foundHeader = str.IsSameAs(begin_section);
    while ((!m_file->Eof()) && (!foundHeader))
    {
        str = m_file->GetNextLine();
        foundHeader = str.IsSameAs(begin_section);
        ++line;
    }
    if (foundHeader)
    {
        str = m_file->GetNextLine();
        while ((!m_file->Eof()) && (str != end_section))
        {
			++line;
            str = str.BeforeFirst('#');
            str = str.Trim();

            ParseString(str, arrstr);
            if (arrstr.GetCount() == 2)
			{
				str = arrstr[0];
				if (str.ToLong(&conv, 16))
				{
					sli = new SystemLabelItem;
					sli->address = static_cast<AbsoluteAddress>(conv);
					sli->label = arrstr[1];
					m_data.push_back(sli);
				}
#ifdef IDZ80_SYSLABELS_DEBUG
				else
					LogIt(wxString::Format("%s [%d] -> Cant convert number !", m_section, line));
#endif
			}
#ifdef IDZ80_SYSLABELS_DEBUG
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
#ifdef IDZ80_SYSLABELS_DEBUG
    if (!foundHeader)
		LogIt(m_section + "-> Header not found !");
#endif
    return (m_data.size() > 0);
}



/// @brief Get the object SystemLabelItem at position t_index from the list
/// @param t_index 
/// @return 
SystemLabelItem *SystemLabelList::index(const unsigned int t_index)
{
	if (t_index > m_data.size())
		return 0;

	return m_data.at(t_index);
}



/// @brief Find label of t_addr
/// @param t_addr 
/// @return the label of t_addr
wxString &SystemLabelList::find(const AbsoluteAddress t_addr)
{
	static wxString str = "";

	if (!m_data.empty())
		for (SystemLabelItem *item : m_data)
			if(item->address == t_addr) {
				str = item->label;
				break;
			}

	return str;
}



/// @brief Get the number of system labels loaded
/// @return 
uint SystemLabelList::getCount()
{
	return (m_data.size());
}



/// @brief Process the line (t_source_string) of System Label file
/// @param t_source_string 
/// @param t_string_list 
void SystemLabelList::ParseString(wxString& t_source_string, wxArrayString& t_string_list)
{
    int			str_len, str_index;
    wxString	stemp;
    bool		found_string = true;

    t_string_list.Clear();
    str_index = t_source_string.Find('"');
    found_string = (str_index > 0);
    if (found_string)
		stemp = t_source_string.Left(str_index);
	else
		stemp = t_source_string;
    str_len = stemp.Len();
    str_index = stemp.First(' ');
    while (str_len > 0)
    {
        t_string_list.Add(stemp.Left(str_index));
        stemp = stemp.AfterFirst(' ');
        str_len = stemp.Len();
        str_index = stemp.Find(' ');
        if ((str_index < 0) && (str_len > 0))
            str_index = str_len;
    }
    if (found_string)
	{
		stemp = t_source_string.AfterFirst('"');
		stemp = stemp.BeforeLast('"');
		t_string_list.Add(stemp);
	}
}



