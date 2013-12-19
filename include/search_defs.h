/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   28-11-2013 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Search definitions
 **************************************************************/


#ifndef SEARCHDEFINITIONS_H_
#define SEARCHDEFINITIONS_H_


enum
{
    SEARCH_NONE         = 0,
    SEARCH_WRAPARAOUND  = 0x0001,
    SEARCH_LITERALS     = 0x0002,
    SEARCH_VARIABLES    = 0x0004,
    SEARCH_JUMPS_CALLS  = 0x0008,

};


#endif // SEARCHDEFINITIONS_H_
