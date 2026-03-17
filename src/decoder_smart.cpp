/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   15-05-2012 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * This module disassembles a program in smart mode.
 **************************************************************/

#include "decoder_smart.hpp"

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



bool SmartDecoder::fullDisassemble()
{
    bool    ret = false,
            processing = true,
            update_item = false;

    int     dasmed_index;

    uint    program_size,
            size_sum = 0,        // stores the sum of instructions' size.
            address;

    DisassembledItem    *dasmed_item;
    AddressVector       CartridgeCalls;

    if ((!m_program) || (!m_disassembled_list))
    return;

    m_labels->clearUserLabels();
    m_sub_routine->clear();
    updateBoundaries();

    m_address_list.clear();
    m_address_list_processed.clear();


	if (m_program->isCartridge())
	{
		SetCartridgeLabels();
	    m_program->getEntries(CartridgeCalls);
	    m_next_address = CartridgeCalls.at(0);      // Next address is the first Cartridge entry point
	}
	else
    {
        m_next_address = m_program->ExecAddress;                    // Next address is the execution address
        m_labels->prog_labels->addLabel(m_next_address, "START");
    }
    program_size = m_program->GetSize();
    dasmed_index = program_size;
    while (processing)
    {
        m_last_prg_counter = m_next_address - m_disassembled_list->GetBaseAddress(0);
        m_actual_address = m_next_address;


#ifdef IDZ80_DEBUG_DECODER
        if (removeAddressFrom(m_actual_address, m_address_list))
            LogIt(wxString::Format("[0x%.4X] Removed this address from list !", m_actual_address));
#else
        removeAddressFrom(m_actual_address, m_address_list);
#endif

        if (!findAddressIn(m_next_address, m_address_list_processed)) {
            m_address_list_processed.push_back(m_actual_address);
        }

        dasmed_item = new DisassembledItem(m_program);
        dasmed_index = Decode(dasmed_item, m_last_prg_counter, dasmed_index);       // Get an instruction (or data)
        m_next_address += dasmed_item->GetLength();                                 // Now next_address points to next instruction
        size_sum += dasmed_item->GetLength();
        m_prg_counter = m_last_prg_counter + dasmed_item->GetLength();              // Similar to the CPU Program counter register.

        address = dasmed_item->GetArgumentValue(0, m_disassembled_list->GetBaseAddress(0));

        SetupArgumentLabels(dasmed_item, dasmed_index);

        switch (dasmed_item->GetMnemonic()->GetGroup())
        {
        case GRP_CALL:
            update_item = callSubroutine(dasmed_item);
#ifdef IDZ80_DEBUG_DECODER
            if (update_item)
            {
                LogIt(wxString::Format("[0x%.4X] Entering subroutine No. %d  [0x%.4X].", m_actual_address, m_sub_routine->getCounter(), address));
            }
#endif
            break;

        case GRP_RETURN:
            update_item = returnSubroutine(dasmed_item, m_next_address);
#ifdef IDZ80_DEBUG_DECODER
            if (update_item)
            {
                LogIt(wxString::Format("[0x%.4X] Returning from subroutine No. %d.", m_actual_address, (m_sub_routine->getCounter() + 1)));
            }
#endif
            break;

        case GRP_JUMP:
            update_item = processBranch(dasmed_item, processing);
            break;

        case GRP_LOAD_16BIT:
        case GRP_LOAD_8BIT:
            m_registers.LoadRegister(dasmed_item);
            break;

        case GRP_RST:
            if (MSXWeirdRST(dasmed_item, (dasmed_index + 1)))
            {
                update_item = true;
                m_next_address += 3;
            }
            break;
//        case GRP_ERROR:
//            LogIt(wxString::Format("[0x%.4X] Error: opcode = %s, dasmitem = %d", m_actual_address, dasmed_item->GetCodeStr(), dasmed_index));
//            break;
        }

        if ((!processing) && (CartridgeCalls.size() != 0))
        {
#ifdef IDZ80_DEBUG_DECODER
            LogIt("Process remaining calls...");
            debugShowList("CALL", CartridgeCalls);
#endif

            m_next_address = CartridgeCalls.at(0);
            removeAddressFrom(m_next_address, CartridgeCalls);
            update_item = true;
            processing = true;
        }

        if (update_item)
        {
            //TODO: Revise it
            dasmed_index = m_disassembled_list->FindAddress(m_next_address);
            if (dasmed_index == -1) // will never be -1. Must be revised
                processing = false;
            update_item = false;
        }
        else
            ++dasmed_index;

        if (size_sum >= program_size)
        {
            processing = false;
            LogIt("End of file ! Finishing...");
        }
    } // end while

    fillData();
    return ret;
}



/// @brief Processes jumps. 
/// @param t_dasmed_item The disassembled instruction
/// @param t_processing_status The status of processing loop at fullDisassemble 
/// @return True if dasmed_index (FirstDisassemble) should be updated
bool SmartDecoder::processBranch(DisassembledItem *t_dasmed_item, bool &t_processing_status)
{
    uint    address,
            tempaddress;
    bool    update_dasm_item = false;

    address = t_dasmed_item->GetArgumentValue(0, m_disassembled_list->GetBaseAddress(0));

    if (t_dasmed_item->GetMnemonic()->GetConditionalBranch())
    {
        if (!testIfOutBoundaries(address))
        {
            if (!findAddressIn(address, m_address_list) && (address > m_actual_address))
                m_address_list.push_back(address);
        }
    }
    else
    {
        if (testIfOutBoundaries(address))
        {
            if (m_sub_routine->isInside())                          // We are inside a subroutine?
            {
                if (returnSubroutine(t_dasmed_item, m_next_address))    // Force a return since the deviation send us to out of bounds inside a subroutine
                {
                    update_dasm_item = true;
#ifdef IDZ80_DEBUG_DECODER
                    LogIt(wxString::Format("[0x%.4X] Jump to out of boundary [0x%.4X] inside a sub-routine. Returning to 0x%.4X !", m_actual_address, address, m_next_address));
#endif
                }
            }   // Is inside a subroutine?
            else
            {   // Not inside a subroutine.
#ifdef IDZ80_DEBUG_DECODER
                LogIt(wxString::Format("[0x%.4X] Jump to out of boundary [0x%.4X]. Should finish processing ?", m_actual_address, address));
#endif
                if (address == 0)
                {
                    t_processing_status = false;        // stop processing
#ifdef IDZ80_DEBUG_DECODER
                    LogIt("End processing.");
#endif
                }
            }   // Not inside a subroutine
        }   // Is out of bounds?
        else
        {   // Not out of bounds
#ifdef IDZ80_DEBUG_DECODER
            if ((address >= m_start_address) && (address < m_exec_address))
            {
                LogIt(wxString::Format("[0x%.4X] Address between start and execution [0x%.4X].", m_actual_address, address));
            }
#endif

            if ((address >= m_exec_address) && (address <= m_actual_address))       // Jumping backwards? It may be a loop.
            {
#ifdef IDZ80_DEBUG_DECODER
                LogIt(wxString::Format("[0x%.4X] Loop detected to 0x%.4X.", m_actual_address, address));
                debugShowList("         CONDITIONAL Back Test", m_address_list);
#endif

                if (getNextNearJump(m_address_list, m_next_address, m_end_address, m_next_address))
                {
                    update_dasm_item = true;
                }
                else
                {
                    if ((m_sub_routine->hasConditionalReturn()) && (m_sub_routine->isInside()) &&
                        (returnSubroutine(t_dasmed_item, m_next_address)))
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
                //TODO: Revise it
                if ((m_next_address > m_end_address) && (t_processing_status))
                {
                    #ifdef IDZ80_DEBUG_DECODER
                    LogIt(wxString::Format("Filtered nextddress [0x%.4X].", m_next_address));
                    #endif
                }
            }

            if (address > m_actual_address)         // Jumping forward?
            {
#ifdef IDZ80_DEBUG_DECODER
                LogIt(wxString::Format("[0x%.4X] Jumping forward to 0x%.4X", m_actual_address, address));
                debugShowList("         CONDITIONAL Forward test", m_address_list);
#endif
                if (getNextNearJump(m_address_list, m_next_address, address, tempaddress)) {
                    if (!findAddressIn(address, m_address_list))
                        m_address_list.push_back(address);              // Save it to process later.
#ifdef IDZ80_DEBUG_DECODER
                    LogIt(wxString::Format("[0x%.4X] Save forward address to process later.", m_actual_address));
#endif
                }
                else
                {       // Not a near jump
#ifdef IDZ80_DEBUG_DECODER
                    debugShowList("Processed List", m_address_list_processed);
#endif
                    if (!findAddressIn(address, m_address_list_processed))
                    {
                        tempaddress = address;
                        update_dasm_item = true;
#ifdef IDZ80_DEBUG_DECODER
                        LogIt("Proceed forward.");
#endif
                    }
                    else
                    {   // It was already processed
                        if (m_address_list.empty())
                        {
                            t_processing_status = false;    // Stop processing
#ifdef IDZ80_DEBUG_DECODER
                            LogIt("No more entries. End processing.");
#endif
                        }
                        else
                        {   // Address already processed, list not empty, process next address
                            tempaddress = m_address_list[0];
                            update_dasm_item = true;
#ifdef IDZ80_DEBUG_DECODER
                            LogIt(wxString::Format("Address 0x%.4X already processed. Jumping to 0x%.4X.", address, tempaddress));
#endif
                        }
                    } // was processed?
                }   // Near jump?

                m_next_address = tempaddress;
            }
        }   // Not out of bounds
    }

    return update_dasm_item;
}



/// @brief Simulates a return instruction (get the address of the next instruction). If it's a conditional return, just flags it.
/// @param t_dasmed_item the disassembled item
/// @param t_dest_address Will contain the address of the next instruction if it's not a conditional return.
/// @return True if the "return" instruction has a condition to return
bool SmartDecoder::returnSubroutine(DisassembledItem *t_dasmed_item, AbsoluteAddress &t_dest_address)
{
    bool ret = false;

    if (t_dasmed_item->GetMnemonic()->GetConditionalBranch()) {
        m_sub_routine->setConditionalReturn();                  // Since it's a conditional return, flags it and continue processing
    }
    else {
        t_dest_address = m_sub_routine->return_subroutine(m_actual_address);    // Performs a return instruction
        if (t_dest_address > 0)
            ret = true;
    }

    return ret;
}



/// @brief Performs a call to a subroutine.
/// @param dasmed_item A pointer to a disassembled call instruction.
/// @return True if it's the first time the subroutine was called.
bool SmartDecoder::callSubroutine(DisassembledItem *t_dasmed_item)
{
    bool ret = false;
    uint address = t_dasmed_item->GetArgumentValue(0, m_disassembled_list->GetBaseAddress(0));

    if (testIfOutBoundaries(address)) {
#ifdef IDZ80_DEBUG_DECODER
        LogIt(wxString::Format("[0x%.4X] Subroutine out of boundaries [0x%.4X].", m_actual_address, address));
#endif
        MSXCheckFunctionRegisters(t_dasmed_item);
    }
    else
    {
        if (m_sub_routine->alreadyCalled(address))
        {
#ifdef IDZ80_DEBUG_DECODER
            LogIt(wxString::Format("[0x%.4X] Subroutine already called [0x%.4X].", m_actual_address, address));
#endif
        }
        else
        {
            ret = m_sub_routine->call_subroutine(address, m_next_address);
            if (ret)
                m_next_address = address;
        }
    }
    return ret;
}



/// @brief Search in t_jmplist for a near jump address (between t_start and t_end).
/// @param t_jmplist a list of addresses to jump
/// @param t_start start address
/// @param t_end end address
/// @param t_nextaddr the found address in the list
/// @return true if found a near address to jump
bool SmartDecoder::getNextNearJump(AddressVector &t_jmplist, AbsoluteAddress t_start, AbsoluteAddress t_end, AbsoluteAddress &t_nextaddr)
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
            if (removeAddressFrom(address, t_jmplist))
            {
                #ifdef IDZ80_DEBUG_DECODER
                LogIt(wxString::Format("Removed : 0x%.4X", address));
                #endif // IDZ80_DEBUG_DECODER
            }
            LogIt(wxString::Format("Chosen : 0x%.4X", t_nextaddr));
#else
            removeAddressFrom(address, t_jmplist);
#endif            

            break;
        }
        else
            ++i;
    }
    return ret;
}




/// @brief Fills DisassembledItem of the processed DATA opcode.
void SmartDecoder::fillData()
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
bool SmartDecoder::findAddressIn(const int &t_address, const AddressVector &t_address_list)
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
bool SmartDecoder::removeAddressFrom(const int &t_address, AddressVector &t_address_list)
{
    bool t_ret = false;

    for (AddressVector::iterator it = t_address_list.begin(); it != t_address_list.end();)
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
void SmartDecoder::updateBoundaries()
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
bool SmartDecoder::testIfOutBoundaries(AbsoluteAddress t_addr)
{
    return ((t_addr > m_end_address) || (t_addr < m_start_address));
}



/// @brief Clear local lists.
void SmartDecoder::clear()
{
    m_address_list.clear();
    m_address_list_processed.clear();
    m_sub_routine->clear();
}



/// @brief Show a string containing space separated int WORD hexadecimal format.
/// @param t_listname 
/// @param t_list 
void SmartDecoder::debugShowList(const wxString &t_listname, const AddressVector &t_list)
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

