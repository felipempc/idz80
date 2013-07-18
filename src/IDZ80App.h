/***************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * APP
 **************************************************************/

#ifndef IDZ80APP_H
#define IDZ80APP_H

#include <wx/app.h>

class IDZ80App : public wxApp
{
    public:
        virtual bool OnInit();

};

#endif // IDZ80APP_H
