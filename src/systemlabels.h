/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2012-09-05
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 * This module holds system's labels
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

