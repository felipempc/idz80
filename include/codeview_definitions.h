/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Defines structs, enums and types for codeview object
 **************************************************************/


#ifndef _CODEVIEW_DEFINITIONS_
#define _CODEVIEW_DEFINITIONS_

enum {
    idPOPUP_SEARCH = 100,
    idPOPUP_SEARCH_ARGUMENT,
    idSEARCH_CONTINUE,
    idPOPUP_GOTO,
    idPOPUP_GOTO_ADDRESS,
    idPOPUP_MAKEDATA,
    idPOPUP_ORGANIZEDATA,
    idPOPUP_OD_STRING,
    idPOPUP_OD_MATRIX,
    idPOPUP_OD_NUMBER,
    idPOPUP_ARG_BIN,
    idPOPUP_ARG_DEC,
    idPOPUP_ARG_HEX,
    idPOPUP_ARG_STYLE,
    idPOPUP_DISASM,
    idPOPUP_CREATELABEL,
    idPOPUP_EDITLABEL,
    idPOPUP_DELLABEL,
    idPOPUP_EDITCOMMENT,
    idPOPUP_DELCOMMENT,
    idPOPUP_ADDCOMMENT,
    idPOPUP_LBL,
    idKEY_DOWN,
    idKEY_UP,
    idKEY_PAGEDOWN,
    idKEY_PAGEUP
};


enum LineType
{
	siUnknown = -1,			// -1
	siInstruction = 0,		//  0
	siInstructionLabel,		//  1
	siData,					//  2
	siLineLabelProg,		//  3
	siLineLabelVar,			//  4
	siComments				//  5
};

struct SelectedItemInfo
{
	LineType		type;
	DisassembledItem* 	dasmitem;
	CodeViewItem* 	lineitem;
	bool			hasComment;

	uint			argSelected,
                    selectedLineCount;

	int             firstLine,
                    lastLine,
                    firstInstruction,
                    lastInstruction,
                    firstAddress,
                    lastAddress,
                    cursorPosition,
                    cursorLastPosition;

};


//TODO: Eliminate styledata
struct styledata
{
	uint item, arg;
};




#endif // _CODEVIEW_DEFINITIONS_
