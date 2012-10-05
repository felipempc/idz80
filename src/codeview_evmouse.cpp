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
			Scroll(-1,GetFirstLine() + 1);
			RefreshRect(CalcCursorRfshRect());
		}

		DoSelection();

		SetFocusIgnoringChildren();
		FillSelectedItemInfo(pt);

		RefreshRect(CalcCursorRfshRect());
	}
	else
		CursorPosition = lastposition;

}


void CodeView::OnMouseLeftUp(wxMouseEvent& event)
{

	/* DEBUG:
    wxString str;
    DAsmElement* de;
    CodeViewItem* cvi;

    str.Printf("Type: %d\nDasmItem: ", m_iteminfo.type);

    if (m_iteminfo.dasmitem != 0)
    {
		de = m_iteminfo.dasmitem;
		str << de->getCodeStr() << "\n";
	}
	else
		str << "NULL\n";

	if (m_iteminfo.lineitem >= 0)
	{
		cvi = m_iteminfo.lineitem;
		str << wxString::Format("LineItem: %d\n", cvi->Dasmitem);
	}
	else
		str << "LineItem: NULL\n";

	if (m_iteminfo.hasComment)
		str << "hasComment: SIM\n";
	else
		str << "hasComment: NAO\n";

    str << wxString::Format("Arg Selected: %d\n\n", m_iteminfo.argSelected);

    LogIt(str);
    */
}



void CodeView::OnMouseRightDown(wxMouseEvent& event)
{
	int	lastposition;
	wxPoint pt;

    if (!IsEnabled())
    {
        return;
    }

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

		SetFocusIgnoringChildren();
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
        SetFocusIgnoringChildren();

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
						                if ((de != 0) && (de->MnItem != 0) &&
						                    (de->MnItem->getBranchType() != BR_NONE))
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
	wxMouseState	ms;

	if (event.Dragging() && Selecting)
		if (ms.LeftIsDown())
			LogIt("Selecting...\n");
		else
			LogIt("Dragging...\n");
/*
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
*/
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


