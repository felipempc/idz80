#include "codegenerator.h"

codeGenerator::codeGenerator(ProcessData *pd)
{
    process = pd;
    end_line = _T("\r\n");
    end_line.Shrink();
    m_cflags = cfM80;
}

codeGenerator::~codeGenerator()
{
    //dtor
}


bool codeGenerator::isNumber(wxChar firstchar)
{
    bool ret;
    ret = (firstchar >= '0') && (firstchar <= '9');
    return ret;
}


wxString codeGenerator::generateTextData(CodeViewItem *cvi)
{
    uint i;
    DAsmElement *de;
    wxString str, str_number_format;

    if (m_cflags == cfM80)
        str_number_format = _T("%.2XH");
    else
        str_number_format = _T("0x%.2X");

    de = process->m_Dasm->GetData(cvi->Dasmitem);
    str.Printf(_T("DB "));
    for (i=0; i < de->Length; i++)
    {
        str << wxString::Format(str_number_format,de->GetData(de->Offset+i));
        if (m_cflags == cfM80)
            //FIXME: Baiano BUG, with more than one item it will crash
            if (!isNumber(str[0]))
                str.Prepend(_T("0"));

        if (i < (de->Length-1))
            str << _T(",");
    }
    return str;
}



wxString codeGenerator::generateLabels(CodeViewItem *cvi)
{
    wxString str_ret,str;
    uint i;

    for (i = 0; i < process->io_labels->GetCount(); i++)
    {
        str = process->io_labels->GetAddress(i);
        if (!isNumber(str[0]))
            str.Prepend(_T("0"));

        if (m_cflags == cfM80)
            str << _T("H");
        else
            str.Prepend(_T("0x"));

        str_ret << process->io_labels->GetLabel(i) << _T("  EQU  ") << str << end_line;
    }
    return str_ret;

}


wxString codeGenerator::generateInstruction(CodeViewItem *cvi)
{
    int nargs,
        argsz,
        strparts;

    wxString    str_1, str_2, str_ret,
                str_number_format;
    bool usedlabel;
    DAsmElement *de;

    usedlabel=false;
    de = process->m_Dasm->GetData(cvi->Dasmitem);
    nargs = de->MnItem->getArgNo();
    argsz = de->MnItem->getArgSize();

    str_ret = de->MnItem->MnemonicString[0];
    strparts = 1;

    str_1.Clear();
    str_2.Clear();

    if (m_cflags == cfM80)
        str_number_format = _T("%XH");
    else
        str_number_format = _T("0x%X");

    if (de->Style.hasArgumentLabel)
    {
        switch (de->MnItem->getArgType(0))
        {
            case ARG_REL_ADDR:
            case ARG_ABS_ADDR:
                            usedlabel = process->prog_labels->GetLabel(de->getArgument(0),str_1);
                            break;
            case ARG_IO_ADDR:
                            usedlabel = process->io_labels->GetLabel(de->getArgument(0),str_1);
                            break;
            case ARG_VARIABLE:
                            usedlabel = process->var_labels->GetLabel(de->getArgument(0),str_1);
                            break;
            case ARG_NONE:
            case ARG_LITERAL:
            case ARG_OFFSET:
                            break;
        }
    }

    if ((nargs == 1) && (!usedlabel))
    {
        str_1.Printf(str_number_format,de->getArgument(0));
        if (m_cflags == cfM80)
            if (!isNumber(str_1[0]))
                str_1.Prepend(_T("0"));
    }
    else    // two arguments
    if ((nargs == 2) && (!usedlabel))
    {
        str_1.Printf(str_number_format,de->getArgument(0));
        if (m_cflags == cfM80)
            if (!isNumber(str_1[0]))
                str_1.Prepend(_T("0"));

        str_1 << de->MnItem->MnemonicString[1];
        strparts++;
        str_2.Printf(str_number_format,de->getArgument(1));
        if (m_cflags == cfM80)
            if (!isNumber(str_2[0]))
                str_2.Prepend(_T("0"));

    }

    str_ret << str_1 << str_2;

    if (de->MnItem->MnemonicString.GetCount() > strparts)
    {
        str_ret << de->MnItem->MnemonicString[strparts];
    }
    return str_ret;
}






wxString codeGenerator::GenerateCode(wxString file, const CompilerFlag cflags)
{
    wxTextFile  file_o_code;
    CodeViewItem *cvi;
    wxString str, str_number_format;

    int     count,i;
    DAsmElement *de;
    bool    first_instruction,
            file_ok;

    textCode.Clear();
    m_cflags = cflags;

    if (m_cflags == cfM80)
    {
        end_line = _T("\r\n");
        end_line.Shrink();
        str_number_format = _T("%XH");
        str_number_format.Shrink();
    }
    else
    {
        end_line = _T("\n");
        end_line.Shrink();
        str_number_format = _T("0x%X");
        str_number_format.Shrink();
    }

    i = 0;
    first_instruction = false;
    count = process->m_CodeViewLine->GetCount();

    file.MakeLower();

    textCode << _T(";") << end_line;
    textCode << wxString::Format(_T("; Filename: %s"), file.c_str()) << end_line << _T(";") << end_line;

    while (i < count)
    {
        cvi = process->m_CodeViewLine->getData(i);

        /* -------------------------------------------------
         *  Render Origin
         * -------------------------------------------------*/
        if (cvi->Org >= 0)
        {
            if (!first_instruction)
            {
                if (m_cflags == cfM80)
                {
                    textCode << end_line << _T(".Z80") << end_line << _T(".RADIX 10") << end_line << end_line;
                    textCode << _T("\t") _T("ASEG") << end_line;
                }
            }

            textCode << end_line << generateLabels(cvi) << end_line;

            str = _T("ORG ");
            str << str_number_format;
            textCode << _T("\t") << wxString::Format(str, cvi->Org);
        }
        else
        {
            /* -------------------------------------------------
             *  Render Instructions
             * -------------------------------------------------*/
            if (cvi->Dasmitem >= 0)    // is It data/code ?
            {
                de = process->m_Dasm->GetData(cvi->Dasmitem);

                first_instruction = true;

                switch (de->ElType)
                {
                    case et_Data:
                                        textCode << _T("\t") << generateTextData(cvi);
                                        break;

                    case et_Instruction:
                                        textCode << _T("\t") << generateInstruction(cvi);
                                        break;
                    default:
                                        break;
                }
            }
            else
            /* -------------------------------------------------
             *  Render Labels
             * -------------------------------------------------*/
            if (cvi->LabelAddr != -1)   // Is it a label ?
            {
                first_instruction = true;
                if (process->prog_labels->GetLabel(cvi->LabelAddr,str))
                    textCode << str << _T(":");
                else
                    if (process->var_labels->GetLabel(cvi->LabelAddr,str))
                        textCode << str << _T(":");
                    else
                        process->m_CodeViewLine->DelItem(cvi);
            }
        }


        /* -------------------------------------------------
         *  Render Comments
         * -------------------------------------------------*/
        if (cvi->Comment != 0)   // has comments ?
        {
            str = cvi->Comment->CommentStr;
            if (first_instruction)
                textCode << _T("\t");

            textCode << str << end_line;
        }
        else
            textCode << end_line;

        i++;
    }

    textCode << end_line << end_line << _T("END") << end_line;

    file_ok = file_o_code.Create(file);
    if (!file_ok)
    {
        file_ok = file_o_code.Open(file);
        if (file_ok)
            file_o_code.Clear();
    }
    if (file_ok)
    {
        file_o_code.AddLine(textCode);
        file_o_code.Write(wxTextFileType_Dos);
        file_o_code.Close();
    }


    return textCode;
}

