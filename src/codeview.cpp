/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * User interface
 **************************************************************/

//TODO: Rewrite all of it

#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/utils.h>
#include <wx/region.h>
#include <wx/textdlg.h>
#include <wx/dcclient.h>
#include <wx/accel.h>
#include "codeview.hpp"
#include "disassembled_item.hpp"



const int CodeView::COL_ADDRESS;
const int CodeView::COL_CODE;
const int CodeView::COL_ASCII;
const int CodeView::COL_LABEL;
const int CodeView::COL_MNEM;



CodeView::CodeView(wxWindow *t_parent,  SourceCode *t_srccode, ProcessData *t_processparent)
        : wxScrolledCanvas(t_parent)
{
    m_process = t_processparent;
    m_lines_shown = 0;

    m_multi_selection = false;
    m_selecting = false;
    m_font_height = 1;
    m_is_focused = false;
    m_is_empty = true;

    //m_style_data.arg = 0;
    //m_style_data.item = 0;
    m_last_cursor_rect = 0;

    resetSelectedItemInfo();

    m_sourcecode = t_srccode;
    m_last_cursor_rect = new wxRect();
    m_incomplete_area.x = 0;
    m_incomplete_area.y = 0;
    m_incomplete_area.width = 0;
    m_incomplete_area.height = 0;

    setupFont();

    SetScrollRate(0, m_font_height);
    SetVirtualSize(wxDefaultCoord, m_font_height);

    setupColors();
    setupEvents();
    setupAcceleratorKeys();

    // Debug Area
    SetTextLog(t_processparent->GetTextLog());
    ModuleName = "CodeView";

    DisableKeyboardScrolling(); //wxScrolledCanvas uses arrows,home,end,pageup and pagedown to scroll by default

}



/// @brief Setups the font used in the viewer
void CodeView::setupFont()
{
    wxClientDC dc(this);

    m_font = new wxFont(9, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL,
            wxFONTWEIGHT_NORMAL, false, "Courier New 10 Pitch");
    dc.SetFont(*m_font);
    m_font_height = dc.GetTextExtent("X").GetHeight();
    m_font_width = dc.GetTextExtent("X").GetWidth();
}



/// @brief Setups the hotkeys used in the viewer
void CodeView::setupAcceleratorKeys()
{
    wxAcceleratorEntry hot_keys[4];

    hot_keys[0].Set(wxACCEL_NORMAL, static_cast<int>('d'), idPOPUP_MAKEDATA);
    hot_keys[1].Set(wxACCEL_NORMAL, static_cast<int>('c'), idPOPUP_DISASM);
    hot_keys[2].Set(wxACCEL_CTRL, static_cast<int>('f'), idPOPUP_SEARCH_ARGUMENT);
    hot_keys[3].Set(wxACCEL_NORMAL, WXK_F3, idSEARCH_CONTINUE);
    //hot_keys[4].Set(wxACCEL_NORMAL, WXK_PAGEDOWN, idKEY_PAGEDOWN);
    //hot_keys[5].Set(wxACCEL_NORMAL, WXK_PAGEUP, idKEY_PAGEUP);

    wxAcceleratorTable hot_keys_list(4, hot_keys);

    SetAcceleratorTable(hot_keys_list);
}



/// @brief Setups the colors used in the viewer
void CodeView::setupColors()
{
    m_color_background.Set("WHITE");
    m_color_foreground.Set("BLACK");
    m_color_selected_item.Set("YELLOW");
    m_color_argument_background.Set("YELLOW");
    m_color_argument_foreground.Set("GOLD");
    m_color_text_foreground.Set("BLACK");
    m_color_label_foreground.Set("BLUE");
}



/// @brief Setups the events of the viewer
void CodeView::setupEvents()
{
    Bind(wxEVT_SIZE, &CodeView::onSize, this);
    Bind(wxEVT_SCROLLWIN_LINEDOWN, &CodeView::onScrollLineDown, this);
    Bind(wxEVT_SCROLLWIN_LINEUP, &CodeView::onScrollLineUp, this);
    Bind(wxEVT_SCROLLWIN_PAGEDOWN, &CodeView::onScrollPageDown, this);
    Bind(wxEVT_SCROLLWIN_PAGEUP, &CodeView::onScrollPageUp, this);
    //Bind(wxEVT_MENU, &CodeView::OnHotKeyPageDown, this, idKEY_PAGEDOWN);
    //Bind(wxEVT_MENU, &CodeView::OnHotKeyPageUp, this, idKEY_PAGEUP);

    Bind(wxEVT_PAINT, &CodeView::onPaint, this);

    Bind(wxEVT_SET_FOCUS, &CodeView::onGetFocus, this);
    Bind(wxEVT_KILL_FOCUS, &CodeView::onKillFocus, this);
    Bind(wxEVT_MOUSE_CAPTURE_LOST, &CodeView::onMouseCaptureLost, this);

    // MOUSE Events
    Bind(wxEVT_LEFT_DOWN, &CodeView::onMouseLeftDown, this);
    Bind(wxEVT_LEFT_UP, &CodeView::onMouseLeftUp, this);
    Bind(wxEVT_RIGHT_DOWN, &CodeView::onMouseRightDown, this);
    Bind(wxEVT_RIGHT_UP, &CodeView::onMouseRightUp, this);
    //TODO: Improve focus highlight
    Bind(wxEVT_MOTION, &CodeView::onMouseMove, this);
    Bind(wxEVT_MOUSEWHEEL, &CodeView::onMouseWheel, this);
    //Bind(wxEVT_ENTER_WINDOW, &CodeView::onMouseEnterWindow, this);
    //Bind(wxEVT_LEAVE_WINDOW, &CodeView::onMouseLeaveWindow, this);

    // KEYBOARD events
    Bind(wxEVT_KEY_DOWN, &CodeView::onKeyPress, this);
    Bind(wxEVT_KEY_UP, &CodeView::onKeyRelease, this);

    // Popup event connections
    Bind(wxEVT_MENU, &CodeView::onPopUpAddComment, this, idPOPUP_ADDCOMMENT);
    Bind(wxEVT_MENU, &CodeView::onPopUpEditComment, this, idPOPUP_EDITCOMMENT);
    Bind(wxEVT_MENU, &CodeView::onPopUpDelComment, this, idPOPUP_DELCOMMENT);
    Bind(wxEVT_MENU, &CodeView::onPopUpMenuGoto, this, idPOPUP_GOTO);
    Bind(wxEVT_MENU, &CodeView::onPopUpMenuGotoAddress, this, idPOPUP_GOTO_ADDRESS);
    Bind(wxEVT_MENU, &CodeView::onPopUpMenuMakeData, this, idPOPUP_MAKEDATA);
    Bind(wxEVT_MENU, &CodeView::onPopUpMenuDisasm, this, idPOPUP_DISASM);
    Bind(wxEVT_MENU, &CodeView::onPopUpMenuOD_Matrix, this, idPOPUP_OD_MATRIX);
    Bind(wxEVT_MENU, &CodeView::onPopUpMenuOD_String, this, idPOPUP_OD_STRING);
    Bind(wxEVT_MENU, &CodeView::onPopUpMenuOD_Number, this, idPOPUP_OD_NUMBER);

    Bind(wxEVT_MENU, &CodeView::onPopUpMenuArgStyleBin, this, idPOPUP_ARG_BIN);
    Bind(wxEVT_MENU, &CodeView::onPopUpMenuArgStyleDec, this, idPOPUP_ARG_DEC);
    Bind(wxEVT_MENU, &CodeView::onPopUpMenuArgStyleHex, this, idPOPUP_ARG_HEX);
    Bind(wxEVT_MENU, &CodeView::onPopUpMenuRenLabel, this, idPOPUP_EDITLABEL);
    Bind(wxEVT_MENU, &CodeView::onPopUpMenuDelLabel, this, idPOPUP_DELLABEL);
    Bind(wxEVT_MENU, &CodeView::onPopUpMenuCreateLabel, this, idPOPUP_CREATELABEL);

    Bind(wxEVT_MENU, &CodeView::onPopUpMenuSearchArgument, this, idPOPUP_SEARCH_ARGUMENT);
    Bind(wxEVT_MENU, &CodeView::onSearchContinue, this, idSEARCH_CONTINUE);
}



/// @brief Counts the lines of code
/// @return the lines of the source code
int CodeView::getCount()
{
    return m_sourcecode->getCount();
}



/// @brief Draws the damaged region of the viewer.
/// @param dc The device context (wxwidgets). A region where graphics and text can be drawn.
void CodeView::onDraw(wxDC &dc)
{
    int fromLine, totalLines, fixedY, fixedHeight;
    wxRegionIterator regIterator(GetUpdateRegion());
    wxRect touchedRegion, fullRect;

    dc.SetFont(*m_font);
    dc.SetBackground(*wxWHITE_BRUSH);

    // cleans the destroyed region
    while (regIterator)
    {
        touchedRegion = regIterator.GetRect();
        fullRect.Union(touchedRegion);
        regIterator++;
    }
    CalcUnscrolledPosition(fullRect.x, fullRect.y, &fullRect.x, &fullRect.y);

    fullRect.x = 0;
    fromLine = fullRect.y / m_font_height;
	fixedY = fromLine * m_font_height;
	fixedHeight = fullRect.height + fullRect.y - fixedY;
    totalLines = fixedHeight / m_font_height;
    if ((fixedHeight % m_font_height) != 0)
        totalLines++;

    fullRect.y = fixedY;
    fullRect.height = totalLines * m_font_height;
    fullRect.width = GetClientSize().GetWidth();

	if (m_sourcecode->isEmpty())
    {
        paintBackground(dc, fullRect.y, fromLine, totalLines, *wxGREY_BRUSH);
    }
    else
    {
        if (IsEnabled())
        {
            paintBackground(dc, fullRect.y, fromLine, totalLines, *wxWHITE_BRUSH);
            if ((fromLine == m_line_info.cursorPosition) || ((m_line_info.cursorPosition >= fromLine) &&
                (m_line_info.cursorPosition < (fromLine + totalLines)))) {
                showCursor(dc);
            }

            render(dc, fullRect.y, fromLine, totalLines);
        }
        else {
            paintBackground(dc, fullRect.y, fromLine, totalLines, *wxWHITE_BRUSH);
            //renderBackgroundBlur(dc, fullRect);
        }
    }
}



/// @brief Blurs the background.
/// @param dc The device context (wxwidgets). A region where graphics and text can be drawn.
/// @param region A rectangle containing the region to be blurred.
void CodeView::renderBackgroundBlur(wxDC &t_dc, wxRect t_region)
{
    wxBitmap window;

    LogIt("Rendering blur...");
    //dc.DestroyClippingRegion();
    LogIt(wxString::Format("Region(%d, %d, %d, %d)", t_region.x, t_region.y, t_region.width, t_region.height));
    window = m_disassemble_window_bitmap.GetSubBitmap(t_region);
    window = window.ConvertToDisabled(128);
    //*window = ;
    t_dc.DrawBitmap(window, t_region.GetPosition());
}



/// @brief Tests the blur effect on the background.
void CodeView::testBlur()
{
    wxClientDC dc(this);

    renderBackgroundBlur(dc, GetClientRect());
}



/// @brief Paints the background from line.
/// @param t_dc The device context (wxwidgets). A region where graphics and text can be drawn.
/// @param t_start_y Initial vertical coordinate.
/// @param t_fromline Initial line of code
/// @param t_toline  Last line of code
/// @param t_backcolour Color
void CodeView::paintBackground(wxDC &t_dc, const int t_start_y, const int t_fromline, const int t_toline, const wxBrush t_backcolour)
{
    int i, y;

    wxSize sz = GetClientSize();

    t_dc.SetPen(*wxTRANSPARENT_PEN);

    for (i = 0; i < t_toline; i++)
    {
        if (m_multi_selection &&
           ((t_fromline + i) >= m_line_info.firstLine) &&
           ((t_fromline + i) <= m_line_info.lastLine))
            t_dc.SetBrush(wxBrush(m_color_selected_item));
        else
            t_dc.SetBrush(t_backcolour);

        y = (i * m_font_height) + t_start_y;
        t_dc.DrawRectangle(0, y, sz.x, m_font_height);
    }
}



/// @brief Event to render the view.
/// @param t_event Event info
void CodeView::onPaint(wxPaintEvent& t_event)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    wxAutoBufferedPaintDC dc(this);
    DoPrepareDC(dc);
    updateLastCursorRect();
    onDraw(dc);
    updateVirtualSize();
}



CodeView::~CodeView()
{
    delete m_last_cursor_rect;
    delete m_font;
}




//TODO: Revise it
void CodeView::clear()
{
    clearSelection();
    m_is_focused = false;
    m_is_empty = true;
/*
    TODO: Since CodeView won't destroy Disassembled and Sourcecode, check if the real owner will do it.
    if (m_process->Disassembled != 0)
        m_process->Disassembled->Clear();

    m_sourcecode->Clear();
*/
    Refresh();
}



/// @brief Clears the selection controls.
void CodeView::clearSelection()
{
    resetSelectedItemInfo();
    m_multi_selection = false;
    m_selecting = false;
}



/// @brief Enables or disables the viewer.
/// @param enable True to enable, false to disable
/// @return 
void CodeView::enable(bool enable)
{
    if (enable)
    {
        wxScrolledCanvas::Enable(true);
        SetVirtualSize(wxDefaultCoord, m_sourcecode->getCount() * m_font_height);
        //m_is_empty = m_sourcecode->isEmpty();
    }
    else
    {
    	SetVirtualSize(0, 0);
        wxScrolledCanvas::Enable(false);
    }
}


// Receives device coordenate

/// @brief Calculates the cursor position
/// @param point The device coordinate.
void CodeView::calcCursorPosition(wxPoint point)
{
    int cursor;

    CalcUnscrolledPosition(point.x,point.y,&point.x,&point.y);
    cursor = int(point.y / m_font_height);

    if (cursor <= getLastLine())
        m_line_info.cursorPosition = cursor;
}



/// @brief Calculates the region of the cursor rectangle to prepare to refresh.
/// @return The region of the cursor rectangle
wxRect CodeView::calcCursorRfshRect()
{
    wxRect cursor(0, m_line_info.cursorPosition * m_font_height, GetClientSize().x, m_font_height);
    CalcScrolledPosition(cursor.x, cursor.y, 0, &cursor.y);
    return (cursor);
}



/// @brief Performs the cursor selection
void CodeView::doSelection()
{
    if (m_selecting)
    {
        updateSelectedRect();
        if (m_multi_selection)
            RefreshRect(calcSelectedRect());
    }
    else
    {
        if (m_multi_selection)
            RefreshRect(calcSelectedRect());

        m_line_info.selectedLineCount = 1;

        m_multi_selection = false;
        m_line_info.firstLine = m_line_info.cursorPosition;
        m_line_info.cursorLastPosition = m_line_info.cursorPosition;
        m_line_info.lastLine = m_line_info.cursorPosition;
    }
}



/// @brief Calculates the selection lines.
void CodeView::updateSelectedRect()
{
    int index;

    if (m_line_info.cursorPosition >= m_line_info.cursorLastPosition)
    {
        index = m_line_info.cursorLastPosition;
        m_line_info.selectedLineCount = m_line_info.cursorPosition - m_line_info.cursorLastPosition + 1;
    }
    else
    {
        index = m_line_info.cursorPosition;
        m_line_info.selectedLineCount = m_line_info.cursorLastPosition - m_line_info.cursorPosition + 1;
    }

    m_line_info.firstLine = index;
    m_line_info.lastLine = index + m_line_info.selectedLineCount - 1;
    if (m_line_info.selectedLineCount > 1)
        m_multi_selection = true;
    else
        m_multi_selection = false;

}



/// @brief Calculates the selection's rectangle.
/// @return The calculated rectangle
wxRect CodeView::calcSelectedRect()
{
    wxRect rect;
    int count;
    uint y;

    if (m_line_info.firstLine < getFirstLine()) {
        y = getFirstLine();
        count = getFirstLine() - m_line_info.firstLine;
    }
    else {
        y = m_line_info.firstLine;
        count = m_line_info.selectedLineCount;
    }
    if ((y + count) >= (getFirstLine() + m_lines_shown)) {
        count = getLastLine();
    }
    rect.x = 0;
    rect.y = y * m_font_height;
    rect.width = GetClientSize().x;
    rect.height = count * m_font_height;
    CalcScrolledPosition(rect.x, rect.y, 0, &rect.y);
    return rect;
}



/// @brief Calculates the uncovered area after a resize window.
void CodeView::calcIncompleteArea()
{
    m_incomplete_area.x = 0;
    m_incomplete_area.y = m_lines_shown * m_font_height + 1;
    m_incomplete_area.width = GetClientSize().GetWidth();
    m_incomplete_area.height = GetClientSize().GetHeight() - m_incomplete_area.y;
}



/// @brief Centralizes the given address in the viewer and selects it.
/// @param address 
void CodeView::centerAddress(uint address)
{
    int     line,
            firstlineshown,
            lastlineshown,
            position;
    bool    needscroll = false;

    firstlineshown = getFirstLine();
    lastlineshown = firstlineshown + m_lines_shown - 1;

    line = m_sourcecode->getLineOfAddress(address);

    if (line >= 0)
    {
        position = m_lines_shown / 2;
        if ((line >= firstlineshown) && (line <= lastlineshown))
        {
            m_line_info.cursorLastPosition = m_line_info.cursorPosition;
            m_line_info.cursorPosition = line;
        }
        else
        {
            m_line_info.cursorLastPosition = line;
            m_line_info.cursorPosition = line;
            needscroll = true;
        }

        RefreshRect(*m_last_cursor_rect);

        if (needscroll)
            Scroll(-1, (line - position));
        RefreshRect(calcCursorRfshRect());
    }
}


/*DEPRECATED: Use Refresh()
void CodeView::Plot(void)
{
    Refresh();
}
*/



/// @brief Calculates the number of lines of code that can be shown in the viewer.
void CodeView::calcLinesShown()
{
    wxSize size = GetClientSize();
    m_lines_shown = size.GetHeight() / m_font_height;
}



/// @brief Gets the first visible line
/// @return in logical scroll units (wxScrolled)
int CodeView::getFirstLine()
{
    int ret;
    GetViewStart(NULL, &ret);
    return ret;
}



/// @brief Gets the number of the last line of code
/// @return The number of the last line of code
int CodeView::getLastLine()
{
    return (getFirstLine() + m_lines_shown - 1);
}



/// @brief Gets the index of the last line of code
/// @return The index of the last line of code
int CodeView::getLastItemIndex()
{
    return (m_sourcecode->getCount() - 1);
}



/// @brief Gets the correct label to the argument
/// @param t_dasmeditem The disassembled item
/// @param t_argument_index The argument's index
/// @param t_argument The argument
/// @return The label string or an empty string if the label was not found
wxString CodeView::getArgumentLabel(const DisassembledItem *t_dasmeditem, const unsigned int &t_argument_index, const unsigned int &t_argument)
{
    wxString stringlabel;

    switch (t_dasmeditem->getMnemonic()->GetArgument(t_argument_index).type)
    {
        case OT_RELATIVE_ADDRESS:
        case OT_ABSOLUTE_ADDRESS:
                        m_process->m_labels->prog_labels->getLabel(t_argument, stringlabel);
                        break;
        case OT_IO_ADDRESS:
                        m_process->m_labels->io_labels->getLabel(t_argument, stringlabel);
                        break;
        case OT_VARIABLE:
                        m_process->m_labels->var_labels->getLabel(t_argument, stringlabel);
                        break;
        case OT_NONE:
        case OT_DATA:
        case OT_OFFSET:
                        break;
    }
    return stringlabel;
}



/// @brief Updates the rectangle corresponding to the last cursor position.
void CodeView::updateLastCursorRect()
{
    if (m_last_cursor_rect != 0) {
        wxCoord width;
        GetClientSize(&width, 0);

        m_last_cursor_rect->x = 2;
        m_last_cursor_rect->y = m_line_info.cursorPosition * m_font_height;
        m_last_cursor_rect->width = width - 4;
        m_last_cursor_rect->height = m_font_height;
        CalcScrolledPosition(0, m_last_cursor_rect->y, 0, &m_last_cursor_rect->y);  // Translates the logical coordinates to the device ones.
    }
}



/// @brief Updates the size, in device units, of the scrollable window area.
void CodeView::updateVirtualSize()
{
	if (IsEnabled())
	{
		wxSize sz = GetVirtualSize();
		sz.y = m_sourcecode->getCount() * m_font_height;
		SetVirtualSize(sz);
	}
}



/// @brief Updates or creates the argument's rectangle dimensions
/// @param t_rectangle Pointer to pointer to the rectangle
/// @param t_x X position
/// @param t_y Y position
/// @param t_width Width
/// @param t_height Height
void CodeView::updateRectangle(wxRect** t_rectangle, int t_x, int t_y, int t_width, int t_height)
{
    wxRect* rectangle;

    if (*t_rectangle == nullptr) {
        *t_rectangle = new wxRect(t_x, t_y, t_width, t_height);
    }
    else {
        rectangle = *t_rectangle;
        rectangle->SetX(t_x);
        rectangle->SetY(t_y);
        rectangle->SetHeight(t_height);
        rectangle->SetWidth(t_width);
    }
}



/// @brief Draws the cursor
/// @param dc Device context
void CodeView::showCursor(wxDC &dc)
{
    wxRect cursor;
    updateLastCursorRect();
    cursor = *m_last_cursor_rect;
    CalcUnscrolledPosition(cursor.x, cursor.y, 0, &cursor.y);   //Translates the device coordinates(pixel) to the logical ones.
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxLIGHT_GREY_BRUSH);
    dc.DrawRectangle(cursor);
}



/// @brief Marks the rectangle of the last cursor position, to be repainted.
void CodeView::clearCursor()
{
    if ((m_line_info.cursorLastPosition >= 0) && (m_line_info.cursorLastPosition <= getLastLine()))
        RefreshRect(*m_last_cursor_rect);
}



/// @brief Gets the type of line code into the selection range lines and if it has comments
/// @param hcomment Return if the line has comment
/// @return Type of the line of code
LineType CodeView::getTypesInSelection(bool &hcomment)

{
    SourceCodeLine      *sc_line;
    DisassembledItem    *dasmed_item;
    int line;
    bool homogeneous = false;
    LineType lastitem = lt_Unknown;


    for (line = m_line_info.firstLine; line < m_line_info.lastLine; ++line) {
        sc_line = m_sourcecode->line(line);
        if (sc_line->dasmedItem >= 0) {
			if (sc_line->comment) {
				hcomment = true;
            }

            dasmed_item = m_sourcecode->getDisassembled()->GetData(sc_line->dasmedItem);
            if (dasmed_item->isData()) {
                if ((lastitem == lt_Unknown) || (lastitem == lt_Data))
                {
                    lastitem = lt_Data;
                    homogeneous = true;
                }
                else
                    homogeneous = false;
            }
            else {  // Instruction
                if ((lastitem == lt_Unknown) || (lastitem == lt_Instruction))
                {
                    lastitem = lt_Instruction;
                    homogeneous = true;
                }
                else
                    homogeneous = false;
            }

            if ((lastitem != lt_Unknown) && (!homogeneous))
                break;
        }
    }
    
    if ((lastitem != lt_Unknown) && homogeneous) {
        return lastitem;
    }

    return (lt_Unknown);
}



/// @brief Create a pop up menu based on the type of multiple lines of code.
/// @param popup The wxMenu to be filled.
void CodeView::createPopupMenuMultiSelection(wxMenu *popup)
{
    bool    hascomments;
    wxMenu  *organizeDataSubMenu = 0;

    switch (getTypesInSelection(hascomments))
    {
        case lt_Instruction:
                        popup->Append(idPOPUP_MAKEDATA, "Make data\td");
                        break;
        case lt_Data:
                        organizeDataSubMenu = new wxMenu();
                        popup->Append(idPOPUP_DISASM, "Disassemble\tc");
                        organizeDataSubMenu->Append(idPOPUP_OD_STRING, "String");
                        organizeDataSubMenu->Append(idPOPUP_OD_MATRIX, "Matrix");
                        organizeDataSubMenu->Append(idPOPUP_OD_NUMBER, "Number");
                        popup->AppendSeparator();
                        popup->Append(idPOPUP_ORGANIZEDATA, "Organize data", organizeDataSubMenu);
                        break;
        default:
                        popup->Append(idPOPUP_MAKEDATA, "Make it data\td");
                        break;
    }
    if (hascomments)
    {
        popup->AppendSeparator();
        popup->Append(idPOPUP_DELCOMMENT,  "Del comments");
    }
}



/// @brief Create a pop up menu based on the type of the line of code.
/// @param popup The wxMenu to be filled.
void CodeView::createPopupMenuSingleSelection(wxMenu *popup)
{
    wxMenu		*argStyleSubMenu = 0,
				*labelMenu = 0;

    popup->Append(idPOPUP_GOTO_ADDRESS, "Goto address...");

    switch(m_line_info.type)
    {
        case 	lt_InstructionLabel:
                                if ((m_line_info.dasmitem) &&
                                    (m_line_info.dasmitem->getMnemonic()->GetGroup() == GRP_CALL
                                    || m_line_info.dasmitem->getMnemonic()->GetGroup() == GRP_JUMP))
                                {
                                    popup->Append(idPOPUP_GOTO, "Goto label");
                                    popup->AppendSeparator();
                                }
        case	lt_LineLabelProg:
        case	lt_LineLabelVar:
                                labelMenu = new wxMenu();
                                labelMenu->Append(idPOPUP_EDITLABEL, "Edit");
                                labelMenu->AppendSeparator();
                                labelMenu->Append(idPOPUP_DELLABEL, "Delete");

        case	lt_Instruction:
                                if ((m_line_info.type != lt_LineLabelProg) &&
                                    (m_line_info.type != lt_LineLabelVar))
                                    popup->Append(idPOPUP_MAKEDATA, "Make data");
                                break;
        case	lt_Data:
                                labelMenu = new wxMenu();
                                labelMenu->Append(idPOPUP_CREATELABEL, "Create");

                                popup->Append(idPOPUP_DISASM, "Disassemble");

        case    lt_Unknown:
        case    lt_Comments:
                                break;
    }

    if (labelMenu)
        popup->Append(idPOPUP_LBL, "Label", labelMenu);

    // Clicked over an argument
    if (m_line_info.argSelected > 0)
    {
        argStyleSubMenu = new wxMenu();

        argStyleSubMenu->Append(idPOPUP_ARG_BIN, "Binary");
        argStyleSubMenu->Append(idPOPUP_ARG_DEC, "Decimal");
        argStyleSubMenu->Append(idPOPUP_ARG_HEX, "Hexadecimal");
        popup->Append(idPOPUP_ARG_STYLE, "Style data", argStyleSubMenu);
    }

    popup->AppendSeparator();
    popup->Append(idPOPUP_SEARCH_ARGUMENT, "Search");
    popup->AppendSeparator();

    if ((m_line_info.type == lt_Comments) || (m_line_info.hasComment))
    {
        popup->Append(idPOPUP_EDITCOMMENT, "Edit comment");
        popup->Append(idPOPUP_DELCOMMENT, "Del comment");
    }
    else
        popup->Append(idPOPUP_ADDCOMMENT, "Add comment");
}



/// @brief Fills the structure m_line_info
/// @param pt Mouse coordinates
void CodeView::fillSelectedItemInfo(const wxPoint &pt)
{
	wxPoint mousept;

	CalcUnscrolledPosition(pt.x, pt.y, &mousept.x, &mousept.y);

	m_line_info.type = lt_Unknown;

    m_line_info.firstInstruction = -1;
    m_line_info.firstAddress = -1;
    m_line_info.lastInstruction = -1;
    m_line_info.lastAddress = -1;

    if (m_line_info.selectedLineCount == 1)
        treatAsSingleSelection();
    if (m_line_info.selectedLineCount > 1)
        treatAsMultiSelection();

    identifyArgumentSelected(mousept);

    //LogIt(wxString::Format("First: line = %d, address = 0x%X\nLast: line = %d, address = %X", m_line_info.firstLine, m_line_info.firstAddress, m_line_info.lastLine, m_line_info.lastAddress));
}



/// @brief Resets all members of m_line_info.
void CodeView::resetSelectedItemInfo()
{
    m_line_info.type = lt_Unknown;
    m_line_info.dasmitem = 0;
    m_line_info.lineitem = 0;
    m_line_info.hasComment = false;
    m_line_info.argSelected = 0;
    m_line_info.firstAddress = -1;
    m_line_info.firstInstruction = -1;
    m_line_info.firstLine = -1;
    m_line_info.lastAddress = -1;
    m_line_info.lastInstruction = -1;
    m_line_info.lastLine = -1;
    m_line_info.selectedLineCount = 0;
    m_line_info.cursorLastPosition = -1;
    m_line_info.cursorPosition = -1;
}



/// @brief Identifies the argument number under the mouse.
/// @param mouse_cursor Mouse pointer coordinate.
void CodeView::identifyArgumentSelected(const wxPoint &mouse_cursor)
{
    if (m_line_info.lineitem)
    {
        if (m_line_info.lineitem->rectArg1)
        {
            if (m_line_info.lineitem->rectArg1->Contains(mouse_cursor))
                m_line_info.argSelected = 1;
            else
                m_line_info.argSelected = 0;
        }

        if (m_line_info.lineitem->rectArg2)
            if (m_line_info.lineitem->rectArg2->Contains(mouse_cursor))
                m_line_info.argSelected = 2;
    }
}



/// @brief Fills m_line_info considering just one selected line.
void CodeView::treatAsSingleSelection()
{
	bool    testcomment = false;

	m_line_info.lineitem = m_sourcecode->line(m_line_info.cursorPosition);
	if (m_line_info.lineitem)
	{
		// Does the line have comment ?
		if (m_line_info.lineitem->comment)
			m_line_info.hasComment = true;
		else
			m_line_info.hasComment = false;

		// If line is a program label or a var label
		if (m_line_info.lineitem->labelProgAddress >= 0)
			m_line_info.type = lt_LineLabelProg;

		if (m_line_info.lineitem->labelVarAddress >= 0)
			m_line_info.type = lt_LineLabelVar;

		// check if line has instruction
        m_line_info.dasmitem = m_sourcecode->getDisassembled()->GetData(m_line_info.lineitem->dasmedItem);
        if (m_line_info.dasmitem)
        {
            if ((m_line_info.dasmitem->getArgumentStyle(0) == STYLE_LABELED)
                || (m_line_info.dasmitem->getArgumentStyle(1) == STYLE_LABELED))
                m_line_info.type = lt_InstructionLabel;
            else
                m_line_info.type = lt_Instruction;

            if (m_line_info.dasmitem->isData())
                m_line_info.type = lt_Data;
            m_line_info.firstInstruction = m_line_info.lineitem->dasmedItem;
            m_line_info.lastInstruction = m_line_info.firstInstruction;
            m_line_info.firstAddress = m_sourcecode->getDisassembled()->GetBaseAddress(m_line_info.firstInstruction) + m_line_info.dasmitem->getOffsetInFile();
            m_line_info.lastAddress = m_line_info.firstAddress;
        }

		// Test if line has only comments
		testcomment = ((m_line_info.lineitem->originAddress + m_line_info.lineitem->dasmedItem) == -2)
                        && !m_line_info.lineitem->labelProgAddress
                        && !m_line_info.lineitem->labelVarAddress;

		if (testcomment)
			m_line_info.type = lt_Comments;
	}
}




/// @brief Fills m_line_info considering multi lines selected.
void CodeView::treatAsMultiSelection()
{
    DisassembledItem *last_disassembled;
    SourceCodeLine *last_line;
    int first_instruction, last_instruction;
    bool    first_found,
            last_found;


    for (first_instruction = m_line_info.firstLine; first_instruction <= m_line_info.lastLine; ++first_instruction) {
        m_line_info.lineitem = m_sourcecode->line(first_instruction);
        first_found = (m_line_info.lineitem && (m_line_info.lineitem->dasmedItem >= 0));
        if (first_found)
            break;
    }

    for (last_instruction = m_line_info.lastLine; last_instruction > first_instruction; --last_instruction) {
        last_line = m_sourcecode->line(last_instruction);
        last_found = (last_line && (last_line->dasmedItem >= 0));
        if (last_found)
            break;
    }

    if (first_found && last_found) {
        m_line_info.firstInstruction = m_line_info.lineitem->dasmedItem;
        m_line_info.lastInstruction = last_line->dasmedItem;
        m_line_info.dasmitem = m_sourcecode->getDisassembled()->GetData(m_line_info.firstInstruction);
        last_disassembled = m_sourcecode->getDisassembled()->GetData(m_line_info.lastInstruction);
        m_line_info.firstAddress = m_sourcecode->getDisassembled()->GetBaseAddress(m_line_info.firstInstruction) + m_line_info.dasmitem->getOffsetInFile();
        m_line_info.lastAddress = m_sourcecode->getDisassembled()->GetBaseAddress(m_line_info.lastInstruction) + last_disassembled->getOffsetInFile();
    }

    m_line_info.hasComment = false;
}

