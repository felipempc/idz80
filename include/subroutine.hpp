/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   29-10-2012 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Controls subroutines
 **************************************************************/


#ifndef _SUBROUTINE_H_
#define _SUBROUTINE_H_

#include <stack>
#include <vector>
#include "idz80_basic_types.hpp"
#include "debug_logbase.hpp"

struct SubRoutineData
{
    uint    Start,
            Size,
            End;
};

typedef std::vector<SubRoutineData *> SubRoutineList;


class SubRoutineCtrl: public DebugLogBase
{
    public:
        bool call_subroutine(uint t_entryaddress, uint t_nextaddress);
        uint return_subroutine(uint t_exitaddress);
        void updateSize();
        bool isInside();
        bool hasConditionalReturn();
        bool alreadyCalled(uint t_address);
        void setConditionalReturn();
        void clear();

        int getCounter();

        SubRoutineCtrl(DebugLogWindow *logparent);
        ~SubRoutineCtrl();

    private:
    std::stack<uint>    m_stack;                // Stack of a CPU
    int                 m_stack_pointer;
    SubRoutineList      m_subroutines;
    SubRoutineData      *m_actual_subroutine;
    AddressVector       m_called_subroutines; // Originally a SortedIntArray
    bool                m_return_conditional;

    void clearSubRoutines();
    bool searchAddressInCalledSubRoutines(unsigned int t_address); // for CalledSubRoutines
};


#endif // _SUBROUTINE_H_
