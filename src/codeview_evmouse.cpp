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
#include "codeview.h"
#include "d_asm_element.h"



void CodeView::OnMouseLeftDown(wxMouseEvent& event)
{
	int lastposition;

    if (!IsEnabled())
    {
        return;
    }

    if (!MultiSelection)
        CursorLastPosition = CursorPosition;

	lastposition = CursorPosition;

    CalcCursorPosition(event.GetPosition());
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

		RefreshRect(CalcCursorRfshRect());
	}
	else
		CursorPosition = lastposition;
}


void CodeView::OnMouseLeftUp(wxMouseEvent& event)
{
    // nothing
}



void CodeView::OnMouseRightDown(wxMouseEvent& event)
{
	int	lastposition;

    if (!IsEnabled())
    {
        return;
    }

    if (!MultiSelection)
        CursorLastPosition = CursorPosition;

	lastposition = CursorPosition;
    CalcCursorPosition(event.GetPosition());
	if (CursorPosition < GetCount())
	{
		ClearCursor();

		if (MultiSelection && ((CursorPosition < SelectedItemIndex) ||
			(CursorPosition > SelectedLastItem)))
		{
			//if (MultiSelection)
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

		RefreshRect(CalcCursorRfshRect());
	}
	else
		CursorPosition = lastposition;

}


void CodeView::OnMouseRightUp(wxMouseEvent& event)
{
    CodeViewItem *cvi;
    DAsmElement *de;
    wxMenu		*submenu1;
    wxPoint		pt;
    uint		pointedline;
    wxClientDC  dc(this);
    bool		argfocus = false,
				arg1focus = false,
				arg2focus = false,
				labeled = false;

    DoPrepareDC(dc);
    pt = event.GetLogicalPosition(dc);
    pointedline = pt.y / m_fontHeight;
    cvi = m_CodeViewLine->getData(pointedline);

    if (cvi->RectArg1 != 0)
		if (cvi->RectArg1->Contains(pt))
		{
			argfocus = true;
			arg1focus = true;
		}

    if (cvi->RectArg2 != 0)
		if (cvi->RectArg2->Contains(pt))
		{
			argfocus = true;
			arg2focus = true;
		}


    if (!IsEmpty)
    {
        SetFocusIgnoringChildren();

        PopUp = new wxMenu();

        if (SelectedCount > 1)
        {
            switch (GetTypeMultiselection())
            {
                case et_Instruction:
                                PopUp->Append(idPOPUP_MAKEDATA,_T("Make it data\td"));
                                break;
                case et_Data:
								submenu1 = new wxMenu();
                                PopUp->Append(idPOPUP_DISASM,_("Disassemble\tc"));
                                submenu1->Append(idPOPUP_OD_STRING, _("String"));
                                submenu1->Append(idPOPUP_OD_MATRIX, _("Matrix"));
                                submenu1->Append(idPOPUP_OD_NUMBER, _("Number"));
                                PopUp->AppendSeparator();
								PopUp->Append(idPOPUP_ORGANIZEDATA, _T("Organize data"), submenu1);
                                break;
                default:
                                PopUp->Append(idPOPUP_MAKEDATA,_T("Make it data\td"));
                                break;
            }
            PopUp->AppendSeparator();
            PopUp->Append(idPOPUP_EDITCOMMENT,_T("Edit comment"));
            PopUp->Append(idPOPUP_DELCOMMENT,_T("Del comment"));
        }
        else
        {
            cvi = m_CodeViewLine->getData(CursorPosition);
            // Label
            //TODO: Implement rename/delete label routine
            if ((cvi->LabelProgAddr >= 0) && (cvi->LabelVarAddr >= 0))
            {
                PopUp->Append(idPOPUP_EDITLABEL,_T("Rename label"));
                PopUp->AppendSeparator();
                PopUp->Append(idPOPUP_DELLABEL,_T("Delete label"));
            }
            else
            // Dasmed item
            if (cvi->Dasmitem >= 0)
            {
                de = m_process->m_Dasm->GetData(cvi->Dasmitem);
                if (de->MnItem != 0)
                    if (de->MnItem->getBranchType() != BR_NONE)
                    {
                        PopUp->Append(idPOPUP_GOTO,_T("Goto label"));
                        labeled = true;
                    }
                if (de->ElType == et_Instruction)
                {
                    PopUp->Append(idPOPUP_MAKEDATA,_T("Make it data"));
                }
                //TODO: Filter VAR and I/O instructions
                if (argfocus && (!labeled))
				{
					submenu1 = new wxMenu();

					if (arg1focus)
						m_styleData.arg = 1;
					else
						m_styleData.arg = 2;
					m_styleData.item = cvi->Dasmitem;

					submenu1->Append(idPOPUP_ARG_BIN, _("Binary"));
					submenu1->Append(idPOPUP_ARG_DEC, _("Decimal"));
					submenu1->Append(idPOPUP_ARG_HEX, _("Hexadecimal"));
					PopUp->Append(idPOPUP_ARG_STYLE, _T("Style data"), submenu1);
				}
            }
            PopUp->AppendSeparator();
            if (cvi->Comment != 0)
            {
                PopUp->Append(idPOPUP_EDITCOMMENT,_T("Edit comment"));
                PopUp->Append(idPOPUP_DELCOMMENT,_T("Del comment"));
            }
            else
                PopUp->Append(idPOPUP_ADDCOMMENT,_T("Add comment"));
        }
        PopupMenu(PopUp);
        delete PopUp;
    }
}


// TODO: Rewrite this
void CodeView::OnMouseMove(wxMouseEvent& event)
{
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


