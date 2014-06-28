/***************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   17-01-2014 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Defines a container to store source code lines
 **************************************************************/


#ifndef SOURCECODECONTAINER_H
#define SOURCECODECONTAINER_H

#include <vector>
#include "source_code_item.h"




typedef std::vector<CodeViewItem *> SourceCodeList;
typedef unsigned int LineNumber;




class SourceCodeContainer
{
    public:
        SourceCodeContainer();
        ~SourceCodeContainer();

        void Clear();
        uint GetCount();
        LineNumber Find(CodeViewItem *cvi);

    protected:
        static const uint LINE_NOT_FOUND = 1;

        SourceCodeList  code_list_;

        void Add(CodeViewItem *cvi);
        void Insert(CodeViewItem *cvi, LineNumber line);
        void Remove(CodeViewItem *cvi);
        void RemoveLine(const LineNumber line);
};

#endif // SOURCECODECONTAINER_H
