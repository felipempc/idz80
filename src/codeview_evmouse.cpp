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
#include "codeview.hpp"
#include "disassembled_item.hpp"



void CodeView::onMouseLeftDown(wxMouseEvent& event)
{
	int lastposition;
	wxPoint pt;

    if (!IsEnabled())
    {
        return;
    }

    SetFocus();
    if (!m_multi_selection)
        m_line_info.cursorLastPosition = m_line_info.cursorPosition;

	lastposition = m_line_info.cursorPosition;
	pt = event.GetPosition();
    calcCursorPosition(pt);
	if (m_line_info.cursorPosition < getCount())
	{
		clearCursor();

		if (m_line_info.cursorPosition > getLastLine())
		{
			Scroll(-1, getFirstLine() + 1);
			RefreshRect(calcCursorRfshRect());
		}

		doSelection();

		fillSelectedItemInfo(pt);

		RefreshRect(calcCursorRfshRect());
	}
	else
		m_line_info.cursorPosition = lastposition;

}


void CodeView::onMouseLeftUp(wxMouseEvent& event)
{
    fillSelectedItemInfo(event.GetPosition());

    if (HasCapture())
        ReleaseMouse();

}



void CodeView::onMouseRightDown(wxMouseEvent& event)
{
	int	lastposition;
	wxPoint pt;

    if (!IsEnabled())
    {
        return;
    }

    SetFocus();

    if (!m_multi_selection)
        m_line_info.cursorLastPosition = m_line_info.cursorPosition;

	lastposition = m_line_info.cursorPosition;
	pt = event.GetPosition();
    calcCursorPosition(pt);
	if (m_line_info.cursorPosition < getCount())
	{
		clearCursor();

		if (m_multi_selection && ((m_line_info.cursorPosition < m_line_info.firstLine) ||
			(m_line_info.cursorPosition > m_line_info.lastLine)))
		{
			RefreshRect(calcSelectedRect());
			m_line_info.selectedLineCount = 1;
			m_multi_selection = false;
			m_line_info.firstLine = m_line_info.cursorPosition;
			m_line_info.cursorLastPosition = m_line_info.cursorPosition;
			m_line_info.lastLine = m_line_info.cursorPosition;
		}
		else
			if (!m_multi_selection)
			{
				m_line_info.selectedLineCount = 1;
				m_line_info.firstLine = m_line_info.cursorPosition;
				m_line_info.cursorLastPosition = m_line_info.cursorPosition;
				m_line_info.lastLine = m_line_info.firstLine;
			}

		fillSelectedItemInfo(pt);

		RefreshRect(calcCursorRfshRect());
	}
	else
		m_line_info.cursorPosition = lastposition;

}


void CodeView::onMouseRightUp(wxMouseEvent& event)
{
    wxMenu      *PopUp;

    if (!m_is_empty)
    {
        PopUp = new wxMenu();

        if (m_line_info.selectedLineCount > 1)
        {
            createPopupMenuMultiSelection(PopUp);
        }
        else
        {
            createPopupMenuSingleSelection(PopUp);
        }

        PopupMenu(PopUp);
        delete PopUp;
    }
}





// TODO: Rewrite this
void CodeView::onMouseMove(wxMouseEvent& event)
{
	if (event.Dragging())
	{
	    if (!m_selecting)
            m_line_info.cursorLastPosition = m_line_info.cursorPosition;

        if (!HasCapture())
            CaptureMouse();

	    calcCursorPosition(event.GetPosition());
        clearCursor();

		if (m_incomplete_area.Contains(event.GetPosition()) && (m_line_info.cursorPosition < getLastItemIndex()))
		{
			Scroll(-1, getFirstLine() + 1);
			RefreshRect(calcCursorRfshRect());
		}

	    if (m_selecting)
        {
            updateSelectedRect();
            RefreshRect(calcSelectedRect());
        }
        else
            RefreshRect(calcCursorRfshRect());
	}
}



void CodeView::onMouseWheel(wxMouseEvent& event)
{
    bool up_motion;
    int motion_step = 3;
    int position;

    up_motion = (event.GetWheelRotation() >= 0);

    if (up_motion)
    {
        position = getFirstLine() - motion_step;
        if (position < 0)
            position = 0;
        Scroll(-1,position);
    }
    else
    {
        position = getFirstLine() + motion_step;
        if (position > static_cast<int>(m_sourcecode->getCount() - m_lines_shown))
            position = m_sourcecode->getCount() - m_lines_shown;
        Scroll(-1,position);
    }
}

void CodeView::onMouseEnterWindow(wxMouseEvent& event)
{
    LogIt("Mouse enter the CodeView window...");
}


void CodeView::onMouseLeaveWindow(wxMouseEvent& event)
{
    LogIt("...Mouse leave the CodeView window.");
}
