#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include <wx/textfile.h>
#include "ProcessData.h"


enum eCompilerFlag
{
    cfSJASM,
    cfM80
};
typedef enum eCompilerFlag CompilerFlag;


class codeGenerator
{
    public:
        codeGenerator(ProcessData *pd);
        virtual ~codeGenerator();
        wxString GenerateCode(wxString file, const CompilerFlag cflags);
    protected:
    private:
        ProcessData     *process;
        wxString        textCode, end_line;
        CompilerFlag    m_cflags;

        wxString generateTextData(CodeViewItem *cvi);
        wxString generateInstruction(CodeViewItem *cvi);
        wxString generateLabels(CodeViewItem *cvi);
        bool isNumber(wxChar firstchar);
};

#endif // CODEGENERATOR_H
