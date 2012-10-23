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
    m_modulename = "DECODER:";

    m_startaddress = 0;
    m_endaddress = 0;
    m_execaddress = 0;

    m_conditionaljumplist = new SortedIntArray(CompareSortedInt);
    m_unconditionaljumplist = new SortedIntArray(CompareSortedInt);
    m_callList = new SortedIntArray(CompareSortedInt);
    m_codesegments = new SegmentMgr();
    m_datasegments = 0;
    m_varlist = new SortedIntArray(CompareSortedInt);
    m_iolist = new SortedIntArray(CompareSortedInt);
    m_entrylist = new SortedIntArray(CompareSortedInt);
}


Decoder::~Decoder()
{
    if (m_conditionaljumplist != 0)
        delete m_conditionaljumplist;
    if (m_unconditionaljumplist != 0)
        delete m_unconditionaljumplist;
    if (m_callList != 0)
        delete m_callList;
    if (m_codesegments != 0)
        delete m_codesegments;
    if (m_datasegments != 0)
        delete m_datasegments;
    if (m_varlist != 0)
        delete m_varlist;
    if (m_iolist != 0)
        delete m_iolist;
    if (m_entrylist != 0)
        delete m_entrylist;
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



SortedIntArray *Decoder::GetVarList()
{
    return m_varlist;
}

SortedIntArray *Decoder::GetIOList()
{
    return m_iolist;
}

SortedIntArray *Decoder::GetEntryList()
{
    return m_entrylist;
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
void Decoder::ProcessArgument(DAsmElement *de, uint index)
{
    enum ArgType	argtype;
    uint            argument;
    wxString		str;
    static int		prog = 0,
					var = 0,
					io = 0;

	if ((m_varlabels->GetCount() == 0) &&
		(m_proglabels->GetCount() == 0) &&
		(m_iolabels->GetCount() == 0))
	{
		prog = 0;
		var = 0;
		io = 0;
	}

	argtype = de->GetArgumentType(0);
	switch (argtype)
	{
		case ARG_VARIABLE:
                            argument = de->getArgument(0, 0);
							str = m_sysvars->Find(argument);
							if (str.IsEmpty())
								str.Printf("VAR%d", var++);
							m_varlabels->AddLabel(argument, str);
							de->SetArgLabel();
                            if (m_varlist->Index(argument) == wxNOT_FOUND)
                                m_varlist->Add(argument);
							break;
		case ARG_ABS_ADDR:
		case ARG_REL_ADDR:
                            argument = de->getArgument(0, m_dasmeditems->GetBaseAddress(index));
							str = m_syscalls->Find(argument);
							if (str.IsEmpty())
								str.Printf("LABEL%d", prog++);
							m_proglabels->AddLabel(argument, str);
							de->SetArgLabel();
                            if (m_entrylist->Index(argument) == wxNOT_FOUND)
                                m_entrylist->Add(argument);
							break;
		case ARG_IO_ADDR:
                            argument = de->getArgument(0, 0);
							str = m_sysio->Find(argument);
							if (str.IsEmpty())
								str.Printf("PORT%d", io++);
							m_iolabels->AddLabel(argument, str);
							de->SetArgLabel();
                            if (m_iolist->Index(argument) == wxNOT_FOUND)
                                m_iolist->Add(argument);
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
        if (!m_codesegments->inSegment(address))
        {
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
        else
        {
            #ifdef IDZ80_DECODER
            LogIt(wxString::Format("[0x%.4X] Address 0x%.4X already processed. Removed !\n", m_actualaddress, address));
            #endif
            if (jmplist->Index(address) != wxNOT_FOUND)
                jmplist->Remove(address);
            f = jmplist->GetCount();
        }
    }
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
        if (!m_codesegments->inSegment(address))
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
        {
            #ifdef IDZ80_DECODER
            LogIt(wxString::Format("[0x%.4X] Far address 0x%.4X already processed. Removed !\n", m_actualaddress, address));
            #endif
            if (jmplist->Index(address) != wxNOT_FOUND)
                jmplist->Remove(address);
        }
    }
    return ret;
}



void Decoder::MSXCheckFunctionRegisters(DAsmElement *de)
{
    switch(de->getArgument(0, 0))
    {
    case 5:
        if (m_Register.C->isLoaded())
        {
            wxString str = m_sysconst->Find(m_Register.C->GetValue());
            if (!str.IsEmpty())
                m_constlabels->AddLabel(m_Register.C->GetValue(), str);
        }

        break;
    }
}



void Decoder::MSXWeirdRST(DAsmElement *de)
{
    uint offset;

    if (de->GetInstructionType() == IT_RST)
    {
        offset = de->GetOffset();

        switch (de->GetInstructionInfo())
        {
            case II_RST_08:
                            break;

            case II_RST_30:
                        de = new DAsmElement(m_program);
                        de->SetLength(1);
                        de->SetOffset(++offset);
                        de->SetMnemonic(0);
                        de->SetType(et_Data);
                        m_dasmeditems->InsertDasm(de, dasm_position);

                            break;
        }
    }
}



bool Decoder::CallSubroutine(DAsmElement *de, uint address)
{
    bool ret = false;

    if ((!OutBoundaryAddress(address)) && (!m_codesegments->inSegment(address)))
    {
        ret = true;
        m_CallStack.push(m_nextaddress);
        m_CallStack.push(m_codesegments->GetSegmentIndex());

        m_nextaddress = address;
        m_codesegments->SetSegmentIndex(m_codesegments->Add(m_nextaddress, m_nextaddress));
        if (m_codesegments->GetSegmentIndex() < 0)
            ret = false;
    }
    if (OutBoundaryAddress(address))
    {
        MSXCheckFunctionRegisters(de);
    }
    return ret;
}



bool Decoder::ReturnSubroutine(uint &dest_address)
{
    bool ret = false;

    if (!m_CallStack.empty())
    {
        ret = true;
        m_codesegments->SetSegmentIndex(m_CallStack.top());
        m_CallStack.pop();
        dest_address = m_CallStack.top();
        m_CallStack.pop();
    }

    return ret;
}


/*
 *  Process CALLs and JUMPS, return
 * true if dsm_item (FirstDisassemble) should be updated
 */
bool Decoder::ProcessBranch(DAsmElement *de, int &num_call, bool &processing_status)
{
    uint        address,
                tempaddress;
    bool        returning = false,
                update_dasm_item = false;
    static bool conditional_return = false;


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

    switch(de->GetBranchType())
    {
        case BR_CALL:
        case BR_CALL_CND:
                        if (CallSubroutine(de, address))
                        {
                            num_call++;
                            update_dasm_item = true;
                            #ifdef IDZ80_DECODER
                            LogIt(wxString::Format("[0x%.4X] Entering subroutine No. %d  [0x%.4X].\n", m_actualaddress, num_call, address));
                            #endif

                        }
                        #ifdef IDZ80_DECODER
                        if (OutBoundaryAddress(address))
                        {
                            LogIt(wxString::Format("[0x%.4X] Call to out of boundary [0x%.4X]. Ignored !\n", m_actualaddress, address));
                        }
                        #endif
                        break;
        case BR_RETURN:
                        if (ReturnSubroutine(m_nextaddress))
                        {
                            #ifdef IDZ80_DECODER
                            LogIt(wxString::Format("[0x%.4X] Returning from subroutine No. %d.\n", m_actualaddress, num_call));
                            #endif
                            num_call--;
                            update_dasm_item = true;
                        }
                        else
                        {
                            processing_status = false;
                            #ifdef IDZ80_DECODER
                            LogIt(wxString::Format("[0x%.4X] Failed to return, Empty stack ! Finishing...\n", m_actualaddress));
                            #endif
                        }
                        break;

        case BR_RETURN_CND:
                        conditional_return = true;
                        break;

        case BR_JUMP:
                        if (OutBoundaryAddress(address))
                        {
                            if ((num_call > 0) && ReturnSubroutine(m_nextaddress))
                            {
                                num_call--;
                                update_dasm_item = true;
                                returning = true;
                                #ifdef IDZ80_DECODER
                                LogIt(wxString::Format("[0x%.4X] Jump to out of boundary [0x%.4X] inside a sub-routine. Returning to 0x%.4X !\n", m_actualaddress, address, m_nextaddress));
                                #endif
                            }

                            if (num_call == 0)
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

                                if (GetNextNearJump(m_conditionaljumplist, m_nextaddress, m_endaddress, tempaddress))
                                {
                                    update_dasm_item = true;
                                }
                                else
                                {
                                    #ifdef IDZ80_DECODER
                                    LogIt("Check for unconditional entries :\n");
                                    #endif

                                    if (GetNextFarJump(m_unconditionaljumplist, tempaddress))
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
                                        if ((conditional_return) && (num_call > 0) &&
                                            (ReturnSubroutine(tempaddress)))
                                        {
                                            num_call--;
                                            conditional_return = false;
                                            returning = true;
                                            update_dasm_item = true;
                                            #ifdef IDZ80_DECODER
                                            LogIt(wxString::Format("[0x%.4X] Conditional return to 0x%.4X.\n", m_actualaddress, tempaddress));
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
                                    if ((processing_status) && (!returning) &&
                                        (tempaddress != m_nextaddress))
                                        m_codesegments->SetSegmentIndex(m_codesegments->Add(tempaddress, tempaddress));
                                }

                                m_nextaddress = tempaddress;
                                returning = false;

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

                                if (tempaddress != m_nextaddress)
                                    m_codesegments->SetSegmentIndex(m_codesegments->Add(tempaddress, tempaddress));

                                m_nextaddress = tempaddress;
                            }
                        }
                        break;

        case BR_JUMP_CND:
                        if (!OutBoundaryAddress(address))
                            m_conditionaljumplist->Add(address);
                        break;
    } // end Switch

    return update_dasm_item;
}



void Decoder::ProcessLoadReg(DAsmElement *de)
{
    enum InstruInfo info;

    m_Register.UpdateLife();

    if (de->GetInstructionType() == IT_LOAD)
    {
        switch (de->GetInstructionInfo())
        {
            case II_LD_A_N:
                            m_Register.A->Init(de->getArgument(0, 0), de->GetOffset());
                            break;
            case II_LD_B_N:
                            m_Register.B->Init(de->getArgument(0, 0), de->GetOffset());
                            break;
            case II_LD_C_N:
                            m_Register.C->Init(de->getArgument(0, 0), de->GetOffset());
                            break;
            case II_LD_D_N:
                            m_Register.D->Init(de->getArgument(0, 0), de->GetOffset());
                            break;
            case II_LD_E_N:
                            m_Register.E->Init(de->getArgument(0, 0), de->GetOffset());
                            break;
            case II_LD_H_N:
                            m_Register.H->Init(de->getArgument(0, 0), de->GetOffset());
                            break;
            case II_LD_L_N:
                            m_Register.L->Init(de->getArgument(0, 0), de->GetOffset());
                            break;
            case II_LD_BC_N:
                            m_Register.BC->Init(de->getArgument(0, 0), de->GetOffset());
                            break;
            case II_LD_DE_N:
                            m_Register.DE->Init(de->getArgument(0, 0), de->GetOffset());
                            break;
            case II_LD_HL_N:
                            m_Register.HL->Init(de->getArgument(0, 0), de->GetOffset());
                            break;
            case II_LD_SP_N:
                            m_Register.SP->Init(de->getArgument(0, 0), de->GetOffset());
                            break;
            case II_LD_IX_N:
                            m_Register.IX->Init(de->getArgument(0, 0), de->GetOffset());
                            break;
            case II_LD_IY_N:
                            m_Register.IY->Init(de->getArgument(0, 0), de->GetOffset());
                            break;
        }
    }
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







bool Decoder::FirstDisassemble(LabelListCtrl *proglabels, LabelListCtrl *varlabels,
							LabelListCtrl *iolabels, LabelListCtrl *constlabels,
							SystemLabelList *syscalls, SystemLabelList *sysvars,
							SystemLabelList *sysio, SystemLabelList *sysconst)
{
    bool    ret = false,
            processing = true,
            update_item = false;

    int     dsm_item,
            num_call = 0,
            i;

    uint    program_size,
            counter = 0;

    DAsmElement     *de;


	m_proglabels = proglabels;
	m_varlabels = varlabels;
	m_iolabels = iolabels;
	m_constlabels = constlabels;
	m_syscalls = syscalls;
	m_sysvars = sysvars;
	m_sysio = sysio;
	m_sysconst = sysconst;

    UpdateBoundary();

	m_proglabels->Clear();
	m_varlabels->Clear();
	m_iolabels->Clear();
	m_constlabels->Clear();

    m_iolist->Clear();
    m_varlist->Clear();
    m_entrylist->Clear();
    m_unconditionaljumplist->Clear();
    m_conditionaljumplist->Clear();
    m_codesegments->Clear();


	if (m_program->isCartridge())
	{
		SetCartridgeLabels();

	    m_program->GetEntries(*m_callList);
		m_nextaddress = m_callList->Item(0);
		m_callList->Remove(m_nextaddress);
		for (i = 0; i < m_callList->GetCount(); i++)
            m_entrylist->Add(m_callList->Item(i));

        num_call++;
	}
	else
    {
        m_nextaddress = m_program->ExecAddress;
        m_proglabels->AddLabel(m_nextaddress, "START");
    }
    program_size = m_program->GetBufferSize();

    m_codesegments->SetSegmentIndex(m_codesegments->Add(m_nextaddress, m_nextaddress));

    while (processing)
    {
        m_last_prg_counter = m_nextaddress - m_dasmeditems->GetBaseAddress(0);
        m_actualaddress = m_nextaddress;

        de = new DAsmElement(m_program);
        dsm_item = Decode(de, m_last_prg_counter, dsm_item);
        m_nextaddress += de->GetLength();
        counter += de->GetLength();
        m_prg_counter = m_last_prg_counter + de->GetLength();

        m_codesegments->GetSegmentPtr()->segend = m_nextaddress - 1;


        if (de->isInstruction())
        {
            ProcessArgument(de, dsm_item);
            ProcessLoadReg(de);
            update_item = ProcessBranch(de, num_call, processing);

        } // endif

        if ((!processing) && (m_callList->GetCount() != 0))
        {
            #ifdef IDZ80_DECODER
            LogIt("Process remaining calls...\n");
            debugShowJmpList("CALL", m_callList);
            #endif

            m_nextaddress = m_callList->Item(0);
            num_call++;
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

    debugShowList("VAR", m_varlist);
    debugShowList("PROG", m_entrylist);
    debugShowList("IO", m_iolist);
    m_codesegments->Sort();

    return ret;
}



void Decoder::SetCartridgeLabels()
{
	uint address;

	address = m_program->GetCartridgeHeader()->init;
	if (address != 0)
		m_proglabels->AddLabel(address, "INIT");

	address = m_program->GetCartridgeHeader()->statement;
	if (address != 0)
		m_proglabels->AddLabel(address, "STATEMENT");

	address = m_program->GetCartridgeHeader()->device;
	if (address != 0)
		m_proglabels->AddLabel(address, "DEVICE");

	address = m_program->GetCartridgeHeader()->text;
	if (address != 0)
		m_proglabels->AddLabel(address, "TEXT");
}




bool Decoder::OutBoundaryAddress(uint _addr)
{
    return ((_addr > m_endaddress) || (_addr < m_startaddress));
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
    int     i,
            total;

    wxString    str = "List empty !\n";
    SegmentItem *si;

    total = m_codesegments->GetCount();

    if (total > 0)
    {
        str = "decoder.cpp: <CodeSegments> {";
        for(i = 0; i < total; i++)
        {
            si = m_codesegments->GetItem(i);
            str << wxString::Format("(%.4X, %.4X)", si->segstart, si->segend);
            if (i != (total - 1))
                str << " ";
        }
        str << "}\n";
    }
    return str;
}

void Decoder::OptimizeCodeSegment()
{
    m_codesegments->Optimize();
}

