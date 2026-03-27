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
#include "codeview.hpp"
#include "disassembled_item.hpp"



/// @brief Draws a line with data item
/// @param dc Device Conext
/// @param t_line_pixel The initial pixel line.
/// @param sc_line Source code line
/// @return the last point of the drawn string.
uint CodeView::renderData(wxDC &dc, const int t_line_pixel, SourceCodeLine *sc_line)
{
    uint column_pixel, argwidth, lenght;
    DisassembledItem *de;
    wxString str = "ERROR";

    column_pixel = COL_MNEM;
    de = m_sourcecode->getDisassembled()->GetData(sc_line->dasmedItem);

    //TODO: Verify why we drawtext of an empty string
    dc.SetTextForeground(m_color_text_foreground);
    dc.DrawText(str, column_pixel, t_line_pixel);
    column_pixel += dc.GetTextExtent(str).GetWidth();

    if ((de->getArgumentStyle(0) == STYLE_WORD_HEX) && (de->getOpcodeSize() % 2 == 0)) { // Must be even
        str.Printf("DW ");
        lenght = de->getOpcodeSize() / 2;
        for (unsigned int i = 0; i < lenght; ++i) {

            argwidth = de->getWordFromFile(i);
            str << wxString::Format("0x%.4X", argwidth);
            if (i < (lenght - 1))
                str << ",";
        }
    }
    else {
        str.Printf("DB ");
        for (unsigned int i = 0; i < de->getOpcodeSize(); ++i)
        {
            str << wxString::Format("0x%.2X", de->getByteOpcode(i));
            if (i < (de->getOpcodeSize() - 1))
                str << ",";
        }
    }

    dc.DrawText(str, column_pixel, t_line_pixel);
    argwidth = dc.GetTextExtent(str).GetWidth();
    column_pixel += argwidth;

    // Create a rectangle around the argument
    if (sc_line->rectArg1 == 0)
    {
        sc_line->rectArg1 = new wxRect(COL_MNEM, t_line_pixel, argwidth, m_font_height);
    }
    else
    {
        sc_line->rectArg1->SetX(COL_MNEM);
        sc_line->rectArg1->SetY(t_line_pixel);
        sc_line->rectArg1->SetHeight(m_font_height);
        sc_line->rectArg1->SetWidth(argwidth);
    }
    return column_pixel;
}



/// @brief Draw a line with instruction item
/// @param dc Device Conext
/// @param t_line_pixel The initial pixel line.
/// @param sc_line Source code line
/// @return the last point of the drawn string.
uint CodeView::renderInstruction(wxDC &dc, const int t_line_pixel, SourceCodeLine *sc_line)
{
    int		nargs,
			column_pixel,
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
    column_pixel = COL_MNEM;
    de = m_sourcecode->getDisassembled()->GetData(sc_line->dasmedItem);
    nargs = de->GetArgumentCount();
    strparts = 0;
    str = de->GetMnemonicStr(0);
    strparts++;
    dc.SetTextForeground(m_color_text_foreground);
    dc.DrawText(str, column_pixel, t_line_pixel);
    column_pixel += dc.GetTextExtent(str).GetWidth();
    argpos1 = column_pixel;

	argument = de->GetArgument(0, m_process->Disassembled->GetBaseAddress(sc_line->dasmedItem));

    if (de->HasArgumentLabel())
    {
        switch (de->GetArgumentType(0))
        {
            case ARG_REL_ADDR:
            case ARG_ABS_ADDR:
                            if (m_process->prog_labels->GetLabel(argument, str) >= 0)
                                usedlabel = true;
                            break;
            case ARG_IO_ADDR:
                            if (m_process->io_labels->GetLabel(argument, str) >= 0)
                                usedlabel = true;
                            break;
            case ARG_VARIABLE:
                            if (m_process->var_labels->GetLabel(argument, str) >= 0)
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
            dc.DrawText(str, x, t_line_pixel);
            argwidth1 = dc.GetTextExtent(str).GetWidth();
            x += argwidth1;
            dc.SetTextForeground(m_color_text_foreground);
            if (sc_line->rectArg1 == 0)
            {
                sc_line->rectArg1 = new wxRect(argpos1, t_line_pixel, argwidth1, m_font_height);
            }
            else
            {
                sc_line->rectArg1->SetX(argpos1);
                sc_line->rectArg1->SetY(t_line_pixel);
                sc_line->rectArg1->SetHeight(m_font_height);
                sc_line->rectArg1->SetWidth(argwidth1);
            }
        }
    }

    if ((nargs == 1) && (!usedlabel))
    {
        dc.SetTextForeground(*wxRED);

		str = formatArg(argument, de->GetStyleArgument(0));

        dc.DrawText(str, x, t_line_pixel);
        argwidth1 = dc.GetTextExtent(str).GetWidth();
        column_pixel += argwidth1;
        if (sc_line->rectArg1 == 0)
        {
            sc_line->rectArg1 = new wxRect(argpos1, t_line_pixel, argwidth1, m_font_height);
        }
        else
        {
            sc_line->rectArg1->SetX(argpos1);
            sc_line->rectArg1->SetY t_line_pixel);
            sc_line->rectArg1->SetHeight(m_font_height);
            sc_line->rectArg1->SetWidth(argwidth1);
        }
    }
    else    // two arguments
    if ((nargs == 2) && (!usedlabel))
    {
        dc.SetTextForeground(*wxRED);

		str = formatArg(argument, de->GetStyleArgument(0));
        dc.DrawText(str, x, t_line_pixel);
        argwidth1 = dc.GetTextExtent(str).GetWidth();
        x += argwidth1;

        dc.SetTextForeground(m_color_text_foreground);
        str = de->GetMnemonicStr(1);
        strparts++;
        dc.DrawText(str, x, t_line_pixel);
        x += dc.GetTextExtent(str).GetWidth();
        argpos2 = x;

        dc.SetTextForeground(*wxRED);
        str = formatArg(de->GetArgument(1, 0), de->GetStyleArgument(1));

        dc.DrawText(str, x, t_line_pixel);
        argwidth2 = dc.GetTextExtent(str).GetWidth();
        x += argwidth2;
        if (sc_line->rectArg1 == 0)
        {
            sc_line->rectArg1 = new wxRect(argpos1, t_line_pixel, argwidth1, m_font_height);
        }
        else
        {
            sc_line->rectArg1->SetX(argpos1);
            sc_line->rectArg1->SetY t_line_pixel);
            sc_line->rectArg1->SetHeight(m_font_height);
            sc_line->rectArg1->SetWidth(argwidth1);
        }

        if (sc_line->rectArg2 == 0)
        {
            sc_line->rectArg2 = new wxRect(argpos2, t_line_pixel, argwidth2, m_font_height);
        }
        else
        {
            sc_line->rectArg2->SetX(argpos2);
            sc_line->rectArg2->SetY t_line_pixel);
            sc_line->rectArg2->SetHeight(m_font_height);
            sc_line->rectArg2->SetWidth(argwidth2);
        }
    }

    dc.SetTextForeground(m_color_text_foreground);
    if (de->GetMnemonicStrCount() > strparts)
    {
        str = de->GetMnemonicStr(strparts);
        dc.DrawText(str, x, t_line_pixel);
        x += dc.GetTextExtent(str).GetWidth();
    }
    return x;
}



uint CodeView::renderProgramLabel(wxDC &dc, const int t_line_pixel,wxString str)
{
    int x;
    x = COL_LABEL;
    dc.SetTextForeground(m_color_label_foreground);
    dc.DrawText(str, x, t_line_pixel);
    x += dc.GetTextExtent(str).GetWidth();
    dc.SetTextForeground(m_color_text_foreground);
    return x;
}



uint CodeView::renderOrigin(wxDC &dc, const int t_line_pixel,uint address)
{
    int x;
    wxString str;

    x = COL_MNEM;
    str.Printf("ORG %XH",address);
    dc.SetTextForeground(m_color_text_foreground);
    dc.DrawText(str, x, t_line_pixel);
    x += dc.GetTextExtent(str).GetWidth();
    return x;
}


void CodeView::render(wxDC &dc, const int t_line_pixel, const int fromline, const int count)
{
    SourceCodeLine	*sc_line;
    wxString		str;
    uint			commentoffset;
    int				linepixel, line_offset, rendering_line;
    uint			address;
    DisassembledItem		*de;
    wxCoord			width, heigh;

    GetClientSize(&width, &heigh);

    linepixel = t_line_pixel;
    line_offset = 0;
    address = 0;

    while (line_offset < count)
    {
        commentoffset = 0;
        rendering_line = fromline + line_offset;
        sc_line = m_sourcecode->line(rendering_line);
        if (sc_line)
		{
			/* -------------------------------------------------
			 *  Render Origin
			 * -------------------------------------------------*/
			if (sc_line->originAddress >= 0)
			{
				commentoffset = renderOrigin(dc, linepixel, sc_line->originAddress);
				address = sc_line->originAddress;
			}
			else
			{
				/* -------------------------------------------------
				 *  Render Instructions
				 * -------------------------------------------------*/
				if (sc_line->dasmedItem >= 0)    // is It data/code ?
				{
					de = m_process->Disassembled->GetData(sc_line->dasmedItem);
					if (de)
					{
						address = m_process->Disassembled->GetBaseAddress(sc_line->dasmedItem) + de->GetOffset();

						dc.SetTextForeground(m_color_text_foreground);
						dc.DrawText(de->GetCodeStr(), COL_CODE, linepixel);
						dc.DrawText(de->GetAsciiStr(), COL_ASCII, linepixel);
						switch (de->GetType())
						{
							case et_Data:
												commentoffset = renderData(dc, linepixel, sc_line);
												break;

							case et_Instruction:
												commentoffset = renderInstruction(dc, linepixel, sc_line);
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
                        if (sc_line->labelProgAddress)   // Is it a label ?
                        {
                            if (!sc_line->labelProgAddress->labelString.IsEmpty())
                            {
                                str.Printf("%s:", sc_line->labelProgAddress->labelString);
                                commentoffset = renderProgramLabel(dc, linepixel, str);
                                address = sc_line->labelProgAddress->address;
                            }
                            else
                            {
                                str = "ERROR_LABEL:";
                                commentoffset = renderProgramLabel(dc, linepixel, str);
                                m_sourcecode->delSCItem(sc_line);
                                delete sc_line;
                                sc_line = 0;
                            }
                        }

                        if (sc_line->labelVarAddress)   // Is it a label ?
                        {
                            if (!sc_line->labelVarAddress->labelString.IsEmpty())
                            {
                                str.Printf("%s:", sc_line->labelVarAddress->labelString);
                                commentoffset = renderProgramLabel(dc, linepixel, str);
                                address = sc_line->labelVarAddress->address;
                            }
                            else
                            {
                                str = "ERROR_LABEL:";
                                commentoffset = renderProgramLabel(dc, linepixel, str);

                                m_sourcecode->delSCItem(sc_line);
                                delete sc_line;
                                sc_line = 0;
                            }
                        }
                }
			}

			/* -------------------------------------------------
			 *  Render Address
			 * -------------------------------------------------*/
			if (rendering_line >= m_sourcecode->getFirstInstructionLine())
			{
				str.Printf("0x%.4X", address);
				dc.DrawText(str, COL_ADDRESS, linepixel);
			}


			/* -------------------------------------------------
			 *  Render Comments
			 * -------------------------------------------------*/
			if (sc_line->comment)   // has comments ?
			{
			    if (!sc_line->comment->IsEmpty())
                {
                    dc.SetTextForeground(*wxGREEN);
                    str = sc_line->comment->Clone();

                    commentoffset += 20;
                    dc.DrawText(str, commentoffset, linepixel);
                    dc.SetTextForeground(m_color_text_foreground);
                }
			}
			line_offset++;
			linepixel += m_font_height;
		}
		else
			line_offset = count;
    }
}



wxString CodeView::formatArg(uint arg, uint style)
{
	wxString str;

	arg = arg & 0xFFFF;

	switch (style)
	{
		case ast_bytedec:
			str.Printf("%dD", arg);
			break;
		case ast_bytebin:
			str.Printf(convertIntToBin(arg));
			str << "B";
			break;
		case ast_bytehex:
		default:
			str.Printf("%XH", arg);
			break;
	}
	return str;
}


wxString convertIntToBin(uint conv)
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
