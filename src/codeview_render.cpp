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
    DisassembledItem *dasmed_item;
    wxString str = "ERROR";

    column_pixel = COL_MNEM;
    dasmed_item = m_sourcecode->getDisassembled()->GetData(sc_line->dasmedItem);

    //TODO: Verify why we drawtext of an empty string
    dc.SetTextForeground(m_color_text_foreground);
    dc.DrawText(str, column_pixel, t_line_pixel);
    column_pixel += dc.GetTextExtent(str).GetWidth();

    if ((dasmed_item->getArgumentStyle(0) == STYLE_WORD_HEX) && (dasmed_item->getOpcodeSize() % 2 == 0)) { // Must be even
        str.Printf("DW ");
        lenght = dasmed_item->getOpcodeSize() / 2;
        for (unsigned int i = 0; i < lenght; ++i) {

            argwidth = dasmed_item->getWordFromFile(i);
            str << wxString::Format("0x%.4X", argwidth);
            if (i < (lenght - 1))
                str << ",";
        }
    }
    else {
        str.Printf("DB ");
        for (unsigned int i = 0; i < dasmed_item->getOpcodeSize(); ++i)
        {
            str << wxString::Format("0x%.2X", dasmed_item->getByteOpcode(i));
            if (i < (dasmed_item->getOpcodeSize() - 1))
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
    int		num_arguments = 0,
            num_arguments_labeled = 0,
            //index_arguments = 0,
			column_pixel = 0,
			argwidth1 = 0,
			argpos1 = 0,
			argwidth2 = 0,
			argpos2 = 0;
    uint	strparts = 0,   // counts the number of mnemonic str used
			argument = 0;
    wxString str;
    bool usedlabel  = false;
    DisassembledItem *dasmed_item;
    std::array<int, MAX_ARGUMENT_COUNT> argwidth = {0, 0};

    usedlabel = false;
    column_pixel = COL_MNEM;
    dasmed_item = m_sourcecode->getDisassembled()->GetData(sc_line->dasmedItem);
    num_arguments = dasmed_item->getMnemonic()->GetArgumentCount();
    if (num_arguments > MAX_ARGUMENT_COUNT) {
        num_arguments = MAX_ARGUMENT_COUNT;
    }
    num_arguments_labeled = dasmed_item->getNumArgumentLabeled();
    strparts = 0;
    str = dasmed_item->getMnemonic()->GetMnemonicStr(0);
    ++strparts;
    dc.SetTextForeground(m_color_text_foreground);
    dc.DrawText(str, column_pixel, t_line_pixel);       // Prints the first part of the instruction
    column_pixel += dc.GetTextExtent(str).GetWidth();
    argpos1 = column_pixel;

    for(int index_arguments = 0; index_arguments < num_arguments; ++index_arguments) {
        argument = dasmed_item->getArgumentValue(index_arguments, m_sourcecode->getDisassembled()->GetBaseAddress(sc_line->dasmedItem));
        if (dasmed_item->getArgumentStyle(index_arguments) == STYLE_LABELED) {
            str = getArgumentLabel(dasmed_item, index_arguments, argument);
            if (!str.IsEmpty()) {
                dc.SetTextForeground(m_color_label_foreground);
            }
        }
        dc.DrawText(str, column_pixel, t_line_pixel);

    }
    // --------------------------------
        if (usedlabel)
        {
            dc.SetTextForeground(*wxBLUE);
            dc.DrawText(str, column_pixel, t_line_pixel);
            argwidth1 = dc.GetTextExtent(str).GetWidth();
            column_pixel += argwidth1;
            dc.SetTextForeground(m_color_text_foreground);
            updateRectangle(sc_line->rectArg1, argpos1, t_line_pixel, argwidth1, m_font_height);
        }
    }   // End rendering labeled arguments

    if ((num_arguments == 1) && (!usedlabel))
    {
        dc.SetTextForeground(*wxRED);

		str = formatArg(argument, dasmed_item->GetStyleArgument(0));

        dc.DrawText(str, column_pixel, t_line_pixel);
        argwidth1 = dc.GetTextExtent(str).GetWidth();
        column_pixel += argwidth1;
        updateRectangle(sc_line->rectArg1, argpos1, t_line_pixel, argwidth1, m_font_height);
    }
    else    // two arguments
    if ((num_arguments == 2) && (!usedlabel))
    {
        dc.SetTextForeground(*wxRED);

		str = formatArg(argument, dasmed_item->GetStyleArgument(0));
        dc.DrawText(str, column_pixel, t_line_pixel);
        argwidth1 = dc.GetTextExtent(str).GetWidth();
        column_pixel += argwidth1;

        dc.SetTextForeground(m_color_text_foreground);
        str = dasmed_item->GetMnemonicStr(1);
        ++strparts;
        dc.DrawText(str, column_pixel, t_line_pixel);
        column_pixel += dc.GetTextExtent(str).GetWidth();
        argpos2 = column_pixel;

        dc.SetTextForeground(*wxRED);
        str = formatArg(dasmed_item->GetArgument(1, 0), dasmed_item->GetStyleArgument(1));

        dc.DrawText(str, column_pixel, t_line_pixel);
        argwidth2 = dc.GetTextExtent(str).GetWidth();
        column_pixel += argwidth2;
    
        updateRectangle(sc_line->rectArg1, argpos1, t_line_pixel, argwidth1, m_font_height);
        updateRectangle(sc_line->rectArg2, argpos2, t_line_pixel, argwidth2, m_font_height);
    }

    dc.SetTextForeground(m_color_text_foreground);
    if (dasmed_item->getMnemonic()->GetMnemonicStrCount() > strparts)
    {
        str = dasmed_item->getMnemonic()->GetMnemonicStr(strparts);
        dc.DrawText(str, column_pixel, t_line_pixel);
        column_pixel += dc.GetTextExtent(str).GetWidth();
    }

    return column_pixel;
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
    DisassembledItem		*dasmed_item;
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
					dasmed_item = m_process->Disassembled->GetData(sc_line->dasmedItem);
					if (dasmed_item)
					{
						address = m_process->Disassembled->GetBaseAddress(sc_line->dasmedItem) + dasmed_item->GetOffset();

						dc.SetTextForeground(m_color_text_foreground);
						dc.DrawText(dasmed_item->GetCodeStr(), COL_CODE, linepixel);
						dc.DrawText(dasmed_item->GetAsciiStr(), COL_ASCII, linepixel);
						switch (dasmed_item->GetType())
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
