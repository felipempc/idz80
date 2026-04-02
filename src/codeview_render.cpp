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
#include <array>
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

    //TODO: Since it's data, a rectangle is needed?
    // Create a rectangle around the argument
    wxRect** data_rectangle = nullptr;
    *data_rectangle = sc_line->rectArg1;
    updateRectangle(data_rectangle, COL_MNEM, t_line_pixel, argwidth, m_font_height);

    return column_pixel;
}



/// @brief Draws a line with instruction item
/// @param dc Device Conext
/// @param t_line_pixel The initial pixel line.
/// @param sc_line Source code line
/// @return the last pixel point of the drawn line of instruction.
uint CodeView::renderInstruction(wxDC &dc, const int t_line_pixel, SourceCodeLine *sc_line)
{
    int		num_arguments = 0,
			column_pixel = 0;
    uint	strparts = 0,   // counts the number of mnemonic string parts used
			argument = 0;
    wxString line_string;
    DisassembledItem *dasmed_item;
    std::array<int, MAX_ARGUMENT_COUNT> argwidth = {0, 0};
    std::array<int, MAX_ARGUMENT_COUNT> arg_pixel_position = {0, 0};
    std::array<wxRect**, MAX_ARGUMENT_COUNT> arg_rectangle = {nullptr, nullptr};

    column_pixel = COL_MNEM;
    dasmed_item = m_sourcecode->getDisassembled()->GetData(sc_line->dasmedItem);
    num_arguments = dasmed_item->getMnemonic()->GetArgumentCount();
    if (num_arguments > MAX_ARGUMENT_COUNT) {
        return 0;
    }
    if (num_arguments == 1) {
        arg_rectangle.at(0) = &sc_line->rectArg1;
    }
    if (num_arguments == 2) {
        arg_rectangle.at(0) = &sc_line->rectArg1;
        arg_rectangle.at(1) = &sc_line->rectArg2;
    }
    line_string = dasmed_item->getMnemonic()->GetMnemonicStr(0);
    ++strparts;
    dc.SetTextForeground(m_color_text_foreground);
    dc.DrawText(line_string, column_pixel, t_line_pixel);       // Draws the first part of the instruction
    column_pixel += dc.GetTextExtent(line_string).GetWidth();
    arg_pixel_position.at(0) = column_pixel;
    line_string.Clear();    // Empty the string to the next stage

    //TODO: Debug here.
    for(int index_arguments = 0; index_arguments < num_arguments; ++index_arguments) {
        argument = dasmed_item->getArgumentValue(index_arguments, m_sourcecode->getDisassembled()->GetBaseAddress(sc_line->dasmedItem));
        if (dasmed_item->getArgumentStyle(index_arguments) == STYLE_LABELED) {
            line_string = getArgumentLabel(dasmed_item, index_arguments, argument);
        }
        if (line_string.IsEmpty()) {   // No label found, format the argument and set the color.
            dc.SetTextForeground(m_color_text_foreground);
            line_string = formatArg(argument, dasmed_item->getArgumentStyle(index_arguments));    // format the value of the argument
        }
        else {  // Found a label, set the color
            dc.SetTextForeground(m_color_label_foreground);
        }
        dc.DrawText(line_string, column_pixel, t_line_pixel);   // Draws the label or the argument's value
        argwidth.at(index_arguments) = dc.GetTextExtent(line_string).GetWidth();
        column_pixel += argwidth.at(index_arguments);
        updateRectangle(arg_rectangle.at(index_arguments), arg_pixel_position.at(index_arguments), t_line_pixel, argwidth.at(index_arguments), m_font_height);

        if (num_arguments == (index_arguments + 2)) {   // Test if need to draw the middle of instruction, if there are two arguments
            dc.SetTextForeground(m_color_text_foreground);
            line_string = dasmed_item->getMnemonic()->GetMnemonicStr(1);
            ++strparts;
            dc.DrawText(line_string, column_pixel, t_line_pixel);   // Draws the middle of the instruction
            column_pixel += dc.GetTextExtent(line_string).GetWidth();
            arg_pixel_position.at(1) = column_pixel;
        }
    }

    dc.SetTextForeground(m_color_text_foreground);
    if (dasmed_item->getMnemonic()->GetMnemonicStrCount() > strparts)
    {
        line_string = dasmed_item->getMnemonic()->GetMnemonicStr(strparts);
        dc.DrawText(line_string, column_pixel, t_line_pixel);   // Draws the last part of instruction
        column_pixel += dc.GetTextExtent(line_string).GetWidth();
    }

    return column_pixel;
}



/// @brief Draws a label of a subroutine
/// @param dc Device Conext
/// @param t_line_pixel The initial pixel line.
/// @param str The label
/// @return the last pixel point of the drawn line of the label.
uint CodeView::renderProgramLabel(wxDC &dc, const int t_line_pixel, wxString t_label_string)
{
    int column_pixel;
    column_pixel = COL_LABEL;
    dc.SetTextForeground(m_color_label_foreground);
    dc.DrawText(t_label_string, column_pixel, t_line_pixel);
    column_pixel += dc.GetTextExtent(t_label_string).GetWidth();
    dc.SetTextForeground(m_color_text_foreground);
    return column_pixel;
}



/// @brief Draws the ORG directive
/// @param t_dc Device Context
/// @param t_line_pixel The initial pixel line.
/// @param address The address of ORG
/// @return the last pixel point of the drawn line of ORG directive.
uint CodeView::renderOrigin(wxDC &dc, const int t_line_pixel, uint t_address)
{
    int column_pixel;
    wxString str;

    column_pixel = COL_MNEM;
    str.Printf("ORG %XH", t_address);
    dc.SetTextForeground(m_color_text_foreground);
    dc.DrawText(str, column_pixel, t_line_pixel);
    column_pixel += dc.GetTextExtent(str).GetWidth();
    return column_pixel;
}



/// @brief Renders the lines of a source code
/// @param t_dc Device Context
/// @param t_line_pixel The initial pixel line.
/// @param t_fromline The initial line of source code
/// @param t_count How many lines to render
void CodeView::render(wxDC &t_dc, const int t_line_pixel, const int t_fromline, const int t_count)
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

    while (line_offset < t_count) {
        commentoffset = 0;
        rendering_line = t_fromline + line_offset;
        sc_line = m_sourcecode->line(rendering_line);
        if (!sc_line) { // if line of code doesn't exists, break.
            break;
        }
        
        //  Render ORG Directive
        if (sc_line->originAddress >= 0) {
            commentoffset = renderOrigin(t_dc, linepixel, sc_line->originAddress);
            address = sc_line->originAddress;
            continue;   // go to next line_offset
        }

        // Render instructions or data
        if (sc_line->dasmedItem >= 0) {    // Tests if there is an index of a disassembled item
            dasmed_item =  m_sourcecode->getDisassembled()->GetData(sc_line->dasmedItem);
            if (dasmed_item) {
                address =  m_sourcecode->getDisassembled()->GetBaseAddress(sc_line->dasmedItem) + dasmed_item->getOffsetInFile();

                t_dc.SetTextForeground(m_color_text_foreground);
                t_dc.DrawText(dasmed_item->getOpcodeAsStringHex(), COL_CODE, linepixel);
                t_dc.DrawText(dasmed_item->getAsciiCodeAsString(), COL_ASCII, linepixel);
                if (dasmed_item->isData()) {
                    commentoffset = renderData(t_dc, linepixel, sc_line);
                }
                else {
                    if (dasmed_item->isInstruction()) {
                        commentoffset = renderInstruction(t_dc, linepixel, sc_line);
                    }
                    else {
                        commentoffset = 0;
                    }
                }
            }
            else {
                t_dc.DrawText("------------------------ ERROR ----------------------------------------", COL_CODE, linepixel);
            }
        }
        else { // Render Labels
            if (sc_line->labelProgAddress) {  // Is it a label ?
                if (!sc_line->labelProgAddress->labelString.IsEmpty()) {
                    str.Printf("%s:", sc_line->labelProgAddress->labelString);
                    commentoffset = renderProgramLabel(t_dc, linepixel, str);
                    address = sc_line->labelProgAddress->address;
                }
                else {
                    str = "ERROR_LABEL:";
                    commentoffset = renderProgramLabel(t_dc, linepixel, str);
                    m_sourcecode->delSCItem(sc_line);
                    delete sc_line;
                    sc_line = 0;
                }
            }
            if (sc_line->labelVarAddress) { // Is it a var label address?
                if (!sc_line->labelVarAddress->labelString.IsEmpty()) {
                    str.Printf("%s:", sc_line->labelVarAddress->labelString);
                    commentoffset = renderProgramLabel(t_dc, linepixel, str);
                    address = sc_line->labelVarAddress->address;
                }
                else {
                    str = "ERROR_LABEL:";
                    commentoffset = renderProgramLabel(t_dc, linepixel, str);

                    m_sourcecode->delSCItem(sc_line);
                    delete sc_line;
                    sc_line = 0;
                }
            }
        }

        // Render Address
        if (rendering_line >= m_sourcecode->getFirstInstructionLine()) {
            str.Printf("0x%.4X", address);
            t_dc.DrawText(str, COL_ADDRESS, linepixel);
        }

        // Render Comments
        if (sc_line->comment) {   // has comments ?
            if (!sc_line->comment->IsEmpty()) {
                t_dc.SetTextForeground(*wxGREEN);
                str = sc_line->comment->Clone();
                commentoffset += 20;
                t_dc.DrawText(str, commentoffset, linepixel);
                t_dc.SetTextForeground(m_color_text_foreground);
            }
        }
        ++line_offset;
        linepixel += m_font_height;
    } // while
} // render



/// @brief Formats a string with the stylized value of the argument.
/// @param arg The argument value
/// @param style The style format to be aplied
/// @return A string containing the stylized value
wxString CodeView::formatArg(uint arg, ArgumentStyleOptions style)
{
	wxString str;

	arg = arg & 0xFFFF;

	switch (style)
	{
		case STYLE_BYTE_DEC:
			str.Printf("%dD", arg);
			break;
		case STYLE_BYTE_BIN:
			str.Printf(convertIntToBin(arg));
			str << "B";
			break;
		case STYLE_BYTE_HEX:
		default:
			str.Printf("%XH", arg);
			break;
	}
	return str;
}



/// @brief Converts a number to a string in binary format
/// @param conv Value to be converted
/// @return A string containing the value in binary format
wxString convertIntToBin(uint conv)
{
	bool printzero = false;
	int temp,
		bitmask = 0b1000000000000000;
	wxString str;

	for (int bit_count = 0; bit_count < 16; ++bit_count)
	{
		temp = conv & bitmask;
		if (temp > 0) {
			str << "1";
			printzero = true;
		}
		else {
			if (printzero) {
				str << "0";
            }
        }
		bitmask = bitmask >> 1;
	}
	if (str.Len() == 0) {
        str = "0";
    }
	return str;
}
