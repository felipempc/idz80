/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe MPC ()
 * License:   GPL
 * This module store one disassembled item
 **************************************************************/



 #include "d_asm_element.h"
 #include "mndb_tools.h"
 #include "IDZ80Base.h"



/*
 * DAsmElement Implementation
 */

DAsmElement::DAsmElement(RawData* rawdata)
{
    FileData = rawdata;
    Clear();
    SetStyleDefault();
    ModuleName = "Dasmelement";
}

DAsmElement::~DAsmElement()
{
    Clear();
}


void DAsmElement::SetStyleDefault()
{
    Style.hasArgumentLabel = 0;
	Style.hasLabel = 0;
	Style.arg1 = ast_bytehex;
	Style.arg2 = ast_bytehex;
	Style.arg1styled = 0;
	Style.arg2styled = 0;
}


uint DAsmElement::getArgument(uint arg, uint _baseaddress)
{
    uint ret = 0;
    unsigned int h, l;
    signed char rel = 0;

    if ((ElType == et_Instruction) && (MnemonicObject != 0))
    {
        if ((MnemonicObject->GetArgumentSize() == 1) && (MnemonicObject->GetArgumentCount() == 1))
            if (MnemonicObject->GetArgumentType(0) == ARG_REL_ADDR)
            {
                rel = (signed char)Args[0];
                ret = convertRelAbs(rel, _baseaddress);
            }
            else
            {
                ret = (int)Args[0] & 0xFF;
            }
        else
        if ((MnemonicObject->GetArgumentSize() == 1) && (MnemonicObject->GetArgumentCount() == 2))
            ret = (int)Args[arg] & 0xFF;
        else
        {
            l = (unsigned int)Args[0] & 0xFF;
            h = (unsigned int)Args[1] & 0xFF;
            ret = (int)(h * 0x100 + l);
        }
    }
    return ret;
}




void DAsmElement::Clear()
{
    MnemonicObject = 0;
    memset(Args, '\0', sizeof(OpCodeArguments));
    memset(Code, '\0', sizeof(ByteCode));
    Offset = Length = 0;
    ElType = et_None;
}




wxString DAsmElement::getCodeStr()
{
    static wxString gcstr;
    uint i;
    byte b;

    gcstr.Clear();
    for (i=0; i < Length; i++)
    {
        b = FileData->GetData(Offset + i);
        gcstr << gcstr.Format("%.2X ", b);
    }
    gcstr.Trim(true);
    return (gcstr);
}


wxString DAsmElement::getAsciiStr()
{
    static wxString gastr;
    uint i, b;

    gastr.Clear();
    for (i=0; i < Length; i++)
    {
        b = uint(FileData->GetData(Offset + i));
        b = b & 0xFF;
        if ((b < 32) || (b > 126))
            b = '.';
        gastr << gastr.Format("%c", b);
    }
    return (gastr);
}




InstructionTypes DAsmElement::GetInstructionType()
{
    if (MnemonicObject != 0)
        return MnemonicObject->GetInstructionType();
    else
        return IT_ERROR;
}



InstructionDetails DAsmElement::GetInstructionDetail()
{
    if (MnemonicObject != 0)
        return MnemonicObject->GetInstructionDetail();
    else
        return II_ERROR;
}




void DAsmElement::CopyOpcode()
{
	uint i;

	for (i = 0; i < MAX_OPCODE_SIZE; i++)
	{
		if (i < MnemonicObject->GetOpCodeSize())
			Code[i] = MnemonicObject->GetOpCode(i);
		else
			Code[i] = 0;
	}
}


bool DAsmElement::CopyArguments()
{
	uint	i,
			totargs;
	bool	ret = false;

	if ((MnemonicObject > 0) && (Offset >= 0))
	{
		totargs = MnemonicObject->GetArgumentCount() * MnemonicObject->GetArgumentSize();
		for (i = 0; i < totargs; i++)
			Args[i] = FileData->GetData(Offset + i + MnemonicObject->GetArgumentPosition());
		ret = true;
	}
	return ret;
}


void DAsmElement::CopyArguments(OpCodeArguments &arg, uint size)
{
	if (size > MAX_NUM_ARG)
		size = MAX_NUM_ARG;

	memcpy(Args, arg, size);
}


void DAsmElement::CopyOpCode(ByteCode &bytecode)
{
	memcpy(Code, bytecode, MAX_OPCODE_SIZE);
}



uint DAsmElement::convertRelAbs(int reladdr, uint _baseaddress)
{
    unsigned int i;
    i = (unsigned int)(_baseaddress + MnemonicObject->GetOpCodeSize() + Offset + reladdr);
    return i;
}


void DAsmElement::SetOffset(uint _offset)
{
	Offset = _offset;
}

void DAsmElement::SetLength(uint _length)
{
	Length = _length;
}


void DAsmElement::SetLength()
{
	if (MnemonicObject > 0)
		Length = MnemonicObject->GetOpCodeSize();
	else
		Length = 0;
}


void DAsmElement::SetType(ElementType _etype)
{
	ElType = _etype;
}

void DAsmElement::SetStyle(ArgStyle &_style)
{
    Style.hasArgumentLabel = _style.hasArgumentLabel;
    Style.hasLabel = _style.hasLabel;
    Style.arg1 = _style.arg1;
    Style.arg2 = _style.arg2;
    Style.arg1styled = _style.arg1styled;
    Style.arg2styled = _style.arg2styled;
}

void DAsmElement::SetStyleArgument(uint arg_idx, StyleType ast)
{
	if (arg_idx == 0)
		Style.arg1 = ast;
	if (arg_idx == 1)
		Style.arg2 = ast;
}

ArgStyle DAsmElement::GetStyle()
{
	return Style;
}


StyleType DAsmElement::GetStyleArgument(uint arg_idx)
{
	StyleType ret = ast_bytehex;
	if (arg_idx == 0)
		ret = Style.arg1;
	if (arg_idx == 1)
		ret = Style.arg2;
	return ret;
}


void DAsmElement::SetMnemonic(MnemonicItem *mnemonic)
{
	MnemonicObject = mnemonic;
	if (mnemonic > 0)
		CopyOpcode();
}

byte DAsmElement::GetData(uint offset)
{
    return FileData->GetData(offset);
}


uint DAsmElement::GetOffset()
{
	return Offset;
}

uint DAsmElement::GetLength()
{
	return Length;
}

ElementType DAsmElement::GetType()
{
	return ElType;
}


ArgumentTypes DAsmElement::GetArgumentType(uint arg_num)
{
	if (MnemonicObject > 0)
		return MnemonicObject->GetArgumentType(arg_num);
	return ARG_NONE;
}

int DAsmElement::GetNumArgs()
{
	if (MnemonicObject > 0)
		return MnemonicObject->GetArgumentCount();
	return -1;
}


int DAsmElement::GetArgSize()
{
	return MnemonicObject->GetArgumentSize();
}

BranchType DAsmElement::GetBranchType()
{
	if (MnemonicObject > 0)
		return MnemonicObject->GetBranchType();
	return BR_NONE;
}


wxString &DAsmElement::GetMnemonicStr(uint index)
{
	static wxString ret;
	ret = "Error !";

	if ((MnemonicObject != 0) && (index < MnemonicObject->mnemonic_strings_.GetCount()))
		return MnemonicObject->mnemonic_strings_[index];
	return ret;
}


uint DAsmElement::GetMnemonicStrNum()
{
	return MnemonicObject->mnemonic_strings_.GetCount();
}



byte DAsmElement::GetCodeItem(uint index)
{
	byte ret = 0;

	if (index < MAX_OPCODE_SIZE)
		ret = Code[index];
	return ret;
}


byte DAsmElement::GetArgItem(uint index)
{
	byte ret = 0;

	if (index < 2)
		ret = Args[index];
	return ret;
}



bool DAsmElement::HasArgumentLabel()
{
	return (Style.hasArgumentLabel == 1);
}



void DAsmElement::SetArgLabel(bool hal)
{
    if (hal)
        Style.hasArgumentLabel = 1;
    else
        Style.hasArgumentLabel = 0;
}




bool DAsmElement::isData()
{
    return (ElType == et_Data);
}

bool DAsmElement::isInstruction()
{
    return (ElType == et_Instruction);
}


bool DAsmElement::isArgumentProgramAddress()
{
    ArgumentTypes argtype = GetArgumentType(0);

    return ((argtype == ARG_ABS_ADDR) || (argtype == ARG_REL_ADDR));
}


bool DAsmElement::isArgumentVariableAddress()
{
    ArgumentTypes argtype = GetArgumentType(0);

    return (argtype == ARG_VARIABLE);
}

