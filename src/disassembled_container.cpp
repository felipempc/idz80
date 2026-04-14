/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * This module stores / manages disassembled code
 **************************************************************/

#include "disassembled_container.hpp"


DisassembledContainer::DisassembledContainer(RawData *t_rawdata)
{
    m_total_allocated = 0;
    m_file_program = t_rawdata;
    SetTextLog(t_rawdata->GetTextLog());
    ModuleName = "DisassembledContainer";
}



DisassembledContainer::~DisassembledContainer()
{
    clear();
}



void DisassembledContainer::clear()
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




DisassembledItem *DisassembledContainer::getData(DisassembledIndex index)
{
    if (index >= m_disassembled_list.size())
        return 0;
    return m_disassembled_list[index];
}




uint DisassembledContainer::getCount()
{
    return m_disassembled_list.size();
}




bool DisassembledContainer::isLoaded()
{
    return (getCount() > 0);
}




int DisassembledContainer::add(DisassembledItem *d_item)
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




uint DisassembledContainer::getUsedMem()
{
    return m_total_allocated;
}




void DisassembledContainer::del(DisassembledIndex position)
{
	DisassembledList::iterator it_d_item;
	DisassembledItem *d_item;

    if (position < m_disassembled_list.size())
	{
		d_item = m_disassembled_list[position];

		if (d_item)
        {

            LogIt(wxString::Format("Erase item %d, mnemonic = %s", position, d_item->getMnemonic()->GetMnemonicStr(0)));
            delete d_item;
            m_total_allocated -= sizeof(DisassembledItem);
        }

        it_d_item = m_disassembled_list.begin() + position;
        m_disassembled_list.erase(it_d_item);
    }
}





void DisassembledContainer::del(DisassembledIndex index, uint count)
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




int DisassembledContainer::insert(DisassembledItem *d_item, DisassembledIndex beforeitem)
{
    uint    numitems = getCount();
    int     ret = -1;
    DisassembledList::iterator it_d_item;

    if (beforeitem >= numitems)
        ret = add(d_item);
    else
    {
		it_d_item = m_disassembled_list.begin() + beforeitem;
		m_disassembled_list.insert(it_d_item, d_item);
        ret = beforeitem;
        m_total_allocated += sizeof(DisassembledItem);
    }
    return ret;
}




uint DisassembledContainer::getBaseAddress(DisassembledIndex index)
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




void DisassembledContainer::addOrigin(DisassembledIndex index, AbsoluteAddress address)
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




void DisassembledContainer::delOrigin(AbsoluteAddress address)
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



/// @brief Search for an item address >= t_address. Return the DissassembledIndex
/// @param t_address 
/// @return The last item address of the DisassembledList, or the item of the above comparison. \
//TODO: Check this nonsense return. Must be rewritten.
DisassembledIndex DisassembledContainer::findAddress(const AbsoluteAddress t_address)
{
    DisassembledIndex   item_index = 0,
                        ret_index = 0;
    AbsoluteAddress     item_address;

    for (DisassembledList::iterator it = m_disassembled_list.begin(); it != m_disassembled_list.end();)
    {
        item_address = getBaseAddress(item_index) + static_cast<DisassembledItem *>(*it)->getOffsetInFile();
        if (item_address >= t_address) {
            ret_index = item_index;
            break;
        }
        else
            ret_index = item_address;
        ++it;
        ++item_index;
    }

#ifdef IDZ80_DASMED
    LogIt(wxString::Format("Returning item: %d.   Address to find near: %.4X.  Address found: %.4X", ret_index, t_address, item_address));
#endif

    return ret_index;
}



/// @brief Returns the program file associated.
/// @return 
RawData *DisassembledContainer::getProgram() const
{
    return m_file_program;
}
