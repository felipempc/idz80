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

Decoder::Decoder(RawData *program, DAsmData *dasmdata, MnemonicDataBase *mnemonics)
{
    m_program = program;
    m_dasmeditems = dasmdata;
    m_mnemonics = mnemonics;
    SubRoutine = new SubRoutineCtrl();
    m_modulename = "DECODER:";

    m_startaddress = 0;
    m_endaddress = 0;
    m_execaddress = 0;

    m_conditionaljumplist = new SortedIntArray(CompareSortedInt);
    m_unconditionaljumplist = new SortedIntArray(CompareSortedInt);
}


Decoder::~Decoder()
{
    delete SubRoutine;

    if (m_conditionaljumplist != 0)
        delete m_conditionaljumplist;
    if (m_unconditionaljumplist != 0)
        delete m_unconditionaljumplist;
}


void Decoder::UpdateBoundary()
{
    if ((m_program != 0) && (m_program->EndAddress > m_program->StartAddress) &&
        (m_program->ExecAddress >= m_program->StartAddress) &&
        (m_program->ExecAddress < m_program->EndAddress))
    {
        m_startaddress = m_program->StartAddress;
        m_endaddress = m_program->EndAddress;
        m_execaddress = m_program->ExecAddress;
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
    MnemonicItem    *mnemonic;

    foundItems.Clear();
    offset = 0;
    if (maxitems > MAX_OPCODE_SIZE)
        maxitems = MAX_OPCODE_SIZE;

    while (offset < maxitems)
    {
        scan = m_program->GetData(prg_index + offset);
        m_mnemonics->FindItems(foundItems, scan, offset);
        offset++;
        if (foundItems.GetCount() < 2)
			break;
    }
    nitems = foundItems.GetCount();
    if (nitems == 1)
    {
        ret = foundItems.Last();
        mnemonic = m_mnemonics->GetData(ret);
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
    next_de = m_dasmeditems->GetData(dasm_position);
    if (next_de == 0)
        scan_limit = m_program->GetBufferSize();
    else
        scan_limit = next_de->GetOffset() - prg_index;

    mnc_item = Fetch(prg_index, scan_limit);

    if (mnc_item == OPCODE_NOT_FOUND)
    {
        de->SetLength(1);
        de->SetOffset(prg_index);
        de->SetMnemonic(0);
        de->SetType(et_Data);
        ret = m_dasmeditems->InsertDasm(de, dasm_position);
    }
    else
    {
        de->SetMnemonic(m_mnemonics->GetData(mnc_item));
        de->SetLength();
        de->SetType(et_Instruction);
        de->SetOffset(prg_index);
        if (de->CopyArguments())
			ret = m_dasmeditems->InsertDasm(de, dasm_position);
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
							Labels->var_labels->AddLabel(argument, str);
							de->SetArgLabel();
							break;
		case ARG_ABS_ADDR:
		case ARG_REL_ADDR:
                            argument = de->getArgument(0, m_dasmeditems->GetBaseAddress(index));
							str = Labels->sys_calls->Find(argument);
							if (str.IsEmpty())
								str.Printf("LABEL%d", program_label_counter++);
							Labels->prog_labels->AddLabel(argument, str);
							de->SetArgLabel();
							break;
		case ARG_IO_ADDR:
                            argument = de->getArgument(0, 0);
							str = Labels->sys_io->Find(argument);
							if (str.IsEmpty())
								str.Printf("PORT%d", io_label_counter++);
							Labels->io_labels->AddLabel(argument, str);
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
    LogIt(wxString::Format("[0x%.4X] Searching for entries between 0x%.4X and 0x%.4X.\n", m_actualaddress, _start, _end));
    #endif

    while (i < f)
    {
        address = jmplist->Item(i);
        if ((address >= _start) && (address < _end))
        {
            ret = true;
            nextaddr = address;
            if (jmplist->Index(address) != wxNOT_FOUND)
                jmplist->Remove(address);
            #ifdef IDZ80_DECODER
            LogIt(wxString::Format("Chosen : 0x%.4X\n", nextaddr));
            #endif
            break;
        }
        else
            i++;
    }
/*
    else
    {
        #ifdef IDZ80_DECODER
        LogIt(wxString::Format("[0x%.4X] Address 0x%.4X already processed. Removed !\n", m_actualaddress, address));
        #endif
        if (jmplist->Index(address) != wxNOT_FOUND)
            jmplist->Remove(address);
        f = jmplist->GetCount();
    }
*/
    return ret;
}




bool Decoder::GetNextFarJump(SortedIntArray *jmplist, uint &nextaddr)
{
    bool    ret = false;
    int     address;

    #ifdef IDZ80_DECODER
    LogIt(wxString::Format("[0x%.4X] Searching for FAR entries.\n", m_actualaddress));
    #endif

    while (jmplist->GetCount() > 0)
    {
        address = jmplist->Item(0);

        ret = true;
        nextaddr = address;
        if (jmplist->Index(address) != wxNOT_FOUND)
            jmplist->Remove(address);
        #ifdef IDZ80_DECODER
        LogIt(wxString::Format("Chosen : 0x%.4X\n", nextaddr));
        #endif
        break;
    }
/*
    else
    {
        #ifdef IDZ80_DECODER
        LogIt(wxString::Format("[0x%.4X] Far address 0x%.4X already processed. Removed !\n", m_actualaddress, address));
        #endif
        if (jmplist->Index(address) != wxNOT_FOUND)
            jmplist->Remove(address);
    }
*/
    return ret;
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
                    LogIt(wxString::Format("[%.4X] Rst 08h detected !\n", m_actualaddress));
                    break;

        case II_RST_30:
                    // ID of Slot
                    LogIt(wxString::Format("[%.4X] Rst 30h detected !\n", m_actualaddress));
                    de = new DAsmElement(m_program);
                    de->SetLength(1);
                    de->SetOffset(++offset);
                    de->SetMnemonic(0);
                    de->SetType(et_Data);
                    m_dasmeditems->InsertDasm(de, dasm_position++);
                    LogIt(wxString::Format("[%.4X] Slot = %d.\n", m_actualaddress,
                                           m_program->GetData(offset)));
                    // address to be called
                    de = new DAsmElement(m_program);
                    de->SetLength(2);
                    de->SetOffset(++offset);
                    de->SetMnemonic(0);
                    de->SetType(et_Data);
                    de->SetStyleArgument(0, ast_wordhex);
                    m_dasmeditems->InsertDasm(de, dasm_position);
                    temp = m_program->GetData(offset) + m_program->GetData(offset + 1) * 0xFF;
                    LogIt(wxString::Format("[%.4X] Calling address = %.4X.\n", m_actualaddress,
                                           temp));
                    ret = true;
                    break;
    }
    return ret;
}



bool Decoder::CallSubroutine(DAsmElement *de)
{
    bool ret = false;
    uint address = de->getArgument(0, m_dasmeditems->GetBaseAddress(0));

    if (OutBoundaryAddress(address))
    {
        MSXCheckFunctionRegisters(de);
    }
    else
    {
        ret = SubRoutine->Call(address, m_nextaddress);
        if (ret)
            m_nextaddress = address;
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


    if (m_unconditionaljumplist->Index(m_actualaddress) != wxNOT_FOUND)
    {
        m_unconditionaljumplist->Remove(m_actualaddress);
        #ifdef IDZ80_DECODER
        LogIt(wxString::Format("[0x%.4X] Removed this address from unconditional list !\n", m_actualaddress));
        #endif
    }

    if (m_conditionaljumplist->Index(m_actualaddress) != wxNOT_FOUND)
    {
        m_conditionaljumplist->Remove(m_actualaddress);
        #ifdef IDZ80_DECODER
        LogIt(wxString::Format("[0x%.4X] Removed this address from conditional list !\n", m_actualaddress));
        #endif
    }

    address = de->getArgument(0, m_dasmeditems->GetBaseAddress(0));

    if (de->MnemonicObject->isConditionalJump())
    {
        if (!OutBoundaryAddress(address))
            m_conditionaljumplist->Add(address);
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
                    LogIt(wxString::Format("[0x%.4X] Jump to out of boundary [0x%.4X] inside a sub-routine. Returning to 0x%.4X !\n", m_actualaddress, address, m_nextaddress));
                    #endif
                }
            }
            else
            {
                #ifdef IDZ80_DECODER
                LogIt(wxString::Format("[0x%.4X] Jump to out of boundary [0x%.4X]. Should finish processing ?\n", m_actualaddress, address));
                #endif
                if (address == 0)
                {
                    processing_status = false;
                    #ifdef IDZ80_DECODER
                    LogIt("End processing.\n");
                    #endif
                }
            }
        }
        else
        {
            #ifdef IDZ80_DECODER
            if ((address >= m_startaddress) && (address < m_execaddress))
            {
                LogIt(wxString::Format("[0x%.4X] Address between start and execution [0x%.4X].\n", m_actualaddress, address));
            }
            #endif

            if ((address >= m_execaddress) && (address <= m_actualaddress))
            {
                #ifdef IDZ80_DECODER
                LogIt(wxString::Format("[0x%.4X] Deviation back [0x%.4X].\n", m_actualaddress, address));
                debugShowJmpList("COND_Backtest", m_conditionaljumplist);
                #endif

                if (GetNextNearJump(m_conditionaljumplist, m_nextaddress, m_endaddress, m_nextaddress))
                {
                    update_dasm_item = true;
                }
                else
                {
                    #ifdef IDZ80_DECODER
                    LogIt("Check for unconditional entries :\n");
                    #endif

                    if (GetNextFarJump(m_unconditionaljumplist, m_nextaddress))
                    {
                        #ifdef IDZ80_DECODER
                        debugShowJmpList("UNCOND_BackTest", m_unconditionaljumplist);
                        #endif
                        update_dasm_item = true;
                    }
                    else
                    {
                        #ifdef IDZ80_DECODER
                        LogIt("No unconditional entries.\n");
                        #endif
                        if ((SubRoutine->HasConditionalReturn()) && (SubRoutine->IsInside()) &&
                            (ReturnSubroutine(de, m_nextaddress)))
                        {
                            update_dasm_item = true;
                            #ifdef IDZ80_DECODER
                            LogIt(wxString::Format("[0x%.4X] Conditional return to 0x%.4X.\n", m_actualaddress, m_nextaddress));
                            #endif
                        }
                        else
                        {
                            processing_status = false;
                            #ifdef IDZ80_DECODER
                            LogIt("End processing.\n");
                            #endif
                        }
                    }
                }

                if ((m_nextaddress > m_endaddress) && (processing_status))
                {
                    #ifdef IDZ80_DECODER
                    LogIt(wxString::Format("Filtered nextddress [0x%.4X].\n", m_nextaddress));
                    #endif
                }
            }

            if (address > m_actualaddress)
            {
                #ifdef IDZ80_DECODER
                LogIt(wxString::Format("[0x%.4X] Deviation point forward to 0x%.4X\n", m_actualaddress, address));
                debugShowJmpList("COND_Forwardtest", m_conditionaljumplist);
                #endif
                if (GetNextNearJump(m_conditionaljumplist, m_nextaddress, address, tempaddress))
                {
                    if (m_unconditionaljumplist->Index(address) == wxNOT_FOUND)
                        m_unconditionaljumplist->Add(address);
                    #ifdef IDZ80_DECODER
                    LogIt("Save forward address to process later.\n");
                    debugShowJmpList("UNCOND_ForwardTest", m_unconditionaljumplist);
                    #endif
                }
                else
                {
                    tempaddress = address;
                    update_dasm_item = true;
                    #ifdef IDZ80_DECODER
                    LogIt("Proceed forward.\n");
                    #endif
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



	processing = (m_dasmeditems->GetCount() > 1);
	i = 0;
	j = 0;
	final = (m_dasmeditems->GetCount() - 2);

	while (processing)
	{
		de_1 = m_dasmeditems->GetData(i);
		de_2 = m_dasmeditems->GetData(i + 1);
		index = (de_1->GetOffset() + de_1->GetLength());
		delta = de_2->GetOffset() - index;
		for(count = 0; count < delta; count++)
		{
			de_1 = new DAsmElement(m_program);
			de_1->SetLength(1);
			de_1->SetOffset(index + count);
			de_1->SetMnemonic(0);
			de_1->SetType(et_Data);
			m_dasmeditems->InsertDasm(de_1, i + 1);
			i++;
		}
		i++;
		if (i > (m_dasmeditems->GetCount() - 2))
			processing = false;
	}

	i = de_2->GetOffset() + de_2->GetLength();

	for(count = i; count < m_program->GetBufferSize(); count++)
	{
		de_1 = new DAsmElement(m_program);
		de_1->SetLength(1);
		de_1->SetOffset(count);
		de_1->SetMnemonic(0);
		de_1->SetType(et_Data);
		m_dasmeditems->AddDasm(de_1);
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

    m_unconditionaljumplist->Clear();
    m_conditionaljumplist->Clear();


	if (m_program->isCartridge())
	{
		SetCartridgeLabels();
	    m_program->GetEntries(CartridgeCalls);
	    m_nextaddress = CartridgeCalls.Index(0);
	}
	else
    {
        m_nextaddress = m_program->ExecAddress;
        Labels->prog_labels->AddLabel(m_nextaddress, "START");
    }
    program_size = m_program->GetBufferSize();
    dsm_item = program_size;
    while (processing)
    {
        m_last_prg_counter = m_nextaddress - m_dasmeditems->GetBaseAddress(0);
        m_actualaddress = m_nextaddress;

        de = new DAsmElement(m_program);
        dsm_item = Decode(de, m_last_prg_counter, dsm_item);
        m_nextaddress += de->GetLength();
        counter += de->GetLength();
        m_prg_counter = m_last_prg_counter + de->GetLength();


        address = de->getArgument(0, m_dasmeditems->GetBaseAddress(0));

        SetupArgumentLabels(de, dsm_item);

        switch (de->GetInstructionType())
        {
        case IT_CALL:
            update_item = CallSubroutine(de);
            #ifdef IDZ80_DECODER
            if (update_item)
            {
                LogIt(wxString::Format("[0x%.4X] Entering subroutine No. %d  [0x%.4X].\n", m_actualaddress, SubRoutine->GetCounter(), address));
            }
            else
            {
                LogIt(wxString::Format("[0x%.4X] Call to out of boundary [0x%.4X]. Ignored !\n", m_actualaddress, address));
            }
            #endif
            break;

        case IT_RET:
            update_item = ReturnSubroutine(de, m_nextaddress);
            #ifdef IDZ80_DECODER
            if (update_item)
            {
                LogIt(wxString::Format("[0x%.4X] Returning from subroutine No. %d.\n", m_actualaddress, SubRoutine->GetCounter()));
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
            LogIt("Error !!!!\n");
            break;
        }

        if ((!processing) && (CartridgeCalls.GetCount() != 0))
        {
            #ifdef IDZ80_DECODER
            LogIt("Process remaining calls...\n");
            debugShowJmpList("CALL", &CartridgeCalls);
            #endif

            m_nextaddress = CartridgeCalls.Item(0);
            CartridgeCalls.Remove(m_nextaddress);
            update_item = true;
            processing = true;
        }

        if (update_item)
        {
            dsm_item = m_dasmeditems->FindAddress(m_nextaddress);
            if (dsm_item == -1)
                processing = false;
            update_item = false;
        }
        else
            dsm_item++;

        if (counter >= program_size)
        {
            processing = false;
            LogIt("End of file ! Finishing...\n");
        }
    } // end while

    FillData();
    return ret;
}



void Decoder::SetCartridgeLabels()
{
	uint address;

	address = m_program->GetCartridgeHeader()->init;
	if (address != 0)
		Labels->prog_labels->AddLabel(address, "INIT");

	address = m_program->GetCartridgeHeader()->statement;
	if (address != 0)
		Labels->prog_labels->AddLabel(address, "STATEMENT");

	address = m_program->GetCartridgeHeader()->device;
	if (address != 0)
		Labels->prog_labels->AddLabel(address, "DEVICE");

	address = m_program->GetCartridgeHeader()->text;
	if (address != 0)
		Labels->prog_labels->AddLabel(address, "TEXT");
}




bool Decoder::OutBoundaryAddress(uint _addr)
{
    return ((_addr > m_endaddress) || (_addr < m_startaddress));
}




void Decoder::FullDisassemble(LabelManager *parent)
{
    uint i, f;
    int percent;
    DAsmElement *de;

    Labels = parent;
    Labels->ClearUserLabels();

    f = m_program->GetBufferSize();
    for (i = 0; i < f; i++)
    {
        de = new DAsmElement(m_program);
        Decode(de, i);
        SetupArgumentLabels(de, f);
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
    x = m_dasmeditems->GetCount();

    if ((dasm_range.Index < x) || (dasm_last < x))
    {
        de = m_dasmeditems->GetData(dasm_range.Index);
        program_first = de->GetOffset();
        de = m_dasmeditems->GetData(dasm_range.Index + dasm_range.Count - 1);
        program_last = de->GetOffset() + de->GetLength();

        LogIt(wxString::Format("Deleting index = %d, count = %d.\n", dasm_range.Index, dasm_range.Count));
        m_dasmeditems->DelDasm(dasm_range.Index, dasm_range.Count);
        x = dasm_range.Index;
        dasm_range.Count = 0;
        for (i = program_first; i < program_last; i++)
        {
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
        str << "]\n";
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
        str << "]\n";
        LogIt(str);
    }
}



wxString Decoder::GetCodeSegmentStr()
{
    return "Abandoned\n";
}

void Decoder::OptimizeCodeSegment()
{
}

