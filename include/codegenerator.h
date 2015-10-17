/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * This module creates the text file containing the
 * disassembled program
 **************************************************************/


#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include <wx/textfile.h>
#include "process_data.h"


enum eCompilerFlag
{
    cfSJASM,
    cfM80
};
typedef enum eCompilerFlag CompilerFlag;


class codeGenerator
{
    public:
        codeGenerator(ProcessData *parent);
        virtual ~codeGenerator();
        wxString GenerateCode(wxString file, const CompilerFlag cflags);
    protected:
    private:
        ProcessData     *Process;
        wxString        textCode, end_line;
        CompilerFlag    m_cflags;

        wxString generateTextData(CodeViewItem *cvi);
        wxString generateInstruction(CodeViewItem *cvi);
        wxString generateLabels(CodeViewItem *cvi);
        bool isNumber(wxChar firstchar);
};

#endif // CODEGENERATOR_H
