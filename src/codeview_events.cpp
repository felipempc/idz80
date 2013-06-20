/****************************************************************
 * Name:      Codeview - Event handlers
 * Purpose:   Renders the view
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   30-12-2011
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 *
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

    if (GetFirstLine() < (m_CodeViewLine->GetCount() - m_linesShown))
    {
        page = m_iteminfo.cursorPosition;
        Scroll(-1, GetFirstLine() + m_linesShown);
        UpdateLastCursorRect();
        m_iteminfo.cursorLastPosition = GetLastLine();
        m_iteminfo.cursorPosition = m_iteminfo.cursorLastPosition;
        page = m_iteminfo.cursorPosition - page;
        if (page < m_linesShown)
            RefreshRect(*LastCursorRect);
    }
}


void CodeView::OnScrollPageUp(wxScrollWinEvent& event)
{
    int page;

    if (GetFirstLine() > 0)
    {
        page = m_iteminfo.cursorPosition;
        Scroll(-1,GetFirstLine() - m_linesShown);
        UpdateLastCursorRect();
        m_iteminfo.cursorLastPosition = GetFirstLine();
        m_iteminfo.cursorPosition = m_iteminfo.cursorLastPosition;
        page -= m_iteminfo.cursorPosition;
        if (page < m_linesShown)
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
                        if (m_iteminfo.cursorPosition < (m_CodeViewLine->GetCount() - 1))
                        {
                            if (!MultiSelection)
                                m_iteminfo.cursorLastPosition = m_iteminfo.cursorPosition;

                            m_iteminfo.cursorPosition++;

                            ClearCursor();
                            DoSelection();

                            if (m_iteminfo.cursorPosition > GetLastLine())
                                AddPendingEvent(evtLineDown);
                            else
                                RefreshRect(CalcCursorRfshRect());

                            if (m_iteminfo.selectedLineCount == 1)
                                TreatSingleSelection();
                            else
                                if (m_iteminfo.selectedLineCount > 1)
                                    TreatMultiSelection();

                            if (Selecting)
                                LogIt(wxString::Format("Selection = (%d, %d)", m_iteminfo.firstLine, m_iteminfo.lastLine));
                        }
                        break;
        case WXK_UP:
                        if (m_iteminfo.cursorPosition > 0)
                        {
                            if (!MultiSelection)
                                m_iteminfo.cursorLastPosition = m_iteminfo.cursorPosition;

                            m_iteminfo.cursorPosition--;

                            ClearCursor();
                            DoSelection();

                            if (m_iteminfo.cursorPosition < GetFirstLine())
                                AddPendingEvent(evtLineUp);
                            else
                                RefreshRect(CalcCursorRfshRect());

                            if (m_iteminfo.selectedLineCount == 1)
                                TreatSingleSelection();
                            else
                                if (m_iteminfo.selectedLineCount > 1)
                                    TreatMultiSelection();

                            if (Selecting)
                                LogIt(wxString::Format("Selection = (%d, %d)", m_iteminfo.firstLine, m_iteminfo.lastLine));
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
    DAsmElement		*de;
    uint 			address;

    cvi = m_CodeViewLine->getData(m_iteminfo.cursorPosition);
    de = Process->Disassembled->GetData(cvi->Dasmitem);
    address = de->getArgument(0, Process->Disassembled->GetBaseAddress(cvi->Dasmitem));
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
 * Delete program labels;
 * Keeps var labels;
 * Keeps comments;
 * keeps org directives;
 */
void CodeView::OnPopUpMenuMakeData(wxCommandEvent& event)
{
    RangeItems		dasmed_items;
    CodeViewItem	*cvi;
    DAsmElement     *de;
    int 			i, newLineCount, lineIndex, lineLast, lineCount,
					oldLineCount, varindex;
    uint            first_address, last_address;
    wxArrayInt		cvlines, proglabels, varlabels;

	//TODO: Create a function to remove unused labels


    if ((m_iteminfo.firstInstruction <= m_iteminfo.lastInstruction) &&
        (m_iteminfo.lastInstruction < Process->Disassembled->GetCount()))
    {
        /*
		for(i = 0; i < proglabels.GetCount(); i++)
		{
			Process->prog_labels->DelLabel(proglabels[i]);
		}
		*/

        if (m_iteminfo.firstLine > 0)
            cvi = m_CodeViewLine->getData(m_iteminfo.firstLine - 1);

        lineIndex = m_iteminfo.firstLine;
        lineLast = m_iteminfo.lastLine;
        lineCount = lineLast - lineIndex + 1;

        dasmed_items.Index = m_iteminfo.firstInstruction;
        dasmed_items.Count = m_iteminfo.lastInstruction - m_iteminfo.firstInstruction + 1;
        oldLineCount = dasmed_items.Count;

        Process->var_labels->GetLabelsBetweenRangeAddress(m_iteminfo.firstAddress, m_iteminfo.lastAddress, &varlabels);

        Process->MakeData(dasmed_items);

        for (i = 0; i < lineCount; i++)
				m_CodeViewLine->Del(lineIndex);

//* TODO:REWRITE THIS
        m_CodeViewLine->linkData(dasmed_items.Index, lineIndex, dasmed_items.Count);

        newLineCount = dasmed_items.Count - oldLineCount;

        m_CodeViewLine->UpdateDasmIndex((lineIndex + dasmed_items.Count), newLineCount);

        for (i = 0; i < varlabels.GetCount(); i++)
        {
            m_CodeViewLine->getLineOfAddress(lineIndex, (lineCount + newLineCount), varlabels.Item(i), varindex);
            if ((cvi) && (cvi->LabelVarAddr != varlabels[i]))
            {
                LogIt(wxString::Format("Found var address 0x%X, line %d\n", varlabels.Item(i), varindex));
                m_CodeViewLine->InsertVarLabel(varlabels[i], "", varindex);
            }
        }
//*/
        m_iteminfo.cursorLastPosition = lineIndex;
        m_iteminfo.cursorPosition = lineIndex + lineCount + newLineCount - 1;
        UpdateSelectedRect();
		Refresh();
    }
}


void CodeView::OnPopUpMenuDisasm(wxCommandEvent& event)
{
    RangeItems		dasmed_items;
    CodeViewItem	*cvi;
    int				i, newLineCount, oldLineCount, varindex;
    int				lineIndex, lineLast, lineCount;
    wxArrayInt		cvlines, varlabels;

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

        Process->var_labels->GetLabelsBetweenRangeAddress(m_iteminfo.firstAddress, m_iteminfo.lastAddress, &varlabels);

		for (i = 0; i < lineCount; i++)
				m_CodeViewLine->Del(lineIndex);

		Process->DisassembleItems(dasmed_items);

		m_CodeViewLine->linkData(dasmed_items.Index, lineIndex, dasmed_items.Count);

		newLineCount = dasmed_items.Count - oldLineCount;

		m_CodeViewLine->UpdateDasmIndex((lineIndex + dasmed_items.Count), newLineCount);

        if (lineIndex > 0)
            cvi = m_CodeViewLine->getData(lineIndex - 1);

        for (i = 0; i < varlabels.GetCount(); i++)
        {
            m_CodeViewLine->getLineOfAddress(lineIndex, (lineCount + newLineCount), varlabels.Item(i), varindex);
            if ((cvi) && (cvi->LabelVarAddr != varlabels[i]))
            {
                LogIt(wxString::Format("Found var address 0x%X, line %d\n", varlabels.Item(i), varindex));
                m_CodeViewLine->InsertVarLabel(varlabels[i], "", varindex);
            }
        }

		m_iteminfo.cursorLastPosition = lineIndex;
        m_iteminfo.cursorPosition = lineIndex + lineCount + newLineCount - 1;
		UpdateSelectedRect();
		Refresh();
	}
}



void CodeView::OnPopUpAddComment(wxCommandEvent& event)
{
    CodeViewItem *cvi;
    wxString comment;

    cvi = m_CodeViewLine->getData(m_iteminfo.cursorPosition);

    if (cvi != 0)
    {
        if (cvi->Comment == 0)
        {
            comment = ::wxGetTextFromUser("Add Comment", "Add");
            if (!comment.IsEmpty())
            {
                comment=comment.Trim(false);
                if (comment.Left(1) != ";")
                    comment.Prepend("; ");
                m_CodeViewLine->AppendComment(comment, m_iteminfo.cursorPosition);
                Refresh();
            }
        }
    }
}

void CodeView::OnPopUpEditComment(wxCommandEvent& event)
{
    CodeViewItem *cvi;
    wxString comment;

    cvi = m_CodeViewLine->getData(m_iteminfo.cursorPosition);
	if ((cvi != 0) && (cvi->Comment != 0))
		comment = ::wxGetTextFromUser("Edit Comment", "Edit", cvi->Comment->CommentStr);
	if (!comment.IsEmpty())
	{
		comment = comment.Trim(false);
		if (comment.Left(1) != ";")
			comment.Prepend("; ");
		m_CodeViewLine->Edit(comment, m_iteminfo.cursorPosition);
		Refresh();
	}
}

void CodeView::OnPopUpDelComment(wxCommandEvent& event)
{
    CodeViewItem *cvi;
    cvi = m_CodeViewLine->getData(m_iteminfo.cursorPosition);
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
    DAsmElement *de;
    cvi = m_CodeViewLine->getData(m_iteminfo.cursorPosition);
    //TODO: Implement label editing in instructions
	if (cvi != 0)
	{
	    de = m_iteminfo.dasmitem;

		if (cvi->LabelProgAddr >= 0)
			Process->prog_labels->EditLabelDialog(cvi->LabelProgAddr);
        else
            if ((de != 0) && (de->isArgumentProgramAddress()))
                Process->prog_labels->EditLabelDialog(de->getArgument(0, 0));
		if (cvi->LabelVarAddr >= 0)
			Process->var_labels->EditLabelDialog(cvi->LabelVarAddr);
        else
            if ((de != 0) && (de->isArgumentVariableAddress()))
                Process->var_labels->EditLabelDialog(de->getArgument(0, 0));

        Refresh();
	}
}

void CodeView::OnPopUpMenuDelLabel(wxCommandEvent& event)
{
    if ((m_iteminfo.type == siLineLabelProg) || (m_iteminfo.type == siLineLabelVar))
        LogIt("Address Label deleted !!");
    if (m_iteminfo.type == siInstructionLabel)
        LogIt("Instruction Label deleted !!");
}




void CodeView::OnPopUpMenuArgStyleBin(wxCommandEvent &event)
{
	if (m_iteminfo.argSelected == 1)
		m_iteminfo.dasmitem->SetStyleArgument(0, ast_bytebin);

	if (m_iteminfo.argSelected == 2)
		m_iteminfo.dasmitem->SetStyleArgument(1, ast_bytebin);

	RefreshRect(CalcCursorRfshRect());
}


void CodeView::OnPopUpMenuArgStyleDec(wxCommandEvent& event)
{
	if (m_iteminfo.argSelected == 1)
		m_iteminfo.dasmitem->SetStyleArgument(0, ast_bytedec);

	if (m_iteminfo.argSelected == 2)
		m_iteminfo.dasmitem->SetStyleArgument(1, ast_bytedec);

	RefreshRect(CalcCursorRfshRect());
}


void CodeView::OnPopUpMenuArgStyleHex(wxCommandEvent& event)
{
	if (m_iteminfo.argSelected == 1)
		m_iteminfo.dasmitem->SetStyleArgument(0, ast_bytehex);

	if (m_iteminfo.argSelected == 2)
		m_iteminfo.dasmitem->SetStyleArgument(1, ast_bytehex);

		RefreshRect(CalcCursorRfshRect());
}



void CodeView::OnMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
    LogIt("Mouse Lost !!!!");
}



