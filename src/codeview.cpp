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



CodeView::CodeView(wxWindow *parent, ProcessData *_proc)
        : wxScrolledWindow(parent)
{
    m_process = _proc;
    //TODO: delete m_fractionXXXX vars ?
    m_fractionLine = 0;
    m_fractionScroll = 0;
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

    Connect(wxEVT_SCROLLWIN_LINEDOWN, wxScrollWinEventHandler(CodeView::OnScrollLineDown));
    Connect(wxEVT_SCROLLWIN_LINEUP, wxScrollWinEventHandler(CodeView::OnScrollLineUp));
    Connect(wxEVT_SCROLLWIN_PAGEDOWN, wxScrollWinEventHandler(CodeView::OnScrollPageDown));
    Connect(wxEVT_SCROLLWIN_PAGEUP, wxScrollWinEventHandler(CodeView::OnScrollPageUp));
    Connect(wxEVT_PAINT, wxPaintEventHandler(CodeView::OnPaint));
    //Connect(wxEVT_ERASE_BACKGROUND,wxEraseEventHandler(CodeView::OnErase));
    Connect(wxEVT_ENTER_WINDOW,wxFocusEventHandler(CodeView::OnGetFocus));
    Connect(wxEVT_LEAVE_WINDOW,wxFocusEventHandler(CodeView::OnKillFocus));



    // MOUSE Events
    Connect(wxEVT_SIZE, wxSizeEventHandler(CodeView::OnSize));
    Connect(wxEVT_LEFT_DOWN,wxMouseEventHandler(CodeView::OnMouseLeftDown));
    //Connect(wxEVT_LEFT_UP,wxMouseEventHandler(CodeView::OnMouseLeftUp));
    Connect(wxEVT_RIGHT_DOWN,wxMouseEventHandler(CodeView::OnMouseRightDown));
    Connect(wxEVT_RIGHT_UP,wxMouseEventHandler(CodeView::OnMouseRightUp));
    //Connect(wxEVT_MOTION,wxMouseEventHandler(CodeView::OnMouseMove));
    Connect(wxEVT_MOUSEWHEEL,wxMouseEventHandler(CodeView::OnMouseWheel));

    // KEYBOARD events
    Connect(wxEVT_KEY_DOWN,wxKeyEventHandler(CodeView::OnKeyPress));
    Connect(wxEVT_KEY_UP,wxKeyEventHandler(CodeView::OnKeyRelease));

    // Popup event connections
    Connect(idPOPUP_ADDCOMMENT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&CodeView::OnPopUpAddComment);
    Connect(idPOPUP_EDITCOMMENT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&CodeView::OnPopUpEditComment);
    Connect(idPOPUP_DELCOMMENT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&CodeView::OnPopUpDelComment);
    Connect(idPOPUP_GOTO,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&CodeView::OnPopUpMenuGoto);
    Connect(idPOPUP_MAKEDATA,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&CodeView::OnPopUpMenuMakeData);
    Connect(idPOPUP_DISASM,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&CodeView::OnPopUpMenuDisasm);

    PopUp=0;

    // Debug Area
    TC_Log=0;
}




int CodeView::GetCount()
{
    return m_CodeViewLine->GetCount();
}



// Plots Data item, returns the last point of the plotted string
uint CodeView::RenderData(wxDC &dc, const int start_y, CodeViewItem *cvi)
{
    uint i,x,argwidth;
    DAsmElement *de;
    wxString str;

    x=COL_MNEM;
    de=m_process->m_Dasm->GetData(cvi->Dasmitem);
    dc.SetTextForeground(FG_TextColor);
    dc.DrawText(str, x, start_y);
    x+=dc.GetTextExtent(str).GetWidth();

    str.Printf(_T("DB "));
    for (i=0;i<de->Length;i++)
    {
        str << wxString::Format(_T("0x%.2X"),de->GetData(de->Offset+i));
        if (i<(de->Length-1))
            str << _T(",");
    }
    dc.DrawText(str, x, start_y);
    argwidth=dc.GetTextExtent(str).GetWidth();
    x+=argwidth;
    if (cvi->RectArg1==0)
    {
        cvi->RectArg1=new wxRect(COL_MNEM, start_y, argwidth, m_fontHeight);
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
    int nargs,
        argsz,
        x,
        argwidth1,
        argpos1,
        argwidth2,
        argpos2;
    uint strparts;   // counts the number of mnemonic str used
    wxString str;
    bool usedlabel;
    DAsmElement *de;

    argwidth1=argwidth2=argpos1=argpos2=0;
    usedlabel=false;
    x=COL_MNEM;
    de=m_process->m_Dasm->GetData(cvi->Dasmitem);
    nargs=de->MnItem->getArgNo();
    strparts=0;
    argsz=de->MnItem->getArgSize();
    str=de->MnItem->MnemonicString[0];
    strparts++;
    dc.SetTextForeground(FG_TextColor);
    dc.DrawText(str, x, start_y);
    x += dc.GetTextExtent(str).GetWidth();
    argpos1=x;

    if (de->hasArgumentLabel)
    {
        switch (de->MnItem->getArgType(0))
        {
            case ARG_REL_ADDR:
            case ARG_ABS_ADDR:
                            if (m_process->prog_labels->GetLabel(de->getArgument(0),str))
                                usedlabel=true;
                            break;
            case ARG_IO_ADDR:
                            if (m_process->io_labels->GetLabel(de->getArgument(0),str))
                                usedlabel=true;
                            break;
            case ARG_VARIABLE:
                            if (m_process->var_labels->GetLabel(de->getArgument(0),str))
                                usedlabel=true;
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
            argwidth1=dc.GetTextExtent(str).GetWidth();
            x+=argwidth1;
            dc.SetTextForeground(FG_TextColor);
            if (cvi->RectArg1==0)
            {
                cvi->RectArg1=new wxRect(argpos1, start_y, argwidth1, m_fontHeight);
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
        str.Printf(_T("0x%X"),de->getArgument(0));
        dc.DrawText(str, x, start_y);
        argwidth1=dc.GetTextExtent(str).GetWidth();
        x+=argwidth1;
        if (cvi->RectArg1==0)
        {
            cvi->RectArg1=new wxRect(argpos1, start_y,argwidth1,m_fontHeight);
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
        str.Printf(_T("0x%X"),de->getArgument(0));
        dc.DrawText(str,x,start_y);
        argwidth1=dc.GetTextExtent(str).GetWidth();
        x+=argwidth1;

        dc.SetTextForeground(FG_TextColor);
        str=de->MnItem->MnemonicString[1];
        strparts++;
        dc.DrawText(str, x, start_y);
        x+=dc.GetTextExtent(str).GetWidth();
        argpos2 = x;

        dc.SetTextForeground(*wxRED);
        str.Printf(_T("0x%X"),de->getArgument(1));
        dc.DrawText(str, x, start_y);
        argwidth2=dc.GetTextExtent(str).GetWidth();
        x+=argwidth2;
        if (cvi->RectArg1==0)
        {
            cvi->RectArg1=new wxRect(argpos1, start_y, argwidth1, m_fontHeight);
        }
        else
        {
            cvi->RectArg1->SetX(argpos1);
            cvi->RectArg1->SetY(start_y);
            cvi->RectArg1->SetHeight(m_fontHeight);
            cvi->RectArg1->SetWidth(argwidth1);
        }

        if (cvi->RectArg2==0)
        {
            cvi->RectArg2=new wxRect(argpos2, start_y,argwidth2,m_fontHeight);
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
    if (de->MnItem->MnemonicString.GetCount()>strparts)
    {
        str=de->MnItem->MnemonicString[strparts];
        dc.DrawText(str, x, start_y);
        x+=dc.GetTextExtent(str).GetWidth();
    }
    return x;
}



uint CodeView::RenderProgramLabel(wxDC &dc, const int start_y,wxString str)
{
    int x;
    x=COL_LABEL;
    dc.SetTextForeground(FG_LabelColor);
    dc.DrawText(str, x, start_y);
    x+=dc.GetTextExtent(str).GetWidth();
    dc.SetTextForeground(FG_TextColor);
    return x;
}



uint CodeView::RenderOrigin(wxDC &dc, const int start_y,uint address)
{
    int x;
    wxString str;

    x=COL_MNEM;
    str.Printf(_T("ORG %Xh"),address);
    dc.SetTextForeground(FG_TextColor);
    dc.DrawText(str, x, start_y);
    x+=dc.GetTextExtent(str).GetWidth();
    return x;
}


void CodeView::Render(wxDC &dc, const int start_y, const int fromline, const int count)
{
    CodeViewItem	*cvi;
    wxString		str;
    uint			commentoffset;
    int				linecode, linepixel, i;
    uint			address;
    bool			labelfailed;
    DAsmElement		*de;
    bool			firstInstruction;
    wxCoord			width, heigh;

    GetClientSize(&width, &heigh);

    linecode = fromline;
    linepixel = start_y;

    commentoffset = 0;
    labelfailed = false;
    firstInstruction = false;
    i = 0;
    address = 0;
    while (i < count)
    {
        cvi = m_CodeViewLine->getData(linecode + i);
        if (cvi != 0)
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
					de = m_process->m_Dasm->GetData(cvi->Dasmitem);
					if (de != 0)
					{
						address = m_process->m_Dasm->GetBaseAddress() + de->Offset;

						dc.SetTextForeground(FG_TextColor);
						dc.DrawText(de->getCodeStr(), COL_CODE, linepixel);
						dc.DrawText(de->getAsciiStr(), COL_ASCII, linepixel);
						switch (de->ElType)
						{
							case et_Data:
												commentoffset = RenderData(dc, linepixel,cvi) + 14;
												break;

							case et_Instruction:
												commentoffset = RenderInstruction(dc, linepixel,cvi) + 14;
												break;
							default:
												commentoffset = 0;
												break;
						}
						firstInstruction = true;
					}
					else
						dc.DrawText(_("------------------------ ERROR ----------------------------------------"), COL_CODE, linepixel);
				}
				else
				/* -------------------------------------------------
				 *  Render Labels
				 * -------------------------------------------------*/
				if (cvi->LabelAddr != -1)   // Is it a label ?
				{
					if (m_process->prog_labels->GetLabel(cvi->LabelAddr, str))
					{
						str << _T(":");
						RenderProgramLabel(dc, linepixel, str);
						address = cvi->LabelAddr;
						firstInstruction = true;
					}
					else
					{
						m_CodeViewLine->DelItem(cvi);
						labelfailed = true;
					}
				}
			}

			/* -------------------------------------------------
			 *  Render Address
			 * -------------------------------------------------*/
			if (firstInstruction)
			{
				str.Printf(_T("0x%.4X"), address);
				dc.DrawText(str, COL_ADDRESS, linepixel);
			}


			/* -------------------------------------------------
			 *  Render Comments
			 * -------------------------------------------------*/
			if (cvi->Comment != 0)   // has comments ?
			{
				dc.SetTextForeground(*wxGREEN);
				str = cvi->Comment->CommentStr;
				if (commentoffset == 0)
				{
					if (firstInstruction)
						dc.DrawText(str, COL_LABEL, linepixel);
					else
						dc.DrawText(str, COL_ADDRESS, linepixel);
				}
				else
					dc.DrawText(str, commentoffset, linepixel);
				dc.SetTextForeground(FG_TextColor);
			}
			i++;
			linepixel += m_fontHeight;
		}
		else
			i = count;
    }
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

    // cleans destroyed region
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
    SetBackgroundStyle(wxBG_STYLE_PAINT);    // wxBG_STYLE_CUSTOM);
    wxAutoBufferedPaintDC dc(this);
    DoPrepareDC(dc);
    UpdateLastCursorRect();
    OnDraw(dc);
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
    //TODO: finish moving CodeviewLine
    //delete m_CodeViewLine;
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
        SetVirtualSize(wxDefaultCoord,m_CodeViewLine->GetCount()*m_fontHeight);
        IsEmpty = m_CodeViewLine->IsEmpty();
        ret = true;
    }
    else
    {
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
    wxRect cursor(0,CursorPosition * m_fontHeight,GetClientSize().x,m_fontHeight);
    CalcScrolledPosition(cursor.x,cursor.y,0,&cursor.y);
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

//TODO: See if m_fractionXXXX will be used
void CodeView::CalcItemsShown(void)
{
    wxSize size = GetClientSize();
    m_linesShown = size.GetHeight() / m_fontHeight;
    m_fractionLine = size.GetHeight() - (m_linesShown * m_fontHeight);
    m_fractionScroll = m_fontHeight - m_fractionLine;
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
    TC_Log=log;
}





/* --------------------------------------------
 * Event Handlers
 * ---------------------------------------------
 */


void CodeView::OnMouseLeftDown(wxMouseEvent& event)
{
	int lastposition;

    if (!IsEnabled())
    {
        return;
    }

    if (!MultiSelection)
        CursorLastPosition=CursorPosition;

	lastposition = CursorPosition;

    CalcCursorPosition(event.GetPosition());
	if (CursorPosition < GetCount())
	{
		ClearCursor();

		if (CursorPosition > GetLastLine())
		{
			Scroll(-1,GetFirstLine() + 1);
			RefreshRect(CalcCursorRfshRect());
		}

		DoSelection();

		SetFocusIgnoringChildren();

		RefreshRect(CalcCursorRfshRect());
	}
	else
		CursorPosition = lastposition;
}


void CodeView::OnMouseLeftUp(wxMouseEvent& event)
{
    // nothing
}



void CodeView::OnMouseRightDown(wxMouseEvent& event)
{
	int	lastposition;

    if (!IsEnabled())
    {
        return;
    }

    if (!MultiSelection)
        CursorLastPosition=CursorPosition;

	lastposition = CursorPosition;
    CalcCursorPosition(event.GetPosition());
	if (CursorPosition < GetCount())
	{
		ClearCursor();

		if (MultiSelection && ((CursorPosition < SelectedItemIndex) ||
			(CursorPosition > SelectedLastItem)))
		{
			if (MultiSelection)
				RefreshRect(CalcSelectedRect());
			SelectedCount=1;
			MultiSelection=false;
			SelectedItemIndex=CursorPosition;
			CursorLastPosition=CursorPosition;
			SelectedLastItem=SelectedItemIndex;
		}
		else
			if (!MultiSelection)
			{
				SelectedCount=1;
				SelectedItemIndex=CursorPosition;
				CursorLastPosition=CursorPosition;
				SelectedLastItem=SelectedItemIndex;
			}

		SetFocusIgnoringChildren();

		RefreshRect(CalcCursorRfshRect());
	}
	else
		CursorPosition = lastposition;

}


void CodeView::OnMouseRightUp(wxMouseEvent& event)
{
    CodeViewItem *cvi;
    DAsmElement *de;


    if (!IsEmpty)
    {
        SetFocusIgnoringChildren();
        PopUp=new wxMenu();
        if (SelectedCount>1)
        {
            switch (GetTypeMultiselection())
            {
                case et_Instruction:
                                PopUp->Append(idPOPUP_MAKEDATA,_T("Make it data\td"));
                                LogIt(_("Instruction !\n"));
                                break;
                case et_Data:
                                PopUp->Append(idPOPUP_DISASM,_("Disassemble\tc"));
                                LogIt(_("Data !\n"));
                                break;
                default:
                                PopUp->Append(idPOPUP_MAKEDATA,_T("Make it data\td"));
                                LogIt(_("Default !\n"));
                                break;
            }
            PopUp->AppendSeparator();
            PopUp->Append(idPOPUP_EDITCOMMENT,_T("Edit comment"));
            PopUp->Append(idPOPUP_DELCOMMENT,_T("Del comment"));

        }
        else
        {
            cvi=m_CodeViewLine->getData(CursorPosition);
            // Label
            if (cvi->LabelAddr>=0)
            {
                PopUp->Append(idPOPUP_EDITLABEL,_T("Rename label"));
                PopUp->AppendSeparator();
                PopUp->Append(idPOPUP_DELLABEL,_T("Delete label"));
            }
            else
            // Dasmed item
            if (cvi->Dasmitem>=0)
            {
                de=m_process->m_Dasm->GetData(cvi->Dasmitem);
                if (de->MnItem != 0)
                    if (de->MnItem->getBranchType()!=BR_NONE)
                    {
                        PopUp->Append(idPOPUP_GOTO,_T("Goto label"));
                    }
                if (de->ElType == et_Instruction)
                {
                    PopUp->Append(idPOPUP_MAKEDATA,_T("Make it data"));
                }
            }
            PopUp->AppendSeparator();
            if (cvi->Comment!=0)
            {
                PopUp->Append(idPOPUP_EDITCOMMENT,_T("Edit comment"));
                PopUp->Append(idPOPUP_DELCOMMENT,_T("Del comment"));
            }
            else
                PopUp->Append(idPOPUP_ADDCOMMENT,_T("Add comment"));
        }
        PopupMenu(PopUp);
        delete PopUp;
    }
}


// TODO: Rewrite this
void CodeView::OnMouseMove(wxMouseEvent& event)
{
    CodeViewItem *cvi;
    wxPoint pt;
    wxClientDC  dc(this);
    DoPrepareDC(dc);

    uint pointedline;
    static uint lastpointedline=0;
    static wxRect *lastrect=0;
    static bool lastfocus1=false;
    static bool lastfocus2=false;
    static bool arg_erased=false;

    bool    m_Arg1focused,
            m_Arg2focused,
            linechanged=false;


    pt=event.GetLogicalPosition(dc);
    pointedline = pt.y/m_fontHeight;
    cvi=m_CodeViewLine->getData(pointedline);
    if (lastpointedline!=pointedline)
    {
        linechanged=true;
        lastpointedline=pointedline;
    }
    else
    {
        linechanged=false;
        lastpointedline=pointedline;
    }

    if (((!m_Arg1focused) && (!m_Arg2focused)) || (linechanged))
    {
        if ((lastrect!=0) && (!arg_erased))
        {
            wxPen pen(*wxWHITE,1,wxSOLID);
            wxBrush brush(*wxWHITE,wxTRANSPARENT);
            dc.SetPen(pen);
            dc.SetBrush(brush);
            dc.DrawRectangle(*lastrect);
            arg_erased=true;
            lastfocus1=false;
            lastfocus2=false;
        }
    }

    if (cvi->RectArg1!=0)
    {
        if (cvi->RectArg1->Contains(wxPoint(pt.x,pt.y)))
        {
            lastrect=cvi->RectArg1;
            m_Arg1focused=true;

            if (m_Arg1focused!=lastfocus1)
            {
                wxPen pen(FGArgumentColor,1,wxSOLID);
                wxBrush brush(BGArgumentColor,wxTRANSPARENT);
                dc.SetPen(pen);
                dc.SetBrush(brush);
                dc.DrawRectangle(*cvi->RectArg1);
                lastfocus1=m_Arg1focused;
                arg_erased=false;
            }
        }
        else
        {
            m_Arg1focused=false;
            lastfocus1=false;
        }
    }
    if  (cvi->RectArg2!=0)
    {
        if (cvi->RectArg2->Contains(wxPoint(pt.x,pt.y)))
        {
            lastrect=cvi->RectArg2;
            m_Arg2focused=true;

            if (m_Arg2focused!=lastfocus2)
            {
                wxPen pen(FGArgumentColor,1,wxSOLID);
                wxBrush brush(BGArgumentColor,wxSOLID);       //wxTRANSPARENT);
                dc.SetPen(pen);
                dc.SetBrush(brush);
                dc.DrawRectangle(*cvi->RectArg2);
                lastfocus2=m_Arg2focused;
                arg_erased=false;
            }
        }
        else
        {
            m_Arg2focused=false;
            lastfocus2=false;
        }
    }
}




void CodeView::OnMouseWheel(wxMouseEvent& event)
{
    bool up_motion;
    int motion_step = 3;
    int position;

    up_motion = (event.GetWheelRotation() >= 0);

    if (up_motion)
    {
        position = GetFirstLine() - motion_step;
        if (position < 0)
            position = 0;
        Scroll(-1,position);
    }
    else
    {
        position = GetFirstLine() + motion_step;
        if (position > (m_CodeViewLine->GetCount() - m_linesShown))
            position = m_CodeViewLine->GetCount() - m_linesShown;
        Scroll(-1,position);
    }
}







// Event handler for Page up/down and direction keys
// Works only if its parent is NOT wxTAB_TRAVERSAL style
// and use SetFocusIgnoringChildren
void CodeView::OnKeyPress(wxKeyEvent& event)
{
    int key;
    key = event.GetKeyCode();

    wxCommandEvent evtMakeData(wxEVT_COMMAND_MENU_SELECTED,idPOPUP_MAKEDATA);
    wxCommandEvent evtDisassemble(wxEVT_COMMAND_MENU_SELECTED,idPOPUP_DISASM);
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
                        Selecting=true;
                        MultiSelection=true;
                        break;
        case C_KEY:
                        LogIt(_("Event Disasm !\n"));
                        AddPendingEvent(evtDisassemble);
                        break;
        case D_KEY:
                        LogIt(_("Event makeData !\n"));
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
                        Selecting=false;
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

    fl=GetFirstLine();
    ll=fl+m_linesShown-1;
    cvi=m_CodeViewLine->getData(CursorPosition);
    de=m_process->m_Dasm->GetData(cvi->Dasmitem);
    address=de->getArgument(0);
    i = m_CodeViewLine->getDataLineAddress(address);

    if (i>=0)
    {
        position=m_linesShown / 2;
        if ((i>=fl) && (i<=ll))
        {
            CursorLastPosition=CursorPosition;
            CursorPosition=i;
        }
        else
        {
            CursorLastPosition=i;
            CursorPosition=i;
            needScroll=true;
        }
        RefreshRect(*LastCursorRect);
        if (needScroll)
            Scroll(-1,(i-position));
        RefreshRect(CalcCursorRfshRect());
    }

}

// Returns the first and the last line of instruction / data
void CodeView::FilterInstructions(wxArrayInt &range)
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
                    foundindex=true;
                }
                last_i=i;
            }
/*            else
                if (foundindex)
                {
                    range.Add(last_i);
                    foundindex=false;
                }
*/
        }
/*
        else
            if (foundindex)
            {
                range.Add(last_i);
                foundindex=false;
            }
*/
    }
    if (foundindex)
        range.Add(last_i);
}



void CodeView::OnPopUpMenuMakeData(wxCommandEvent& event)
{
    RangeItems		ri;
    CodeViewItem	*cvi;
    int 			i, newLineCount, lineIndex, lineLast, lineCount;
    wxArrayInt		dasm_range;

    FilterInstructions(dasm_range);
    if (dasm_range.GetCount() > 0)
    {
        lineIndex = dasm_range[0];
        lineLast = dasm_range[1];
        lineCount = lineLast - lineIndex + 1;

        cvi = m_CodeViewLine->getData(lineIndex);
        ri.Index = cvi->Dasmitem;
        cvi = m_CodeViewLine->getData(lineLast);
        ri.Count = cvi->Dasmitem - ri.Index + 1;

        m_process->MakeData(ri);

        for (i = 0; i < lineCount; i++)
            m_CodeViewLine->Del(lineIndex);

        m_CodeViewLine->linkData(ri.Index, lineIndex, ri.Count);

        newLineCount = ri.Count - lineCount;

        m_CodeViewLine->UpdateDasmIndex((lineIndex + ri.Count), newLineCount);

        CursorLastPosition = lineIndex;
        CursorPosition = lineIndex + ri.Count - 1;
        UpdateSelectedRect();
        wxSize sz = GetVirtualSize();
        sz.y += newLineCount * m_fontHeight;
        SetVirtualSize(sz);
        Refresh();
    }
}


void CodeView::OnPopUpMenuDisasm(wxCommandEvent& event)
{
    RangeItems ri;
    CodeViewItem *cvi;
    int i, newLineCount;
    int lineIndex, lineLast, lineCount;

    lineIndex = SelectedItemIndex;
    lineLast = lineIndex + SelectedCount - 1;
    lineCount = SelectedCount;

    cvi = m_CodeViewLine->getData(lineIndex);
    ri.Index = cvi->Dasmitem;
    cvi = m_CodeViewLine->getData(lineLast);
    ri.Count = lineCount;

    for (i = 0; i < lineCount; i++)
        m_CodeViewLine->Del(lineIndex);

    m_process->DisassembleItems(ri);

    m_CodeViewLine->linkData(ri.Index,lineIndex,ri.Count);

    newLineCount = ri.Count - lineCount;		//lineCount - ri.Count;

	m_CodeViewLine->UpdateDasmIndex((lineIndex + ri.Count), newLineCount);

    CursorLastPosition = lineIndex;
    CursorPosition = lineIndex + ri.Count - 1;
    UpdateSelectedRect();
    wxSize sz = GetVirtualSize();
    sz.y -= newLineCount * m_fontHeight;
    SetVirtualSize(sz);
    Refresh();
}



void CodeView::OnPopUpAddComment(wxCommandEvent& event)
{
    CodeViewItem *cvi;
    wxString comment;
    cvi=m_CodeViewLine->getData(CursorPosition);

    if (cvi!=0)
    {
        if (cvi->Comment==0)
        {
            comment=::wxGetTextFromUser(_("Add Comment"),_("Add"));
            if (!comment.IsEmpty())
            {
                comment=comment.Trim(false);
                if (comment.Left(1) != _T(";"))
                    comment.Prepend(_("; "));
                m_CodeViewLine->AppendComment(comment,CursorPosition);
                Refresh();
            }
        }
    }
}

void CodeView::OnPopUpEditComment(wxCommandEvent& event)
{
    CodeViewItem *cvi;
    wxString comment;
    cvi=m_CodeViewLine->getData(CursorPosition);
    if (cvi!=0)
    {
        if (cvi->Comment!=0)
            comment=::wxGetTextFromUser(_("Edit Comment"),_("Edit"),cvi->Comment->CommentStr);
            if (!comment.IsEmpty())
            {
                comment=comment.Trim(false);
                if (comment.Left(1) != _T(";"))
                    comment.Prepend(_("; "));
                m_CodeViewLine->Edit(comment,CursorPosition);
                Refresh();
            }
    }
}

void CodeView::OnPopUpDelComment(wxCommandEvent& event)
{
    CodeViewItem *cvi;
    cvi=m_CodeViewLine->getData(CursorPosition);
    if (cvi!=0)
    {
        if ((cvi->Dasmitem==-1) && (cvi->LabelAddr==-1) && (cvi->Org==-1))
            m_CodeViewLine->DelItem(cvi);
        else
            m_CodeViewLine->DelComment(cvi);
        Refresh();
    }
}






// TODO: Remove debug log it function
void CodeView::LogIt(const wxString &str)
{
    if (TC_Log!=0)
        TC_Log->AppendText(str);
}
