/***************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   22-12-2025 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Manager of multiple source code files
 **************************************************************/


#ifndef _SOURCE_CODE_MGR_HPP_
#define _SOURCE_CODE_MGR_HPP_

#include <vector>
#include "debug_logbase.hpp"
#include "source_code.hpp"

// Define DEBUG MODE in this class
#define IDZ80_SRC_DEBUG


class SourceCodeManager: public DebugLogBase
{
    public:
        SourceCodeManager(DebugLogBase *t_parent);
        ~SourceCodeManager();

        bool addSourceCode(SourceCode *t_src_code);
        void deleteSourceCode(int t_index);
        void clear();
        unsigned int size();

        SourceCode *first();
        SourceCode *last();
        SourceCode *current();
        SourceCode *previous();
        SourceCode *next();
        SourceCode *index(int t_index);

        int currentIndex();

        bool isEnd();

    private:
    std::vector<SourceCode *> m_source_code;

    SourceCode *m_current_item;
    int m_current_item_index;

};
#endif
