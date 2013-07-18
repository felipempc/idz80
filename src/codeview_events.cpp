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
#include "codeview.h"



void CodeView::OnScrollLineDown(wxScrollWinEvent& event)
{
    Scroll(-1, GetFirstLine() + 1);
    RefreshRect(CalcCursorRfshRect());
}



void CodeView::OnScrollLineUp(wxScrollWinEvent& event)
{
    Scroll(-1, GetFirstLine() - 1);
}


void CodeView::OnScrollPageDown(wxScrollWinEvent& event)
{
    int page;

    if (GetFirstLine() < static_cast<int>(m_CodeViewLine->GetCount() - m_linesShown))
    {
        page = line_info.cursorPosition;
        Scroll(-1, GetFirstLine() + m_linesShown);
        UpdateLastCursorRect();
        line_info.cursorLastPosition = GetLastLine();
        line_info.cursorPosition = line_info.cursorLastPosition;
        page = line_info.cursorPosition - page;
        if (page < static_cast<int>(m_linesShown))
            RefreshRect(*LastCursorRect);
    }
}


void CodeView::OnScrollPageUp(wxScrollWinEvent& event)
{
    int page;

    if (GetFirstLine() > 0)
    {
        page = line_info.cursorPosition;
        Scroll(-1,GetFirstLine() - m_linesShown);
        UpdateLastCursorRect();
        line_info.cursorLastPosition = GetFirstLine();
        line_info.cursorPosition = line_info.cursorLastPosition;
        page -= line_info.cursorPosition;
        if (page < static_cast<int>(m_linesShown))
            RefreshRect(*LastCursorRect);
    }
}



// Event handler for Page up/down and direction keys
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
                        if (line_info.cursorPosition < static_cast<int>(m_CodeViewLine->GetCount() - 1))
                        {
                            if (!MultiSelection)
                                line_info.cursorLastPosition = line_info.cursorPosition;

                            line_info.cursorPosition++;

                            ClearCursor();
                            DoSelection();

                            if (line_info.cursorPosition > GetLastLine())
                                AddPendingEvent(evtLineDown);
                            else
                                RefreshRect(CalcCursorRfshRect());

                            if (line_info.selectedLineCount == 1)
                                TreatSingleSelection();
                            else
                                if (line_info.selectedLineCount > 1)
                                    TreatMultiSelection();

                            if (Selecting)
                                LogIt(wxString::Format("Selection = (%d, %d)", line_info.firstLine, line_info.lastLine));
                        }
                        break;
        case WXK_UP:
                        if (line_info.cursorPosition > 0)
                        {
                            if (!MultiSelection)
                                line_info.cursorLastPosition = line_info.cursorPosition;

                            line_info.cursorPosition--;

                            ClearCursor();
                            DoSelection();

                            if (line_info.cursorPosition < GetFirstLine())
                                AddPendingEvent(evtLineUp);
                            else
                                RefreshRect(CalcCursorRfshRect());

                            if (line_info.selectedLineCount == 1)
                                TreatSingleSelection();
                            else
                                if (line_info.selectedLineCount > 1)
                                    TreatMultiSelection();

                            if (Selecting)
                                LogIt(wxString::Format("Selection = (%d, %d)", line_info.firstLine, line_info.lastLine));
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
                        AddPendingEvent(evtDisassemble);
                        break;
        case D_KEY:
                        AddPendingEvent(evtMakeData);
                        break;

        default:
                        //LogIt(wxString::Format("%d pressed !\n",key));
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
    {
        IsFocused = true;
        LogIt("Entrei...");
    }
}

void CodeView::OnKillFocus(wxFocusEvent& event)
{
    if (IsFocused)
    {
        IsFocused = false;
        LogIt("Saí...");
        wxClientDC dc(this);
        DisassembleWindowBitmap = dc.GetAsBitmap();
    }
}



void CodeView::OnSize(wxSizeEvent& event)
{
    CalcItemsShown();
    CalcIncompleteArea();
    Refresh();
}



void CodeView::OnPopUpMenuSearch(wxCommandEvent& event)
{
    LogIt("Search Menu Clicked !!!\n");
}




void CodeView::OnPopUpMenuGoto(wxCommandEvent& event)
{
    CodeViewItem	*cvi;
    DisassembledItem		*de;
    uint 			address;

    cvi = m_CodeViewLine->getData(line_info.cursorPosition);
    de = Process->Disassembled->GetData(cvi->Dasmitem);
    address = de->GetArgument(0, Process->Disassembled->GetBaseAddress(cvi->Dasmitem));
    CenterAddress(address);

}



void CodeView::OnPopUpMenuGotoAddress(wxCommandEvent& event)
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

            CenterAddress(address);
        }
    }
}



/*
 * Take selected lines of code and convert to lines
 * of data.
 */
void CodeView::OnPopUpMenuMakeData(wxCommandEvent& event)
{
        Process->TransformToData(line_info);
        UpdateSelectedRect();
		Refresh();
}


void CodeView::OnPopUpMenuDisasm(wxCommandEvent& event)
{
        Process->DisassembleData(line_info);
		UpdateSelectedRect();
		Refresh();
}



void CodeView::OnPopUpAddComment(wxCommandEvent& event)
{
    CodeViewItem *cvi;
    wxString comment;

    cvi = m_CodeViewLine->getData(line_info.cursorPosition);

    if (cvi)
    {
        if (cvi->Comment == 0)
        {
            comment = ::wxGetTextFromUser("Add Comment", "Add");
            if (!comment.IsEmpty())
            {
                comment = comment.Trim(false);
                if (comment.Left(1) != ";")
                    comment.Prepend("; ");
                m_CodeViewLine->AppendComment(comment, line_info.cursorPosition);
                Refresh();
            }
        }
    }
}

void CodeView::OnPopUpEditComment(wxCommandEvent& event)
{
    CodeViewItem *cvi;
    wxString comment;

    cvi = m_CodeViewLine->getData(line_info.cursorPosition);
	if ((cvi) && (cvi->Comment))
		comment = ::wxGetTextFromUser("Edit Comment", "Edit", cvi->Comment->c_str());
	if (!comment.IsEmpty())
	{
		comment = comment.Trim(false);
		if (comment.Left(1) != ";")
			comment.Prepend("; ");
		m_CodeViewLine->Edit(comment, line_info.cursorPosition);
		Refresh();
	}
}

void CodeView::OnPopUpDelComment(wxCommandEvent& event)
{
    CodeViewItem *cvi;
    cvi = m_CodeViewLine->getData(line_info.cursorPosition);
    if (cvi != 0)
    {
        if ((cvi->Dasmitem == -1) && (cvi->LabelProgAddr) && (cvi->LabelVarAddr) && (cvi->Org == -1))
            m_CodeViewLine->DelItem(cvi);
        else
            m_CodeViewLine->DelComment(cvi);
        Refresh();
    }
}


void CodeView::OnPopUpMenuOD_Matrix(wxCommandEvent &event)
{
	LogIt("Data Matrixed !!\n");
}

void CodeView::OnPopUpMenuOD_String(wxCommandEvent& event)
{
	LogIt("Data stringed !!\n");
}

void CodeView::OnPopUpMenuOD_Number(wxCommandEvent& event)
{
	LogIt("Data numbered !!\n");
}

void CodeView::OnPopUpMenuRenLabel(wxCommandEvent& event)
{
    CodeViewItem *cvi;
    DisassembledItem *de;
    cvi = m_CodeViewLine->getData(line_info.cursorPosition);
    //TODO: Implement label editing in instructions
	if (cvi != 0)
	{
	    de = line_info.dasmitem;

		if (cvi->LabelProgAddr >= 0)
			Process->prog_labels->EditLabelDialog(cvi->LabelProgAddr->Address);
        else
            if ((de != 0) && (de->IsArgumentProgramAddress()))
                Process->prog_labels->EditLabelDialog(de->GetArgument(0, 0));
		if (cvi->LabelVarAddr >= 0)
			Process->var_labels->EditLabelDialog(cvi->LabelVarAddr->Address);
        else
            if ((de != 0) && (de->IsArgumentVariableAddress()))
                Process->var_labels->EditLabelDialog(de->GetArgument(0, 0));

        Refresh();
	}
}

void CodeView::OnPopUpMenuDelLabel(wxCommandEvent& event)
{
    if (line_info.type == siLineLabelProg)
        Process->RemoveLineAndProgLabels(line_info.firstLine);

    if (line_info.type == siLineLabelVar)
        Process->RemoveLineAndVarLabels(line_info.firstLine);

    if (line_info.type == siInstructionLabel)
        Process->RemoveFromLabelUserList(line_info.dasmitem, line_info.firstInstruction);
}

void CodeView::OnPopUpMenuCreateLabel(wxCommandEvent& event)
{
    if (line_info.type == siData)
        LogIt(wxString::Format("Label created for address %.4X.", line_info.firstAddress));
}




void CodeView::OnPopUpMenuArgStyleBin(wxCommandEvent &event)
{
	if (line_info.argSelected == 1)
		line_info.dasmitem->SetStyleArgument(0, ast_bytebin);

	if (line_info.argSelected == 2)
		line_info.dasmitem->SetStyleArgument(1, ast_bytebin);

	RefreshRect(CalcCursorRfshRect());
}


void CodeView::OnPopUpMenuArgStyleDec(wxCommandEvent& event)
{
	if (line_info.argSelected == 1)
		line_info.dasmitem->SetStyleArgument(0, ast_bytedec);

	if (line_info.argSelected == 2)
		line_info.dasmitem->SetStyleArgument(1, ast_bytedec);

	RefreshRect(CalcCursorRfshRect());
}


void CodeView::OnPopUpMenuArgStyleHex(wxCommandEvent& event)
{
	if (line_info.argSelected == 1)
		line_info.dasmitem->SetStyleArgument(0, ast_bytehex);

	if (line_info.argSelected == 2)
		line_info.dasmitem->SetStyleArgument(1, ast_bytehex);

		RefreshRect(CalcCursorRfshRect());
}



void CodeView::OnMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
    LogIt("Mouse Lost !!!!");
}



