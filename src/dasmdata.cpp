/****************************************************************
 * Name:      IDZ80
 * Purpose:   Z80 code disassembler
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 * This module stores / manages disassembled code
 **************************************************************/


/*
 * Unassemble and keep unassembled data
 *
 */


#include "dasmdata.h"
#include "d_asm_element.h"




/*
 * DAsmData implementation
 */

DAsmData::DAsmData()
{
    totalAllocated = 0;
    BaseAddress = 0;
}


DAsmData::~DAsmData()
{
    Clear();
}

void DAsmData::Clear()
{
    int 		i,f;
    DAsmElement *dae;

    totalAllocated = 0;
    f = Data.GetCount();
    if (f > 0)
    {
        for (i = 0; i < f; i++)
        {
            dae = (DAsmElement *)Data[i];
            delete dae;
        }
        Data.Clear();
    }
}

DAsmElement *DAsmData::GetData(uint index)
{
    if (index >= Data.GetCount())
        return (DAsmElement *)0;
    return (DAsmElement *)Data[index];
}

uint DAsmData::GetCount()
{
    return Data.GetCount();
}

bool DAsmData::IsLoaded()
{
    return (GetCount() > 0);
}

void DAsmData::AddDasm(DAsmElement *dasmelement)
{
    if (dasmelement != NULL)
    {
        Data.Add(dasmelement);
        totalAllocated += sizeof(DAsmElement);
    }
}

uint DAsmData::GetUsedMem()
{
    return totalAllocated;
}


void DAsmData::DelDasm(DAsmElement *dasmelement)
{
    if (dasmelement != NULL)
	{	//delete dasmelement;
        Data.Remove((void *)dasmelement);
		totalAllocated -= sizeof(DAsmElement);
	}
}


void DAsmData::DelDasm(uint index, uint count)
{
    uint		i,f;
    DAsmElement *dtemp;

    f = index + count;
    if (index < GetCount())
    {
        if (f >= GetCount())
            f = GetCount() - 1;
        for (i = index; i < f; i++)
        {
            dtemp = GetData(i);
            if (dtemp != NULL)
            {
                delete dtemp;
                totalAllocated -= sizeof(DAsmElement);
            }
        }
        Data.RemoveAt(index, count);
    }
}

void DAsmData::InsertDasm(DAsmElement *dasmelement, uint beforeitem)
{
    uint index = Data.GetCount() - 1;

    if (beforeitem > index)
        AddDasm(dasmelement);
    else
    {
        Data.Insert((void *)dasmelement, beforeitem);
        totalAllocated += sizeof(DAsmElement);
    }
}


uint DAsmData::GetBaseAddress()
{
    return BaseAddress;
}


