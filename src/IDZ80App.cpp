/***************************************************************
 * Name:      IDZ80App.cpp
 * Purpose:   Code for Application Class
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 **************************************************************/

#include "IDZ80App.h"
#include <wx/arrstr.h>
//(*AppHeaders
#include "IDZ80.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(IDZ80App);

bool IDZ80App::OnInit()
{
	int i;
	wxArrayString	astr;

    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	for (i = 0; i < argc; i++)
			astr.Add(argv[i]);
    	IDZ80* Frame = new IDZ80(0, astr);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
