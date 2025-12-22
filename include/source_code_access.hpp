/***************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   15-01-2014 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Base class for line code access
 **************************************************************/

#ifndef SOURCE_CODE_ACCESS_H
#define SOURCE_CODE_ACCESS_H

#include "source_code_container.hpp"



/// @brief Class to access/navigate source code lines
class SourceCodeAccess: public SourceCodeContainer
{
    public:
        SourceCodeAccess();

        SourceCodeLine *firstLine();
        SourceCodeLine *lastLine();
        SourceCodeLine *currentLine();
        SourceCodeLine *previousLine();
        SourceCodeLine *nextLine();
        SourceCodeLine *line(const LineNumber linenumber);

        LineNumber currentLineNumber();


    protected:
    private:
        SourceCodeLine    *m_current_item;
        LineNumber      m_current_index;
};

#endif // SOURCECODENAVIGATION_H
