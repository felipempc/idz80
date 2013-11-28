/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * User interface
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
#include "codeview_definitions.h"






class CodeView : public wxScrolledCanvas, public LogBase
{
public:
	CodeView(wxWindow *parent, ProcessData *processparent, LogWindow *logparent);
	~CodeView();

    void Plot(void);
    void DebugLog(wxTextCtrl *log);
    bool IsSelected();
    int GetIndex();
    int GetCount();
    void Clear();
    void ClearSelection();
    bool Enable(bool enable=true);
	void CenterAddress(uint address);
	void TestBlur();





private:
	static const int COL_ADDRESS = 7;
	static const int COL_CODE = 70;
	static const int COL_ASCII = 160;
	static const int COL_LABEL = 200;
	static const int COL_MNEM = 230;

    ProcessData     *Process;
    CodeViewLine    *m_CodeViewLine;

    SelectedItemInfo line_info;	// Holds info about the selected item


    uint        m_linesShown,       // Number of Items shown
                m_fontHeight,
                m_fontWidth;
	styledata	m_styleData;		// Holds info to change style handlers (BIN/DEC/HEX)

    wxRect      *LastCursorRect,    // The rectangle of last cursor
                IncompleteArea;

    bool        IsFocused,
                IsEmpty,            // Is the Data Base of disassembled items empty ?
                MultiSelection,     // we have selected more than one item
                Selecting;          // we're selecting items



    wxFont      *font;
    wxBitmap    DisassembleWindowBitmap;

    wxColour    BackGroundColor;
    wxColour    ForeGroundColor;
    wxColour    SelectedItemColor;
    wxColour    FGArgumentColor;
    wxColour    BGArgumentColor;
    wxColour    FG_TextColor;
    wxColour    FG_LabelColor;


    // Configuration
    void SetupEvents();
    void SetupColors();
    void SetupFont();

    // window render
    void PaintBackground(wxDC &dc, const int start_y, const int fromline, const int toline, const wxBrush backcolour);
    void Render(wxDC &dc, const int start_y, const int fromline, const int count);
    uint RenderData(wxDC &dc, const int start_y, CodeViewItem *cvi);
    uint RenderInstruction(wxDC &dc, const int start_y, CodeViewItem *cvi);
    uint RenderProgramLabel(wxDC &dc, const int start_y, wxString str);
    uint RenderOrigin(wxDC &dc, const int start_y, uint address);
    void RenderBackGroundBlur(wxDC &dc, wxRect region);

    //utilities
    void CalcItemsShown(void);
    void CalcCursorPosition(wxPoint point);
    wxRect CalcCursorRfshRect();
    void CalcIncompleteArea();
    wxRect CalcSelectedRect();
	wxString FormatArg(uint arg, uint style);
    int GetFirstLine();
    int GetLastLine();
    int GetLastItem();
    void UpdateSelectedRect();
    void UpdateLastCursorRect();
    void UpdateVirtualSize(void);

    //cursor
    void ShowCursor(wxDC &dc);
    void ClearCursor();

    //selection
    void DoSelection();
	void FillSelectedItemInfo(const wxPoint &pt);
    void ResetSelectedItemInfo();
	void TreatSingleSelection();
	void TreatMultiSelection();
	void IdentifyArgumentSelected(const wxPoint &mouse_cursor);

    //Pop up Menu
    ElementType GetTypeMultiselection(bool &hcomment);
    void CreatePopupMenuMultiSelection(wxMenu *popup);
    void CreatePopupMenuSingleSelection(wxMenu *popup);


    /*
     * Event Handlers
     */

    // Window event handlers
    void OnSize(wxSizeEvent& event);
    void OnDraw(wxDC &dc);
    void OnPaint(wxPaintEvent& event);
    void OnGetFocus(wxFocusEvent& event);
    void OnKillFocus(wxFocusEvent& event);
    void OnScrollLineDown(wxScrollWinEvent& event);
    void OnScrollLineUp(wxScrollWinEvent& event);
    void OnScrollPageDown(wxScrollWinEvent& event);
    void OnScrollPageUp(wxScrollWinEvent& event);
    void OnMouseCaptureLost(wxMouseCaptureLostEvent& event);

    // Mouse Event handlers
    void OnMouseLeftUp(wxMouseEvent& event);
    void OnMouseLeftDown(wxMouseEvent& event);
    void OnMouseRightUp(wxMouseEvent& event);
    void OnMouseRightDown(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnMouseWheel(wxMouseEvent& event);
    void OnMouseEnterWindow(wxMouseEvent& event);
    void OnMouseLeaveWindow(wxMouseEvent& event);

    // Keyboard event handlers
    void OnKeyPress(wxKeyEvent& event);
    void OnKeyRelease(wxKeyEvent& event);


    // Pop up Menu Event Handlers
    void OnPopUpMenuSearch(wxCommandEvent& event);
    void OnPopUpMenuGoto(wxCommandEvent& event);
    void OnPopUpMenuGotoAddress(wxCommandEvent& event);
    void OnPopUpMenuMakeData(wxCommandEvent& event);
    void OnPopUpMenuOD_Matrix(wxCommandEvent& event);
    void OnPopUpMenuOD_String(wxCommandEvent& event);
    void OnPopUpMenuOD_Number(wxCommandEvent& event);
    void OnPopUpMenuDisasm(wxCommandEvent& event);

    void OnPopUpMenuArgStyleBin(wxCommandEvent& event);
    void OnPopUpMenuArgStyleDec(wxCommandEvent& event);
    void OnPopUpMenuArgStyleHex(wxCommandEvent& event);

    void OnPopUpMenuRenLabel(wxCommandEvent& event);
    void OnPopUpMenuDelLabel(wxCommandEvent& event);
    void OnPopUpMenuCreateLabel(wxCommandEvent& event);

    void OnPopUpMenuSearchArgument(wxCommandEvent& event);

    // Pop Up Comment event handlers
    void OnPopUpAddComment(wxCommandEvent& event);
    void OnPopUpEditComment(wxCommandEvent& event);
    void OnPopUpDelComment(wxCommandEvent& event);
};


// Tools
wxString IntToBin(uint conv);

#endif


