/****************************************************************
 * Name:      IDZ80
 * Purpose:   Disassembler for Z80 MSX
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 ***
 * This module disassemble a program
 **************************************************************/




#include "decoder.h"
#include <wx/string.h>




const int Decoder::OPCODE_NOT_FOUND;

Decoder::Decoder(ProcessBase *parent, LogWindow *logparent)
{
    Process = parent;
    SetTextLog(logparent);
    ModuleName = "DECODER";

    m_startaddress = 0;
    m_endaddress = 0;
    m_execaddress = 0;

    //m_conditionaljumplist = new SortedIntArray(CompareSortedInt);
    //m_unconditionaljumplist = new SortedIntArray(CompareSortedInt);
    AddressList = new SortedIntArray(CompareSortedInt);
    AddressListProcessed = new SortedIntArray(CompareSortedInt);

    SubRoutine = new SubRoutineCtrl(logparent);
}


Decoder::~Decoder()
{
    delete SubRoutine;

//    if (m_conditionaljumplist != 0)
//        delete m_conditionaljumplist;
//    if (m_unconditionaljumplist != 0)
//        delete m_unconditionaljumplist;
    if (AddressList != 0)
        delete AddressList;
    if (AddressListProcessed != 0)
        delete AddressListProcessed;
}



void Decoder::Clear()
{
    //m_conditionaljumplist->Clear();
    //m_unconditionaljumplist->Clear();
    AddressList->Clear();
    AddressListProcessed->Clear();
    SubRoutine->Clear();
}



//TODO: REwrite it
void Decoder::UpdateBoundary()
{
    if ((Process->Program->EndAddress > Process->Program->StartAddress) &&
        (Process->Program->ExecAddress >= Process->Program->StartAddress) &&
        (Process->Program->ExecAddress < Process->Program->EndAddress))
    {
        m_startaddress = Process->Program->StartAddress;
        m_endaddress = Process->Program->EndAddress;
        m_execaddress = Process->Program->ExecAddress;
    }
}




/*
 * Search for an opcode pattern (from Program)
 * in mndb.
 */
uint Decoder::Fetch(const uint prg_index, uint maxitems)
{
    uint 		offset,
                nitems,
                ret = 0;
    wxArrayInt	foundItems;
    byte		scan;
    MnemonicItem    *mnemonic = 0;

    foundItems.Clear();
    offset = 0;
    if (maxitems > MAX_OPCODE_SIZE)
        maxitems = MAX_OPCODE_SIZE;

    while (offset < maxitems)
    {
        scan = Process->Program->GetData(prg_index + offset);
        Process->Mnemonics->FindItems(foundItems, scan, offset);
        offset++;
        if (foundItems.GetCount() < 2)
			break;
    }
    nitems = foundItems.GetCount();
    if (nitems == 1)
    {
        ret = foundItems.Last();
        mnemonic = Process->Mnemonics->GetData(ret);
        if (mnemonic->getBytesNo() > maxitems)
            ret = OPCODE_NOT_FOUND;
    }
    else
        ret = OPCODE_NOT_FOUND;

    return ret;
}



/*
 * Fill the dasmelement with intruction or data information
 * and insert in the list of dasmelements
 * Arguments: de - an already initiated dasmelement
 *            prg_index - of program file to disassemble
 *            position - of item to be inserted in the list
 * Return = index of the item inserted in the list
 */
uint Decoder::Decode(DAsmElement *de, uint prg_index, uint dasm_position)
{
    int     mnc_item,
            ret = -1;
    uint    scan_limit;
    DAsmElement     *next_de;
    ArgStyle		style;


    // ensure it won't overwrite existing dasmitems
    next_de = Process->Disassembled->GetData(dasm_position);
    if (next_de == 0)
        scan_limit = Process->Program->GetBufferSize();
    else
        scan_limit = next_de->GetOffset() - prg_index;

    mnc_item = Fetch(prg_index, scan_limit);

    if (mnc_item == OPCODE_NOT_FOUND)
    {
        de->SetLength(1);
        de->SetOffset(prg_index);
        de->SetMnemonic(0);
        de->SetType(et_Data);
        ret = Process->Disassembled->InsertDasm(de, dasm_position);
    }
    else
    {
        de->SetMnemonic(Process->Mnemonics->GetData(mnc_item));
        de->SetLength();
        de->SetType(et_Instruction);
        de->SetOffset(prg_index);
        if (de->CopyArguments())
			ret = Process->Disassembled->InsertDasm(de, dasm_position);
    }

    return ret;
}



/*
 *  Fill label list
 */
void Decoder::SetupArgumentLabels(DAsmElement *de, uint index)
{
    enum ArgumentTypes	argtype;
    uint            argument;
    wxString		str;

	argtype = de->GetArgumentType(0);
	switch (argtype)
	{
		case ARG_VARIABLE:
                            argument = de->getArgument(0, 0);
							str = Labels->sys_vars->Find(argument);
							if (str.IsEmpty())
								str.Printf("VAR%d", var_label_counter++);
							Labels->var_labels->AddLabel(argument, str, index);
							de->SetArgLabel();
							break;
		case ARG_ABS_ADDR:
		case ARG_REL_ADDR:
                            argument = de->getArgument(0, Process->Disassembled->GetBaseAddress(index));
							str = Labels->sys_calls->Find(argument);
							if (str.IsEmpty())
								str.Printf("LABEL%d", program_label_counter++);
							Labels->prog_labels->AddLabel(argument, str, index);
							de->SetArgLabel();
							break;
		case ARG_IO_ADDR:
                            argument = de->getArgument(0, 0);
							str = Labels->sys_io->Find(argument);
							if (str.IsEmpty())
								str.Printf("PORT%d", io_label_counter++);
							Labels->io_labels->AddLabel(argument, str, index);
							de->SetArgLabel();
							break;
	}
}




bool Decoder::GetNextNearJump(SortedIntArray *jmplist, uint _start, uint _end, uint &nextaddr)
{
    bool    ret = false;
    int     i = 0,
            f,
            address;

    f = jmplist->GetCount();
    #ifdef IDZ80_DECODER
    LogIt(wxString::Format("[0x%.4X] Searching for entries between 0x%.4X and 0x%.4X.", m_actualaddress, _start, _end));
    #endif

    while (i < f)
    {
        address = jmplist->Item(i);
        if ((address >= _start) && (address < _end))
        {
            ret = true;
            nextaddr = address;
            if (jmplist->Index(address) != wxNOT_FOUND)
            {
                jmplist->Remove(address);
                #ifdef IDZ80_DECODER
                LogIt(wxString::Format("Removed : 0x%.4X", address));
                #endif // IDZ80_DECODER
            }
            #ifdef IDZ80_DECODER
            LogIt(wxString::Format("Chosen : 0x%.4X", nextaddr));
            #endif
            break;
        }
        else
            i++;
    }
}





void Decoder::MSXCheckFunctionRegisters(DAsmElement *de)
{
    switch(de->getArgument(0, 0))
    {
    case 5:
        if (Registers.C->isLoaded())
        {
            wxString str = Labels->sys_const->Find(Registers.C->GetValue());
            if (!str.IsEmpty())
                Labels->constant_labels->AddLabel(Registers.C->GetValue(), str);
        }

        break;
    }
}





bool Decoder::MSXWeirdRST(DAsmElement *de, uint dasm_position)
{
    uint offset;
    uint temp;
    bool ret = false;

    offset = de->GetOffset();

    switch (de->GetInstructionDetail())
    {
        case II_RST_08:
                    LogIt(wxString::Format("[0x%.4X] Rst 08h detected !", m_actualaddress));
                    break;

        case II_RST_30:
                    // ID of Slot
                    LogIt(wxString::Format("[0x%.4X] Rst 30h detected !", m_actualaddress));
                    de = new DAsmElement(Process->Program);
                    de->SetLength(1);
                    de->SetOffset(++offset);
                    de->SetMnemonic(0);
                    de->SetType(et_Data);
                    Process->Disassembled->InsertDasm(de, dasm_position++);
                    LogIt(wxString::Format("[0x%.4X] Slot = %d.", m_actualaddress,
                                           Process->Program->GetData(offset)));
                    // address to be called
                    de = new DAsmElement(Process->Program);
                    de->SetLength(2);
                    de->SetOffset(++offset);
                    de->SetMnemonic(0);
                    de->SetType(et_Data);
                    de->SetStyleArgument(0, ast_wordhex);
                    Process->Disassembled->InsertDasm(de, dasm_position);
                    temp = Process->Program->GetData(offset) + Process->Program->GetData(offset + 1) * 0xFF;
                    LogIt(wxString::Format("[0x%.4X] Calling address = %.4X.", m_actualaddress,
                                           temp));
                    ret = true;
                    break;
    }
    return ret;
}



bool Decoder::CallSubroutine(DAsmElement *de)
{
    bool ret = false;
    uint address = de->getArgument(0, Process->Disassembled->GetBaseAddress(0));

    if (OutBoundaryAddress(address))
    {
        LogIt(wxString::Format("[0x%.4X] Subroutine out of boundaries [0x%.4X].", m_actualaddress, address));
        MSXCheckFunctionRegisters(de);
    }
    else
    {
        if (SubRoutine->AlreadyCalled(address))
        {
            LogIt(wxString::Format("[0x%.4X] Subroutine already called [0x%.4X].", m_actualaddress, address));
        }
        else
        {
            ret = SubRoutine->Call(address, m_nextaddress);
            if (ret)
                m_nextaddress = address;
        }
    }
    return ret;
}






bool Decoder::ReturnSubroutine(DAsmElement *de, uint &dest_address)
{
    bool ret = false;

    if (de->MnemonicObject->isConditionalReturn())
        SubRoutine->SetConditionalReturn();
    else
    {
        dest_address = SubRoutine->Return(m_actualaddress);
        if (dest_address > 0)
            ret = true;
    }

    return ret;
}






/*
 *  Process CALLs and JUMPS, return
 * true if dsm_item (FirstDisassemble) should be updated
 */
bool Decoder::ProcessBranch(DAsmElement *de, bool &processing_status)
{
    uint    address,
            tempaddress;
    bool    update_dasm_item = false;

    address = de->getArgument(0, Process->Disassembled->GetBaseAddress(0));

    if (de->MnemonicObject->isConditionalJump())
    {
        if (!OutBoundaryAddress(address))
        {
            if ((AddressList->Index(address) == wxNOT_FOUND) &&
                (address > m_actualaddress))
                AddressList->Add(address);
        }
    }
    else
    {
        if (OutBoundaryAddress(address))
        {
            if (SubRoutine->IsInside())
            {
                if (ReturnSubroutine(de, m_nextaddress))
                {
                    update_dasm_item = true;
                    #ifdef IDZ80_DECODER
                    LogIt(wxString::Format("[0x%.4X] Jump to out of boundary [0x%.4X] inside a sub-routine. Returning to 0x%.4X !", m_actualaddress, address, m_nextaddress));
                    #endif
                }
            }
            else
            {
                #ifdef IDZ80_DECODER
                LogIt(wxString::Format("[0x%.4X] Jump to out of boundary [0x%.4X]. Should finish processing ?", m_actualaddress, address));
                #endif
                if (address == 0)
                {
                    processing_status = false;
                    #ifdef IDZ80_DECODER
                    LogIt("End processing.");
                    #endif
                }
            }
        }
        else
        {
            #ifdef IDZ80_DECODER
            if ((address >= m_startaddress) && (address < m_execaddress))
            {
                LogIt(wxString::Format("[0x%.4X] Address between start and execution [0x%.4X].", m_actualaddress, address));
            }
            #endif

            if ((address >= m_execaddress) && (address <= m_actualaddress))
            {
                #ifdef IDZ80_DECODER
                LogIt(wxString::Format("[0x%.4X] Loop detected to 0x%.4X.", m_actualaddress, address));
                debugShowJmpList("         CONDITIONAL Back Test", AddressList);
                #endif

                if (GetNextNearJump(AddressList, m_nextaddress, m_endaddress, m_nextaddress))
                {
                    update_dasm_item = true;
                }
                else
                {
                    if ((SubRoutine->HasConditionalReturn()) && (SubRoutine->IsInside()) &&
                        (ReturnSubroutine(de, m_nextaddress)))
                    {
                        update_dasm_item = true;
                        #ifdef IDZ80_DECODER
                        LogIt(wxString::Format("[0x%.4X] Conditional return to 0x%.4X.", m_actualaddress, m_nextaddress));
                        #endif
                    }
                    else
                    {
                        processing_status = false;
                        #ifdef IDZ80_DECODER
                        LogIt("End processing.");
                        #endif
                    }
                }

                if ((m_nextaddress > m_endaddress) && (processing_status))
                {
                    #ifdef IDZ80_DECODER
                    LogIt(wxString::Format("Filtered nextddress [0x%.4X].", m_nextaddress));
                    #endif
                }
            }

            if (address > m_actualaddress)
            {
                #ifdef IDZ80_DECODER
                LogIt(wxString::Format("[0x%.4X] Jumping forward to 0x%.4X", m_actualaddress, address));
                debugShowJmpList("         CONDITIONAL Forward test", AddressList);
                #endif
                if (GetNextNearJump(AddressList, m_nextaddress, address, tempaddress))
                {
                    if (AddressList->Index(address) == wxNOT_FOUND)
                        AddressList->Add(address);
                    #ifdef IDZ80_DECODER
                    LogIt(wxString::Format("[0x%.4X] Save forward address to process later.", m_actualaddress));
                    #endif
                }
                else
                {
                    #ifdef IDZ80_DECODER
                    debugShowJmpList("Processed List", AddressListProcessed);
                    #endif
                    if (AddressListProcessed->Index(address) == wxNOT_FOUND)
                    {
                        tempaddress = address;
                        update_dasm_item = true;
                        #ifdef IDZ80_DECODER
                        LogIt("Proceed forward.");
                        #endif
                    }
                    else
                    {
                        if (AddressList->IsEmpty())
                        {
                            processing_status = false;
                            #ifdef IDZ80_DECODER
                            LogIt("No more entries. End processing.");
                            #endif
                        }
                        else
                        {
                            tempaddress = AddressList->Item(0);
                            update_dasm_item = true;
                            #ifdef IDZ80_DECODER
                            LogIt(wxString::Format("Address 0x%.4X already processed. Jumping to 0x%.4X.", address, tempaddress));
                            #endif
                        }
                    }
                }

                m_nextaddress = tempaddress;
            }
        }
    }

    return update_dasm_item;
}




void Decoder::FillData()
{
	int			i,
				j,
				delta,
				final,
				count, index;
	DAsmElement *de_1,
				*de_2;
	bool		processing;



	processing = (Process->Disassembled->GetCount() > 1);
	i = 0;
	j = 0;
	final = (Process->Disassembled->GetCount() - 2);

	while (processing)
	{
		de_1 = Process->Disassembled->GetData(i);
		de_2 = Process->Disassembled->GetData(i + 1);
		index = (de_1->GetOffset() + de_1->GetLength());
		delta = de_2->GetOffset() - index;
		for(count = 0; count < delta; count++)
		{
			de_1 = new DAsmElement(Process->Program);
			de_1->SetLength(1);
			de_1->SetOffset(index + count);
			de_1->SetMnemonic(0);
			de_1->SetType(et_Data);
			Process->Disassembled->InsertDasm(de_1, i + 1);
			i++;
		}
		i++;
		if (i > (Process->Disassembled->GetCount() - 2))
			processing = false;
	}

	i = de_2->GetOffset() + de_2->GetLength();

	for(count = i; count < Process->Program->GetBufferSize(); count++)
	{
		de_1 = new DAsmElement(Process->Program);
		de_1->SetLength(1);
		de_1->SetOffset(count);
		de_1->SetMnemonic(0);
		de_1->SetType(et_Data);
		Process->Disassembled->AddDasm(de_1);
	}
}







bool Decoder::FirstDisassemble(LabelManager *parent)
{
    bool    ret = false,
            processing = true,
            update_item = false;

    int     dsm_item,
            i;

    uint    program_size,
            counter = 0,
            address;

    DAsmElement     *de;
    SortedIntArray  CartridgeCalls(CompareSortedInt);


    Labels = parent;
    Labels->ClearUserLabels();
    SubRoutine->Clear();
    var_label_counter = 0;
    io_label_counter = 0;
    program_label_counter = 0;

    UpdateBoundary();

    AddressList->Clear();
    AddressListProcessed->Clear();


	if (Process->Program->isCartridge())
	{
		SetCartridgeLabels();
	    Process->Program->GetEntries(CartridgeCalls);
	    m_nextaddress = CartridgeCalls.Index(0);
	}
	else
    {
        m_nextaddress = Process->Program->ExecAddress;
        Labels->prog_labels->AddLabel(m_nextaddress, "START");
    }
    program_size = Process->Program->GetBufferSize();
    dsm_item = program_size;
    while (processing)
    {
        m_last_prg_counter = m_nextaddress - Process->Disassembled->GetBaseAddress(0);
        m_actualaddress = m_nextaddress;

        if (AddressList->Index(m_actualaddress) != wxNOT_FOUND)
        {
            AddressList->Remove(m_actualaddress);
            #ifdef IDZ80_DECODER
            LogIt(wxString::Format("[0x%.4X] Removed this address from list !", m_actualaddress));
            #endif
        }

        if (AddressListProcessed->Index(m_nextaddress) == wxNOT_FOUND)
            AddressListProcessed->Add(m_actualaddress);

        de = new DAsmElement(Process->Program);
        dsm_item = Decode(de, m_last_prg_counter, dsm_item);
        m_nextaddress += de->GetLength();
        counter += de->GetLength();
        m_prg_counter = m_last_prg_counter + de->GetLength();

        address = de->getArgument(0, Process->Disassembled->GetBaseAddress(0));

        SetupArgumentLabels(de, dsm_item);

        switch (de->GetInstructionType())
        {
        case IT_CALL:
            update_item = CallSubroutine(de);
            #ifdef IDZ80_DECODER
            if (update_item)
            {
                LogIt(wxString::Format("[0x%.4X] Entering subroutine No. %d  [0x%.4X].", m_actualaddress, SubRoutine->GetCounter(), address));
            }
            #endif
            break;

        case IT_RET:
            update_item = ReturnSubroutine(de, m_nextaddress);
            #ifdef IDZ80_DECODER
            if (update_item)
            {
                LogIt(wxString::Format("[0x%.4X] Returning from subroutine No. %d.", m_actualaddress, (SubRoutine->GetCounter() + 1)));
            }
            #endif
            break;

        case IT_JUMP:
            update_item = ProcessBranch(de, processing);
            break;

        case IT_LOAD:
            Registers.LoadRegister(de);
            break;

        case IT_RST:
            if (MSXWeirdRST(de, (dsm_item + 1)))
            {
                update_item = true;
                m_nextaddress += 3;
            }
            break;
        case IT_ERROR:
            LogIt(wxString::Format("[0x%.4X] Error: opcode = %s, dasmitem = %d", m_actualaddress, de->getCodeStr(), dsm_item));
            break;
        }

        if ((!processing) && (CartridgeCalls.GetCount() != 0))
        {
            #ifdef IDZ80_DECODER
            LogIt("Process remaining calls...");
            debugShowJmpList("CALL", &CartridgeCalls);
            #endif

            m_nextaddress = CartridgeCalls.Item(0);
            CartridgeCalls.Remove(m_nextaddress);
            update_item = true;
            processing = true;
        }

        if (update_item)
        {
            dsm_item = Process->Disassembled->FindAddress(m_nextaddress);
            if (dsm_item == -1)
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



void Decoder::SetCartridgeLabels()
{
	uint address;

	address = Process->Program->GetCartridgeHeader()->init;
	if (address != 0)
		Labels->prog_labels->AddLabel(address, "INIT");

	address = Process->Program->GetCartridgeHeader()->statement;
	if (address != 0)
		Labels->prog_labels->AddLabel(address, "STATEMENT");

	address = Process->Program->GetCartridgeHeader()->device;
	if (address != 0)
		Labels->prog_labels->AddLabel(address, "DEVICE");

	address = Process->Program->GetCartridgeHeader()->text;
	if (address != 0)
		Labels->prog_labels->AddLabel(address, "TEXT");
}




bool Decoder::OutBoundaryAddress(uint _addr)
{
    return ((_addr > m_endaddress) || (_addr < m_startaddress));
}




void Decoder::FullDisassemble(LabelManager *parent)
{
    uint i, f, item;
    int percent;
    DAsmElement *de;

    Labels = parent;
    Labels->ClearUserLabels();
    var_label_counter = 0;
    io_label_counter = 0;
    program_label_counter = 0;

    UpdateBoundary();


    f = Process->Program->GetBufferSize();
    for (i = 0; i < f; i++)
    {
        de = new DAsmElement(Process->Program);
        item = Decode(de, i);
        SetupArgumentLabels(de, item);
        Registers.LoadRegister(de);
        if (MSXWeirdRST(de, f))
            i += 3;
        i += (de->GetLength() - 1);
/*
        if (!(m_gauge == NULL))
        {
            percent = i * 100 / f;
            m_gauge->SetValue(percent);
        }
*/
    }
}



void Decoder::DisassembleItems(RangeItems &dasm_range)
{
    uint 		i,
				dasm_last,
				x,
				program_first,
				program_last;
    DAsmElement	*de;


    dasm_last = dasm_range.Index + dasm_range.Count - 1;
    x = Process->Disassembled->GetCount();

    if ((dasm_range.Index < x) || (dasm_last < x))
    {
        de = Process->Disassembled->GetData(dasm_range.Index);
        program_first = de->GetOffset();
        de = Process->Disassembled->GetData(dasm_range.Index + dasm_range.Count - 1);
        program_last = de->GetOffset() + de->GetLength();

        LogIt(wxString::Format("Deleting index = %d, count = %d.", dasm_range.Index, dasm_range.Count));
        Process->Disassembled->DelDasm(dasm_range.Index, dasm_range.Count);
        x = dasm_range.Index;
        dasm_range.Count = 0;
        for (i = program_first; i < program_last; i++)
        {
            de = new DAsmElement(Process->Program);
            Decode(de, i, x++);
            i += (de->GetLength() - 1);
            dasm_range.Count++;
        }
    }
}





void Decoder::debugShowList(const wxString &listname, SortedIntArray *_list)
{
    int i;
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




void Decoder::debugShowJmpList(const wxString &listname, SortedIntArray *_list)
{
    int i;
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



wxString Decoder::GetCodeSegmentStr()
{
    return "Abandoned";
}

void Decoder::OptimizeCodeSegment()
{
}

