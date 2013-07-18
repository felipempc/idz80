/****************************************************************
* Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Represents one disassembled item
 **************************************************************/


 #include "disassembled_item.h"
 #include "mndb_tools.h"
 #include "IDZ80Base.h"



DisassembledItem::DisassembledItem(RawData* rawdata)
{
    data_file_ = rawdata;
    Clear();
    SetStyleDefault();
    ModuleName = "Dasmelement";
}

DisassembledItem::~DisassembledItem()
{
    Clear();
}


void DisassembledItem::SetStyleDefault()
{
    arguments_style_.hasArgumentLabel = 0;
	arguments_style_.hasLabel = 0;
	arguments_style_.arg1 = ast_bytehex;
	arguments_style_.arg2 = ast_bytehex;
	arguments_style_.arg1styled = 0;
	arguments_style_.arg2styled = 0;
}


word DisassembledItem::GetArgument(uint argument_index, uint baseaddress)
{
    uint ret = 0;
    word h, l;
    signed char relative_address = 0;

    if ((item_type_ == et_Instruction) && mnemonic_)
    {
        if ((mnemonic_->GetArgumentSize() == 1) && (mnemonic_->GetArgumentCount() == 1))
            if (mnemonic_->GetArgumentType(0) == ARG_REL_ADDR)
            {
                relative_address = static_cast<signed char>(opcode_arguments_[0]);
                ret = ConvertRelativeToAbsoluteAddress(relative_address, baseaddress);
            }
            else
            {
                ret = static_cast<uint>(opcode_arguments_[0]) & 0xFF;
            }
        else
        if ((mnemonic_->GetArgumentSize() == 1) && (mnemonic_->GetArgumentCount() == 2))
            ret = static_cast<word>(opcode_arguments_[argument_index]) & 0xFF;
        else
        {
            l = static_cast<word>(opcode_arguments_[0]) & 0xFF;
            h = static_cast<word>(opcode_arguments_[1]) & 0xFF;
            ret = static_cast<word>(h * 0x100 + l);
        }
    }
    return ret;
}




void DisassembledItem::Clear()
{
    mnemonic_ = 0;
    memset(opcode_arguments_, '\0', sizeof(OpCodeArguments));
    memset(opcode_, '\0', sizeof(ByteCode));
    offset_in_file_ = opcode_size_ = 0;
    item_type_ = et_None;
}




wxString DisassembledItem::GetCodeStr()
{
    static wxString gcstr;
    uint i;
    byte b;

    gcstr.Clear();
    for (i = 0; i < opcode_size_; i++)
    {
        b = data_file_->GetData(offset_in_file_ + i);
        gcstr << gcstr.Format("%.2X ", b);
    }
    gcstr.Trim(true);
    return (gcstr);
}


wxString DisassembledItem::GetAsciiStr()
{
    static wxString gastr;
    uint i, b;

    gastr.Clear();
    for (i = 0; i < opcode_size_; i++)
    {
        b = static_cast<uint>(data_file_->GetData(offset_in_file_ + i));
        b = b & 0xFF;
        if ((b < 32) || (b > 126))
            b = '.';
        gastr << gastr.Format("%c", b);
    }
    return (gastr);
}




InstructionTypes DisassembledItem::GetInstructionType()
{
    if (mnemonic_)
        return mnemonic_->GetInstructionType();
    else
        return IT_ERROR;
}



InstructionDetails DisassembledItem::GetInstructionDetail()
{
    if (mnemonic_)
        return mnemonic_->GetInstructionDetail();
    else
        return II_ERROR;
}




void DisassembledItem::CopyOpcode()
{
	uint i;

	for (i = 0; i < MAX_OPCODE_SIZE; i++)
	{
		if (i < mnemonic_->GetOpCodeSize())
			opcode_[i] = mnemonic_->GetOpCode(i);
		else
			opcode_[i] = 0;
	}
}


bool DisassembledItem::CopyArguments()
{
	uint	i,
			totargs;
	bool	ret = false;

	if ((mnemonic_ > 0) && (offset_in_file_ >= 0))
	{
		totargs = mnemonic_->GetArgumentCount() * mnemonic_->GetArgumentSize();
		for (i = 0; i < totargs; i++)
			opcode_arguments_[i] = data_file_->GetData(offset_in_file_ + i + mnemonic_->GetArgumentPosition());
		ret = true;
	}
	return ret;
}


void DisassembledItem::CopyArguments(OpCodeArguments &arg, uint size)
{
	if (size > MAX_NUM_ARG)
		size = MAX_NUM_ARG;

	memcpy(opcode_arguments_, arg, size);
}


void DisassembledItem::CopyOpCode(ByteCode &bytecode)
{
	memcpy(opcode_, bytecode, MAX_OPCODE_SIZE);
}



ProgramAddress DisassembledItem::ConvertRelativeToAbsoluteAddress(int reladdr, ProgramAddress baseaddress)
{
    return static_cast<ProgramAddress>(baseaddress + mnemonic_->GetOpCodeSize() + offset_in_file_ + reladdr);
}


void DisassembledItem::SetOffset(uint _offset)
{
	offset_in_file_ = _offset;
}

void DisassembledItem::SetLength(uint _length)
{
	opcode_size_ = _length;
}


void DisassembledItem::SetLength()
{
	if (mnemonic_ > 0)
		opcode_size_ = mnemonic_->GetOpCodeSize();
	else
		opcode_size_ = 0;
}


void DisassembledItem::SetType(ElementType _etype)
{
	item_type_ = _etype;
}

void DisassembledItem::SetStyle(ArgStyle &_style)
{
    arguments_style_.hasArgumentLabel = _style.hasArgumentLabel;
    arguments_style_.hasLabel = _style.hasLabel;
    arguments_style_.arg1 = _style.arg1;
    arguments_style_.arg2 = _style.arg2;
    arguments_style_.arg1styled = _style.arg1styled;
    arguments_style_.arg2styled = _style.arg2styled;
}

void DisassembledItem::SetStyleArgument(uint argument_index, StyleType ast)
{
	if (argument_index == 0)
		arguments_style_.arg1 = ast;
	if (argument_index == 1)
		arguments_style_.arg2 = ast;
}

ArgStyle DisassembledItem::GetStyle()
{
	return arguments_style_;
}


StyleType DisassembledItem::GetStyleArgument(uint argument_index)
{
	StyleType ret = ast_bytehex;
	if (argument_index == 0)
		ret = arguments_style_.arg1;
	if (argument_index == 1)
		ret = arguments_style_.arg2;
	return ret;
}


void DisassembledItem::SetMnemonic(MnemonicItem *mnemonic)
{
	mnemonic_ = mnemonic;
	if (mnemonic > 0)
		CopyOpcode();
}

byte DisassembledItem::GetData(uint offset)
{
    return data_file_->GetData(offset);
}


uint DisassembledItem::GetOffset()
{
	return offset_in_file_;
}

uint DisassembledItem::GetLength()
{
	return opcode_size_;
}

ElementType DisassembledItem::GetType()
{
	return item_type_;
}


ArgumentTypes DisassembledItem::GetArgumentType(uint argument_index)
{
	if (mnemonic_ > 0)
		return mnemonic_->GetArgumentType(argument_index);
	return ARG_NONE;
}

int DisassembledItem::GetArgumentCount()
{
	if (mnemonic_ > 0)
		return mnemonic_->GetArgumentCount();
	return -1;
}


int DisassembledItem::GetArgumentSize()
{
	return mnemonic_->GetArgumentSize();
}

BranchType DisassembledItem::GetBranchType()
{
	if (mnemonic_ > 0)
		return mnemonic_->GetBranchType();
	return BR_NONE;
}


wxString &DisassembledItem::GetMnemonicStr(uint index)
{
	static wxString ret;
	ret = "Error !";

	if (mnemonic_ && (index < mnemonic_->mnemonic_strings_.GetCount()))
		return mnemonic_->mnemonic_strings_[index];
	return ret;
}


uint DisassembledItem::GetMnemonicStrCount()
{
	return mnemonic_->mnemonic_strings_.GetCount();
}



byte DisassembledItem::GetByteOpCode(uint opcode_index)
{
	byte ret = 0;

	if (opcode_index < MAX_OPCODE_SIZE)
		ret = opcode_[opcode_index];
	return ret;
}


byte DisassembledItem::GetByteArgument(uint argument_index)
{
	byte ret = 0;

	if (argument_index < 2)
		ret = opcode_arguments_[argument_index];
	return ret;
}



bool DisassembledItem::HasArgumentLabel()
{
	return (arguments_style_.hasArgumentLabel == 1);
}



void DisassembledItem::SetArgLabel(bool hal)
{
    if (hal)
        arguments_style_.hasArgumentLabel = 1;
    else
        arguments_style_.hasArgumentLabel = 0;
}




bool DisassembledItem::IsData()
{
    return (item_type_ == et_Data);
}

bool DisassembledItem::IsInstruction()
{
    return (item_type_ == et_Instruction);
}


bool DisassembledItem::IsArgumentProgramAddress()
{
    ArgumentTypes argtype = GetArgumentType(0);

    return ((argtype == ARG_ABS_ADDR) || (argtype == ARG_REL_ADDR));
}


bool DisassembledItem::IsArgumentVariableAddress()
{
    ArgumentTypes argtype = GetArgumentType(0);

    return (argtype == ARG_VARIABLE);
}

