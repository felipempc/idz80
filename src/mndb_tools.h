/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Tools for mndb
 **************************************************************/

#ifndef _IDZ80_MNDBTOOLS_H
#define _IDZ80_MNDBTOOLS_H

#include <wx/string.h>
#include <wx/arrstr.h>


void TrimComment(wxString& str);
int GetSection(wxString& s);
void ParseString(wxString& s, wxArrayString& sl);

#endif
