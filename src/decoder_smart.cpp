/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   2012-05-15
 * Copyright: Felipe MPC ()
 * License:   GPL
 * This module disassemble in smart mode.
 **************************************************************/



#include "decoder_smart.h"



SmartDecoder::SmartDecoder(ProcessBase *parent, LogWindow *logparent)
                : Decoder(parent, logparent)
{
    start_address_ = 0;
    end_address_ = 0;
    exec_address_ = 0;


    address_list_ = new SortedIntArray(CompareSortedInt);
    address_list_processed_ = new SortedIntArray(CompareSortedInt);
    sub_routine_ = new SubRoutineCtrl(logparent);

}

SmartDecoder::~SmartDecoder()
{
    delete sub_routine_;

    if (address_list_)
        delete address_list_;
    if (address_list_processed_)
        delete address_list_processed_;
}




bool SmartDecoder::FullDisassemble(LabelManager *parent)
{
    bool    ret = false,
            processing = true,
            update_item = false;

    int     dsm_item;

    uint    program_size,
            counter = 0,
            address;

    DisassembledItem     *de;
    SortedIntArray  CartridgeCalls(CompareSortedInt);


    labels_ = parent;
    labels_->ClearUserLabels();
    sub_routine_->Clear();
    UpdateBoundaries();

    address_list_->Clear();
    address_list_processed_->Clear();


	if (process_->Program->isCartridge())
	{
		SetCartridgeLabels();
	    process_->Program->GetEntries(CartridgeCalls);
	    next_address_ = CartridgeCalls.Index(0);
	}
	else
    {
        next_address_ = process_->Program->ExecAddress;
        labels_->prog_labels->AddLabel(next_address_, "START");
    }
    program_size = process_->Program->GetBufferSize();
    dsm_item = program_size;
    while (processing)
    {
        last_prg_counter_ = next_address_ - process_->Disassembled->GetBaseAddress(0);
        actual_address_ = next_address_;

        if (address_list_->Index(actual_address_) != wxNOT_FOUND)
        {
            address_list_->Remove(actual_address_);
            #ifdef IDZ80_DECODER
            LogIt(wxString::Format("[0x%.4X] Removed this address from list !", actual_address_));
            #endif
        }

        if (address_list_processed_->Index(next_address_) == wxNOT_FOUND)
            address_list_processed_->Add(actual_address_);

        de = new DisassembledItem(process_->Program);
        dsm_item = Decode(de, last_prg_counter_, dsm_item);
        next_address_ += de->GetLength();
        counter += de->GetLength();
        prg_counter_ = last_prg_counter_ + de->GetLength();

        address = de->GetArgument(0, process_->Disassembled->GetBaseAddress(0));

        SetupArgumentLabels(de, dsm_item);

        switch (de->GetInstructionType())
        {
        case IT_CALL:
            update_item = CallSubroutine(de);
            #ifdef IDZ80_DECODER
            if (update_item)
            {
                LogIt(wxString::Format("[0x%.4X] Entering subroutine No. %d  [0x%.4X].", actual_address_, sub_routine_->GetCounter(), address));
            }
            #endif
            break;

        case IT_RET:
            update_item = ReturnSubroutine(de, next_address_);
            #ifdef IDZ80_DECODER
            if (update_item)
            {
                LogIt(wxString::Format("[0x%.4X] Returning from subroutine No. %d.", actual_address_, (sub_routine_->GetCounter() + 1)));
            }
            #endif
            break;

        case IT_JUMP:
            update_item = ProcessBranch(de, processing);
            break;

        case IT_LOAD:
            registers_.LoadRegister(de);
            break;

        case IT_RST:
            if (MSXWeirdRST(de, (dsm_item + 1)))
            {
                update_item = true;
                next_address_ += 3;
            }
            break;
        case IT_ERROR:
            LogIt(wxString::Format("[0x%.4X] Error: opcode = %s, dasmitem = %d", actual_address_, de->GetCodeStr(), dsm_item));
            break;
        }

        if ((!processing) && (CartridgeCalls.GetCount() != 0))
        {
            #ifdef IDZ80_DECODER
            LogIt("Process remaining calls...");
            DebugShowJmpList("CALL", &CartridgeCalls);
            #endif

            next_address_ = CartridgeCalls.Item(0);
            CartridgeCalls.Remove(next_address_);
            update_item = true;
            processing = true;
        }

        if (update_item)
        {
            dsm_item = process_->Disassembled->FindAddress(next_address_);
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






/*
 *  Process CALLs and JUMPS, return
 * true if dsm_item (FirstDisassemble) should be updated
 */
bool SmartDecoder::ProcessBranch(DisassembledItem *de, bool &processing_status)
{
    uint    address,
            tempaddress;
    bool    update_dasm_item = false;

    address = de->GetArgument(0, process_->Disassembled->GetBaseAddress(0));

    if (de->mnemonic_->IsConditionalJump())
    {
        if (!TestIfOutBoundaries(address))
        {
            if ((address_list_->Index(address) == wxNOT_FOUND) &&
                (address > actual_address_))
                address_list_->Add(address);
        }
    }
    else
    {
        if (TestIfOutBoundaries(address))
        {
            if (sub_routine_->IsInside())
            {
                if (ReturnSubroutine(de, next_address_))
                {
                    update_dasm_item = true;
                    #ifdef IDZ80_DECODER
                    LogIt(wxString::Format("[0x%.4X] Jump to out of boundary [0x%.4X] inside a sub-routine. Returning to 0x%.4X !", actual_address_, address, next_address_));
                    #endif
                }
            }
            else
            {
                #ifdef IDZ80_DECODER
                LogIt(wxString::Format("[0x%.4X] Jump to out of boundary [0x%.4X]. Should finish processing ?", actual_address_, address));
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
            if ((address >= start_address_) && (address < exec_address_))
            {
                LogIt(wxString::Format("[0x%.4X] Address between start and execution [0x%.4X].", actual_address_, address));
            }
            #endif

            if ((address >= exec_address_) && (address <= actual_address_))
            {
                #ifdef IDZ80_DECODER
                LogIt(wxString::Format("[0x%.4X] Loop detected to 0x%.4X.", actual_address_, address));
                DebugShowJmpList("         CONDITIONAL Back Test", address_list_);
                #endif

                if (GetNextNearJump(address_list_, next_address_, end_address_, next_address_))
                {
                    update_dasm_item = true;
                }
                else
                {
                    if ((sub_routine_->HasConditionalReturn()) && (sub_routine_->IsInside()) &&
                        (ReturnSubroutine(de, next_address_)))
                    {
                        update_dasm_item = true;
                        #ifdef IDZ80_DECODER
                        LogIt(wxString::Format("[0x%.4X] Conditional return to 0x%.4X.", actual_address_, next_address_));
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

                if ((next_address_ > end_address_) && (processing_status))
                {
                    #ifdef IDZ80_DECODER
                    LogIt(wxString::Format("Filtered nextddress [0x%.4X].", next_address_));
                    #endif
                }
            }

            if (address > actual_address_)
            {
                #ifdef IDZ80_DECODER
                LogIt(wxString::Format("[0x%.4X] Jumping forward to 0x%.4X", actual_address_, address));
                DebugShowJmpList("         CONDITIONAL Forward test", address_list_);
                #endif
                if (GetNextNearJump(address_list_, next_address_, address, tempaddress))
                {
                    if (address_list_->Index(address) == wxNOT_FOUND)
                        address_list_->Add(address);
                    #ifdef IDZ80_DECODER
                    LogIt(wxString::Format("[0x%.4X] Save forward address to process later.", actual_address_));
                    #endif
                }
                else
                {
                    #ifdef IDZ80_DECODER
                    DebugShowJmpList("Processed List", address_list_processed_);
                    #endif
                    if (address_list_processed_->Index(address) == wxNOT_FOUND)
                    {
                        tempaddress = address;
                        update_dasm_item = true;
                        #ifdef IDZ80_DECODER
                        LogIt("Proceed forward.");
                        #endif
                    }
                    else
                    {
                        if (address_list_->IsEmpty())
                        {
                            processing_status = false;
                            #ifdef IDZ80_DECODER
                            LogIt("No more entries. End processing.");
                            #endif
                        }
                        else
                        {
                            tempaddress = address_list_->Item(0);
                            update_dasm_item = true;
                            #ifdef IDZ80_DECODER
                            LogIt(wxString::Format("Address 0x%.4X already processed. Jumping to 0x%.4X.", address, tempaddress));
                            #endif
                        }
                    }
                }

                next_address_ = tempaddress;
            }
        }
    }

    return update_dasm_item;
}





bool SmartDecoder::ReturnSubroutine(DisassembledItem *de, ProgramAddress &dest_address)
{
    bool ret = false;

    if (de->mnemonic_->IsConditionalReturn())
        sub_routine_->SetConditionalReturn();
    else
    {
        dest_address = sub_routine_->Return(actual_address_);
        if (dest_address > 0)
            ret = true;
    }

    return ret;
}



bool SmartDecoder::CallSubroutine(DisassembledItem *de)
{
    bool ret = false;
    uint address = de->GetArgument(0, process_->Disassembled->GetBaseAddress(0));

    if (TestIfOutBoundaries(address))
    {
        LogIt(wxString::Format("[0x%.4X] Subroutine out of boundaries [0x%.4X].", actual_address_, address));
        MSXCheckFunctionRegisters(de);
    }
    else
    {
        if (sub_routine_->AlreadyCalled(address))
        {
            LogIt(wxString::Format("[0x%.4X] Subroutine already called [0x%.4X].", actual_address_, address));
        }
        else
        {
            ret = sub_routine_->Call(address, next_address_);
            if (ret)
                next_address_ = address;
        }
    }
    return ret;
}





bool SmartDecoder::GetNextNearJump(SortedIntArray *jmplist, ProgramAddress _start, ProgramAddress _end, ProgramAddress &nextaddr)
{
    bool    ret = false;
    int     i = 0,
            f;
    ProgramAddress  address;

    f = jmplist->GetCount();
    #ifdef IDZ80_DECODER
    LogIt(wxString::Format("[0x%.4X] Searching for entries between 0x%.4X and 0x%.4X.", actual_address_, _start, _end));
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
    return ret;
}




void SmartDecoder::FillData()
{
	uint    i,
			delta,
			count,
			index;
	DisassembledItem    *de_1,
                        *de_2;
	bool    processing;

	processing = (process_->Disassembled->GetCount() > 1);
	i = 0;

	while (processing)
	{
		de_1 = process_->Disassembled->GetData(i);
		de_2 = process_->Disassembled->GetData(i + 1);
		index = (de_1->GetOffset() + de_1->GetLength());
		delta = de_2->GetOffset() - index;
		for(count = 0; count < delta; count++)
		{
			de_1 = new DisassembledItem(process_->Program);
			de_1->SetLength(1);
			de_1->SetOffset(index + count);
			de_1->SetMnemonic(0);
			de_1->SetType(et_Data);
			process_->Disassembled->InsertDasm(de_1, i + 1);
			i++;
		}
		i++;
		if (i > (process_->Disassembled->GetCount() - 2))
			processing = false;
	}

	i = de_2->GetOffset() + de_2->GetLength();

	for(count = i; count < process_->Program->GetBufferSize(); count++)
	{
		de_1 = new DisassembledItem(process_->Program);
		de_1->SetLength(1);
		de_1->SetOffset(count);
		de_1->SetMnemonic(0);
		de_1->SetType(et_Data);
		process_->Disassembled->AddDasm(de_1);
	}
}




//TODO: REwrite it
void SmartDecoder::UpdateBoundaries()
{
    if ((process_->Program->EndAddress > process_->Program->StartAddress) &&
        (process_->Program->ExecAddress >= process_->Program->StartAddress) &&
        (process_->Program->ExecAddress < process_->Program->EndAddress))
    {
        start_address_ = process_->Program->StartAddress;
        end_address_ = process_->Program->EndAddress;
        exec_address_ = process_->Program->ExecAddress;
    }
}



bool SmartDecoder::TestIfOutBoundaries(ProgramAddress addr)
{
    return ((addr > end_address_) || (addr < start_address_));
}






void SmartDecoder::Clear()
{
    address_list_->Clear();
    address_list_processed_->Clear();
    sub_routine_->Clear();
}







void SmartDecoder::DebugShowList(const wxString &listname, SortedIntArray *_list)
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



