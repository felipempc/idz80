/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * This module creates the text file containing the
 * disassembled program
 **************************************************************/

#include "codegenerator.h"

codeGenerator::codeGenerator(ProcessData *parent)
{
    Process = parent;
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
    DisassembledItem *de;
    wxString str, str_number_format;

    if (m_cflags == cfM80)
        str_number_format = "%.2XH";
    else
        str_number_format = "0x%.2X";

    de = Process->Disassembled->GetData(cvi->Dasmitem);
    str.Printf("DB ");
    for (i=0; i < de->GetLength(); i++)
    {
        str << wxString::Format(str_number_format, de->GetData(de->GetOffset() + i));
        if (m_cflags == cfM80)
            //FIXME: Baiano BUG, with more than one item it will crash
            if (!isNumber(str[0]))
                str.Prepend("0");

        if (i < (de->GetLength() - 1))
            str << ",";
    }
    return str;
}



wxString codeGenerator::generateLabels(CodeViewItem *cvi)
{
    wxString str_ret,str;
    uint i;

    for (i = 0; i < Process->io_labels->GetCount(); i++)
    {
        str = Process->io_labels->GetAddress(i);
        if (!isNumber(str[0]))
            str.Prepend("0");

        if (m_cflags == cfM80)
            str << "H";
        else
            str.Prepend("0x");

        str_ret << Process->io_labels->GetLabel(i) << "  EQU  " << str << end_line;
    }
    return str_ret;

}


wxString codeGenerator::generateInstruction(CodeViewItem *cvi)
{
    int		nargs;
    uint	argument, strparts;

    wxString    str_1, str_2, str_ret,
                str_number_format;
    bool usedlabel;
    DisassembledItem *de;

    usedlabel = false;
    de = Process->Disassembled->GetData(cvi->Dasmitem);
    nargs = de->GetArgumentCount();

    str_ret = de->GetMnemonicStr(0);
    strparts = 1;

    str_1.Clear();
    str_2.Clear();

    argument = de->GetArgument(0, Process->Disassembled->GetBaseAddress(cvi->Dasmitem));

    if (m_cflags == cfM80)
        str_number_format = "%XH";
    else
        str_number_format = "0x%X";

    if (de->HasArgumentLabel())
    {
        switch (de->GetArgumentType(0))
        {
            case ARG_REL_ADDR:
            case ARG_ABS_ADDR:
                            usedlabel = Process->prog_labels->GetLabel(argument, str_1);
                            break;
            case ARG_IO_ADDR:
                            usedlabel = Process->io_labels->GetLabel(argument, str_1);
                            break;
            case ARG_VARIABLE:
                            usedlabel = Process->var_labels->GetLabel(argument, str_1);
                            break;
            case ARG_NONE:
            case ARG_LITERAL:
            case ARG_OFFSET:
                            break;
        }
    }

    if ((nargs == 1) && (!usedlabel))
    {
        str_1.Printf(str_number_format, argument);
        if (m_cflags == cfM80)
            if (!isNumber(str_1[0]))
                str_1.Prepend("0");
    }
    else    // two arguments
    if ((nargs == 2) && (!usedlabel))
    {
        str_1.Printf(str_number_format, argument);
        if (m_cflags == cfM80)
            if (!isNumber(str_1[0]))
                str_1.Prepend("0");

        str_1 << de->GetMnemonicStr(1);
        strparts++;
        str_2.Printf(str_number_format, de->GetArgument(1, 0));
        if (m_cflags == cfM80)
            if (!isNumber(str_2[0]))
                str_2.Prepend("0");

    }

    str_ret << str_1 << str_2;

    if (de->GetMnemonicStrCount() > strparts)
    {
        str_ret << de->GetMnemonicStr(strparts);
    }
    return str_ret;
}






wxString codeGenerator::GenerateCode(wxString file, const CompilerFlag cflags)
{
    wxTextFile  file_o_code;
    CodeViewItem *cvi;
    wxString str, str_number_format;

    int     count,i;
    bool    first_instruction,
            file_ok;
    DisassembledItem	*de;

    textCode.Clear();
    m_cflags = cflags;

    if (m_cflags == cfM80)
    {
        end_line = "\r\n";
        end_line.Shrink();
        str_number_format = "%XH";
        str_number_format.Shrink();
    }
    else
    {
        end_line = "\n";
        end_line.Shrink();
        str_number_format = "0x%X";
        str_number_format.Shrink();
    }

    i = 0;
    first_instruction = false;
    count = Process->CodeViewLines->GetCount();

    file.MakeLower();

    textCode << ";" << end_line;
    textCode << wxString::Format("; Filename: %s", file.c_str()) << end_line << _T(";") << end_line;

    while (i < count)
    {
        cvi = Process->CodeViewLines->getData(i);

        /* -------------------------------------------------
         *  Render Origin
         * -------------------------------------------------*/
        if (cvi->Org >= 0)
        {
            if (!first_instruction)
            {
                if (m_cflags == cfM80)
                {
                    textCode << end_line << ".Z80" << end_line << ".RADIX 10" << end_line << end_line;
                    textCode << "\tASEG" << end_line;
                }
            }

            textCode << end_line << generateLabels(cvi) << end_line;

            str = "ORG ";
            str << str_number_format;
            textCode << "\t" << wxString::Format(str, cvi->Org);
        }
        else
        {
            /* -------------------------------------------------
             *  Render Instructions
             * -------------------------------------------------*/
            if (cvi->Dasmitem >= 0)    // is It data/code ?
            {
                de = Process->Disassembled->GetData(cvi->Dasmitem);

                first_instruction = true;

                switch (de->GetType())
                {
                    case et_Data:
                                        textCode << "\t" << generateTextData(cvi);
                                        break;

                    case et_Instruction:
                                        textCode << "\t" << generateInstruction(cvi);
                                        break;
                    default:
                                        break;
                }
            }
            else
            /* -------------------------------------------------
             *  Render Labels
             * -------------------------------------------------*/
            if (cvi->LabelProgAddr)
            {
                first_instruction = true;
                textCode << cvi->LabelProgAddr->LabelStr << ":";
            }
            if (cvi->LabelVarAddr)
            {
                first_instruction = true;
                textCode << cvi->LabelVarAddr->LabelStr << ":";
            }
        }


        /* -------------------------------------------------
         *  Render Comments
         * -------------------------------------------------*/
        if (cvi->Comment)   // has comments ?
        {
            str = cvi->Comment->c_str(); //->CommentStr;
            if (first_instruction)
                textCode << "\t";

            textCode << str << end_line;
        }
        else
            textCode << end_line;

        i++;
    }

    textCode << end_line << end_line << "END" << end_line;

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

