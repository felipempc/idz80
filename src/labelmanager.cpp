/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Base class to manage system and user labels
 **************************************************************/


#include "labelmanager.hpp"


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



void LabelManager::clearUserLabels()
{
    if (var_labels != 0) {
        var_labels->clear();
        var_labels = 0;
    }
    if (prog_labels != 0) {
        prog_labels->clear();
        prog_labels = 0;
    }
    if (io_labels != 0) {
        io_labels->clear();
        io_labels = 0;
    }
    if (constant_labels != 0) {
        constant_labels->clear();
        constant_labels = 0;
    }
}



void LabelManager::destroyAll()
{
    clearUserLabels();

    if(sys_calls) {
        sys_calls->clear();
        sys_calls = 0;
    }
    if(sys_vars) {
        sys_vars->clear();
        sys_vars = 0;
    }
    if(sys_io) {
        sys_io->clear();
        sys_io = 0;
    }
    if(sys_const) {
        sys_const->clear();
        sys_const = 0;
    }
}



bool LabelManager::loadSystemLabels(const wxString &filename)
{
    int ret = 0;
    if ((sys_calls != 0)  && (sys_vars != 0) &&
        (sys_io != 0) && (sys_const != 0))
    {
        if(sys_calls->open(filename))
            ++ret;
        if (sys_vars->open(filename))
            ++ret;
        if (sys_io->open(filename))
            ++ret;
        if (sys_const->open(filename))
            ++ret;
    }
    return (ret > 0);
}



