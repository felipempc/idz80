/****************************************************************
 * Name:      IDZ80
 * Purpose:   Defines Application Frame
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 **************************************************************/



/*
 * Holds one disassembled element
 *
 */

 #include "d_asm_element.h"
 #include "mndb_tools.h"
 #include "IDZ80Base.h"



/*
 * DAsmElement Implementation
 */

DAsmElement::DAsmElement(RawData* rawdata, uint *baseaddress)
{
    FileData=rawdata;
    if (baseaddress==0)
        *BaseAddress=0;
    else
        BaseAddress=baseaddress;
    Clear();
}

DAsmElement::~DAsmElement()
{
    Clear();
}



uint DAsmElement::getArgument(uint arg)
{
    uint ret=0;
    unsigned int h,l;
    signed char rel=0;
    if ((ElType == et_Instruction) && (!(MnItem==0)))
    {
        if ((MnItem->getArgSize()==1) && (MnItem->getArgNo()==1))
            if (MnItem->getArgType(0)==ARG_REL_ADDR)
            {
                rel=(signed char)Args[0];
                ret=convertRelAbs(rel);
            }
            else
            {
                ret=(int)Args[0] & 0xFF;
            }
        else
        if ((MnItem->getArgSize()==1) && (MnItem->getArgNo()==2))
            ret=(int)Args[arg] & 0xFF;
        else
        {
            l=(unsigned int)Args[0] & 0xFF;
            h=(unsigned int)Args[1] & 0xFF;
            ret=(int)(h*0x100+l);
        }
    }
    return ret;
}




void DAsmElement::Clear()
{
    MnItem=0;
    memset(Args,'\0',sizeof(OpCodeArguments));
    Offset=Length=0;
    ElType=et_None;
}




wxString DAsmElement::getCodeStr()
{
    static wxString gcstr;
    uint i;
    byte b;

    gcstr.Clear();
    for (i=0;i<Length;i++)
    {
        b=FileData->GetData(Offset+i);
        gcstr << gcstr.Format(_T("%.2X "),b);
    }
    gcstr.Trim(true);
    return (gcstr);
}


wxString DAsmElement::getAsciiStr()
{
    static wxString gastr;
    uint i,b;

    gastr.Clear();
    for (i=0;i<Length;i++)
    {
        b=uint(FileData->GetData(Offset+i));
        b=b & 0xFF;
        if ((b<32) || (b>126))
            b='.';
        gastr << gastr.Format(_T("%c"),b);
    }
    return (gastr);
}



uint DAsmElement::convertRelAbs(int reladdr)
{
    unsigned int i;
    i=(unsigned int)(*BaseAddress + MnItem->getBytesNo() + Offset + reladdr);
    return i;
}


byte DAsmElement::GetData(uint offset)
{
    return FileData->GetData(offset);
}
