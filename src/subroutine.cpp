/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   29-10-2012 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Controls subroutines
 **************************************************************/

#include "subroutine.hpp"


SubRoutineCtrl::SubRoutineCtrl(DebugLogWindow *logparent)
{
    StackPointer = 0;
    ActualSubRoutine = 0;
    ReturnConditional = false;
    CalledSubroutines = new SortedIntArray(CompareSortedInt);
    CalledSubroutines->Clear();
    SetTextLog(logparent);
    ModuleName = "SubRoutine";
}

SubRoutineCtrl::~SubRoutineCtrl()
{
    Clear();
    delete CalledSubroutines;
}


bool SubRoutineCtrl::Call(uint entryaddress, uint nextaddress)
{
    SubRoutineData *sri;
    uint ret_cond;
    bool ret = false;

    if (CalledSubroutines->Index(entryaddress) == wxNOT_FOUND)
    {
        ret = true;
        Stack.push(nextaddress);
        if (ReturnConditional)
            ret_cond = 0;
        else
            ret_cond = 1;
        Stack.push(ret_cond);
        ReturnConditional = false;

        sri = new SubRoutineData;
        sri->Start = entryaddress;
        sri->Size = 0;
        sri->End = entryaddress;
        SubRoutines.push_back(sri);

        ActualSubRoutine = sri;
        StackPointer++;
        CalledSubroutines->Add(entryaddress);
    }
    return ret;
}


uint SubRoutineCtrl::Return(uint exitaddress)
{
    uint returnaddress = 0;
    uint ret_cond;

    if (StackPointer > 0)
    {
        ActualSubRoutine->End = exitaddress;
        ret_cond = Stack.top();
        Stack.pop();
        if (ret_cond == 0)
            ReturnConditional = true;
        else
            ReturnConditional = false;

        returnaddress = Stack.top();
        Stack.pop();
        StackPointer--;
    }
    if (StackPointer == 0)
        ActualSubRoutine = 0;
    else
        ActualSubRoutine = SubRoutines[StackPointer - 1];

    return returnaddress;
}


void SubRoutineCtrl::UpdateSize()
{
    if (ActualSubRoutine != 0)
        ActualSubRoutine->Size++;
}


bool SubRoutineCtrl::IsInside()
{
    return (StackPointer > 0);
}

bool SubRoutineCtrl::HasConditionalReturn()
{
    return ReturnConditional;
}


bool SubRoutineCtrl::AlreadyCalled(uint address)
{
    return (CalledSubroutines->Index(address) != wxNOT_FOUND);
}

void SubRoutineCtrl::Clear()
{
    CalledSubroutines->Clear();
    ActualSubRoutine = 0;
    ClearSubRoutines();
    StackPointer = 0;
}




void SubRoutineCtrl::ClearSubRoutines()
{
    SubRoutineData *srd;
    int size;

    size = SubRoutines.size();

    for(int i = 0; i < size; i++)
    {
        srd = SubRoutines.back();
        if (srd != 0)
            delete srd;
        SubRoutines.pop_back();
    }
}



int SubRoutineCtrl::GetCounter()
{
    return StackPointer;
}


void SubRoutineCtrl::SetConditionalReturn()
{
    ReturnConditional = true;
}

