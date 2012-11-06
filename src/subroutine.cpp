/****************************************************************
 * Name:      IDZ80
 * Purpose:   Z80 code disassembler
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2012-10-29
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 * This module controls subroutines
 **************************************************************/

#include "subroutine.h"


SubRoutineCtrl::SubRoutineCtrl()
{
    StackPointer = 0;
    ActualSubRoutine = 0;
    ReturnConditional = false;
    CalledSubroutines = new SortedIntArray(CompareSortedInt);
    CalledSubroutines->Clear();
    m_modulename = "SubRoutine: ";
}

SubRoutineCtrl::~SubRoutineCtrl()
{
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


void SubRoutineCtrl::Clear()
{
    CalledSubroutines->Clear();
    ActualSubRoutine = 0;
    SubRoutines.clear();
    StackPointer = 0;
}


int SubRoutineCtrl::GetCounter()
{
    return StackPointer;
}


void SubRoutineCtrl::SetConditionalReturn()
{
    ReturnConditional = true;
}


/*
uint SubRoutineCtrl::GetAddress()
{
    return ActualSubRoutine->Start;
}

*/
