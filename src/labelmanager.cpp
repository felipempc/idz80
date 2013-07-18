/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Base class to manage system and user labels
 **************************************************************/


#include "labelmanager.h"


LabelManager::LabelManager()
{
    var_labels = 0;
    prog_labels = 0;
    io_labels = 0;
    constant_labels = 0;

    sys_calls = 0;
    sys_vars = 0;
    sys_io = 0;
    sys_const = 0;
}



void LabelManager::ClearUserLabels()
{
    if (var_labels != 0)
        var_labels->Clear();
    if (prog_labels != 0)
        prog_labels->Clear();
    if (io_labels != 0)
        io_labels->Clear();
    if (constant_labels != 0)
        constant_labels->Clear();
}



bool LabelManager::LoadSystemLabels(wxString filename)
{
    bool ret = false;
    if ((sys_calls != 0)  && (sys_vars != 0) &&
        (sys_io != 0) && (sys_const != 0))
    {
        ret = sys_calls->Open(filename);
        if (ret)
            ret = sys_vars->Open(filename);
        if (ret)
            ret = sys_io->Open(filename);
        if (ret)
            ret = sys_const->Open(filename);
    }
    return ret;
}



