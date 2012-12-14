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
        page = CursorPosition;
        Scroll(-1, GetFirstLine() + m_linesShown);
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
                        LogIt("Event Disasm !\n");
                        #endif
                        AddPendingEvent(evtDisassemble);
                        break;
        case D_KEY:
						#ifdef IDZ80DEBUG
                        LogIt("Event makeData !\n");
                        #endif
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

    cvi = m_CodeViewLine->getData(CursorPosition);
    de = Process->Disassembled->GetData(cvi->Dasmitem);
    address = de->getArgument(0, Process->Disassembled->GetBaseAddress(cvi->Dasmitem));
    CenterAddress(address);

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
    int 			i, newLineCount, lineIndex, lineLast, lineCount,
					j, oldLineCount;
    wxArrayInt		cvlines, proglabels;

	//TODO: Create a function to remove unused labels
	//FIXME: It's removing line with var labels

    if (!FilterInstructions(cvlines, &proglabels))
		return;

    if (cvlines.GetCount() > 0)
    {
		for(i = 0; i < proglabels.GetCount(); i++)
		{
			Process->prog_labels->DelLabel(proglabels[i]);
		}

        lineIndex = cvlines[0];
        lineLast = cvlines[1];
        lineCount = lineLast - lineIndex + 1;

        cvi = m_CodeViewLine->getData(lineIndex);
        dasmed_items.Index = cvi->Dasmitem;
        cvi = m_CodeViewLine->getData(lineLast);
        dasmed_items.Count = cvi->Dasmitem - dasmed_items.Index + 1;
        oldLineCount = dasmed_items.Count;

        Process->MakeData(dasmed_items);

		j = 0;
        for (i = 0; i < lineCount; i++)
		{
			if (m_CodeViewLine->getData(lineIndex + j)->LabelVarAddr >= 0)
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

	if (!FilterInstructions(cvlines, 0))
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

		Process->DisassembleItems(dasmed_items);

		m_CodeViewLine->linkData(dasmed_items.Index, lineIndex, dasmed_items.Count);

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
            comment = ::wxGetTextFromUser("Add Comment", "Add");
            if (!comment.IsEmpty())
            {
                comment=comment.Trim(false);
                if (comment.Left(1) != ";")
                    comment.Prepend("; ");
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
		comment = ::wxGetTextFromUser("Edit Comment", "Edit", cvi->Comment->CommentStr);
	if (!comment.IsEmpty())
	{
		comment = comment.Trim(false);
		if (comment.Left(1) != ";")
			comment.Prepend("; ");
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
    cvi = m_CodeViewLine->getData(CursorPosition);
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



