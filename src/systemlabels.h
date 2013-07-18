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

#include "idz80base.h"
#include "logbase.h"

struct st_systemlabel
{
    uint Address;
    wxString Label;
};

typedef struct st_systemlabel SystemLabelItem;

class SystemLabelList: public LogBase
{
	public:
		SystemLabelList(const wxString& section, LogWindow *logparent);
		~SystemLabelList();

		bool Open(const wxString& file);
		void Clear();
		SystemLabelItem *GetItem(const uint item);
		wxString &Find(const uint addr);
		uint GetCount();
		void SetLog(wxTextCtrl *lg);

	private:
		wxArrayPtrVoid	m_data;
		wxString		m_section;
		wxTextFile		*m_file;
		wxTextCtrl		*m_log;

		bool readData();
};




 #endif

