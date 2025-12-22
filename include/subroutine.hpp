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
        bool Call(uint entryaddress, uint nextaddress);
        uint Return(uint exitaddress);
        void UpdateSize();
        bool IsInside();
        bool HasConditionalReturn();
        bool AlreadyCalled(uint address);
        void SetConditionalReturn();
        void Clear();

        int GetCounter();

        SubRoutineCtrl(DebugLogWindow *logparent);
        ~SubRoutineCtrl();

    private:
    std::stack<uint>    Stack;
    int                 StackPointer;
    SubRoutineList      SubRoutines;
    SubRoutineData      *ActualSubRoutine;
    SortedIntArray      *CalledSubroutines;
    bool                ReturnConditional;

    void ClearSubRoutines();
};


#endif // _SUBROUTINE_H_
