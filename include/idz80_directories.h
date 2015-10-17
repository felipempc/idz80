/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   12-10-2015 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Base class for store directories
 **************************************************************/

#ifndef _IDZ80_DIRECTORIES_
#define _IDZ80_DIRECTORIES_


class IDZ80Directories
{
    public:
        wxString app_root_dir_,
                 app_resource_dir_,
                 fileopen_last_dir_;
};



#endif // _IDZ80_DIRECTORIES_
