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
    m_log = 0;
}


DAsmData::~DAsmData()
{
    Clear();
    m_baseAddress.Clear();
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

int DAsmData::AddDasm(DAsmElement *dasmelement)
{
    int     ret = -1;

    if (dasmelement != NULL)
    {
        Data.Add(dasmelement);
        ret = Data.GetCount() - 1;
        totalAllocated += sizeof(DAsmElement);
    }

    return ret;
}

uint DAsmData::GetUsedMem()
{
    return totalAllocated;
}


void DAsmData::DelDasm(DAsmElement *dasmelement)
{
    if (dasmelement != NULL)
	{
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

int DAsmData::InsertDasm(DAsmElement *dasmelement, uint beforeitem)
{
    uint    numitems = Data.GetCount();
    int     ret = -1;

    if (beforeitem >= numitems)
        ret = AddDasm(dasmelement);
    else
    {
        Data.Insert((void *)dasmelement, beforeitem);
        ret = beforeitem;
        totalAllocated += sizeof(DAsmElement);
    }
    return ret;
}

// index = index of dasmdata
uint DAsmData::GetBaseAddress(uint index)
{
	uint	i,
			j,
			count,
			baseaddress = 0;

	count = m_baseAddress.GetCount();
	for(i = 0; i < count; i++)
	{
		j = m_baseAddress[i];
		if (index >= j)
			baseaddress = m_baseAddress[++i];
	}
    return baseaddress;
}


void DAsmData::AddOrgAddress(uint index, uint address)
{
	uint	i,
			j,
			count;
	bool	found = false;

	count = m_baseAddress.GetCount();
	if (count > 0)
	{
		for(i = 0; i < count; i++)
		{
			j = m_baseAddress[i];
			if (index == j)
			{
				m_baseAddress[++i] = address;
				found = true;
				break;
			}
		}

		if (!found)
		{
			m_baseAddress.Add(index);
			m_baseAddress.Add(address);
		}
	}
	else
	{
		m_baseAddress.Add(index);
		m_baseAddress.Add(address);
	}
}


void DAsmData::DelOrgAddress(uint address)
{
	uint	i,
			j,
			count;

	count = m_baseAddress.GetCount();
	for(i = 0; i < count; i++)
	{
		j = m_baseAddress[++i];
		if (j == address)
		{
			m_baseAddress.RemoveAt(--i, 2);
			break;
		}
	}
}



/*
 * Find a item which address >= given address
 * if there is no item, return -1
 * if not found return last item plus 1
 */
int DAsmData::FindAddress(uint address)
{
    DAsmElement *de;
    int     i,
            f,
            findaddress,
            ret = -1;

    f = Data.GetCount();
    for(i = 0; i < f; i++)
    {
        de = GetData(i);
        findaddress = GetBaseAddress(i) + de->Offset;
        if (findaddress >= address)
        {
            ret = i;
            break;
        }
    }
    if (ret == -1)
        ret = f + 1;

    #ifdef IDZ80_DASMED
    LogIt(wxString::Format("Number of dasmed items: %d.  Returning item: %d.   Address to find near: %.4X.  Address found: %.4X\n", f, ret, address, findaddress));
    #endif

    return ret;
}



void DAsmData::SetLog(wxTextCtrl *_lg)
{
    m_log = _lg;
}

void DAsmData::LogIt(wxString logstr)
{
    if (m_log != 0)
        m_log->AppendText("dasmdata.cpp: " + logstr);
}

