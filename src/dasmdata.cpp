/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * This module stores / manages disassembled code
 **************************************************************/

#include "dasmdata.h"


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
    DisassembledItem *de;
    int size;

    size = m_DasmList.size();

    for(int i = 0; i < size; i++)
    {
        de = m_DasmList.back();
        if (de != 0)
            delete de;
        m_DasmList.pop_back();
    }
}



DisassembledItem *DAsmData::GetData(uint index)
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


int DAsmData::AddDasm(DisassembledItem *dasmelement)
{
    int     ret = -1;

    if (dasmelement != 0)
    {
		m_DasmList.push_back(dasmelement);
        ret = m_DasmList.size() - 1;
        totalAllocated += sizeof(DisassembledItem);
    }

    return ret;
}

uint DAsmData::GetUsedMem()
{
    return totalAllocated;
}


void DAsmData::DelDasm(DisassembledIndex position)
{
	DasmArray::iterator it;
	DisassembledItem *de;

    if (position < GetCount())
	{
		de = GetData(position);
		LogIt(wxString::Format("Erase item %d, mnemonic = %s", position, de->GetMnemonicStr(0)));
		it = m_DasmList.begin() + position;
		m_DasmList.erase(it);
		totalAllocated -= sizeof(DisassembledItem);
	}
}


void DAsmData::DelDasm(DisassembledIndex index, uint count)
{
    DasmArray::iterator	it_begin,
						it_end;

    if (index < GetCount())
    {
		it_begin = m_DasmList.begin() + index;
		it_end = it_begin + count;
		m_DasmList.erase(it_begin, it_end);
    }
}


int DAsmData::InsertDasm(DisassembledItem *dasmelement, DisassembledIndex beforeitem)
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
        totalAllocated += sizeof(DisassembledItem);
    }
    return ret;
}





// index = index of dasmdata
uint DAsmData::GetBaseAddress(DisassembledIndex index)
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


void DAsmData::AddOrgAddress(DisassembledIndex index, ProgramAddress address)
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


void DAsmData::DelOrgAddress(ProgramAddress address)
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
 * if not found, return last item plus 1
 * TODO: What a hell is it? it will never return -1. Try to think...
 */
int DAsmData::FindAddress(ProgramAddress address)
{
    DisassembledItem *de;
    int     i,
            f,
            ret = -1;
    ProgramAddress  findaddress;

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
    LogIt(wxString::Format("Number of dasmed items: %d.  Returning item: %d.   Address to find near: %.4X.  Address found: %.4X", f, ret, address, findaddress));
    #endif

    return ret;
}

