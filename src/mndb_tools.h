/****************************************************************
 * Name:      IDZ80
 * Purpose:   Defines Application Frame
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
#include "mndb.h"


void TrimComment(wxString& str);
int GetSection(wxString& s);
void ParseString(wxString& s, wxArrayString& sl);

#endif
