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


/// @brief Processes the line scroll down event
/// @param event 
void CodeView::onScrollLineDown(wxScrollWinEvent& event)
{
    if (m_line_info.cursorPosition < static_cast<int>(m_sourcecode->getCount() - 1)) {
        if (!m_multi_selection) {
            m_line_info.cursorLastPosition = m_line_info.cursorPosition;
        }
        ++m_line_info.cursorPosition;
        clearCursor();
        doSelection();

        if (m_line_info.cursorPosition > getLastLine()) {
            Scroll(-1, getFirstLine() + 1);
        }
        RefreshRect(calcCursorRfshRect());

        if (m_line_info.selectedLineCount == 1) {
            treatAsSingleSelection();
        }
        else {
            if (m_line_info.selectedLineCount > 1) {
                treatAsMultiSelection();
            }
        }

        if (m_selecting) {
            LogIt(wxString::Format("Selection = (%d, %d)", m_line_info.firstLine, m_line_info.lastLine));
        }
    }
}



/// @brief Processes the line scroll up event
/// @param event 
void CodeView::onScrollLineUp(wxScrollWinEvent& event)
{
    if (m_line_info.cursorPosition > 0) {
        if (!m_multi_selection) {
            m_line_info.cursorLastPosition = m_line_info.cursorPosition;
        }

        m_line_info.cursorPosition--;
        clearCursor();
        doSelection();

        if (m_line_info.cursorPosition < getFirstLine()) {
            Scroll(-1, getFirstLine() - 1);
        }
        RefreshRect(calcCursorRfshRect());

        if (m_line_info.selectedLineCount == 1) {
            treatAsSingleSelection();
        }
        else {
            if (m_line_info.selectedLineCount > 1) {
                treatAsMultiSelection();
            }
        }

        if (m_selecting) {
            LogIt(wxString::Format("Selection = (%d, %d)", m_line_info.firstLine, m_line_info.lastLine));
        }
    }
}



/// @brief Processes the page scroll down event
/// @param event 
void CodeView::onScrollPageDown(wxScrollWinEvent& event)
{
    int page;

    if (getFirstLine() < static_cast<int>(m_sourcecode->getCount() - m_lines_shown)) {
        page = m_line_info.cursorPosition;
        Scroll(-1, getFirstLine() + m_lines_shown);
        updateLastCursorRect();
        m_line_info.cursorLastPosition = getLastLine();
        m_line_info.cursorPosition = m_line_info.cursorLastPosition;
        page = m_line_info.cursorPosition - page;
        if (page < static_cast<int>(m_lines_shown)) {
            RefreshRect(*m_last_cursor_rect);
        }
    }
}



/// @brief Processes the page scroll up event
/// @param event 
void CodeView::onScrollPageUp(wxScrollWinEvent& event)
{
    int page;

    if (getFirstLine() > 0) {
        page = m_line_info.cursorPosition;
        Scroll(-1,getFirstLine() - m_lines_shown);
        updateLastCursorRect();
        m_line_info.cursorLastPosition = getFirstLine();
        m_line_info.cursorPosition = m_line_info.cursorLastPosition;
        page -= m_line_info.cursorPosition;
        if (page < static_cast<int>(m_lines_shown)) {
            RefreshRect(*m_last_cursor_rect);
        }
    }
}



/// @brief Processes the page down key pressed event
void CodeView::cursorPageDown()
{
    wxScrollWinEvent evtPageDown(wxEVT_SCROLLWIN_PAGEDOWN);
    AddPendingEvent(evtPageDown);
}



/// @brief Processes the page up key pressed event
void CodeView::cursorPageUp()
{
    wxScrollWinEvent evtPageUp(wxEVT_SCROLLWIN_PAGEUP);
    AddPendingEvent(evtPageUp);
}



/// @brief Processes the key down pressed event
void CodeView::cursorDown()
{
    wxScrollWinEvent evtLineDown(wxEVT_SCROLLWIN_LINEDOWN);
    AddPendingEvent(evtLineDown);
}



/// @brief Processes the key up pressed event
void CodeView::cursorUp()
{
    wxScrollWinEvent evtLineUp(wxEVT_SCROLLWIN_LINEUP);
    AddPendingEvent(evtLineUp);
}



/// @brief Event handler to key pressed events
/// @param event 
void CodeView::onKeyPress(wxKeyEvent& event)
{
    int key = event.GetKeyCode();

    switch (key) {
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
						m_selecting = true;
                        m_multi_selection = true;
                        break;
        default:
                        //LogIt(wxString::Format("onKeyPress: <%X> Pressed !", key));
                        event.Skip();
    }
}



/// @brief Event handler to key release events
/// @param event 
void CodeView::onKeyRelease(wxKeyEvent& event)
{
    int key = event.GetKeyCode();

    switch (key) {
        case WXK_SHIFT:
                        m_selecting = false;
                        break;
        default:
                        //LogIt(wxString::Format("onKeyRelease: <%X> Released !", key));
                        event.Skip();
    }
}


/// @brief Event handler to continue search commands
/// @param event 
void CodeView::onSearchContinue(wxCommandEvent& event)
{
    AbsoluteAddress address;

    /*TODO: Rewrite searchs
    if (m_process->searchInstructionArgumentContinue(address)) {
        centerAddress(address);
    }
    */
}



/// @brief Event handler to window get focus
/// @param event 
void CodeView::onGetFocus(wxFocusEvent& event)
{
    if (!m_is_focused) {
        m_is_focused = true;
        LogIt("Entrei...");
    }
}



/// @brief Event handler to window kill focus
/// @param event 
void CodeView::onKillFocus(wxFocusEvent& event)
{
    if (m_is_focused) {
        m_is_focused = false;
        LogIt("Saí...");
        wxClientDC dc(this);
        m_disassemble_window_bitmap = dc.GetAsBitmap();
    }
}



/// @brief Event handler to resize codeview
/// @param event 
void CodeView::onSize(wxSizeEvent& event)
{
    calcLinesShown();
    calcIncompleteArea();
    Refresh();
}



/// @brief Event handler to menu search
/// @param event 
void CodeView::onPopUpMenuSearch(wxCommandEvent& event)
{
    LogIt("Search Menu Clicked !!!\n");
}



/// @brief Event handler to go to a line
/// @param event 
void CodeView::onPopUpMenuGoto(wxCommandEvent& event)
{
    SourceCodeLine      *sc_line;
    DisassembledItem    *dasm_item;
    uint    address;

    sc_line = m_sourcecode->line(m_line_info.cursorPosition);
    dasm_item = m_sourcecode->getDisassembled()->getData(sc_line->dasmedItem);
    address = dasm_item->getArgumentValue(0, m_sourcecode->getDisassembled()->getBaseAddress(sc_line->dasmedItem));
    centerAddress(address);
}



/// @brief Event handler to go to an address line. Shows a dialog.
/// @param event 
void CodeView::onPopUpMenuGotoAddress(wxCommandEvent& event)
{
    long address = 0;
    wxTextEntryDialog enter_address(0, "Enter Address", "Address to go");

    if (enter_address.ShowModal() == wxID_OK) {
        if (enter_address.GetValue().ToLong(&address) ||
            enter_address.GetValue().ToLong(&address, 16)) {

            #ifdef IDZ80DEBUG
            LogIt(wxString::Format("Going to %X", address));
            #endif

            centerAddress(address);
        }
    }
}



/// @brief Event handler to transform instruction lines to data lines
/// @param event 
void CodeView::onPopUpMenuMakeData(wxCommandEvent& event)
{
        m_process->transformToData(m_sourcecode, m_line_info);
        updateSelectedRect();
		Refresh();
}



/// @brief Event handler to perform a disassembly of data, from a pop up menu command.
/// @param event 
void CodeView::onPopUpMenuDisasm(wxCommandEvent& event)
{
        m_process->disassembleData(m_sourcecode, m_line_info);
		updateSelectedRect();
		Refresh();
}



/// @brief Event handler to add a comment, from a pop up menu command.
/// @param event 
void CodeView::onPopUpAddComment(wxCommandEvent& event)
{
    SourceCodeLine *sc_line;
    wxString comment;

    sc_line = m_sourcecode->line(m_line_info.cursorPosition);
    if (sc_line) {
        if (sc_line->comment == 0) {
            comment = ::wxGetTextFromUser("Add Comment", "Add");
            if (!comment.IsEmpty()) {
                comment = comment.Trim(false);
                if (comment.Left(1) != ";") {
                    comment.Prepend("; ");
                }
                m_sourcecode->appendComment(comment, m_line_info.cursorPosition);
                Refresh();
            }
        }
    }
}



/// @brief Event handler to edit a comment, from a pop up menu command.
/// @param event 
void CodeView::onPopUpEditComment(wxCommandEvent& event)
{
    SourceCodeLine *sc_line;
    wxString comment;

    sc_line = m_sourcecode->line(m_line_info.cursorPosition);
	if ((sc_line) && (sc_line->comment)) {
		comment = ::wxGetTextFromUser("Edit Comment", "Edit", sc_line->comment->c_str());
    }
	if (!comment.IsEmpty()) {
		comment = comment.Trim(false);
		if (comment.Left(1) != ";") {
			comment.Prepend("; ");
        }
		m_sourcecode->editComment(comment, m_line_info.cursorPosition);
		Refresh();
	}
}



/// @brief Event handler to delete a comment, from a pop up menu command.
/// @param event 
void CodeView::onPopUpDelComment(wxCommandEvent& event)
{
    SourceCodeLine *sc_line;
    sc_line = m_sourcecode->line(m_line_info.cursorPosition);
    if (sc_line) {
        if ((sc_line->dasmedItem == -1) && (sc_line->labelProgAddress) && (sc_line->labelVarAddress) && (sc_line->originAddress == -1)) {
            m_sourcecode->delSCItem(sc_line);
        }
        else {
            m_sourcecode->delComment(sc_line);
        }
    }
    Refresh();
}



/// @brief Event handler to format data sequence to a matrix, from a pop up menu command.
/// @param event 
void CodeView::onPopUpMenuOD_Matrix(wxCommandEvent &event)
{
	LogIt("Data Matrixed !!\n");
}



/// @brief Event handler to format data sequence to a string, from a pop up menu command.
/// @param event 
void CodeView::onPopUpMenuOD_String(wxCommandEvent& event)
{
	LogIt("Data stringed !!\n");
}



/// @brief Event handler to format data sequence to numbers, from a pop up menu command.
/// @param event 
void CodeView::onPopUpMenuOD_Number(wxCommandEvent& event)
{
	LogIt("Data numbered !!\n");
}



/// @brief Event handler to rename a label, from a pop up menu command.
/// @param event 
void CodeView::onPopUpMenuRenLabel(wxCommandEvent& event)
{
    SourceCodeLine *sc_line;
    DisassembledItem *dasm_item;
    sc_line = m_sourcecode->line(m_line_info.cursorPosition);
    //TODO: Implement label editing in instructions
	if (sc_line)
	{
	    dasm_item = m_line_info.dasmitem;

		if (sc_line->labelProgAddress) {
			m_process->m_labels->prog_labels->editLabelDialog(sc_line->labelProgAddress->address);
        }
        else {
            if (dasm_item && (dasm_item->isArgumentProgramAddress())) {
                m_process->m_labels->prog_labels->editLabelDialog(dasm_item->getArgumentValue(0, 0));
            }
        }

		if (sc_line->labelVarAddress) {
			m_process->m_labels->var_labels->editLabelDialog(sc_line->labelVarAddress->address);
        }
        else {
            if (dasm_item && (dasm_item->isArgumentVariableAddress())) {
                m_process->m_labels->var_labels->editLabelDialog(dasm_item->getArgumentValue(0, 0));
            }
        }

        Refresh();
	}
}



/// @brief Event handler to delete a label, from a pop up menu command.
/// @param event 
void CodeView::onPopUpMenuDelLabel(wxCommandEvent& event)
{
    if (m_line_info.type == lt_LineLabelProg)
    {
        m_process->removeLineAndProgLabels(m_sourcecode, m_line_info.firstLine);
        Refresh();
    }

    if (m_line_info.type == lt_LineLabelVar)
    {
        m_process->removeLineAndVarLabels(m_sourcecode, m_line_info.firstLine);
        Refresh();
    }

    if (m_line_info.type == lt_InstructionLabel)
    {
        m_process->removeFromLabelUserList(m_line_info.dasmitem, m_line_info.firstInstruction);
        m_line_info.dasmitem->resetArgumentStyle();
        RefreshRect(calcCursorRfshRect());
    }
}



/// @brief Event handler to create a label, from a pop up menu command.
/// @param event 
void CodeView::onPopUpMenuCreateLabel(wxCommandEvent& event)
{
    //TODO: Fully implement this method
    if (m_line_info.type == lt_Data) {
        LogIt(wxString::Format("Label created for address %.4X.", m_line_info.firstAddress));
    }
}




/// @brief Event handler to change the numerical system of an argument to binary, from a pop up menu command.
/// @param event 
void CodeView::onPopUpMenuArgStyleBin(wxCommandEvent &event)
{
    ArgumentStyleOptions style = STYLE_NONE;
    MnemonicItem *mnemonic = m_line_info.dasmitem->getMnemonic();

    if (mnemonic) {
        switch(mnemonic->GetArgumentSize()) {
            case 2:
                style = STYLE_WORD_BIN;
                break;
            case 1:
                style = STYLE_BYTE_BIN;
                break;
            case 0:
            default:
                style = STYLE_NONE;
        }
    }

	if (m_line_info.argSelected == 1) {
		m_line_info.dasmitem->setArgumentStyle(0, style);
    }

	if (m_line_info.argSelected == 2) {
		m_line_info.dasmitem->setArgumentStyle(1, style);
    }

	RefreshRect(calcCursorRfshRect());
}



/// @brief Event handler to change the numerical system of an argument to decimal, from a pop up menu command.
/// @param event 
void CodeView::onPopUpMenuArgStyleDec(wxCommandEvent& event)
{
    ArgumentStyleOptions style = STYLE_NONE;
    MnemonicItem *mnemonic = m_line_info.dasmitem->getMnemonic();

    if (mnemonic) {
        switch(mnemonic->GetArgumentSize()) {
            case 2:
                style = STYLE_WORD_DEC;
                break;
            case 1:
                style = STYLE_BYTE_DEC;
                break;
            case 0:
            default:
                style = STYLE_NONE;
        }
    }

	if (m_line_info.argSelected == 1) {
		m_line_info.dasmitem->setArgumentStyle(0, style);
    }

	if (m_line_info.argSelected == 2) {
		m_line_info.dasmitem->setArgumentStyle(1, style);
    }

	RefreshRect(calcCursorRfshRect());
}



/// @brief Event handler to change the numerical system of an argument to hexadecimal, from a pop up menu command.
/// @param event 
void CodeView::onPopUpMenuArgStyleHex(wxCommandEvent& event)
{
    ArgumentStyleOptions style = STYLE_NONE;
    MnemonicItem *mnemonic = m_line_info.dasmitem->getMnemonic();

    if (mnemonic) {
        switch(mnemonic->GetArgumentSize()) {
            case 2:
                style = STYLE_WORD_HEX;
                break;
            case 1:
                style = STYLE_BYTE_HEX;
                break;
            case 0:
            default:
                style = STYLE_NONE;
        }
    }

	if (m_line_info.argSelected == 1) {
		m_line_info.dasmitem->setArgumentStyle(0, style);
    }

	if (m_line_info.argSelected == 2) {
		m_line_info.dasmitem->setArgumentStyle(1, style);
    }

	RefreshRect(calcCursorRfshRect());
}




/// @brief Event handler to search for an argument, from a pop up menu command. (Not written)
/// @param event 
void CodeView::onPopUpMenuSearchArgument(wxCommandEvent& event)
{
    /*  TODO: Rewrite this
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
    */
}



/// @brief Event handler to report the lost of the mouse capture
/// @param event 
void CodeView::onMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
    LogIt("Mouse Lost !!!!");
}

