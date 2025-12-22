/***************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   17-01-2014 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Defines a container to store source code lines
 **************************************************************/


#ifndef SOURCECODECONTAINER_HPP
#define SOURCECODECONTAINER_HPP

#include <vector>
#include "source_code_line.hpp"


typedef std::vector<SourceCodeLine *> SourceCodeList;
typedef unsigned int LineNumber;


/// @brief Stores the lines of source code
class SourceCodeContainer
{
    public:
        SourceCodeContainer();
        ~SourceCodeContainer();

        void clear();
        unsigned int getCount();
        LineNumber findLine(const SourceCodeLine *t_scline);

    protected:
        static const unsigned int LINE_NOT_FOUND = 1;

        SourceCodeList  m_sourcecode_list;

        void addLine(SourceCodeLine *t_scline);
        void insertLine(SourceCodeLine *t_scline, const LineNumber t_line);
        void removeLine(const SourceCodeLine *t_scline);
        void removeLine(const LineNumber t_line);
};

#endif // SOURCECODECONTAINER_HPP
