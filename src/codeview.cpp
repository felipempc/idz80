/****************************************************************
 * Name:      CodeView
 * Purpose:   User interface for the disassembled code
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe MPC ()
 * License:   GPL
 **************************************************************/

#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/utils.h>
#include <wx/region.h>
#include <wx/textdlg.h>
#include <wx/dcclient.h>
#include "codeview.h"
#include "disassembled_item.h"



const int CodeView::COL_ADDRESS;
const int CodeView::COL_CODE;
const int CodeView::COL_ASCII;
const int CodeView::COL_LABEL;
const int CodeView::COL_MNEM;



CodeView::CodeView(wxWindow *parent, ProcessData *processparent, LogWindow *logparent)
        : wxScrolledCanvas(parent)
{
    Process = processparent;
    m_linesShown = 0;

    MultiSelection = false;
    Selecting = false;
    m_fontHeight = 1;
    IsFocused = false;
    IsEmpty = true;

    m_styleData.arg = 0;
    m_styleData.item = 0;
    LastCursorRect = 0;

    ResetSelectedItemInfo();

    m_CodeViewLine = Process->CodeViewLines;
    LastCursorRect = new wxRect();
    IncompleteArea.x = 0;
    IncompleteArea.y = 0;
    IncompleteArea.width = 0;
    IncompleteArea.height = 0;

    SetupFont();

    SetScrollRate(0, m_fontHeight);
    SetVirtualSize(wxDefaultCoord, m_fontHeight);

    SetupColors();

    SetupEvents();


    // Debug Area
    SetTextLog(logparent);
    ModuleName = "CodeView";

}



void CodeView::SetupFont()
{
    wxClientDC dc(this);

    font = new wxFont(9, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL,
            wxFONTWEIGHT_NORMAL, false, "Courier New 10 Pitch");
    dc.SetFont(*font);
    m_fontHeight = dc.GetTextExtent("X").GetHeight();
    m_fontWidth = dc.GetTextExtent("X").GetWidth();
}




void CodeView::SetupColors()
{
    BackGroundColor.Set("WHITE");
    ForeGroundColor.Set("BLACK");
    SelectedItemColor.Set("YELLOW");
    BGArgumentColor.Set("YELLOW");
    FGArgumentColor.Set("GOLD");
    FG_TextColor.Set("BLACK");
    FG_LabelColor.Set("BLUE");
}




void CodeView::SetupEvents()
{
    Bind(wxEVT_SIZE, &CodeView::OnSize, this);
    Bind(wxEVT_SCROLLWIN_LINEDOWN, &CodeView::OnScrollLineDown, this);
    Bind(wxEVT_SCROLLWIN_LINEUP, &CodeView::OnScrollLineUp, this);
    Bind(wxEVT_SCROLLWIN_PAGEDOWN, &CodeView::OnScrollPageDown, this);
    Bind(wxEVT_SCROLLWIN_PAGEUP, &CodeView::OnScrollPageUp, this);


    Bind(wxEVT_PAINT, &CodeView::OnPaint, this);

    Bind(wxEVT_SET_FOCUS, &CodeView::OnGetFocus, this);
    Bind(wxEVT_KILL_FOCUS, &CodeView::OnKillFocus, this);
    Bind(wxEVT_MOUSE_CAPTURE_LOST, &CodeView::OnMouseCaptureLost, this);


    // MOUSE Events
    Bind(wxEVT_LEFT_DOWN, &CodeView::OnMouseLeftDown, this);
    Bind(wxEVT_LEFT_UP, &CodeView::OnMouseLeftUp, this);
    Bind(wxEVT_RIGHT_DOWN, &CodeView::OnMouseRightDown, this);
    Bind(wxEVT_RIGHT_UP, &CodeView::OnMouseRightUp, this);
    //TODO: Improve focus highlight
    Bind(wxEVT_MOTION, &CodeView::OnMouseMove, this);
    Bind(wxEVT_MOUSEWHEEL, &CodeView::OnMouseWheel, this);
    //Bind(wxEVT_ENTER_WINDOW, &CodeView::OnMouseEnterWindow, this);
    //Bind(wxEVT_LEAVE_WINDOW, &CodeView::OnMouseLeaveWindow, this);

    // KEYBOARD events
    Bind(wxEVT_KEY_DOWN, &CodeView::OnKeyPress, this);
    Bind(wxEVT_KEY_UP, &CodeView::OnKeyRelease, this);

    // Popup event connections
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpAddComment, this, idPOPUP_ADDCOMMENT);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpEditComment, this, idPOPUP_EDITCOMMENT);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpDelComment, this, idPOPUP_DELCOMMENT);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpMenuGoto, this, idPOPUP_GOTO);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpMenuGotoAddress, this, idPOPUP_GOTO_ADDRESS);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpMenuMakeData, this, idPOPUP_MAKEDATA);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpMenuDisasm, this, idPOPUP_DISASM);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpMenuOD_Matrix, this, idPOPUP_OD_MATRIX);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpMenuOD_String, this, idPOPUP_OD_STRING);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpMenuOD_Number, this, idPOPUP_OD_NUMBER);

    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpMenuArgStyleBin, this, idPOPUP_ARG_BIN);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpMenuArgStyleDec, this, idPOPUP_ARG_DEC);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpMenuArgStyleHex, this, idPOPUP_ARG_HEX);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpMenuRenLabel, this, idPOPUP_EDITLABEL);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpMenuDelLabel, this, idPOPUP_DELLABEL);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpMenuCreateLabel, this, idPOPUP_CREATELABEL);
}





int CodeView::GetCount()
{
    return m_CodeViewLine->GetCount();
}




void CodeView::OnDraw(wxDC &dc)
{
    int			fromLine, totalLines, fixedY, fixedHeight;
    wxRegionIterator
				regIterator(GetUpdateRegion());
    wxRect		touchedRegion, fullRect;


    dc.SetFont(*font);
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
    fromLine = fullRect.y / m_fontHeight;
	fixedY = fromLine * m_fontHeight;
	fixedHeight = fullRect.height + fullRect.y - fixedY;
    totalLines = fixedHeight / m_fontHeight;
    if ((fixedHeight % m_fontHeight) != 0)
        totalLines++;

    fullRect.y = fixedY;
    fullRect.height = totalLines * m_fontHeight;
    fullRect.width = GetClientSize().GetWidth();

	if (IsEmpty)
    {
        PaintBackground(dc, fullRect.y, fromLine, totalLines, *wxGREY_BRUSH);
    }
    else
    {
        if (IsEnabled())
        {
            PaintBackground(dc, fullRect.y, fromLine, totalLines, *wxWHITE_BRUSH);
            if ((fromLine == line_info.cursorPosition) || ((line_info.cursorPosition >= fromLine) &&
                (line_info.cursorPosition < (fromLine + totalLines))))
                ShowCursor(dc);
            Render(dc, fullRect.y, fromLine, totalLines);
        }
        else
        {
            RenderBackGroundBlur(dc, fullRect);
        }
    }
}



void CodeView::RenderBackGroundBlur(wxDC &dc, wxRect region)
{
    wxBitmap window;

    LogIt("Rendering blur...");
    //dc.DestroyClippingRegion();
    LogIt(wxString::Format("Region(%d, %d, %d, %d)", region.x, region.y, region.width, region.height));
    window = DisassembleWindowBitmap.GetSubBitmap(region);
    window = window.ConvertToDisabled(128);
    //*window = ;
    dc.DrawBitmap(window, region.GetPosition());
}


void CodeView::TestBlur()
{
    wxClientDC dc(this);

    RenderBackGroundBlur(dc, GetClientRect());
}


void CodeView::PaintBackground(wxDC &dc, const int start_y, const int fromline, const int toline, const wxBrush backcolour)
{
    int i, y;

    wxSize sz = GetClientSize();

    dc.SetPen(*wxTRANSPARENT_PEN);

    for (i = 0; i < toline; i++)
    {
        if (MultiSelection &&
           ((fromline + i) >= line_info.firstLine) &&
           ((fromline + i) <= line_info.lastLine))
            dc.SetBrush(wxBrush(SelectedItemColor));
        else
            dc.SetBrush(backcolour);

        y = (i * m_fontHeight) + start_y;
        dc.DrawRectangle(0, y, sz.x, m_fontHeight);
    }
}




void CodeView::OnPaint(wxPaintEvent& event)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    wxAutoBufferedPaintDC dc(this);
    DoPrepareDC(dc);
    UpdateLastCursorRect();
    OnDraw(dc);
    UpdateVirtualSize();
}



CodeView::~CodeView()
{
    delete LastCursorRect;
    delete font;
}




void CodeView::Clear()
{
    ClearSelection();
    IsFocused = false;
    IsEmpty = true;
    if (Process->Disassembled != 0)
        Process->Disassembled->Clear();

    m_CodeViewLine->Clear();
    Refresh();
}



void CodeView::ClearSelection()
{
    ResetSelectedItemInfo();
    MultiSelection = false;
    Selecting = false;
}



bool CodeView::Enable(bool enable)
{
    bool ret = false;

    if (enable)
    {
        wxScrolledCanvas::Enable(true);
        SetVirtualSize(wxDefaultCoord, m_CodeViewLine->GetCount() * m_fontHeight);
        IsEmpty = m_CodeViewLine->IsEmpty();
        ret = true;
    }
    else
    {
    	SetVirtualSize(0, 0);
        wxScrolledCanvas::Enable(false);
    }
    return ret;
}

// Receives device coordenate
void CodeView::CalcCursorPosition(wxPoint point)
{
    int cursor;

    CalcUnscrolledPosition(point.x,point.y,&point.x,&point.y);
    cursor = int(point.y / m_fontHeight);

    if (cursor <= GetLastLine())
        line_info.cursorPosition = cursor;
}

wxRect CodeView::CalcCursorRfshRect()
{
    wxRect cursor(0, line_info.cursorPosition * m_fontHeight, GetClientSize().x, m_fontHeight);
    CalcScrolledPosition(cursor.x, cursor.y, 0, &cursor.y);
    return (cursor);
}



void CodeView::DoSelection()
{
    if (Selecting)
    {
        UpdateSelectedRect();
        if (MultiSelection)
            RefreshRect(CalcSelectedRect());
    }
    else
    {
        if (MultiSelection)
            RefreshRect(CalcSelectedRect());

        line_info.selectedLineCount = 1;

        MultiSelection = false;
        line_info.firstLine = line_info.cursorPosition;
        line_info.cursorLastPosition = line_info.cursorPosition;
        line_info.lastLine = line_info.cursorPosition;
    }
}


void CodeView::UpdateSelectedRect()
{
    int index;

    if (line_info.cursorPosition >= line_info.cursorLastPosition)
    {
        index = line_info.cursorLastPosition;
        line_info.selectedLineCount = line_info.cursorPosition - line_info.cursorLastPosition + 1;
    }
    else
    {
        index = line_info.cursorLastPosition;
        line_info.selectedLineCount = line_info.cursorLastPosition - line_info.cursorPosition + 1;
    }

    line_info.firstLine = index;
    line_info.lastLine = index + line_info.selectedLineCount - 1;
    if (line_info.selectedLineCount > 1)
        MultiSelection = true;
    else
        MultiSelection = false;

}




// Returns the current Selected items' rectangle
wxRect CodeView::CalcSelectedRect()
{
    wxRect rect;
    int count;
    uint y;

    if (line_info.firstLine < GetFirstLine())
    {
        y = GetFirstLine();
        count = GetFirstLine() - line_info.firstLine;
    }
    else
    {
        y = line_info.firstLine;
        count = line_info.selectedLineCount;
    }
    if ((y + count) >= (GetFirstLine() + m_linesShown))
        count = GetLastLine();
    rect.x = 0;
    rect.y = y * m_fontHeight;
    rect.width = GetClientSize().x;
    rect.height = count * m_fontHeight;
    CalcScrolledPosition(rect.x, rect.y, 0, &rect.y);
    return rect;
}





void CodeView::CalcIncompleteArea()
{
    IncompleteArea.x = 0;
    IncompleteArea.y = m_linesShown * m_fontHeight + 1;
    IncompleteArea.width = GetClientSize().GetWidth();
    IncompleteArea.height = GetClientSize().GetHeight() - IncompleteArea.y;
}


/*
 * Take an address and show it
 * in the center of codeview
 */
void CodeView::CenterAddress(uint address)
{
    int     i,
            firstlineshown,
            lastlineshown,
            position;
    bool    needscroll = false;

    firstlineshown = GetFirstLine();
    lastlineshown = firstlineshown + m_linesShown - 1;

    m_CodeViewLine->getLineOfAddress(address, i);

    if (i >= 0)
    {
        position = m_linesShown / 2;
        if ((i >= firstlineshown) && (i <= lastlineshown))
        {
            line_info.cursorLastPosition = line_info.cursorPosition;
            line_info.cursorPosition = i;
        }
        else
        {
            line_info.cursorLastPosition = i;
            line_info.cursorPosition = i;
            needscroll = true;
        }

        RefreshRect(*LastCursorRect);

        if (needscroll)
            Scroll(-1, (i - position));
        RefreshRect(CalcCursorRfshRect());
    }
}




void CodeView::Plot(void)
{
    Refresh();
}


void CodeView::CalcItemsShown(void)
{
    wxSize size = GetClientSize();
    m_linesShown = size.GetHeight() / m_fontHeight;
}



int CodeView::GetFirstLine()
{
    int ret;
    GetViewStart(NULL, &ret);
    return ret;
}




int CodeView::GetLastLine()
{
    return (GetFirstLine() + m_linesShown - 1);
}


int CodeView::GetLastItem()
{
    return (Process->CodeViewLines->GetCount() - 1);
}


void CodeView::UpdateLastCursorRect()
{
    if (LastCursorRect != 0)
    {
        wxCoord width;
        GetClientSize(&width, 0);

        LastCursorRect->x = 2;
        LastCursorRect->y = line_info.cursorPosition * m_fontHeight;
        LastCursorRect->width = width - 4;
        LastCursorRect->height = m_fontHeight;
        CalcScrolledPosition(0, LastCursorRect->y, 0, &LastCursorRect->y);
    }
}



void CodeView::UpdateVirtualSize(void)
{
	if (IsEnabled())
	{
		wxSize sz = GetVirtualSize();
		sz.y = m_CodeViewLine->GetCount() * m_fontHeight;
		SetVirtualSize(sz);
	}
}



void CodeView::ShowCursor(wxDC &dc)
{
    wxRect cursor;
    UpdateLastCursorRect();
    cursor = *LastCursorRect;
    CalcUnscrolledPosition(cursor.x, cursor.y, 0, &cursor.y);
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxLIGHT_GREY_BRUSH);
    dc.DrawRectangle(cursor);
}



void CodeView::ClearCursor()
{
    if ((line_info.cursorLastPosition >= 0) && (line_info.cursorLastPosition <= GetLastLine()))
        RefreshRect(*LastCursorRect);
}






ElementType CodeView::GetTypeMultiselection(bool &hcomment)

{
    CodeViewItem *cvi;
    DisassembledItem *de;
    int i;
    bool homogeneous = false;
    ElementType lastitem = et_None;


    for (i = line_info.firstLine; i < line_info.lastLine; i++)
    {
        cvi = m_CodeViewLine->getData(i);
        if (cvi->Dasmitem >= 0)
        {
			if (cvi->Comment != 0)
				hcomment = true;

            de = Process->Disassembled->GetData(cvi->Dasmitem);
            if (de->IsInstruction())
            {
                if ((lastitem == et_None) || (lastitem == et_Instruction))
                {
                    lastitem = et_Instruction;
                    homogeneous = true;
                }
                else
                    homogeneous = false;
            }
            else
                if (de->IsData())
                {
                    if ((lastitem == et_None) || (lastitem == et_Data))
                    {
                        lastitem = et_Data;
                        homogeneous = true;
                    }
                    else
                        homogeneous = false;
                }
            if ((lastitem != et_None) && (!homogeneous))
                break;
        }
    }
    if ((lastitem != et_None) && homogeneous)
        return lastitem;
    return (et_None);
}




void CodeView::CreatePopupMenuMultiSelection(wxMenu *popup)
{
    bool    hascomments;
    wxMenu  *organizeDataSubMenu = 0;

    switch (GetTypeMultiselection(hascomments))
    {
        case et_Instruction:
                        popup->Append(idPOPUP_MAKEDATA, "Make data\td");
                        break;
        case et_Data:
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



void CodeView::CreatePopupMenuSingleSelection(wxMenu *popup)
{
    wxMenu		*argStyleSubMenu = 0,
				*labelMenu = 0;

    popup->Append(idPOPUP_GOTO_ADDRESS, "Goto address...");

    switch(line_info.type)
    {
        case 	siInstructionLabel:
                                if ((line_info.dasmitem) &&
                                    (line_info.dasmitem->mnemonic_->IsCall() || line_info.dasmitem->mnemonic_->IsJump()))
                                {
                                    popup->Append(idPOPUP_GOTO, "Goto label");
                                    popup->AppendSeparator();
                                }
        case	siLineLabelProg:
        case	siLineLabelVar:
                                labelMenu = new wxMenu();
                                labelMenu->Append(idPOPUP_EDITLABEL, "Edit");
                                labelMenu->AppendSeparator();
                                labelMenu->Append(idPOPUP_DELLABEL, "Delete");

        case	siInstruction:
                                if ((line_info.type != siLineLabelProg) &&
                                    (line_info.type != siLineLabelVar))
                                    popup->Append(idPOPUP_MAKEDATA, "Make data");
                                break;
        case	siData:
                                labelMenu = new wxMenu();
                                labelMenu->Append(idPOPUP_CREATELABEL, "Create");

                                popup->Append(idPOPUP_DISASM, "Disassemble");

        case    siUnknown:
        case    siComments:
                                break;
    }

    if (labelMenu)
        popup->Append(idPOPUP_LBL, "Label", labelMenu);

    // Clicked over an argument
    if (line_info.argSelected > 0)
    {
        argStyleSubMenu = new wxMenu();

        argStyleSubMenu->Append(idPOPUP_ARG_BIN, "Binary");
        argStyleSubMenu->Append(idPOPUP_ARG_DEC, "Decimal");
        argStyleSubMenu->Append(idPOPUP_ARG_HEX, "Hexadecimal");
        popup->Append(idPOPUP_ARG_STYLE, "Style data", argStyleSubMenu);
    }

    popup->AppendSeparator();

    if ((line_info.type == siComments) || (line_info.hasComment))
    {
        popup->Append(idPOPUP_EDITCOMMENT, "Edit comment");
        popup->Append(idPOPUP_DELCOMMENT, "Del comment");
    }
    else
        popup->Append(idPOPUP_ADDCOMMENT, "Add comment");

}





void CodeView::FillSelectedItemInfo(const wxPoint &pt)
{
	wxPoint         mousept;

	CalcUnscrolledPosition(pt.x, pt.y, &mousept.x, &mousept.y);

	line_info.type = siUnknown;

    line_info.firstInstruction = -1;
    line_info.firstAddress = -1;
    line_info.lastInstruction = -1;
    line_info.lastAddress = -1;

    if (line_info.selectedLineCount == 1)
        TreatSingleSelection();
    if (line_info.selectedLineCount > 1)
        TreatMultiSelection();

    IdentifyArgumentSelected(mousept);

    LogIt(wxString::Format("First: line = %d, address = 0x%X\nLast: line = %d, address = %X", line_info.firstLine, line_info.firstAddress, line_info.lastLine, line_info.lastAddress));
}





void CodeView::ResetSelectedItemInfo()
{
    line_info.type = siUnknown;
    line_info.dasmitem = 0;
    line_info.lineitem = 0;
    line_info.hasComment = false;
    line_info.argSelected = 0;
    line_info.firstAddress = -1;
    line_info.firstInstruction = -1;
    line_info.firstLine = -1;
    line_info.lastAddress = -1;
    line_info.lastInstruction = -1;
    line_info.lastLine = -1;
    line_info.selectedLineCount = 0;
    line_info.cursorLastPosition = -1;
    line_info.cursorPosition = -1;
}



// If clicked over an instruction argument, discover which was
void CodeView::IdentifyArgumentSelected(const wxPoint &mouse_cursor)
{
    if (line_info.lineitem)
    {
        if (line_info.lineitem->RectArg1)
        {
            if (line_info.lineitem->RectArg1->Contains(mouse_cursor))
                line_info.argSelected = 1;
            else
                line_info.argSelected = 0;
        }

        if (line_info.lineitem->RectArg2)
            if (line_info.lineitem->RectArg2->Contains(mouse_cursor))
                line_info.argSelected = 2;
    }
}





void CodeView::TreatSingleSelection()
{
	bool    testcomment = false;

	line_info.lineitem = m_CodeViewLine->getData(line_info.cursorPosition);
	if (line_info.lineitem)
	{
		// Does the line have comment ?
		if (line_info.lineitem->Comment)
			line_info.hasComment = true;
		else
			line_info.hasComment = false;

		// If line is a program label or a var label
		if (line_info.lineitem->LabelProgAddr >= 0)
			line_info.type = siLineLabelProg;

		if (line_info.lineitem->LabelVarAddr >= 0)
			line_info.type = siLineLabelVar;

		// check if line has instruction
        line_info.dasmitem = Process->Disassembled->GetData(line_info.lineitem->Dasmitem);
        if (line_info.dasmitem)
        {
            if (line_info.dasmitem->HasArgumentLabel())
                line_info.type = siInstructionLabel;
            else
                line_info.type = siInstruction;

            if (line_info.dasmitem->IsData())
                line_info.type = siData;
            line_info.firstInstruction = line_info.lineitem->Dasmitem;
            line_info.lastInstruction = line_info.firstInstruction;
            line_info.firstAddress = Process->Disassembled->GetBaseAddress(line_info.firstInstruction) + line_info.dasmitem->GetOffset();
            line_info.lastAddress = line_info.firstAddress;
        }


		// Test if line has only comments
		testcomment = ((line_info.lineitem->Org + line_info.lineitem->LabelProgAddr + line_info.lineitem->LabelVarAddr +
						line_info.lineitem->Dasmitem) == -4);
		if (testcomment)
			line_info.type = siComments;
	}
}




void CodeView::TreatMultiSelection()
{
    DisassembledItem *last_disassembled;
    CodeViewItem *last_line;
    int first_instruction, last_instruction;
    bool    first_found,
            last_found;


    for (first_instruction = line_info.firstLine; first_instruction <= line_info.lastLine; first_instruction++)
    {
        line_info.lineitem = m_CodeViewLine->getData(first_instruction);
        first_found = (line_info.lineitem && (line_info.lineitem->Dasmitem >= 0));
        if (first_found)
            break;
    }

    for (last_instruction = line_info.lastLine; last_instruction > first_instruction; last_instruction--)
    {
        last_line = m_CodeViewLine->getData(last_instruction);
        last_found = (last_line && (last_line->Dasmitem >= 0));
        if (last_found)
            break;
    }

    if (first_found && last_found)
    {
        line_info.firstInstruction = line_info.lineitem->Dasmitem;
        line_info.lastInstruction = last_line->Dasmitem;
        line_info.dasmitem = Process->Disassembled->GetData(line_info.firstInstruction);
        last_disassembled = Process->Disassembled->GetData(line_info.lastInstruction);
        line_info.firstAddress = Process->Disassembled->GetBaseAddress(line_info.firstInstruction) + line_info.dasmitem->GetOffset();
        line_info.lastAddress = Process->Disassembled->GetBaseAddress(line_info.lastInstruction) + last_disassembled->GetOffset();
    }

    line_info.hasComment = false;

}



