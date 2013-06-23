/****************************************************************
 * Name:      MNDB_TOOLS.H
 * Purpose:   Helper tools for mnemonic database (MNDB)
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 **************************************************************/


/*
 *
 */


#ifndef _IDZ80_MNDBTOOLS_H
#define _IDZ80_MNDBTOOLS_H

#include <wx/string.h>
#include <wx/arrstr.h>


void TrimComment(wxString& str);
int GetSection(wxString& s);
void ParseString(wxString& s, wxArrayString& sl);

#endif
