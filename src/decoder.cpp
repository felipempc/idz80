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




const uint Decoder::OPCODE_NOT_FOUND;

Decoder::Decoder(ProcessBase *parent, LogWindow *logparent)
{
    process_ = parent;
    SetTextLog(logparent);
    ModuleName = "DECODER";

}

/*
Decoder::~Decoder()
{

}
*/

/*
void Decoder::Clear()
{
    AddressList->Clear();
    AddressListProcessed->Clear();
    SubRoutine->Clear();
}
*/





/*
 * Search for an opcode pattern (from Program)
 * in mndb.
 */
uint Decoder::Fetch(const FileOffset prg_index, uint maxitems)
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
        scan = process_->Program->GetData(prg_index + offset);
        process_->Mnemonics->Find(foundItems, scan, offset);
        offset++;
        if (foundItems.GetCount() < 2)
			break;
    }
    nitems = foundItems.GetCount();
    if (nitems == 1)
    {
        ret = foundItems.Last();
        mnemonic = process_->Mnemonics->GetData(ret);
        if (mnemonic->GetOpCodeSize() > maxitems)
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
uint Decoder::Decode(DisassembledItem *de, FileOffset prg_index, DisassembledIndex dasm_position)
{
    uint     mnc_item,
            ret = -1;
    uint    scan_limit;
    DisassembledItem     *next_de;



    // ensure it won't overwrite existing dasmitems
    next_de = process_->Disassembled->GetData(dasm_position);
    if (next_de == 0)
        scan_limit = process_->Program->GetBufferSize();
    else
        scan_limit = next_de->GetOffset() - prg_index;

    mnc_item = Fetch(prg_index, scan_limit);

    if (mnc_item == OPCODE_NOT_FOUND)
    {
        de->SetLength(1);
        de->SetOffset(prg_index);
        de->SetMnemonic(0);
        de->SetType(et_Data);
        ret = process_->Disassembled->InsertDasm(de, dasm_position);
    }
    else
    {
        de->SetMnemonic(process_->Mnemonics->GetData(mnc_item));
        de->SetLength();
        de->SetType(et_Instruction);
        de->SetOffset(prg_index);
        if (de->CopyArguments())
			ret = process_->Disassembled->InsertDasm(de, dasm_position);
    }

    return ret;
}



/*
 *  Fill label list
 */
void Decoder::SetupArgumentLabels(DisassembledItem *de, DisassembledIndex index)
{
    enum ArgumentTypes	argtype;
    uint            argument;
    wxString		str;

	argtype = de->GetArgumentType(0);
	switch (argtype)
	{
		case ARG_VARIABLE:
                            argument = de->GetArgument(0, 0);
							str = labels_->sys_vars->Find(argument);
							labels_->var_labels->AddLabel(argument, str, index);
							de->SetArgLabel();
							break;
		case ARG_ABS_ADDR:
		case ARG_REL_ADDR:
                            argument = de->GetArgument(0, process_->Disassembled->GetBaseAddress(index));
							str = labels_->sys_calls->Find(argument);
							labels_->prog_labels->AddLabel(argument, str, index);
							de->SetArgLabel();
							break;
		case ARG_IO_ADDR:
                            argument = de->GetArgument(0, 0);
							str = labels_->sys_io->Find(argument);
							labels_->io_labels->AddLabel(argument, str, index);
							de->SetArgLabel();
							break;
        case ARG_NONE:
        case ARG_LITERAL:
        case ARG_OFFSET:
                            break;
	}
}









void Decoder::MSXCheckFunctionRegisters(DisassembledItem *de)
{
    switch(de->GetArgument(0, 0))
    {
    case 5:
        if (registers_.C->isLoaded())
        {
            wxString str = labels_->sys_const->Find(registers_.C->GetValue());
            if (!str.IsEmpty())
                labels_->constant_labels->AddLabel(registers_.C->GetValue(), str);
        }

        break;
    }
}





bool Decoder::MSXWeirdRST(DisassembledItem *de, DisassembledIndex dasm_position)
{
    uint offset;
    bool ret = false;

    offset = de->GetOffset();

    switch (de->GetInstructionDetail())
    {
        case II_RST_08:
                    //LogIt(wxString::Format("[0x%.4X] Rst 08h detected !", m_actualaddress));
                    break;

        case II_RST_30:
                    // ID of Slot
                    de = new DisassembledItem(process_->Program);
                    de->SetLength(1);
                    de->SetOffset(++offset);
                    de->SetMnemonic(0);
                    de->SetType(et_Data);
                    process_->Disassembled->InsertDasm(de, dasm_position++);

                    // address to be called
                    de = new DisassembledItem(process_->Program);
                    de->SetLength(2);
                    de->SetOffset(++offset);
                    de->SetMnemonic(0);
                    de->SetType(et_Data);
                    de->SetStyleArgument(0, ast_wordhex);
                    process_->Disassembled->InsertDasm(de, dasm_position);
                    ret = true;
                    break;
    }
    return ret;
}








void Decoder::SetCartridgeLabels()
{
	uint address;

	address = process_->Program->GetCartridgeHeader()->init;
	if (address != 0)
		labels_->prog_labels->AddLabel(address, "INIT");

	address = process_->Program->GetCartridgeHeader()->statement;
	if (address != 0)
		labels_->prog_labels->AddLabel(address, "STATEMENT");

	address = process_->Program->GetCartridgeHeader()->device;
	if (address != 0)
		labels_->prog_labels->AddLabel(address, "DEVICE");

	address = process_->Program->GetCartridgeHeader()->text;
	if (address != 0)
		labels_->prog_labels->AddLabel(address, "TEXT");
}








void Decoder::FullDisassemble(LabelManager *parent)
{
    uint i, f, item;
    DisassembledItem *de;

    labels_ = parent;
    labels_->ClearUserLabels();
    //UpdateBoundary();

    if (process_->Program->isCartridge())
        SetCartridgeLabels();

    f = process_->Program->GetBufferSize();
    for (i = 0; i < f; i++)
    {
        de = new DisassembledItem(process_->Program);
        item = Decode(de, i);
        SetupArgumentLabels(de, item);
        registers_.LoadRegister(de);
        if (MSXWeirdRST(de, f))
            i += 3;
        i += (de->GetLength() - 1);
    }
}



void Decoder::DisassembleItems(RangeItems &dasm_range)
{
    uint 		i,
				dasm_last,
				x,
				program_first,
				program_last;
    DisassembledItem	*de;


    dasm_last = dasm_range.Index + dasm_range.Count - 1;
    x = process_->Disassembled->GetCount();

    if ((dasm_range.Index < x) || (dasm_last < x))
    {
        de = process_->Disassembled->GetData(dasm_range.Index);
        program_first = de->GetOffset();
        de = process_->Disassembled->GetData(dasm_range.Index + dasm_range.Count - 1);
        program_last = de->GetOffset() + de->GetLength();

        LogIt(wxString::Format("Deleting index = %d, count = %d.", dasm_range.Index, dasm_range.Count));
        process_->Disassembled->DelDasm(dasm_range.Index, dasm_range.Count);
        x = dasm_range.Index;
        dasm_range.Count = 0;
        for (i = program_first; i < program_last; i++)
        {
            de = new DisassembledItem(process_->Program);
            Decode(de, i, x++);
            i += (de->GetLength() - 1);
            dasm_range.Count++;
        }
    }
}





