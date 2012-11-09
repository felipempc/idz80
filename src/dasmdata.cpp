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




/*
 * DAsmData implementation
 */

DAsmData::DAsmData(LogWindow *logparent)
{
    totalAllocated = 0;
    SetTextLog(logparent);
    ModuleName = "DASMDATA";
}


DAsmData::~DAsmData()
{
    Clear();
    m_baseAddress.Clear();
}

void DAsmData::Clear()
{
	m_DasmList.clear();
}



DAsmElement *DAsmData::GetData(uint index)
{
    if (index >= m_DasmList.size())
        return 0;
    return m_DasmList[index];
}


uint DAsmData::GetCount()
{
    return m_DasmList.size();
}


bool DAsmData::IsLoaded()
{
    return (GetCount() > 0);
}


int DAsmData::AddDasm(DAsmElement *dasmelement)
{
    int     ret = -1;

    if (dasmelement != 0)
    {
		m_DasmList.push_back(dasmelement);
        ret = m_DasmList.size() - 1;
        totalAllocated += sizeof(DAsmElement);
    }

    return ret;
}

uint DAsmData::GetUsedMem()
{
    return totalAllocated;
}


void DAsmData::DelDasm(uint position)
{
	DasmArray::iterator it;
	DAsmElement *de;

    if (position < GetCount())
	{
		de = GetData(position);
		LogIt(wxString::Format("Erase item %d, mnemonic = %s\n", position, de->GetMnemonicStr(0)));
		it = m_DasmList.begin() + position;
		m_DasmList.erase(it);
		totalAllocated -= sizeof(DAsmElement);
	}
}


void DAsmData::DelDasm(uint index, uint count)
{
    DAsmElement *de;
    int i;

    DasmArray::iterator	it_begin,
						it_end;

    if (index < GetCount())
    {
		it_begin = m_DasmList.begin() + index;
		it_end = it_begin + count;
		m_DasmList.erase(it_begin, it_end);
    }
}


int DAsmData::InsertDasm(DAsmElement *dasmelement, uint beforeitem)
{
    uint    numitems = GetCount();
    int     ret = -1;
    DasmArray::iterator it;

    if (beforeitem >= numitems)
        ret = AddDasm(dasmelement);
    else
    {
		it = m_DasmList.begin() + beforeitem;
		m_DasmList.insert(it, dasmelement);
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

    f = GetCount();
    for(i = 0; i < f; i++)
    {
        de = GetData(i);
        findaddress = GetBaseAddress(i) + de->GetOffset();
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

