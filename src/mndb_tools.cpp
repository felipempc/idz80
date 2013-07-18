/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Tools for mndb
 **************************************************************/


/*
 * Tools for mndb
 */

#include "mndb_tools.h"


void TrimComment(wxString& str)
{
    str = str.BeforeFirst('#');
    str = str.Trim();
}



int GetSection(wxString& s)
{
    int i, f;
    long r = -1;
    i = s.First('[');
    f = s.Last(']');
    if (f > i)
    {
        s = s.Mid((i + 1), (f - i - 1));
        if (s.IsNumber())
            s.ToLong(&r);
        else
            r = -1;
    }
    return ((int) r);
}



void ParseString(wxString& s, wxArrayString& sl)
{
    int			f, x;
    wxString	stemp;
    bool		found_string = true;

    sl.Clear();
    x = s.Find('"');
    found_string = (x > 0);
    if (found_string)
		stemp = s.Left(x);
	else
		stemp = s;
    f = stemp.Len();
    x = stemp.First(' ');
    while (f > 0)
    {
        sl.Add(stemp.Left(x));
        stemp = stemp.AfterFirst(' ');
        f = stemp.Len();
        x = stemp.Find(' ');
        if ((x < 0) && (f > 0))
            x = f;
    }
    if (found_string)
	{
		stemp = s.AfterFirst('"');
		stemp = stemp.BeforeLast('"');
		sl.Add(stemp);
	}
}

