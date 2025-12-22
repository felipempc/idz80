/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   05-09-2012 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Shows and controls system labels
 **************************************************************/

#ifndef SYSTEMLABELS_H
#define SYSTEMLABELS_H

#include <vector>
#include <wx/string.h>
#include <wx/textfile.h>
#include <wx/textctrl.h>

#include "debug_logbase.hpp"
#include "rawdata.hpp"

// DEFINE DEBUG STRING
#define IDZ80_SYSLABELS_DEBUG



struct st_systemlabel
{
    AbsoluteAddress address;
    wxString label;
};

typedef struct st_systemlabel SystemLabelItem;

class SystemLabelList: public DebugLogBase
{
	public:
		SystemLabelList(const wxString& t_section, DebugLogBase *t_logparent);
		~SystemLabelList();

		bool open(const wxString& t_file);
		void clear();
		SystemLabelItem *index(const unsigned int t_index);
		wxString &find(const AbsoluteAddress t_addr);
		uint getCount();


	private:
		std::vector<SystemLabelItem *> m_data;
		wxString		m_section;
		wxTextFile		*m_file;
		wxTextCtrl		*m_log;

		bool readData();
		void ParseString(wxString& t_source_string, wxArrayString& t_string_list);
};

 #endif

