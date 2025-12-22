/***************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * APP
 **************************************************************/


#include "idz80_app.hpp"

#include "main_dialog.hpp"

#include <wx/image.h>


IMPLEMENT_APP(IDZ80App);

bool IDZ80App::OnInit()
{
	int i;
	wxArrayString	astr;

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

    return wxsOK;

}


