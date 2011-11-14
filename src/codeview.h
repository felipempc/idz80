/****************************************************************
 * Name:      IDZ80
 * Purpose:   Defines Application Frame
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 *
 * This module shows/controls disassembled data
 **************************************************************/


#ifndef CODEVIEW_H
#define CODEVIEW_H

#include <wx/wx.h>
#include <wx/scrolbar.h>
#include <wx/dynarray.h>
#include <wx/arrstr.h>
#include <wx/scrolwin.h>


#include "dasmdata.h"
#include "ProcessData.h"
#include "codeviewline.h"



#define COL_ADDRESS 7
#define COL_CODE    70
#define COL_ASCII   160
#define COL_LABEL   200
#define COL_MNEM    230



enum {
    idPOPUP_SEARCH=100,
    idPOPUP_GOTO,
    idPOPUP_MAKEDATA,
    idPOPUP_DISASM,
    idPOPUP_EDITLABEL,
    idPOPUP_DELLABEL,
    idPOPUP_EDITCOMMENT,
    idPOPUP_DELCOMMENT,
    idPOPUP_ADDCOMMENT
};


class CodeView : public wxScrolledWindow
{
public:
	CodeView(wxWindow *parent, ProcessData *_proc);
	~CodeView();

    void Plot(void);
    void DebugLog(wxTextCtrl *log);
    bool IsSelected();
    int GetIndex();
    int GetCount();
    void Clear();
    void ClearSelection();
    bool Enable(bool enable=true);


    CodeViewLine    *m_CodeViewLine;


private:
    ProcessData     *m_process;


    uint        m_linesShown,       // Number of Items shown
                m_fractionLine,     // Pixels' number of a non fully shown line.
                m_fractionScroll,   // Pixels' number to complete a fractional line
                m_fontHeight,
                m_fontWidth;

    int         SelectedCount,      // Number of Items Selected
                SelectedItemIndex,  // First Disassembled Item Selected
                SelectedLastItem,   // Last Disassembled Item Selected
                CursorPosition,     // Index of Disassembled item that cursor is over
                CursorLastPosition;
    wxRect      *LastCursorRect;    // The rectangle of last cursor

    bool        IsFocused,
                IsEmpty,            // Is the Data Base of disassembled items empty ?
                MultiSelection,     // we have selected more than one item
                Selecting;          // we're selecting items

    wxFont      *font;

    wxColour    BackGroundColor;
    wxColour    ForeGroundColor;
    wxColour    SelectedItemColor;
    wxColour    FGArgumentColor;
    wxColour    BGArgumentColor;
    wxColour    FG_TextColor;
    wxColour    FG_LabelColor;

    wxMenu      *PopUp;

    // Debug Area
    wxTextCtrl *TC_Log;
    void LogIt(const wxString &str);

    // Utilities
    void PaintBackground(wxDC &dc, const int start_y, const int fromline, const int toline, const wxBrush backcolour);
    void Render(wxDC &dc, const int start_y, const int fromline, const int count);
    uint RenderData(wxDC &dc, const int start_y, CodeViewItem *cvi);
    uint RenderInstruction(wxDC &dc, const int start_y, CodeViewItem *cvi);
    uint RenderProgramLabel(wxDC &dc, const int start_y, wxString str);
    uint RenderOrigin(wxDC &dc, const int start_y, uint address);
    void CalcItemsShown(void);
    void CalcCursorPosition(wxPoint point);
    wxRect CalcCursorRfshRect();
    void UpdateSelectedRect();
    wxRect CalcSelectedRect();
    void UpdateLastCursorRect();
    void ShowCursor(wxDC &dc);
    void ClearCursor();
    void DoSelection();

    int GetFirstLine();
    int GetLastLine();

    void FilterInstructions(wxArrayInt &range);
    ElementType GetTypeMultiselection();


    /*
     * Event Handlers
     */

    // Window event handlers
    void OnSize(wxSizeEvent& event);
    void OnDraw(wxDC &dc);
    void OnPaint(wxPaintEvent& event);
    //void OnErase(wxEraseEvent &event);
    void OnGetFocus(wxFocusEvent& event);
    void OnKillFocus(wxFocusEvent& event);
    void OnScrollLineDown(wxScrollWinEvent& event);
    void OnScrollLineUp(wxScrollWinEvent& event);
    void OnScrollPageDown(wxScrollWinEvent& event);
    void OnScrollPageUp(wxScrollWinEvent& event);

    // Mouse Event handlers
    void OnMouseLeftUp(wxMouseEvent& event);
    void OnMouseLeftDown(wxMouseEvent& event);
    void OnMouseRightUp(wxMouseEvent& event);
    void OnMouseRightDown(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnMouseWheel(wxMouseEvent& event);

    // Keyboard event handlers
    void OnKeyPress(wxKeyEvent& event);
    void OnKeyRelease(wxKeyEvent& event);


    // Pop up Menu Event Handlers
    void OnPopUpMenuSearch(wxCommandEvent& event);
    void OnPopUpMenuGoto(wxCommandEvent& event);
    void OnPopUpMenuMakeData(wxCommandEvent& event);
    void OnPopUpMenuDisasm(wxCommandEvent& event);

    // Pop Up Comment event handlers
    void OnPopUpAddComment(wxCommandEvent& event);
    void OnPopUpEditComment(wxCommandEvent& event);
    void OnPopUpDelComment(wxCommandEvent& event);
};

#endif


