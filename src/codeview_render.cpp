/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   30-12-2011 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * User Interface - Rendering
 **************************************************************/

#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <string>
#include "codeview.h"
#include "disassembled_item.h"



// Plots Data item, returns the last point of the plotted string
uint CodeView::RenderData(wxDC &dc, const int start_y, CodeViewItem *cvi)
{
    uint i, x, argwidth, lenght;
    DisassembledItem *de;
    wxString str;

    x = COL_MNEM;
    de = Process->Disassembled->GetData(cvi->Dasmitem);
    dc.SetTextForeground(FG_TextColor);
    dc.DrawText(str, x, start_y);
    x += dc.GetTextExtent(str).GetWidth();

    if (de->GetStyleArgument(0) == ast_wordhex)
    {
        str.Printf("DW ");
        lenght = de->GetLength() / 2;
        for (i = 0; i < lenght; i++)
        {
            argwidth = de->GetData(de->GetOffset() + i + 1) * 0xFF + de->GetData(de->GetOffset() + i);
            argwidth = argwidth & 0xFFFF;
            str << wxString::Format("0x%.4X", argwidth);
            if (i < (lenght - 1))
                str << ",";
        }

    }
    else
    {
        str.Printf("DB ");

        for (i = 0; i < de->GetLength(); i++)
        {
            str << wxString::Format("0x%.2X", de->GetData(de->GetOffset() + i));
            if (i < (de->GetLength() - 1))
                str << ",";
        }
    }

    dc.DrawText(str, x, start_y);
    argwidth = dc.GetTextExtent(str).GetWidth();
    x += argwidth;
    if (cvi->RectArg1 == 0)
    {
        cvi->RectArg1 = new wxRect(COL_MNEM, start_y, argwidth, m_fontHeight);
    }
    else
    {
        cvi->RectArg1->SetX(COL_MNEM);
        cvi->RectArg1->SetY(start_y);
        cvi->RectArg1->SetHeight(m_fontHeight);
        cvi->RectArg1->SetWidth(argwidth);
    }
    return x;
}



// Plots the instructions, return the last point of the plotted string
uint CodeView::RenderInstruction(wxDC &dc, const int start_y, CodeViewItem *cvi)
{
    int		nargs,
			x,
			argwidth1,
			argpos1,
			argwidth2,
			argpos2;
    uint	strparts,   // counts the number of mnemonic str used
			argument;
    wxString str;
    bool usedlabel;
    DisassembledItem *de;

    argwidth1 = argwidth2 = argpos1 = argpos2 = 0;
    usedlabel = false;
    x = COL_MNEM;
    de = Process->Disassembled->GetData(cvi->Dasmitem);
    nargs = de->GetArgumentCount();
    strparts = 0;
    str = de->GetMnemonicStr(0);
    strparts++;
    dc.SetTextForeground(FG_TextColor);
    dc.DrawText(str, x, start_y);
    x += dc.GetTextExtent(str).GetWidth();
    argpos1 = x;

	argument = de->GetArgument(0, Process->Disassembled->GetBaseAddress(cvi->Dasmitem));

    if (de->HasArgumentLabel())
    {
        switch (de->GetArgumentType(0))
        {
            case ARG_REL_ADDR:
            case ARG_ABS_ADDR:
                            if (Process->prog_labels->GetLabel(argument, str) >= 0)
                                usedlabel = true;
                            break;
            case ARG_IO_ADDR:
                            if (Process->io_labels->GetLabel(argument, str) >= 0)
                                usedlabel = true;
                            break;
            case ARG_VARIABLE:
                            if (Process->var_labels->GetLabel(argument, str) >= 0)
                                usedlabel = true;
                            break;
            case ARG_NONE:
            case ARG_LITERAL:
            case ARG_OFFSET:
                            break;
        }
        if (usedlabel)
        {
            dc.SetTextForeground(*wxBLUE);
            dc.DrawText(str, x, start_y);
            argwidth1 = dc.GetTextExtent(str).GetWidth();
            x += argwidth1;
            dc.SetTextForeground(FG_TextColor);
            if (cvi->RectArg1 == 0)
            {
                cvi->RectArg1 = new wxRect(argpos1, start_y, argwidth1, m_fontHeight);
            }
            else
            {
                cvi->RectArg1->SetX(argpos1);
                cvi->RectArg1->SetY(start_y);
                cvi->RectArg1->SetHeight(m_fontHeight);
                cvi->RectArg1->SetWidth(argwidth1);
            }
        }
    }

    if ((nargs == 1) && (!usedlabel))
    {
        dc.SetTextForeground(*wxRED);

		str = FormatArg(argument, de->GetStyleArgument(0));

        dc.DrawText(str, x, start_y);
        argwidth1 = dc.GetTextExtent(str).GetWidth();
        x += argwidth1;
        if (cvi->RectArg1 == 0)
        {
            cvi->RectArg1 = new wxRect(argpos1, start_y, argwidth1, m_fontHeight);
        }
        else
        {
            cvi->RectArg1->SetX(argpos1);
            cvi->RectArg1->SetY(start_y);
            cvi->RectArg1->SetHeight(m_fontHeight);
            cvi->RectArg1->SetWidth(argwidth1);
        }
    }
    else    // two arguments
    if ((nargs == 2) && (!usedlabel))
    {
        dc.SetTextForeground(*wxRED);

		str = FormatArg(argument, de->GetStyleArgument(0));
        dc.DrawText(str, x, start_y);
        argwidth1 = dc.GetTextExtent(str).GetWidth();
        x += argwidth1;

        dc.SetTextForeground(FG_TextColor);
        str = de->GetMnemonicStr(1);
        strparts++;
        dc.DrawText(str, x, start_y);
        x += dc.GetTextExtent(str).GetWidth();
        argpos2 = x;

        dc.SetTextForeground(*wxRED);
        str = FormatArg(de->GetArgument(1, 0), de->GetStyleArgument(1));

        dc.DrawText(str, x, start_y);
        argwidth2 = dc.GetTextExtent(str).GetWidth();
        x += argwidth2;
        if (cvi->RectArg1 == 0)
        {
            cvi->RectArg1 = new wxRect(argpos1, start_y, argwidth1, m_fontHeight);
        }
        else
        {
            cvi->RectArg1->SetX(argpos1);
            cvi->RectArg1->SetY(start_y);
            cvi->RectArg1->SetHeight(m_fontHeight);
            cvi->RectArg1->SetWidth(argwidth1);
        }

        if (cvi->RectArg2 == 0)
        {
            cvi->RectArg2 = new wxRect(argpos2, start_y, argwidth2, m_fontHeight);
        }
        else
        {
            cvi->RectArg2->SetX(argpos2);
            cvi->RectArg2->SetY(start_y);
            cvi->RectArg2->SetHeight(m_fontHeight);
            cvi->RectArg2->SetWidth(argwidth2);
        }
    }

    dc.SetTextForeground(FG_TextColor);
    if (de->GetMnemonicStrCount() > strparts)
    {
        str = de->GetMnemonicStr(strparts);
        dc.DrawText(str, x, start_y);
        x += dc.GetTextExtent(str).GetWidth();
    }
    return x;
}



uint CodeView::RenderProgramLabel(wxDC &dc, const int start_y,wxString str)
{
    int x;
    x = COL_LABEL;
    dc.SetTextForeground(FG_LabelColor);
    dc.DrawText(str, x, start_y);
    x += dc.GetTextExtent(str).GetWidth();
    dc.SetTextForeground(FG_TextColor);
    return x;
}



uint CodeView::RenderOrigin(wxDC &dc, const int start_y,uint address)
{
    int x;
    wxString str;

    x = COL_MNEM;
    str.Printf("ORG %XH",address);
    dc.SetTextForeground(FG_TextColor);
    dc.DrawText(str, x, start_y);
    x += dc.GetTextExtent(str).GetWidth();
    return x;
}


void CodeView::Render(wxDC &dc, const int start_y, const int fromline, const int count)
{
    CodeViewItem	*cvi;
    wxString		str;
    uint			commentoffset;
    int				linepixel, line_offset, rendering_line;
    uint			address;
    DisassembledItem		*de;
    wxCoord			width, heigh;

    GetClientSize(&width, &heigh);

    linepixel = start_y;
    line_offset = 0;
    address = 0;

    while (line_offset < count)
    {
        commentoffset = 0;
        rendering_line = fromline + line_offset;
        cvi = m_CodeViewLine->getData(rendering_line);
        if (cvi)
		{
			/* -------------------------------------------------
			 *  Render Origin
			 * -------------------------------------------------*/
			if (cvi->Org >= 0)
			{
				commentoffset = RenderOrigin(dc, linepixel, cvi->Org);
				address = cvi->Org;
			}
			else
			{
				/* -------------------------------------------------
				 *  Render Instructions
				 * -------------------------------------------------*/
				if (cvi->Dasmitem >= 0)    // is It data/code ?
				{
					de = Process->Disassembled->GetData(cvi->Dasmitem);
					if (de)
					{
						address = Process->Disassembled->GetBaseAddress(cvi->Dasmitem) + de->GetOffset();

						dc.SetTextForeground(FG_TextColor);
						dc.DrawText(de->GetCodeStr(), COL_CODE, linepixel);
						dc.DrawText(de->GetAsciiStr(), COL_ASCII, linepixel);
						switch (de->GetType())
						{
							case et_Data:
												commentoffset = RenderData(dc, linepixel, cvi);
												break;

							case et_Instruction:
												commentoffset = RenderInstruction(dc, linepixel, cvi);
												break;
							default:
												commentoffset = 0;
												break;
						}
					}
					else
						dc.DrawText("------------------------ ERROR ----------------------------------------", COL_CODE, linepixel);
				}
				else
				/* -------------------------------------------------
				 *  Render Labels
				 * -------------------------------------------------*/
                {
                        if (cvi->LabelProgAddr)   // Is it a label ?
                        {
                            if (!cvi->LabelProgAddr->LabelStr.IsEmpty())
                            {
                                str.Printf("%s:", cvi->LabelProgAddr->LabelStr);
                                commentoffset = RenderProgramLabel(dc, linepixel, str);
                                address = cvi->LabelProgAddr->Address;
                            }
                            else
                            {
                                str = "ERROR_LABEL:";
                                commentoffset = RenderProgramLabel(dc, linepixel, str);
                                m_CodeViewLine->DelItem(cvi);
                                delete cvi;
                                cvi = 0;
                            }
                        }

                        if (cvi->LabelVarAddr)   // Is it a label ?
                        {
                            if (!cvi->LabelVarAddr->LabelStr.IsEmpty())
                            {
                                str.Printf("%s:", cvi->LabelVarAddr->LabelStr);
                                commentoffset = RenderProgramLabel(dc, linepixel, str);
                                address = cvi->LabelVarAddr->Address;
                            }
                            else
                            {
                                str = "ERROR_LABEL:";
                                commentoffset = RenderProgramLabel(dc, linepixel, str);

                                m_CodeViewLine->DelItem(cvi);
                                delete cvi;
                                cvi = 0;
                            }
                        }
                }
			}

			/* -------------------------------------------------
			 *  Render Address
			 * -------------------------------------------------*/
			if (rendering_line >= m_CodeViewLine->getFirstInstructionLine())
			{
				str.Printf("0x%.4X", address);
				dc.DrawText(str, COL_ADDRESS, linepixel);
			}


			/* -------------------------------------------------
			 *  Render Comments
			 * -------------------------------------------------*/
			if (cvi->Comment)   // has comments ?
			{
			    if (!cvi->Comment->IsEmpty())
                {
                    dc.SetTextForeground(*wxGREEN);
                    str = cvi->Comment->Clone();

                    commentoffset += 20;
                    dc.DrawText(str, commentoffset, linepixel);
                    dc.SetTextForeground(FG_TextColor);
                }
			}
			line_offset++;
			linepixel += m_fontHeight;
		}
		else
			line_offset = count;
    }
}



wxString CodeView::FormatArg(uint arg, uint style)
{
	wxString str;

	arg = arg & 0xFFFF;

	switch (style)
	{
		case ast_bytedec:
			str.Printf("%dD", arg);
			break;
		case ast_bytebin:
			str.Printf(IntToBin(arg));
			str << "B";
			break;
		case ast_bytehex:
		default:
			str.Printf("%XH", arg);
			break;
	}
	return str;
}


wxString IntToBin(uint conv)
{
	bool printzero = false;
	int i,
		temp,
		bitmask = 0b1000000000000000;
	wxString str;

	for (i = 0; i < 16; i++)
	{
		temp = conv & bitmask;
		if (temp > 0)
		{
			str << "1";
			printzero = true;
		}
		else
			if (printzero)
				str << "0";
		bitmask = bitmask >> 1;
	}
	if (str.Len() == 0)
        str = "0";
	return str;
}
