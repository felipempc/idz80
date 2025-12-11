/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * This module stores / manages disassembled code
 **************************************************************/

#include "disassembled_container.h"


DisassembledContainer::DisassembledContainer(DebugLogBase *logparent)
{
    m_total_allocated = 0;
    SetTextLog(logparent->GetTextLog());
    ModuleName = "DisassembledContainer";
}




DisassembledContainer::~DisassembledContainer()
{
    Clear();
}




void DisassembledContainer::Clear()
{
    DisassembledItem *d_item;
    int size;

    size = m_disassembled_list.size();

    for(int i = 0; i < size; i++)
    {
        d_item = m_disassembled_list.back();
        if (d_item)
            delete d_item;
        m_disassembled_list.pop_back();
    }

    m_origin_list.clear();
}




DisassembledItem *DisassembledContainer::GetData(DisassembledIndex index)
{
    if (index >= m_disassembled_list.size())
        return 0;
    return m_disassembled_list[index];
}




uint DisassembledContainer::GetCount()
{
    return m_disassembled_list.size();
}




bool DisassembledContainer::IsLoaded()
{
    return (GetCount() > 0);
}




int DisassembledContainer::Add(DisassembledItem *d_item)
{
    int     ret = -1;

    if (d_item)
    {
		m_disassembled_list.push_back(d_item);
        ret = m_disassembled_list.size() - 1;
        m_total_allocated += sizeof(DisassembledItem);
    }

    return ret;
}




uint DisassembledContainer::GetUsedMem()
{
    return m_total_allocated;
}




void DisassembledContainer::Del(DisassembledIndex position)
{
	DisassembledList::iterator it_d_item;
	DisassembledItem *d_item;

    if (position < m_disassembled_list.size())
	{
		d_item = m_disassembled_list[position];

		if (d_item)
        {

            LogIt(wxString::Format("Erase item %d, mnemonic = %s", position, d_item->GetMnemonic()->GetMnemonicStr(0)));
            delete d_item;
            m_total_allocated -= sizeof(DisassembledItem);
        }

        it_d_item = m_disassembled_list.begin() + position;
        m_disassembled_list.erase(it_d_item);
    }
}





void DisassembledContainer::Del(DisassembledIndex index, uint count)
{
    DisassembledList::iterator	it_begin,
                                it_end;

    if (index < m_disassembled_list.size())
    {
		it_begin = m_disassembled_list.begin() + index;
		it_end = it_begin + count;
		m_disassembled_list.erase(it_begin, it_end);
    }
}




int DisassembledContainer::Insert(DisassembledItem *d_item, DisassembledIndex beforeitem)
{
    uint    numitems = GetCount();
    int     ret = -1;
    DisassembledList::iterator it_d_item;

    if (beforeitem >= numitems)
        ret = Add(d_item);
    else
    {
		it_d_item = m_disassembled_list.begin() + beforeitem;
		m_disassembled_list.insert(it_d_item, d_item);
        ret = beforeitem;
        m_total_allocated += sizeof(DisassembledItem);
    }
    return ret;
}




uint DisassembledContainer::GetBaseAddress(DisassembledIndex index)
{
    AbsoluteAddress baseaddress = 0;
    AddressList::iterator  it_origin;
    OriginData *origin;

    for(it_origin = m_origin_list.begin(); it_origin != m_origin_list.end(); ++it_origin)
	{
	    origin = *it_origin;
		if (index >= origin->index)
			baseaddress = origin->address;
	}
    return baseaddress;
}




void DisassembledContainer::AddOrigin(DisassembledIndex index, AbsoluteAddress address)
{
    OriginData  *origin;
    AddressList::iterator  it_origin;

    origin->index = 0;
    origin->address = 0;

    if(m_origin_list.size() > 0)
    {
        for(it_origin = m_origin_list.begin(); it_origin != m_origin_list.end(); ++it_origin)
        {
            origin = *it_origin;
            if(index == origin->index)
            {
                origin->address = address;
                return;
            }
        }
    }

    origin = new OriginData;
    origin->index = index;
    origin->address = address;
    m_origin_list.push_back(origin);
}




void DisassembledContainer::DelOrigin(AbsoluteAddress address)
{
    AddressList::iterator  it_origin;
    OriginData *origin;

    for(it_origin = m_origin_list.begin(); it_origin != m_origin_list.end(); ++it_origin)
	{
	    origin = *it_origin;
	    if(address == origin->address)
		{
		    m_origin_list.erase(it_origin);
			return;
		}
	}
}




/*
 * Find a item which address >= given address
 * if there is no item, throws exception ?
 * if not found, return last item plus 1 ?
 *
 */
DisassembledIndex DisassembledContainer::FindAddress(AbsoluteAddress address)
{
    DisassembledList::iterator it_disassembled;
    DisassembledItem *d_item;

    DisassembledIndex     index = 0, ret_index = 0;
    AbsoluteAddress  findaddress;
    bool out_of_bound = true;

    for(it_disassembled = m_disassembled_list.begin(); it_disassembled != m_disassembled_list.end(); ++it_disassembled)
    {
        d_item = *it_disassembled;
        findaddress = GetBaseAddress(index) + d_item->GetOffsetInFile();
        if (findaddress >= address)
        {
            ret_index = index;
            out_of_bound = false;
            break;
        }
        index++;
    }

    if(out_of_bound)
    {
        //if out of bound, return last address
        ret_index = findaddress;

        // or, if out of bound, throws exception
        // throw SearchAddressOutOfBounds
    }

    #ifdef IDZ80_DASMED
    LogIt(wxString::Format("Returning item: %d.   Address to find near: %.4X.  Address found: %.4X", ret_index, address, findaddress));
    #endif

    return ret_index;
}

