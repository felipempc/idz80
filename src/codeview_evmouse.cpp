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
        CursorLastPosition = CursorPosition;

	lastposition = CursorPosition;
	pt = event.GetPosition();
    CalcCursorPosition(pt);
	if (CursorPosition < GetCount())
	{
		ClearCursor();

		if (CursorPosition > GetLastLine())
		{
			Scroll(-1, GetFirstLine() + 1);
			RefreshRect(CalcCursorRfshRect());
		}

		DoSelection();

		FillSelectedItemInfo(pt);

		RefreshRect(CalcCursorRfshRect());
	}
	else
		CursorPosition = lastposition;

}


void CodeView::OnMouseLeftUp(wxMouseEvent& event)
{
    FillSelectedItemInfo(event.GetPosition());
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
        CursorLastPosition = CursorPosition;

	lastposition = CursorPosition;
	pt = event.GetPosition();
    CalcCursorPosition(pt);
	if (CursorPosition < GetCount())
	{
		ClearCursor();

		if (MultiSelection && ((CursorPosition < SelectedItemIndex) ||
			(CursorPosition > SelectedLastItem)))
		{
			RefreshRect(CalcSelectedRect());
			SelectedCount = 1;
			MultiSelection = false;
			SelectedItemIndex = CursorPosition;
			CursorLastPosition = CursorPosition;
			SelectedLastItem = SelectedItemIndex;
		}
		else
			if (!MultiSelection)
			{
				SelectedCount = 1;
				SelectedItemIndex = CursorPosition;
				CursorLastPosition = CursorPosition;
				SelectedLastItem = SelectedItemIndex;
			}

		//SetFocusIgnoringChildren();
		FillSelectedItemInfo(pt);

		RefreshRect(CalcCursorRfshRect());
	}
	else
		CursorPosition = lastposition;

}


void CodeView::OnMouseRightUp(wxMouseEvent& event)
{
    DAsmElement *de;
    wxMenu		*argStyleSubMenu = 0,
				*organizeDataSubMenu = 0,
				*labelMenu = 0;
    wxPoint		pt;
    wxClientDC  dc(this);
	bool		labeled = false,
				hascomments = false;

    if (!IsEmpty)
    {

        PopUp = new wxMenu();
        de = m_iteminfo.dasmitem;

		// ************** MULTISELECTION ********************>
        if (SelectedCount > 1)
        {
            switch (GetTypeMultiselection(hascomments))
            {
                case et_Instruction:
                                PopUp->Append(idPOPUP_MAKEDATA, "Make data\td");
                                break;
                case et_Data:
								organizeDataSubMenu = new wxMenu();
                                PopUp->Append(idPOPUP_DISASM, "Disassemble\tc");
                                organizeDataSubMenu->Append(idPOPUP_OD_STRING, "String");
                                organizeDataSubMenu->Append(idPOPUP_OD_MATRIX, "Matrix");
                                organizeDataSubMenu->Append(idPOPUP_OD_NUMBER, "Number");
                                PopUp->AppendSeparator();
								PopUp->Append(idPOPUP_ORGANIZEDATA, "Organize data", organizeDataSubMenu);
                                break;
                default:
                                PopUp->Append(idPOPUP_MAKEDATA, "Make it data\td");
                                break;
            }
            if (hascomments)
            {
				PopUp->AppendSeparator();
				PopUp->Append(idPOPUP_DELCOMMENT,  "Del comments");
			}
        }
        // ************** MULTISELECTION ********************<
        else
        { // ************** ONE SELECTION ********************>
            //TODO: Implement rename/delete label routine

            switch(m_iteminfo.type)
            {
				case 	siInstructionLabel:
						                if ((de != 0) &&
						                    (de->MnemonicObject->isCall() || de->MnemonicObject->isJump()))//de->GetBranchType() != BR_NONE))
						                {
											PopUp->Append(idPOPUP_GOTO, "Goto label");
											PopUp->AppendSeparator();
										}
				case	siLineLabelProg:
				case	siLineLabelVar:
										labelMenu = new wxMenu();
										labelMenu->Append(idPOPUP_EDITLABEL, "Edit");
										labelMenu->AppendSeparator();
										labelMenu->Append(idPOPUP_DELLABEL, "Delete");

				case	siInstruction:
										if ((m_iteminfo.type != siLineLabelProg) &&
										    (m_iteminfo.type != siLineLabelVar))
											PopUp->Append(idPOPUP_MAKEDATA,_T("Make data"));
										break;
				case	siData:
										PopUp->Append(idPOPUP_DISASM, "Disassemble");


			}

			if (labelMenu != 0)
				PopUp->Append(idPOPUP_LBL, "Label", labelMenu);

			// Clicked over an argument
			if (m_iteminfo.argSelected > 0)
			{
				argStyleSubMenu = new wxMenu();

				argStyleSubMenu->Append(idPOPUP_ARG_BIN, "Binary");
				argStyleSubMenu->Append(idPOPUP_ARG_DEC, "Decimal");
				argStyleSubMenu->Append(idPOPUP_ARG_HEX, "Hexadecimal");
				PopUp->Append(idPOPUP_ARG_STYLE, "Style data", argStyleSubMenu);
			}

			PopUp->AppendSeparator();

            if ((m_iteminfo.type == siComments) || (m_iteminfo.hasComment))
            {
                PopUp->Append(idPOPUP_EDITCOMMENT, "Edit comment");
                PopUp->Append(idPOPUP_DELCOMMENT, "Del comment");
            }
            else
                PopUp->Append(idPOPUP_ADDCOMMENT, "Add comment");

        } // ************** ONE SELECTION ********************<

        PopupMenu(PopUp);
        delete PopUp;
    }
}





// TODO: Rewrite this
void CodeView::OnMouseMove(wxMouseEvent& event)
{
	if (event.Dragging())
	{
	    CalcCursorPosition(event.GetPosition());
        ClearCursor();

		if (CursorPosition > GetLastLine())
		{
			Scroll(-1, GetFirstLine() + 1);
			RefreshRect(CalcCursorRfshRect());
		}

	    if (Selecting)
        {
            UpdateSelectedRect();
            RefreshRect(CalcSelectedRect());
        }

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


