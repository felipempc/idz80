/****************************************************************
 * Name:      Codeview - Mouse Event handlers
 * Purpose:   Renders the view
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   30-12-2011
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 *
 * This module is part of the codeview class that renders
 * the view
 **************************************************************/




#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/mousestate.h>
#include "codeview.h"
#include "d_asm_element.h"



void CodeView::OnMouseLeftDown(wxMouseEvent& event)
{
	int lastposition;
	wxPoint pt;

    if (!IsEnabled())
    {
        return;
    }

    SetFocus();
    if (!MultiSelection)
        m_iteminfo.cursorLastPosition = m_iteminfo.cursorPosition;

	lastposition = m_iteminfo.cursorPosition;
	pt = event.GetPosition();
    CalcCursorPosition(pt);
	if (m_iteminfo.cursorPosition < GetCount())
	{
		ClearCursor();

		if (m_iteminfo.cursorPosition > GetLastLine())
		{
			Scroll(-1, GetFirstLine() + 1);
			RefreshRect(CalcCursorRfshRect());
		}

		DoSelection();

		FillSelectedItemInfo(pt);

		RefreshRect(CalcCursorRfshRect());
	}
	else
		m_iteminfo.cursorPosition = lastposition;

}


void CodeView::OnMouseLeftUp(wxMouseEvent& event)
{
    FillSelectedItemInfo(event.GetPosition());

    if (HasCapture())
        ReleaseMouse();

}



void CodeView::OnMouseRightDown(wxMouseEvent& event)
{
	int	lastposition;
	wxPoint pt;

    if (!IsEnabled())
    {
        return;
    }

    SetFocus();

    if (!MultiSelection)
        m_iteminfo.cursorLastPosition = m_iteminfo.cursorPosition;

	lastposition = m_iteminfo.cursorPosition;
	pt = event.GetPosition();
    CalcCursorPosition(pt);
	if (m_iteminfo.cursorPosition < GetCount())
	{
		ClearCursor();

		if (MultiSelection && ((m_iteminfo.cursorPosition < m_iteminfo.firstLine) ||
			(m_iteminfo.cursorPosition > m_iteminfo.lastLine)))
		{
			RefreshRect(CalcSelectedRect());
			m_iteminfo.selectedLineCount = 1;
			MultiSelection = false;
			m_iteminfo.firstLine = m_iteminfo.cursorPosition;
			m_iteminfo.cursorLastPosition = m_iteminfo.cursorPosition;
			m_iteminfo.lastLine = m_iteminfo.cursorPosition;
		}
		else
			if (!MultiSelection)
			{
				m_iteminfo.selectedLineCount = 1;
				m_iteminfo.firstLine = m_iteminfo.cursorPosition;
				m_iteminfo.cursorLastPosition = m_iteminfo.cursorPosition;
				m_iteminfo.lastLine = m_iteminfo.firstLine;
			}

		FillSelectedItemInfo(pt);

		RefreshRect(CalcCursorRfshRect());
	}
	else
		m_iteminfo.cursorPosition = lastposition;

}


void CodeView::OnMouseRightUp(wxMouseEvent& event)
{
    wxMenu      *PopUp;

    if (!IsEmpty)
    {
        PopUp = new wxMenu();

        if (m_iteminfo.selectedLineCount > 1)
        {
            CreatePopupMenuMultiSelection(PopUp);
        }
        else
        {
            CreatePopupMenuSingleSelection(PopUp);
        }

        PopupMenu(PopUp);
        delete PopUp;
    }
}





// TODO: Rewrite this
void CodeView::OnMouseMove(wxMouseEvent& event)
{
	if (event.Dragging())
	{
	    if (!Selecting)
            m_iteminfo.cursorLastPosition = m_iteminfo.cursorPosition;

        if (!HasCapture())
            CaptureMouse();

	    CalcCursorPosition(event.GetPosition());
        ClearCursor();

		if (IncompleteArea.Contains(event.GetPosition()) && (m_iteminfo.cursorPosition < GetLastItem()))
		{
			Scroll(-1, GetFirstLine() + 1);
			RefreshRect(CalcCursorRfshRect());
		}

	    if (Selecting)
        {
            UpdateSelectedRect();
            RefreshRect(CalcSelectedRect());
        }
        else
            RefreshRect(CalcCursorRfshRect());
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

void CodeView::OnMouseEnterWindow(wxMouseEvent& event)
{
    LogIt("Mouse enter the CodeView window...");
}


void CodeView::OnMouseLeaveWindow(wxMouseEvent& event)
{
    LogIt("...Mouse leave the CodeView window.");
}
