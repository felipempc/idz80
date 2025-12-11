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

#include <wx/dynarray.h>
#include <wx/string.h>
#include <wx/textfile.h>
#include <wx/textctrl.h>

#include "debug_logbase.h"
#include "rawdata.h"

struct st_systemlabel
{
    AbsoluteAddress Address;
    wxString Label;
};

typedef struct st_systemlabel SystemLabelItem;

class SystemLabelList: public DebugLogBase
{
	public:
		SystemLabelList(const wxString& section, DebugLogBase *logparent);
		~SystemLabelList();

		bool Open(const wxString& file);
		void Clear();
		SystemLabelItem *GetItem(const uint item);
		wxString &Find(const uint addr);
		uint GetCount();


	private:
		wxArrayPtrVoid	m_data;
		wxString		m_section;
		wxTextFile		*m_file;
		wxTextCtrl		*m_log;

		bool readData();
		void ParseString(wxString& source_string, wxArrayString& string_list);
};

 #endif

