/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   15-05-2012 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * This module disassembles a program in smart mode.
 **************************************************************/

#include "decoder_smart.h"

 // REWRITE: Work in progress

SmartDecoder::SmartDecoder(ProjectBase *t_parent)
                : Decoder(t_parent)
{
    m_start_address = 0;
    m_end_address = 0;
    m_exec_address = 0;

    m_sub_routine = new SubRoutineCtrl(t_parent->GetTextLog());
    ModuleName = "DECODER SMART";
}



SmartDecoder::~SmartDecoder()
{
    delete m_sub_routine;
}



bool SmartDecoder::FullDisassemble()
{
    bool    ret = false,
            processing = true,
            update_item = false;

    int     dsm_item;

    uint    program_size,
            counter = 0,
            address;

    DisassembledItem    *de;
    IntArray            CartridgeCalls;

    if ((!m_program) || (!m_disassembled_list))
    return;

    m_labels->ClearUserLabels();
    m_sub_routine->Clear();
    UpdateBoundaries();

    m_address_list.clear();
    m_address_list_processed.clear();


	if (m_program->isCartridge())
	{
		SetCartridgeLabels();
	    m_program->GetEntries(CartridgeCalls);
	    m_next_address = CartridgeCalls.at(0);
	}
	else
    {
        m_next_address = m_program->ExecAddress;
        m_labels->prog_labels->addLabel(m_next_address, "START");
    }
    program_size = m_program->GetSize();
    dsm_item = program_size;
    while (processing)
    {
        m_last_prg_counter = m_next_address - m_disassembled_list->GetBaseAddress(0);
        m_actual_address = m_next_address;


#ifdef IDZ80_DEBUG_DECODER
        if (RemoveAddressFrom(m_actual_address, m_address_list))
            LogIt(wxString::Format("[0x%.4X] Removed this address from list !", m_actual_address));
#else
        RemoveAddressFrom(m_actual_address, m_address_list);
#endif

        if (!FindAddressIn(m_next_address, m_address_list_processed))
            m_address_list_processed.push_back(m_actual_address);

        de = new DisassembledItem(m_program);
        dsm_item = Decode(de, m_last_prg_counter, dsm_item);
        m_next_address += de->GetLength();
        counter += de->GetLength();
        m_prg_counter = m_last_prg_counter + de->GetLength();

        address = de->GetArgumentValue(0, m_disassembled_list->GetBaseAddress(0));

        SetupArgumentLabels(de, dsm_item);

        switch (de->GetMnemonic()->GetGroup())
        {
        case GRP_CALL:
            update_item = CallSubroutine(de);
#ifdef IDZ80_DEBUG_DECODER
            if (update_item)
            {
                LogIt(wxString::Format("[0x%.4X] Entering subroutine No. %d  [0x%.4X].", m_actual_address, m_sub_routine->GetCounter(), address));
            }
#endif
            break;

        case GRP_RETURN:
            update_item = ReturnSubroutine(de, m_next_address);
#ifdef IDZ80_DEBUG_DECODER
            if (update_item)
            {
                LogIt(wxString::Format("[0x%.4X] Returning from subroutine No. %d.", m_actual_address, (m_sub_routine->GetCounter() + 1)));
            }
#endif
            break;

        case GRP_JUMP:
            update_item = ProcessBranch(de, processing);
            break;

        case GRP_LOAD_16BIT:
        case GRP_LOAD_8BIT:
            m_registers.LoadRegister(de);
            break;

        case GRP_RST:
            if (MSXWeirdRST(de, (dsm_item + 1)))
            {
                update_item = true;
                m_next_address += 3;
            }
            break;
//        case GRP_ERROR:
//            LogIt(wxString::Format("[0x%.4X] Error: opcode = %s, dasmitem = %d", m_actual_address, de->GetCodeStr(), dsm_item));
//            break;
        }

        if ((!processing) && (CartridgeCalls.size() != 0))
        {
#ifdef IDZ80_DEBUG_DECODER
            LogIt("Process remaining calls...");
            DebugShowList("CALL", CartridgeCalls);
#endif

            m_next_address = CartridgeCalls.at(0);
            RemoveAddressFrom(m_next_address, CartridgeCalls);
            update_item = true;
            processing = true;
        }

        if (update_item)
        {
            dsm_item = m_disassembled_list->FindAddress(m_next_address);
            if (dsm_item == -1) // will never be -1
                processing = false;
            update_item = false;
        }
        else
            dsm_item++;

        if (counter >= program_size)
        {
            processing = false;
            LogIt("End of file ! Finishing...");
        }
    } // end while

    FillData();
    return ret;
}



/*
 *  Process CALLs and JUMPS, return
 * true if dsm_item (FirstDisassemble) should be updated
 */
bool SmartDecoder::ProcessBranch(DisassembledItem *t_de, bool &t_processing_status)
{
    uint    address,
            tempaddress;
    bool    update_dasm_item = false;

    address = t_de->GetArgumentValue(0, m_disassembled_list->GetBaseAddress(0));

    if (t_de->GetMnemonic()->GetConditionalBranch())
    {
        if (!TestIfOutBoundaries(address))
        {
            if (!FindAddressIn(address, m_address_list) && (address > m_actual_address))
                m_address_list.push_back(address);
        }
    }
    else
    {
        if (TestIfOutBoundaries(address))
        {
            if (m_sub_routine->IsInside())
            {
                if (ReturnSubroutine(t_de, m_next_address))
                {
                    update_dasm_item = true;
#ifdef IDZ80_DEBUG_DECODER
                    LogIt(wxString::Format("[0x%.4X] Jump to out of boundary [0x%.4X] inside a sub-routine. Returning to 0x%.4X !", m_actual_address, address, m_next_address));
#endif
                }
            }
            else
            {
#ifdef IDZ80_DEBUG_DECODER
                LogIt(wxString::Format("[0x%.4X] Jump to out of boundary [0x%.4X]. Should finish processing ?", m_actual_address, address));
#endif
                if (address == 0)
                {
                    t_processing_status = false;
#ifdef IDZ80_DEBUG_DECODER
                    LogIt("End processing.");
#endif
                }
            }
        }
        else
        {
#ifdef IDZ80_DEBUG_DECODER
            if ((address >= m_start_address) && (address < m_exec_address))
            {
                LogIt(wxString::Format("[0x%.4X] Address between start and execution [0x%.4X].", m_actual_address, address));
            }
#endif

            if ((address >= m_exec_address) && (address <= m_actual_address))
            {
#ifdef IDZ80_DEBUG_DECODER
                LogIt(wxString::Format("[0x%.4X] Loop detected to 0x%.4X.", m_actual_address, address));
                DebugShowList("         CONDITIONAL Back Test", m_address_list);
#endif

                if (GetNextNearJump(m_address_list, m_next_address, m_end_address, m_next_address))
                {
                    update_dasm_item = true;
                }
                else
                {
                    if ((m_sub_routine->HasConditionalReturn()) && (m_sub_routine->IsInside()) &&
                        (ReturnSubroutine(t_de, m_next_address)))
                    {
                        update_dasm_item = true;
#ifdef IDZ80_DEBUG_DECODER
                        LogIt(wxString::Format("[0x%.4X] Conditional return to 0x%.4X.", m_actual_address, m_next_address));
#endif
                    }
                    else
                    {
                        t_processing_status = false;
#ifdef IDZ80_DEBUG_DECODER
                        LogIt("End processing.");
#endif
                    }
                }

                if ((m_next_address > m_end_address) && (t_processing_status))
                {
                    #ifdef IDZ80_DEBUG_DECODER
                    LogIt(wxString::Format("Filtered nextddress [0x%.4X].", m_next_address));
                    #endif
                }
            }

            if (address > m_actual_address)
            {
#ifdef IDZ80_DEBUG_DECODER
                LogIt(wxString::Format("[0x%.4X] Jumping forward to 0x%.4X", m_actual_address, address));
                DebugShowList("         CONDITIONAL Forward test", m_address_list);
#endif
                if (GetNextNearJump(m_address_list, m_next_address, address, tempaddress))
                {
                    if (!FindAddressIn(address, m_address_list))
                        m_address_list.push_back(address);
#ifdef IDZ80_DEBUG_DECODER
                    LogIt(wxString::Format("[0x%.4X] Save forward address to process later.", m_actual_address));
#endif
                }
                else
                {
#ifdef IDZ80_DEBUG_DECODER
                    DebugShowList("Processed List", m_address_list_processed);
#endif
                    if (!FindAddressIn(address, m_address_list_processed))
                    {
                        tempaddress = address;
                        update_dasm_item = true;
#ifdef IDZ80_DEBUG_DECODER
                        LogIt("Proceed forward.");
#endif
                    }
                    else
                    {
                        if (m_address_list.empty())
                        {
                            t_processing_status = false;
#ifdef IDZ80_DEBUG_DECODER
                            LogIt("No more entries. End processing.");
#endif
                        }
                        else
                        {
                            tempaddress = m_address_list[0];
                            update_dasm_item = true;
#ifdef IDZ80_DEBUG_DECODER
                            LogIt(wxString::Format("Address 0x%.4X already processed. Jumping to 0x%.4X.", address, tempaddress));
#endif
                        }
                    }
                }

                m_next_address = tempaddress;
            }
        }
    }

    return update_dasm_item;
}



/// @brief Verify if RET instruction has a condition to return (REVISE IT)
/// @param t_de the disassembled item
/// @param t_dest_address 
/// @return 
bool SmartDecoder::ReturnSubroutine(DisassembledItem *t_de, AbsoluteAddress &t_dest_address)
{
    bool ret = false;

    if (t_de->GetMnemonic()->GetConditionalBranch())
        m_sub_routine->SetConditionalReturn();
    else
    {
        t_dest_address = m_sub_routine->Return(m_actual_address);
        if (t_dest_address > 0)
            ret = true;
    }

    return ret;
}



bool SmartDecoder::CallSubroutine(DisassembledItem *de)
{
    bool ret = false;
    uint address = de->GetArgumentValue(0, m_disassembled_list->GetBaseAddress(0));

    if (TestIfOutBoundaries(address))
    {
#ifdef IDZ80_DEBUG_DECODER
        LogIt(wxString::Format("[0x%.4X] Subroutine out of boundaries [0x%.4X].", m_actual_address, address));
#endif
        MSXCheckFunctionRegisters(de);
    }
    else
    {
        if (m_sub_routine->AlreadyCalled(address))
        {
#ifdef IDZ80_DEBUG_DECODER
            LogIt(wxString::Format("[0x%.4X] Subroutine already called [0x%.4X].", m_actual_address, address));
#endif
        }
        else
        {
            ret = m_sub_routine->Call(address, m_next_address);
            if (ret)
                m_next_address = address;
        }
    }
    return ret;
}



/// @brief Search in t_jmplist for a near jump address (between t_start and t_end). t_nextaddr will contain the found address if succeed.
/// @param t_jmplist a list of addresses to jump
/// @param t_start start address
/// @param t_end end address
/// @param t_nextaddr the found address in the list
/// @return true if found a near address to jump
bool SmartDecoder::GetNextNearJump(IntArray &t_jmplist, AbsoluteAddress t_start, AbsoluteAddress t_end, AbsoluteAddress &t_nextaddr)
{
    bool    ret = false;
    int     i = 0,
            f;
    AbsoluteAddress  address;

    f = t_jmplist.size();
#ifdef IDZ80_DEBUG_DECODER
    LogIt(wxString::Format("[0x%.4X] Searching for entries between 0x%.4X and 0x%.4X.", m_actual_address, t_start, t_end));
#endif

    while (i < f)
    {
        address = t_jmplist.at(i);
        if ((address >= t_start) && (address < t_end)) {
            ret = true;
            t_nextaddr = address;

#ifdef IDZ80_DEBUG_DECODER
            if (RemoveAddressFrom(address, t_jmplist))
            {
                #ifdef IDZ80_DEBUG_DECODER
                LogIt(wxString::Format("Removed : 0x%.4X", address));
                #endif // IDZ80_DEBUG_DECODER
            }
            LogIt(wxString::Format("Chosen : 0x%.4X", t_nextaddr));
#else
            RemoveAddressFrom(address, t_jmplist);
#endif            

            break;
        }
        else
            ++i;
    }
    return ret;
}




/// @brief Fill DisassembledItem of the processed DATA opcode.
void SmartDecoder::FillData()
{
	uint    i,
			delta,
			count,
			index;
	DisassembledItem    *de_1,
                        *de_2;
	bool    processing;

	processing = (m_disassembled_list->GetCount() > 1);
	i = 0;

	while (processing)
	{
		de_1 = m_disassembled_list->GetData(i);
		de_2 = m_disassembled_list->GetData(i + 1);
		index = (de_1->GetOffsetInFile() + de_1->GetLength());
		delta = de_2->GetOffsetInFile() - index;
		for(count = 0; count < delta; ++count)
		{
			de_1 = new DisassembledItem(m_program);
            de_1->SetupDataItem(index + count);
			m_disassembled_list->Insert(de_1, i + 1);
			++i;
		}
		++i;
		if (i > (m_disassembled_list->GetCount() - 2))
			processing = false;
	}

	i = de_2->GetOffsetInFile() + de_2->GetLength();

	for(count = i; count < m_program->GetSize(); ++count)
	{
		de_1 = new DisassembledItem(m_program);
        de_1->SetupDataItem(count);
		m_disassembled_list->Add(de_1);
	}
}



/// @brief Search for t_address in t_address_list, return true if found it.
/// @param t_address address
/// @param t_address_list address list
/// @return true if found
bool SmartDecoder::FindAddressIn(const int &t_address, const IntArray &t_address_list)
{
    bool t_ret = false;

    for (int address : t_address_list)
        if (t_address == address) {
            t_ret = true;
            break;
        }
    return t_ret;
}



/// @brief Remove t_address from t_address_list, if found.
/// @param t_address 
/// @param t_address_list 
bool SmartDecoder::RemoveAddressFrom(const int &t_address, IntArray &t_address_list)
{
    bool t_ret = false;

    for (IntArray::iterator it = t_address_list.begin(); it != t_address_list.end();)
    {
        if (*it == t_address) {
            it = t_address_list.erase(it);
            t_ret = true;
            break;
        }
        else
            ++it;
    }
    return t_ret;
}



/// @brief Check sanity of the bounds.
/// TODO: Revise it.
void SmartDecoder::UpdateBoundaries()
{
    if ((m_program->EndAddress > m_program->StartAddress)
        && (m_program->ExecAddress >= m_program->StartAddress)
        && (m_program->ExecAddress < m_program->EndAddress))
    {
        m_start_address = m_program->StartAddress;
        m_end_address = m_program->EndAddress;
        m_exec_address = m_program->ExecAddress;
    }
}



/// @brief Check if t_address is out of bounds.
/// @param t_addr 
/// @return true if it's OK.
bool SmartDecoder::TestIfOutBoundaries(AbsoluteAddress t_addr)
{
    return ((t_addr > m_end_address) || (t_addr < m_start_address));
}



/// @brief Clear local lists.
void SmartDecoder::Clear()
{
    m_address_list.clear();
    m_address_list_processed.clear();
    m_sub_routine->Clear();
}



/// @brief Show a string containing space separated int WORD hexadecimal format.
/// @param t_listname 
/// @param t_list 
void SmartDecoder::DebugShowList(const wxString &t_listname, const IntArray &t_list)
{
    uint i;
    wxString str;

    if (t_list.size() > 0)
    {
        str = "<" + t_listname + "> [";
        for (int i : t_list)
            str << wxString::Format("%.4X", i) << " ";

        str.Trim();
        str << "]";
        LogIt(str);
    }
}


/*   IT BECAME REDUNDANT
void SmartDecoder::DebugShowJmpList(const wxString &listname, SortedIntArray *_list)
{
    uint i;
    wxString str;
    if ((_list != 0) && (_list->GetCount() > 0))
    {
        str = "<" + listname + "> [";
        for(i = 0; i < _list->GetCount(); i++)
        {
            str << wxString::Format("%.4X", _list->Item(i));
            if (i != (_list->GetCount() - 1))
                str << " ";
        }
        str << "]";
        LogIt(str);
    }
}
*/


