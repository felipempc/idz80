/****************************************************************
 * Name:      IDZ80
 * Purpose:   Defines Application Frame
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 **************************************************************/


#ifndef DEBUGVIEW_H
#define DEBUGVIEW_H

//(*Headers(DebugView)
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class DebugView: public wxDialog
{
	public:

		DebugView(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~DebugView();

		//(*Declarations(DebugView)
		wxTextCtrl* TC_Log;
		wxButton* B_Limpa;
		//*)

        void BlackPen(void);
        void RedPen(void);

	protected:

		//(*Identifiers(DebugView)
		static const long ID_BUTTON1;
		static const long ID_TEXTCTRL1;
		//*)

	private:

		//(*Handlers(DebugView)
		void OnResize(wxSizeEvent& event);
		void OnB_LimpaClick(wxCommandEvent& event);
		void OnB_LimpaClick1(wxCommandEvent& event);
		//*)

		void DoResize(void);

		DECLARE_EVENT_TABLE()
};

#endif
