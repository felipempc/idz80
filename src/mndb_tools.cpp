/****************************************************************
 * Name:      IDZ80
 * Purpose:   Defines Application Frame
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 **************************************************************/



/*
 * Tools for mndb
 */


#include <wx/dynarray.h>
#include "mndb_tools.h"


void TrimComment(wxString& str)
{
    str = str.BeforeFirst('#');
    str = str.Trim();
}

int GetSection(wxString& s)
{
    int i,f;
    long r=-1;
    i=s.First('[');
    f=s.Last(']');
    if (f>i)
    {
        s=s.Mid((i+1),(f-i-1));
        if (s.IsNumber())
            s.ToLong(&r);
        else
            r=-1;
    }
    return ((int) r);
}


void ParseString(wxString& s, wxArrayString& sl)
{
    int			f,x;
    wxString	stemp;
    bool		have_string = true;

    sl.Clear();
    x = s.Find('"');
    have_string = (x > 0);
    if (have_string)
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
    if (have_string)
	{
		stemp = s.AfterFirst('"');
		stemp = stemp.BeforeLast('"');
		sl.Add(stemp);
	}
}

