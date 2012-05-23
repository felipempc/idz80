/****************************************************************
 * Name:      IDZ80
 * Purpose:   Defines Application Frame
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 *
 * This module shows/controls disassembled data
 **************************************************************/

#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/utils.h>
#include <wx/region.h>
#include "codeview.h"
#include "d_asm_element.h"



const int CodeView::COL_ADDRESS;
const int CodeView::COL_CODE;
const int CodeView::COL_ASCII;
const int CodeView::COL_LABEL;
const int CodeView::COL_MNEM;



CodeView::CodeView(wxWindow *parent, ProcessData *_proc)
        : wxScrolledWindow(parent)
{
    m_process = _proc;
    m_linesShown = 0;
    CursorPosition = -1;
    CursorLastPosition = -1;
    LastCursorRect = 0;
    SelectedItemIndex = -1;
    SelectedLastItem = -1;
    SelectedCount = -1;
    MultiSelection = false;
    Selecting = false;
    m_fontHeight = 1;
    IsFocused = false;
    IsEmpty = true;
    m_styleData.arg = 0;
    m_styleData.item = 0;

    m_iteminfo.type = siUnknown;
    m_iteminfo.dasmitem = (DAsmElement *)0;
    m_iteminfo.lineitem = (CodeViewItem *)0;
    m_iteminfo.hasComment = false;

    m_CodeViewLine = m_process->m_CodeViewLine;
    LastCursorRect = new wxRect();

    wxClientDC dc(this);

    font = new wxFont(9, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL,
            wxFONTWEIGHT_NORMAL, false, wxT("Courier New 10 Pitch"));
    dc.SetFont(*font);
    m_fontHeight = dc.GetTextExtent(_("X")).GetHeight();
    m_fontWidth = dc.GetTextExtent(_("X")).GetWidth();

    SetScrollRate(0,m_fontHeight);
    SetVirtualSize(wxDefaultCoord,m_fontHeight);

    BackGroundColor.Set(_("WHITE"));
    ForeGroundColor.Set(_("BLACK"));
    SelectedItemColor.Set(_("YELLOW"));
    BGArgumentColor.Set(_T("YELLOW"));
    FGArgumentColor.Set(_T("GOLD"));
    FG_TextColor.Set(_T("BLACK"));
    FG_LabelColor.Set(_T("BLUE"));

    //Connect(wxEVT_SCROLLWIN_LINEDOWN, wxScrollWinEventHandler(CodeView::OnScrollLineDown));
    Bind(wxEVT_SCROLLWIN_LINEDOWN, &CodeView::OnScrollLineDown, this);
    //Connect(wxEVT_SCROLLWIN_LINEUP, wxScrollWinEventHandler(CodeView::OnScrollLineUp));
    Bind(wxEVT_SCROLLWIN_LINEUP, &CodeView::OnScrollLineUp, this);
    //Connect(wxEVT_SCROLLWIN_PAGEDOWN, wxScrollWinEventHandler(CodeView::OnScrollPageDown));
    Bind(wxEVT_SCROLLWIN_PAGEDOWN, &CodeView::OnScrollPageDown, this);
    //Connect(wxEVT_SCROLLWIN_PAGEUP, wxScrollWinEventHandler(CodeView::OnScrollPageUp));
    Bind(wxEVT_SCROLLWIN_PAGEUP, &CodeView::OnScrollPageUp, this);
    //Connect(wxEVT_PAINT, wxPaintEventHandler(CodeView::OnPaint));
    Bind(wxEVT_PAINT, &CodeView::OnPaint, this);
    //Connect(wxEVT_ENTER_WINDOW,wxFocusEventHandler(CodeView::OnGetFocus));
    Bind(wxEVT_SET_FOCUS, &CodeView::OnGetFocus, this);
    //Connect(wxEVT_LEAVE_WINDOW,wxFocusEventHandler(CodeView::OnKillFocus));
    Bind(wxEVT_KILL_FOCUS, &CodeView::OnKillFocus, this);


    // MOUSE Events
    //Connect(wxEVT_SIZE, wxSizeEventHandler(CodeView::OnSize));
    Bind(wxEVT_SIZE, &CodeView::OnSize, this);
    //Connect(wxEVT_LEFT_DOWN,wxMouseEventHandler(CodeView::OnMouseLeftDown));
    Bind(wxEVT_LEFT_DOWN, &CodeView::OnMouseLeftDown, this);
    Bind(wxEVT_LEFT_UP, &CodeView::OnMouseLeftUp, this);
    //Connect(wxEVT_RIGHT_DOWN,wxMouseEventHandler(CodeView::OnMouseRightDown));
    Bind(wxEVT_RIGHT_DOWN, &CodeView::OnMouseRightDown, this);
    //Connect(wxEVT_RIGHT_UP,wxMouseEventHandler(CodeView::OnMouseRightUp));
    Bind(wxEVT_RIGHT_UP, &CodeView::OnMouseRightUp, this);
    Bind(wxEVT_MOTION, &CodeView::OnMouseMove, this);
    //Connect(wxEVT_MOUSEWHEEL,wxMouseEventHandler(CodeView::OnMouseWheel));
    Bind(wxEVT_MOUSEWHEEL, &CodeView::OnMouseWheel, this);

    // KEYBOARD events
    //Connect(wxEVT_KEY_DOWN,wxKeyEventHandler(CodeView::OnKeyPress));
    Bind(wxEVT_KEY_DOWN, &CodeView::OnKeyPress, this);
    //Connect(wxEVT_KEY_UP,wxKeyEventHandler(CodeView::OnKeyRelease));
    Bind(wxEVT_KEY_UP, &CodeView::OnKeyRelease, this);

    // Popup event connections
    //Connect(idPOPUP_ADDCOMMENT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&CodeView::OnPopUpAddComment);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpAddComment, this, idPOPUP_ADDCOMMENT);
    //Connect(idPOPUP_EDITCOMMENT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&CodeView::OnPopUpEditComment);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpEditComment, this, idPOPUP_EDITCOMMENT);
    //Connect(idPOPUP_DELCOMMENT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&CodeView::OnPopUpDelComment);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpDelComment, this, idPOPUP_DELCOMMENT);
    //Connect(idPOPUP_GOTO,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&CodeView::OnPopUpMenuGoto);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpMenuGoto, this, idPOPUP_GOTO);
    //Connect(idPOPUP_MAKEDATA,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&CodeView::OnPopUpMenuMakeData);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpMenuMakeData, this, idPOPUP_MAKEDATA);
    //Connect(idPOPUP_DISASM,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&CodeView::OnPopUpMenuDisasm);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpMenuDisasm, this, idPOPUP_DISASM);
    //Connect(idPOPUP_OD_MATRIX, wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&CodeView::OnPopUpMenuOD_Matrix);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpMenuOD_Matrix, this, idPOPUP_OD_MATRIX);
    //Connect(idPOPUP_OD_STRING, wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&CodeView::OnPopUpMenuOD_String);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpMenuOD_String, this, idPOPUP_OD_STRING);
    //Connect(idPOPUP_OD_NUMBER, wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&CodeView::OnPopUpMenuOD_Number);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpMenuOD_Number, this, idPOPUP_OD_NUMBER);

    //Connect(idPOPUP_ARG_BIN, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CodeView::OnPopUpMenuArgStyleBin);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpMenuArgStyleBin, this, idPOPUP_ARG_BIN);
    //Connect(idPOPUP_ARG_DEC, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CodeView::OnPopUpMenuArgStyleDec);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpMenuArgStyleDec, this, idPOPUP_ARG_DEC);
    //Connect(idPOPUP_ARG_HEX, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&CodeView::OnPopUpMenuArgStyleHex);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpMenuArgStyleHex, this, idPOPUP_ARG_HEX);
    //Connect(idPOPUP_EDITLABEL, wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&CodeView::OnPopUpMenuRenLabel);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpMenuRenLabel, this, idPOPUP_EDITLABEL);
    //Connect(idPOPUP_DELLABEL, wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&CodeView::OnPopUpMenuDelLabel);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CodeView::OnPopUpMenuDelLabel, this, idPOPUP_DELLABEL);

    PopUp = 0;

    // Debug Area
    TC_Log = 0;
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
    wxBrush		bkcolor(*wxWHITE_BRUSH);

    dc.SetFont(*font);
    dc.SetBackground(bkcolor);

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

	if (!IsEnabled())
		bkcolor = *wxGREY_BRUSH;

    PaintBackground(dc, fullRect.y, fromLine, totalLines, bkcolor);

    if (IsEnabled())
    {
        if ((fromLine == CursorPosition) || ((CursorPosition >= fromLine) &&
            (CursorPosition <(fromLine+totalLines))))
            ShowCursor(dc);
        Render(dc, fullRect.y, fromLine, totalLines);
    }
}




void CodeView::PaintBackground(wxDC &dc, const int start_y, const int fromline, const int toline, const wxBrush backcolour)
{
    int i, y;

    wxSize sz = GetClientSize();

    dc.SetPen(*wxTRANSPARENT_PEN);

    for (i = 0; i < toline; i++)
    {
        if (MultiSelection &&
           ((fromline + i) >= SelectedItemIndex) &&
           ((fromline + i) <= SelectedLastItem))
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


void CodeView::OnScrollLineDown(wxScrollWinEvent& event)
{
    Scroll(-1,GetFirstLine() + 1);
    RefreshRect(CalcCursorRfshRect());
}



void CodeView::OnScrollLineUp(wxScrollWinEvent& event)
{
    Scroll(-1,GetFirstLine() - 1);
}


void CodeView::OnScrollPageDown(wxScrollWinEvent& event)
{
    int page;

    if (GetFirstLine() < (m_CodeViewLine->GetCount() - m_linesShown))
    {
        page = CursorPosition;
        Scroll(-1,GetFirstLine() + m_linesShown);
        UpdateLastCursorRect();
        CursorLastPosition = GetLastLine();
        CursorPosition = CursorLastPosition;
        page = CursorPosition - page;
        if (page < m_linesShown)
            RefreshRect(*LastCursorRect);
    }
}


void CodeView::OnScrollPageUp(wxScrollWinEvent& event)
{
    int page;

    if (GetFirstLine() > 0)
    {
        page = CursorPosition;
        Scroll(-1,GetFirstLine() - m_linesShown);
        UpdateLastCursorRect();
        CursorLastPosition = GetFirstLine();
        CursorPosition = CursorLastPosition;
        page -= CursorPosition;
        if (page < m_linesShown)
            RefreshRect(*LastCursorRect);
    }
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
    if (!(m_process->m_Dasm == 0))
        m_process->m_Dasm->Clear();

    m_CodeViewLine->Clear();
    Refresh();
}



void CodeView::ClearSelection()
{
    SelectedCount = -1;
    SelectedItemIndex = -1;
    SelectedLastItem = -1;
    CursorPosition = -1;
    MultiSelection = false;
    Selecting = false;
}



bool CodeView::Enable(bool enable)
{
    bool ret = false;

    if (enable)
    {
        wxScrolledWindow::Enable(true);
        SetVirtualSize(wxDefaultCoord, m_CodeViewLine->GetCount() * m_fontHeight);
        IsEmpty = m_CodeViewLine->IsEmpty();
        ret = true;
    }
    else
    {
    	SetVirtualSize(0, 0);
        wxScrolledWindow::Enable(false);
    }
    return ret;
}

// Receives device coordenate
void CodeView::CalcCursorPosition(wxPoint point)
{
    CalcUnscrolledPosition(point.x,point.y,&point.x,&point.y);
    CursorPosition = int(point.y / m_fontHeight);
}

wxRect CodeView::CalcCursorRfshRect()
{
    wxRect cursor(0, CursorPosition * m_fontHeight, GetClientSize().x, m_fontHeight);
    CalcScrolledPosition(cursor.x, cursor.y, 0, &cursor.y);
    return (cursor);
}


void CodeView::UpdateSelectedRect()
{
    int index;

    if (CursorPosition >= CursorLastPosition)
    {
        index = CursorLastPosition;
        SelectedCount = CursorPosition-CursorLastPosition + 1;
    }
    else
    {
        index = CursorPosition;
        SelectedCount = CursorLastPosition-CursorPosition + 1;
    }
    SelectedItemIndex = index;
    SelectedLastItem = SelectedItemIndex + SelectedCount - 1;
    if (SelectedCount > 1)
        MultiSelection=true;
    else
        MultiSelection=false;
}

// Returns the current Selected items' rectangle
wxRect CodeView::CalcSelectedRect()
{
    wxRect rect;
    int y,count;
    if (SelectedItemIndex < GetFirstLine())
    {
        y = GetFirstLine();
        count = GetFirstLine() - SelectedItemIndex;
    }
    else
    {
        y = SelectedItemIndex;
        count = SelectedCount;
    }
    if ((y + count) >= (GetFirstLine() + m_linesShown))
        count = GetLastLine();
    rect.x = 0;
    rect.y = y * m_fontHeight;
    rect.width = GetClientSize().x;
    rect.height = count * m_fontHeight;
    CalcScrolledPosition(rect.x,rect.y,0,&rect.y);
    return rect;
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
    GetViewStart(NULL,&ret);
    return ret;
}


int CodeView::GetLastLine()
{
    return (GetFirstLine() + m_linesShown - 1);
}


void CodeView::UpdateLastCursorRect()
{
    if (LastCursorRect!=0)
    {
        wxCoord width;
        GetClientSize(&width, 0);

        LastCursorRect->x = 2;
        LastCursorRect->y = CursorPosition * m_fontHeight;
        LastCursorRect->width = width - 4;
        LastCursorRect->height = m_fontHeight;
        CalcScrolledPosition(0,LastCursorRect->y,0,&LastCursorRect->y);
    }
}


void CodeView::ShowCursor(wxDC &dc)
{
    wxRect cursor;
    UpdateLastCursorRect();
    cursor = *LastCursorRect;
    CalcUnscrolledPosition(cursor.x,cursor.y,0,&cursor.y);
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxLIGHT_GREY_BRUSH);
    dc.DrawRectangle(cursor);
}



void CodeView::ClearCursor()
{
    if ((CursorLastPosition >= 0) && (CursorLastPosition <= GetLastLine()))
        RefreshRect(*LastCursorRect);
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
        SelectedCount = 1;
        MultiSelection = false;
        SelectedItemIndex = CursorPosition;
        CursorLastPosition = CursorPosition;
        SelectedLastItem = SelectedItemIndex;
    }
}



ElementType CodeView::GetTypeMultiselection()
{
    CodeViewItem *cvi;
    DAsmElement *de;
    int i;
    bool homogeneous = false;
    ElementType lastitem = et_None;


    for (i = SelectedItemIndex; i < SelectedLastItem; i++)
    {
        cvi = m_CodeViewLine->getData(i);
        if (cvi->Dasmitem >= 0)
        {
            de = m_process->m_Dasm->GetData(cvi->Dasmitem);
            if (de->ElType == et_Instruction)
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
                if (de->ElType == et_Data)
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



void CodeView::DebugLog(wxTextCtrl *log)
{
    TC_Log = log;
}





/* --------------------------------------------
 * Event Handlers
 * ---------------------------------------------
 */


// Event handler for Page up/down and direction keys
// Works only if its parent is NOT wxTAB_TRAVERSAL style
// and use SetFocusIgnoringChildren
void CodeView::OnKeyPress(wxKeyEvent& event)
{
    int key;
    key = event.GetKeyCode();

    wxCommandEvent evtMakeData(wxEVT_COMMAND_MENU_SELECTED, idPOPUP_MAKEDATA);
    wxCommandEvent evtDisassemble(wxEVT_COMMAND_MENU_SELECTED, idPOPUP_DISASM);
    wxScrollWinEvent evtLineDown(wxEVT_SCROLLWIN_LINEDOWN);
    wxScrollWinEvent evtLineUp(wxEVT_SCROLLWIN_LINEUP);
    wxScrollWinEvent evtPageDown(wxEVT_SCROLLWIN_PAGEDOWN);
    wxScrollWinEvent evtPageUp(wxEVT_SCROLLWIN_PAGEUP);

#define C_KEY   67
#define D_KEY   68

    switch (key)
    {
        case WXK_DOWN:
                        if (CursorPosition < (m_CodeViewLine->GetCount() - 1))
                        {
                            if (!MultiSelection)
                                CursorLastPosition = CursorPosition;

                            CursorPosition++;

                            ClearCursor();
                            DoSelection();

                            if (CursorPosition > GetLastLine())
                                AddPendingEvent(evtLineDown);
                            else
                                RefreshRect(CalcCursorRfshRect());
                        }
                        break;
        case WXK_UP:
                        if (CursorPosition > 0)
                        {
                            if (!MultiSelection)
                                CursorLastPosition = CursorPosition;

                            CursorPosition--;

                            ClearCursor();
                            DoSelection();

                            if (CursorPosition < GetFirstLine())
                                AddPendingEvent(evtLineUp);
                            else
                                RefreshRect(CalcCursorRfshRect());
                        }
                        break;
        case WXK_NUMPAD_PAGEDOWN:
        case WXK_PAGEDOWN:
                        AddPendingEvent(evtPageDown);
                        break;
        case WXK_NUMPAD_PAGEUP:
        case WXK_PAGEUP:
                        AddPendingEvent(evtPageUp);
                        break;
        case WXK_SHIFT:
                        if (!Selecting)
							Selecting = true;
                        MultiSelection = true;
                        break;
        case C_KEY:
						#ifdef IDZ80DEBUG
                        LogIt(_("Event Disasm !\n"));
                        #endif
                        AddPendingEvent(evtDisassemble);
                        break;
        case D_KEY:
						#ifdef IDZ80DEBUG
                        LogIt(_("Event makeData !\n"));
                        #endif
                        AddPendingEvent(evtMakeData);
                        break;

        default:
                        //LogIt(wxString::Format(_("%d pressed !\n"),key));
                        event.Skip();
    }
}



void CodeView::OnKeyRelease(wxKeyEvent& event)
{
    int key;
    key = event.GetKeyCode();

    switch (key)
    {
        case WXK_SHIFT:
                        Selecting = false;
                        break;
        default:        event.Skip();
    }
}




void CodeView::OnGetFocus(wxFocusEvent& event)
{
    if (!IsFocused)
        IsFocused = true;
}

void CodeView::OnKillFocus(wxFocusEvent& event)
{
    if (IsFocused)
        IsFocused = false;
}




void CodeView::OnSize(wxSizeEvent& event)
{
    CalcItemsShown();
    Refresh();
}



void CodeView::OnPopUpMenuSearch(wxCommandEvent& event)
{
    TC_Log->AppendText(_("Search Menu Clicked !!!\n"));
}




void CodeView::OnPopUpMenuGoto(wxCommandEvent& event)
{
    CodeViewItem	*cvi;
    DAsmElement		*de;
    uint 			address, position;
    int				i, fl, ll;
    bool 			needScroll = false;

    fl = GetFirstLine();
    ll = fl + m_linesShown - 1;
    cvi = m_CodeViewLine->getData(CursorPosition);
    de = m_process->m_Dasm->GetData(cvi->Dasmitem);
    address = de->getArgument(0);
    m_CodeViewLine->getDataLineAddress(address, i);

    if (i >= 0)
    {
        position = m_linesShown / 2;
        if ((i >= fl) && (i <= ll))
        {
            CursorLastPosition = CursorPosition;
            CursorPosition = i;
        }
        else
        {
            CursorLastPosition = i;
            CursorPosition = i;
            needScroll = true;
        }
        RefreshRect(*LastCursorRect);
        if (needScroll)
            Scroll(-1, (i - position));
        RefreshRect(CalcCursorRfshRect());
    }

}

// Returns the first and the last line of instruction / data
bool CodeView::FilterInstructions(wxArrayInt &range)
{
    bool	foundindex;
    int		i, last_i;
    CodeViewItem
			*cvi;
    DAsmElement
			*de;


    foundindex = false;
    last_i = 0;
    for (i = SelectedItemIndex; i <= SelectedLastItem; i++)
    {
        cvi = m_CodeViewLine->getData(i);
        if (cvi->Dasmitem >= 0)
        {
            de = m_process->m_Dasm->GetData(cvi->Dasmitem);
            if ((de->ElType == et_Instruction) ||
                (de->ElType == et_Data))
            {
                if (!foundindex)
                {
                    range.Add(i);
                    foundindex = true;
                }
                last_i = i;
            }
        }
    }
    if (foundindex)
        range.Add(last_i);

	if (range.GetCount() > 0)
		return true;
	else
		return false;
}



void CodeView::OnPopUpMenuMakeData(wxCommandEvent& event)
{
    RangeItems		dasmed_items;
    CodeViewItem	*cvi;
    int 			i, newLineCount, lineIndex, lineLast, lineCount,
					j, oldLineCount;
    wxArrayInt		cvlines;

    if (!FilterInstructions(cvlines))
		return;

    if (cvlines.GetCount() > 0)
    {
        lineIndex = cvlines[0];
        lineLast = cvlines[1];
        lineCount = lineLast - lineIndex + 1;

        cvi = m_CodeViewLine->getData(lineIndex);
        dasmed_items.Index = cvi->Dasmitem;
        cvi = m_CodeViewLine->getData(lineLast);
        dasmed_items.Count = cvi->Dasmitem - dasmed_items.Index + 1;
        oldLineCount = dasmed_items.Count;

        m_process->MakeData(dasmed_items);

		j = 0;
        for (i = 0; i < lineCount; i++)
		{
			if (m_CodeViewLine->getData(lineIndex + j)->LabelProgAddr >= 0)
				j++;
			else
				m_CodeViewLine->Del(lineIndex + j);
		}

        m_CodeViewLine->linkData(dasmed_items.Index, lineIndex, dasmed_items.Count);

        newLineCount = dasmed_items.Count - oldLineCount;

        m_CodeViewLine->UpdateDasmIndex((lineIndex + dasmed_items.Count), newLineCount);

        CursorLastPosition = lineIndex;
        CursorPosition = lineIndex + lineCount + newLineCount -1;
        UpdateSelectedRect();
		Refresh();
    }
}


void CodeView::OnPopUpMenuDisasm(wxCommandEvent& event)
{
    RangeItems		dasmed_items;
    CodeViewItem	*cvi;
    int				i, newLineCount, j, oldLineCount;
    int				lineIndex, lineLast, lineCount;
    wxArrayInt		cvlines;

	if (!FilterInstructions(cvlines))
		return;

	if (cvlines.GetCount() > 0)
	{
        lineIndex = cvlines[0];
        lineLast = cvlines[1];
        lineCount = lineLast - lineIndex + 1;

		cvi = m_CodeViewLine->getData(lineIndex);
		dasmed_items.Index = cvi->Dasmitem;
		cvi = m_CodeViewLine->getData(lineLast);
		dasmed_items.Count = cvi->Dasmitem - dasmed_items.Index + 1;
		oldLineCount = dasmed_items.Count;

		j = 0;
		for (i = 0; i < lineCount; i++)
		{
			if (m_CodeViewLine->getData(lineIndex + j)->LabelProgAddr >= 0)
				j++;
			else
				m_CodeViewLine->Del(lineIndex + j);
		}

		m_process->DisassembleItems(dasmed_items);

		m_CodeViewLine->linkData(dasmed_items.Index,lineIndex,dasmed_items.Count);

		newLineCount = dasmed_items.Count - oldLineCount;

		m_CodeViewLine->UpdateDasmIndex((lineIndex + dasmed_items.Count), newLineCount);

		CursorLastPosition = lineIndex;
		CursorPosition = lineIndex + lineCount + newLineCount -1;
		UpdateSelectedRect();
		Refresh();
	}
}



void CodeView::OnPopUpAddComment(wxCommandEvent& event)
{
    CodeViewItem *cvi;
    wxString comment;

    cvi = m_CodeViewLine->getData(CursorPosition);

    if (cvi != 0)
    {
        if (cvi->Comment == 0)
        {
            comment=::wxGetTextFromUser(_("Add Comment"), _("Add"));
            if (!comment.IsEmpty())
            {
                comment=comment.Trim(false);
                if (comment.Left(1) != _T(";"))
                    comment.Prepend(_("; "));
                m_CodeViewLine->AppendComment(comment, CursorPosition);
                Refresh();
            }
        }
    }
}

void CodeView::OnPopUpEditComment(wxCommandEvent& event)
{
    CodeViewItem *cvi;
    wxString comment;

    cvi = m_CodeViewLine->getData(CursorPosition);
	if ((cvi != 0) && (cvi->Comment != 0))
		comment = ::wxGetTextFromUser(_("Edit Comment"), _("Edit"), cvi->Comment->CommentStr);
	if (!comment.IsEmpty())
	{
		comment = comment.Trim(false);
		if (comment.Left(1) != _T(";"))
			comment.Prepend(_("; "));
		m_CodeViewLine->Edit(comment,CursorPosition);
		Refresh();
	}
}

void CodeView::OnPopUpDelComment(wxCommandEvent& event)
{
    CodeViewItem *cvi;
    cvi = m_CodeViewLine->getData(CursorPosition);
    if (cvi != 0)
    {
        if ((cvi->Dasmitem == -1) && (cvi->LabelProgAddr == -1) && (cvi->LabelVarAddr == -1) && (cvi->Org == -1))
            m_CodeViewLine->DelItem(cvi);
        else
            m_CodeViewLine->DelComment(cvi);
        Refresh();
    }
}


void CodeView::OnPopUpMenuOD_Matrix(wxCommandEvent &event)
{
	LogIt(_("Data Matrixed !!\n"));
}

void CodeView::OnPopUpMenuOD_String(wxCommandEvent& event)
{
	LogIt(_("Data stringed !!\n"));
}

void CodeView::OnPopUpMenuOD_Number(wxCommandEvent& event)
{
	LogIt(_("Data numbered !!\n"));
}

void CodeView::OnPopUpMenuRenLabel(wxCommandEvent& event)
{
    CodeViewItem *cvi;
    cvi = m_CodeViewLine->getData(CursorPosition);
    //TODO: Implement label editing in instructions
	if (cvi != 0)
	{
		if (cvi->LabelProgAddr >= 0)
			m_process->prog_labels->EditLabelDialog(cvi->LabelProgAddr);
		if (cvi->LabelVarAddr >= 0)
			m_process->var_labels->EditLabelDialog(cvi->LabelVarAddr);
	}
	LogIt("Label renamed !!\n");
}

void CodeView::OnPopUpMenuDelLabel(wxCommandEvent& event)
{
	LogIt(_("Label deleted !!\n"));
}




void CodeView::OnPopUpMenuArgStyleBin(wxCommandEvent &event)
{
//	DAsmElement *de;


	if (m_iteminfo.argSelected == 1)
		m_iteminfo.dasmitem->Style.arg1 = ast_bin;

	if (m_iteminfo.argSelected == 2)
		m_iteminfo.dasmitem->Style.arg2 = ast_bin;

/*
	if (m_styleData.arg != 0)
	{
		de = m_process->m_Dasm->GetData(m_styleData.item);
		if (m_styleData.arg == 1)
			de->Style.arg1 = ast_bin;
		else
			de->Style.arg2 = ast_bin;

		m_styleData.arg = 0;
		m_styleData.item = 0;
*/
	RefreshRect(CalcCursorRfshRect());
//	}
}

void CodeView::OnPopUpMenuArgStyleDec(wxCommandEvent& event)
{
//	DAsmElement *de;


	if (m_iteminfo.argSelected == 1)
		m_iteminfo.dasmitem->Style.arg1 = ast_dec;

	if (m_iteminfo.argSelected == 2)
		m_iteminfo.dasmitem->Style.arg2 = ast_dec;

/*
	if (m_styleData.arg != 0)
	{
		de = m_process->m_Dasm->GetData(m_styleData.item);
		if (m_styleData.arg == 1)
			de->Style.arg1 = ast_dec;
		else
			de->Style.arg2 = ast_dec;

		m_styleData.arg = 0;
		m_styleData.item = 0;
*/
	RefreshRect(CalcCursorRfshRect());
//	}
}

void CodeView::OnPopUpMenuArgStyleHex(wxCommandEvent& event)
{
	//DAsmElement *de;

	if (m_iteminfo.argSelected == 1)
		m_iteminfo.dasmitem->Style.arg1 = ast_hex;

	if (m_iteminfo.argSelected == 2)
		m_iteminfo.dasmitem->Style.arg2 = ast_hex;

/*
		de = m_process->m_Dasm->GetData(m_styleData.item);
		if (m_styleData.arg == 1)
			de->Style.arg1 = ast_hex;
		else
			de->Style.arg2 = ast_hex;

		m_styleData.arg = 0;
		m_styleData.item = 0;
*/
		RefreshRect(CalcCursorRfshRect());
//	}
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


void CodeView::FillSelectedItemInfo(const wxPoint &pt)
{
	DAsmElement *de;
	CodeViewItem *cvi;
	bool testcomment = false;

	m_iteminfo.type = siUnknown;
	cvi = m_CodeViewLine->getData(CursorPosition);
	if (cvi != 0)
	{
		m_iteminfo.lineitem = cvi;
		// Does the line have comment ?
		if (cvi->Comment != 0)
			m_iteminfo.hasComment = true;
		else
			m_iteminfo.hasComment = false;

		// If line is a program label or a var label
		if (cvi->LabelProgAddr >= 0)
			m_iteminfo.type = siLineLabelProg;

		if (cvi->LabelVarAddr >= 0)
			m_iteminfo.type = siLineLabelVar;

		// If clicked over an instruction argument, discover which was
		if ((cvi->RectArg1 != 0) && (cvi->RectArg1->Contains(pt)))
			m_iteminfo.argSelected = 1;
		else
			m_iteminfo.argSelected = 0;

		if ((cvi->RectArg2 != 0) && (cvi->RectArg2->Contains(pt)))
			m_iteminfo.argSelected = 2;
		// BUG
		//else
		//	m_iteminfo.argSelected = 0;

		// check if line has instruction
		if (cvi->Dasmitem >= 0)
		{
			de = m_process->m_Dasm->GetData(cvi->Dasmitem);
			if (de != 0)
			{
				m_iteminfo.dasmitem = de;
				if (de->Style.hasArgumentLabel)
					m_iteminfo.type = siInstructionLabel;
				else
					m_iteminfo.type = siInstruction;

				if (de->ElType == et_Data)
					m_iteminfo.type = siData;

			}
			else
				m_iteminfo.dasmitem = (DAsmElement *)0;
		}
		else
				m_iteminfo.dasmitem = (DAsmElement *)0;

		// Test if line has only comments
		testcomment = ((cvi->Org + cvi->Dasmitem + cvi->LabelProgAddr +
					cvi->LabelVarAddr) < 0);
		if (testcomment)
			m_iteminfo.type = siComments;
	}
	else
		m_iteminfo.lineitem = (CodeViewItem *)0;
}


// TODO: Remove debug log it function
void CodeView::LogIt(const wxString &str)
{
    if (TC_Log!=0)
        TC_Log->AppendText(str);
}
