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
//#include <wx/dynarray.h>
#include <wx/arrstr.h>
#include <wx/scrolwin.h>


#include "disassembled_container.hpp"
#include "process_data.hpp"
#include "source_code.hpp"
#include "codeview_definitions.hpp"



class CodeView : public wxScrolledCanvas, public DebugLogBase
{
public:
	CodeView(wxWindow *t_parent, SourceCode *t_srccode, ProcessData *t_processparent);
	~CodeView();

    void debugLog(wxTextCtrl *log);
    bool isSelected();
    int getIndex();
    int getCount();
    void clear();
    void clearSelection();
    void enable(bool enable=true);
	void centerAddress(uint address);
	void testBlur();



private:
	static const int COL_ADDRESS = 7;
	static const int COL_CODE = 70;
	static const int COL_ASCII = 160;
	static const int COL_LABEL = 200;
	static const int COL_MNEM = 230;

    ProcessData     *m_process;
    SourceCode      *m_sourcecode;

    SelectedItemInfo m_line_info;	// Holds info about the selected item


    uint        m_lines_shown,       // Number of Items shown
                m_font_height,
                m_font_width;
//	styledata	m_style_data;		// Holds info to change style handlers (BIN/DEC/HEX)

    wxRect      *m_last_cursor_rect,    // The rectangle of last cursor
                m_incomplete_area;

    bool        m_is_focused,
                m_is_empty,            // Is the Data Base of disassembled items empty ?
                m_multi_selection,     // we have selected more than one item
                m_selecting;          // are we selecting items?

    wxFont      *m_font;
    wxBitmap    m_disassemble_window_bitmap;

    wxColour    m_color_background;
    wxColour    m_color_foreground;
    wxColour    m_color_selected_item;
    wxColour    m_color_argument_foreground;
    wxColour    m_color_argument_background;
    wxColour    m_color_text_foreground;
    wxColour    m_color_label_foreground;


    // Configuration
    void setupEvents();
    void setupColors();
    void setupFont();
    void setupAcceleratorKeys();

    // window render
    void paintBackground(wxDC &t_dc, const int t_line_pixel, const int t_fromline, const int t_toline, const wxBrush t_backcolour);
    void render(wxDC &dc, const int t_line_pixel, const int fromline, const int count);
    uint renderData(wxDC &dc, const int t_line_pixel, SourceCodeLine *cvi);
    uint renderInstruction(wxDC &dc, const int t_line_pixel, SourceCodeLine *cvi);
    uint renderProgramLabel(wxDC &dc, const int t_line_pixel, wxString str);
    uint renderOrigin(wxDC &dc, const int t_line_pixel, uint address);
    void renderBackgroundBlur(wxDC &t_dc, wxRect t_region);

    //utilities
    void calcLinesShown();
    void calcCursorPosition(wxPoint point);
    wxRect calcCursorRfshRect();
    void calcIncompleteArea();
    wxRect calcSelectedRect();
	wxString formatArg(uint arg, uint style);
    int getFirstLine();
    int getLastLine();
    int getLastItemIndex();
    void updateSelectedRect();
    void updateLastCursorRect();
    void updateVirtualSize();

    //cursor
    void showCursor(wxDC &dc);
    void clearCursor();
    void cursorUp();
    void cursorDown();
    void cursorPageUp();
    void cursorPageDown();

    //selection
    void doSelection();
	void fillSelectedItemInfo(const wxPoint &pt);
    void resetSelectedItemInfo();
	void treatAsSingleSelection();
	void treatAsMultiSelection();
	void identifyArgumentSelected(const wxPoint &mouse_cursor);

    //Pop up Menu
    LineType getTypesInSelection(bool &hcomment);
    void createPopupMenuMultiSelection(wxMenu *popup);
    void createPopupMenuSingleSelection(wxMenu *popup);


    /*
     * Event Handlers
     */

    // Window event handlers
    void onSize(wxSizeEvent& event);
    void onDraw(wxDC &dc);
    void onPaint(wxPaintEvent& event);
    void onGetFocus(wxFocusEvent& event);
    void onKillFocus(wxFocusEvent& event);
    void onScrollLineDown(wxScrollWinEvent& event);
    void onScrollLineUp(wxScrollWinEvent& event);
    void onScrollPageDown(wxScrollWinEvent& event);
    void onScrollPageUp(wxScrollWinEvent& event);
    void onMouseCaptureLost(wxMouseCaptureLostEvent& event);

    // Mouse Event handlers
    void onMouseLeftUp(wxMouseEvent& event);
    void onMouseLeftDown(wxMouseEvent& event);
    void onMouseRightUp(wxMouseEvent& event);
    void onMouseRightDown(wxMouseEvent& event);
    void onMouseMove(wxMouseEvent& event);
    void onMouseWheel(wxMouseEvent& event);
    void onMouseEnterWindow(wxMouseEvent& event);
    void onMouseLeaveWindow(wxMouseEvent& event);

    // Keyboard event handlers
    void onKeyPress(wxKeyEvent& event);
    void onKeyRelease(wxKeyEvent& event);


    // Pop up Menu Event Handlers
    void onPopUpMenuSearch(wxCommandEvent& event);
    void onPopUpMenuGoto(wxCommandEvent& event);
    void onPopUpMenuGotoAddress(wxCommandEvent& event);
    void onPopUpMenuMakeData(wxCommandEvent& event);
    void onPopUpMenuOD_Matrix(wxCommandEvent& event);
    void onPopUpMenuOD_String(wxCommandEvent& event);
    void onPopUpMenuOD_Number(wxCommandEvent& event);
    void onPopUpMenuDisasm(wxCommandEvent& event);

    void onPopUpMenuArgStyleBin(wxCommandEvent& event);
    void onPopUpMenuArgStyleDec(wxCommandEvent& event);
    void onPopUpMenuArgStyleHex(wxCommandEvent& event);

    void onPopUpMenuRenLabel(wxCommandEvent& event);
    void onPopUpMenuDelLabel(wxCommandEvent& event);
    void onPopUpMenuCreateLabel(wxCommandEvent& event);

    void onPopUpMenuSearchArgument(wxCommandEvent& event);
    void onSearchContinue(wxCommandEvent& event);

    // Pop Up Comment event handlers
    void onPopUpAddComment(wxCommandEvent& event);
    void onPopUpEditComment(wxCommandEvent& event);
    void onPopUpDelComment(wxCommandEvent& event);
};


// Tools
wxString convertIntToBin(uint conv);

#endif


