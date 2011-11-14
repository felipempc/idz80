/***************************************************************
 * Name:      IDZ80App.cpp
 * Purpose:   Code for Application Class
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 **************************************************************/

#include "IDZ80App.h"

//(*AppHeaders
#include "IDZ80.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(IDZ80App);

bool IDZ80App::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	IDZ80* Frame = new IDZ80(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
