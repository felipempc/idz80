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
    if (var_labels != 0) {
        var_labels->Clear();
        var_labels = 0;
    }
    if (prog_labels != 0) {
        prog_labels->Clear();
        prog_labels = 0;
    }
    if (io_labels != 0) {
        io_labels->Clear();
        io_labels = 0;
    }
    if (constant_labels != 0) {
        constant_labels->Clear();
        constant_labels = 0;
    }
}



void LabelManager::DestroyAll()
{
    ClearUserLabels();

    if(sys_calls) {
        sys_calls->Clear();
        sys_calls = 0;
    }
    if(sys_vars) {
        sys_vars->Clear();
        sys_vars = 0;
    }
    if(sys_io) {
        sys_io->Clear();
        sys_io = 0;
    }
    if(sys_const) {
        sys_const->Clear();
        sys_const = 0;
    }
}



bool LabelManager::LoadSystemLabels(const wxString &filename)
{
    int ret = 0;
    if ((sys_calls != 0)  && (sys_vars != 0) &&
        (sys_io != 0) && (sys_const != 0))
    {
        if(sys_calls->Open(filename))
            ++ret;
        if (sys_vars->Open(filename))
            ++ret;
        if (sys_io->Open(filename))
            ++ret;
        if (sys_const->Open(filename))
            ++ret;
    }
    return (ret > 0);
}



