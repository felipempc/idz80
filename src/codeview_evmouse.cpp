/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   30-12-2011 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * User interface - Mouse event handlers
 **************************************************************/


#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/mousestate.h>
#include "codeview.h"
#include "disassembled_item.h"



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
        line_info.cursorLastPosition = line_info.cursorPosition;

	lastposition = line_info.cursorPosition;
	pt = event.GetPosition();
    CalcCursorPosition(pt);
	if (line_info.cursorPosition < GetCount())
	{
		ClearCursor();

		if (line_info.cursorPosition > GetLastLine())
		{
			Scroll(-1, GetFirstLine() + 1);
			RefreshRect(CalcCursorRfshRect());
		}

		DoSelection();

		FillSelectedItemInfo(pt);

		LogIt(wxString::Format("Label Prog = %p", line_info.lineitem->LabelProgAddr));
		LogIt(wxString::Format("Label Var = %p", line_info.lineitem->LabelVarAddr));

		RefreshRect(CalcCursorRfshRect());
	}
	else
		line_info.cursorPosition = lastposition;

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
        line_info.cursorLastPosition = line_info.cursorPosition;

	lastposition = line_info.cursorPosition;
	pt = event.GetPosition();
    CalcCursorPosition(pt);
	if (line_info.cursorPosition < GetCount())
	{
		ClearCursor();

		if (MultiSelection && ((line_info.cursorPosition < line_info.firstLine) ||
			(line_info.cursorPosition > line_info.lastLine)))
		{
			RefreshRect(CalcSelectedRect());
			line_info.selectedLineCount = 1;
			MultiSelection = false;
			line_info.firstLine = line_info.cursorPosition;
			line_info.cursorLastPosition = line_info.cursorPosition;
			line_info.lastLine = line_info.cursorPosition;
		}
		else
			if (!MultiSelection)
			{
				line_info.selectedLineCount = 1;
				line_info.firstLine = line_info.cursorPosition;
				line_info.cursorLastPosition = line_info.cursorPosition;
				line_info.lastLine = line_info.firstLine;
			}

		FillSelectedItemInfo(pt);

		RefreshRect(CalcCursorRfshRect());
	}
	else
		line_info.cursorPosition = lastposition;

}


void CodeView::OnMouseRightUp(wxMouseEvent& event)
{
    wxMenu      *PopUp;

    if (!IsEmpty)
    {
        PopUp = new wxMenu();

        if (line_info.selectedLineCount > 1)
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
            line_info.cursorLastPosition = line_info.cursorPosition;

        if (!HasCapture())
            CaptureMouse();

	    CalcCursorPosition(event.GetPosition());
        ClearCursor();

		if (IncompleteArea.Contains(event.GetPosition()) && (line_info.cursorPosition < GetLastItem()))
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
        if (position > static_cast<int>(m_CodeViewLine->GetCount() - m_linesShown))
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
