/****************************************************************
* Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   30-12-2011 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * User interface - Event handlers
 **************************************************************/


#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include "codeview.hpp"
#include "search_argument_dialog.hpp"


void CodeView::onScrollLineDown(wxScrollWinEvent& event)
{
    if (m_line_info.cursorPosition < static_cast<int>(m_sourcecode->getCount() - 1))
    {
        if (!m_multi_selection)
            m_line_info.cursorLastPosition = m_line_info.cursorPosition;

        m_line_info.cursorPosition++;

        clearCursor();
        doSelection();

        if (m_line_info.cursorPosition > getLastLine())
        {
                Scroll(-1, getFirstLine() + 1);
        }
            RefreshRect(calcCursorRfshRect());

        if (m_line_info.selectedLineCount == 1)
            treatAsSingleSelection();
        else
            if (m_line_info.selectedLineCount > 1)
                treatAsMultiSelection();

        if (m_selecting)
            LogIt(wxString::Format("Selection = (%d, %d)", m_line_info.firstLine, m_line_info.lastLine));
    }
}



void CodeView::onScrollLineUp(wxScrollWinEvent& event)
{
    if (m_line_info.cursorPosition > 0)
    {
        if (!m_multi_selection)
            m_line_info.cursorLastPosition = m_line_info.cursorPosition;

        m_line_info.cursorPosition--;

        clearCursor();
        doSelection();

        if (m_line_info.cursorPosition < getFirstLine())
        {
            Scroll(-1, getFirstLine() - 1);
        }
        RefreshRect(calcCursorRfshRect());

        if (m_line_info.selectedLineCount == 1)
            treatAsSingleSelection();
        else
            if (m_line_info.selectedLineCount > 1)
                treatAsMultiSelection();

        if (m_selecting)
            LogIt(wxString::Format("Selection = (%d, %d)", m_line_info.firstLine, m_line_info.lastLine));
    }
}


void CodeView::onScrollPageDown(wxScrollWinEvent& event)
{
    int page;

    if (getFirstLine() < static_cast<int>(m_sourcecode->getCount() - m_lines_shown))
    {
        page = m_line_info.cursorPosition;
        Scroll(-1, getFirstLine() + m_lines_shown);
        updateLastCursorRect();
        m_line_info.cursorLastPosition = getLastLine();
        m_line_info.cursorPosition = m_line_info.cursorLastPosition;
        page = m_line_info.cursorPosition - page;
        if (page < static_cast<int>(m_lines_shown))
            RefreshRect(*m_last_cursor_rect);
    }
}


void CodeView::onScrollPageUp(wxScrollWinEvent& event)
{
    int page;

    if (getFirstLine() > 0)
    {
        page = m_line_info.cursorPosition;
        Scroll(-1,getFirstLine() - m_lines_shown);
        updateLastCursorRect();
        m_line_info.cursorLastPosition = getFirstLine();
        m_line_info.cursorPosition = m_line_info.cursorLastPosition;
        page -= m_line_info.cursorPosition;
        if (page < static_cast<int>(m_lines_shown))
            RefreshRect(*m_last_cursor_rect);
    }
}


void CodeView::cursorPageDown()
{
    wxScrollWinEvent evtPageDown(wxEVT_SCROLLWIN_PAGEDOWN);
    AddPendingEvent(evtPageDown);
}




void CodeView::cursorPageUp()
{
    wxScrollWinEvent evtPageUp(wxEVT_SCROLLWIN_PAGEUP);
    AddPendingEvent(evtPageUp);
}


void CodeView::cursorDown()
{
    wxScrollWinEvent evtLineDown(wxEVT_SCROLLWIN_LINEDOWN);
    AddPendingEvent(evtLineDown);
}

void CodeView::cursorUp()
{
    wxScrollWinEvent evtLineUp(wxEVT_SCROLLWIN_LINEUP);
    AddPendingEvent(evtLineUp);
}


// Event handler for Page up/down and direction keys
void CodeView::onKeyPress(wxKeyEvent& event)
{
    int key;
    key = event.GetKeyCode();

    switch (key)
    {

        case WXK_DOWN:
                        cursorDown();
                        break;
        case WXK_UP:
                        cursorUp();
                        break;
        case WXK_NUMPAD_PAGEDOWN:
        case WXK_PAGEDOWN:
                        cursorPageDown();
                        break;
        case WXK_NUMPAD_PAGEUP:
        case WXK_PAGEUP:
                        cursorPageUp();
                        break;
        case WXK_SHIFT:
                        if (!m_selecting)
							m_selecting = true;
                        m_multi_selection = true;
                        break;
        default:
                        //LogIt(wxString::Format("onKeyPress: <%X> Pressed !", key));
                        event.Skip();
    }
}



void CodeView::onKeyRelease(wxKeyEvent& event)
{
    int key;
    key = event.GetKeyCode();

    switch (key)
    {
        case WXK_SHIFT:
                        m_selecting = false;
                        break;
        default:
                        //LogIt(wxString::Format("onKeyRelease: <%X> Released !", key));
                        event.Skip();
    }
}


void CodeView::onSearchContinue(wxCommandEvent& event)
{
    AbsoluteAddress address;

    if (m_process->searchInstructionArgumentContinue(address))
    {
        centerAddress(address);
    }
}



void CodeView::onGetFocus(wxFocusEvent& event)
{
    if (!m_is_focused)
    {
        m_is_focused = true;
        LogIt("Entrei...");
    }
}

void CodeView::onKillFocus(wxFocusEvent& event)
{
    if (m_is_focused)
    {
        m_is_focused = false;
        LogIt("Saí...");
        wxClientDC dc(this);
        m_disassemble_window_bitmap = dc.GetAsBitmap();
    }
}



void CodeView::onSize(wxSizeEvent& event)
{
    calcLinesShown();
    calcIncompleteArea();
    Refresh();
}



void CodeView::onPopUpMenuSearch(wxCommandEvent& event)
{
    LogIt("Search Menu Clicked !!!\n");
}




void CodeView::onPopUpMenuGoto(wxCommandEvent& event)
{
    SourceCodeLine	*cvi;
    DisassembledItem		*de;
    uint 			address;

    cvi = m_sourcecode->line(m_line_info.cursorPosition);
    de = m_process->Disassembled->GetData(cvi->dasmedItem);
    address = de->GetArgument(0, m_process->Disassembled->GetBaseAddress(cvi->dasmedItem));
    centerAddress(address);

}



void CodeView::onPopUpMenuGotoAddress(wxCommandEvent& event)
{
    long address = 0;
    wxTextEntryDialog enter_address(0, "Enter Address", "Address to go");

    if (enter_address.ShowModal() == wxID_OK)
    {
        if (enter_address.GetValue().ToLong(&address) ||
            enter_address.GetValue().ToLong(&address, 16))
        {
            #ifdef IDZ80DEBUG
            LogIt(wxString::Format("Going to %X", address));
            #endif

            centerAddress(address);
        }
    }
}



/*
 * Take selected lines of code and convert to lines
 * of data.
 */
void CodeView::onPopUpMenuMakeData(wxCommandEvent& event)
{
        m_process->transformToData(m_line_info);
        updateSelectedRect();
		Refresh();
}


void CodeView::onPopUpMenuDisasm(wxCommandEvent& event)
{
        m_process->disassembleData(m_line_info);
		updateSelectedRect();
		Refresh();
}



void CodeView::onPopUpAddComment(wxCommandEvent& event)
{
    SourceCodeLine *cvi;
    wxString comment;

    cvi = m_sourcecode->line(m_line_info.cursorPosition);

    if (cvi)
    {
        if (cvi->comment == 0)
        {
            comment = ::wxGetTextFromUser("Add Comment", "Add");
            if (!comment.m_is_empty())
            {
                comment = comment.Trim(false);
                if (comment.Left(1) != ";")
                    comment.Prepend("; ");
                m_sourcecode->appendComment(comment, m_line_info.cursorPosition);
                Refresh();
            }
        }
    }
}

void CodeView::onPopUpEditComment(wxCommandEvent& event)
{
    SourceCodeLine *cvi;
    wxString comment;

    cvi = m_sourcecode->line(m_line_info.cursorPosition);
	if ((cvi) && (cvi->comment))
		comment = ::wxGetTextFromUser("Edit Comment", "Edit", cvi->comment->c_str());
	if (!comment.m_is_empty())
	{
		comment = comment.Trim(false);
		if (comment.Left(1) != ";")
			comment.Prepend("; ");
		m_sourcecode->editComment(comment, m_line_info.cursorPosition);
		Refresh();
	}
}

void CodeView::onPopUpDelComment(wxCommandEvent& event)
{
    SourceCodeLine *cvi;
    cvi = m_sourcecode->line(m_line_info.cursorPosition);
    if (cvi != 0)
    {
        if ((cvi->dasmedItem == -1) && (cvi->labelProgAddress) && (cvi->labelVarAddress) && (cvi->originAddress == -1))
            m_sourcecode->delSCItem(cvi);
        else
            m_sourcecode->delComment(cvi);
        Refresh();
    }
}


void CodeView::onPopUpMenuOD_Matrix(wxCommandEvent &event)
{
	LogIt("Data Matrixed !!\n");
}

void CodeView::onPopUpMenuOD_String(wxCommandEvent& event)
{
	LogIt("Data stringed !!\n");
}

void CodeView::onPopUpMenuOD_Number(wxCommandEvent& event)
{
	LogIt("Data numbered !!\n");
}

void CodeView::onPopUpMenuRenLabel(wxCommandEvent& event)
{
    SourceCodeLine *cvi;
    DisassembledItem *de;
    cvi = m_sourcecode->line(m_line_info.cursorPosition);
    //TODO: Implement label editing in instructions
	if (cvi)
	{
	    de = m_line_info.dasmitem;

		if (cvi->labelProgAddress)
			m_process->prog_labels->EditLabelDialog(cvi->labelProgAddress->address);
        else
            if (de && (de->IsArgumentProgramAddress()))
                m_process->prog_labels->EditLabelDialog(de->GetArgument(0, 0));
		if (cvi->labelVarAddress)
			m_process->var_labels->EditLabelDialog(cvi->labelVarAddress->address);
        else
            if (de && (de->IsArgumentVariableAddress()))
                m_process->var_labels->EditLabelDialog(de->GetArgument(0, 0));

        Refresh();
	}
}

void CodeView::onPopUpMenuDelLabel(wxCommandEvent& event)
{
    if (m_line_info.type == lt_LineLabelProg)
    {
        m_process->removeLineAndProgLabels(m_line_info.firstLine);
        Refresh();
    }

    if (m_line_info.type == lt_LineLabelVar)
    {
        m_process->removeLineAndVarLabels(m_line_info.firstLine);
        Refresh();
    }


    if (m_line_info.type == lt_InstructionLabel)
    {
        m_process->removeFromLabelUserList(m_line_info.dasmitem, m_line_info.firstInstruction);
        m_line_info.dasmitem->SetArgLabel(false);
        RefreshRect(calcCursorRfshRect());
    }
}

void CodeView::onPopUpMenuCreateLabel(wxCommandEvent& event)
{
    if (m_line_info.type == lt_Data)
        LogIt(wxString::Format("Label created for address %.4X.", m_line_info.firstAddress));
}




void CodeView::onPopUpMenuArgStyleBin(wxCommandEvent &event)
{
	if (m_line_info.argSelected == 1)
		m_line_info.dasmitem->SetStyleArgument(0, ast_bytebin);

	if (m_line_info.argSelected == 2)
		m_line_info.dasmitem->SetStyleArgument(1, ast_bytebin);

	RefreshRect(calcCursorRfshRect());
}


void CodeView::onPopUpMenuArgStyleDec(wxCommandEvent& event)
{
	if (m_line_info.argSelected == 1)
		m_line_info.dasmitem->SetStyleArgument(0, ast_bytedec);

	if (m_line_info.argSelected == 2)
		m_line_info.dasmitem->SetStyleArgument(1, ast_bytedec);

	RefreshRect(calcCursorRfshRect());
}


void CodeView::onPopUpMenuArgStyleHex(wxCommandEvent& event)
{
	if (m_line_info.argSelected == 1)
		m_line_info.dasmitem->SetStyleArgument(0, ast_bytehex);

	if (m_line_info.argSelected == 2)
		m_line_info.dasmitem->SetStyleArgument(1, ast_bytehex);

		RefreshRect(calcCursorRfshRect());
}




void CodeView::onPopUpMenuSearchArgument(wxCommandEvent& event)
{
    SearchArgumentDialog *search_dialog = new SearchArgumentDialog(this);
    AbsoluteAddress address = 0;

    if (search_dialog->ShowModal() == wxID_OK)
    {
        m_process->searchInstructionArgument(search_dialog->getValue(), search_dialog->getSearchConfig());

        if (m_process->searchInstructionArgumentContinue(address))
        {
            centerAddress(address);
        }

    }



    delete search_dialog;
}




void CodeView::onMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
    LogIt("Mouse Lost !!!!");
}



