/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   26-12-2013 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Binary file controls
 **************************************************************/


#ifndef BINFILE_H
#define BINFILE_H

#include "idz80_base.h"

struct stBinHeader
{
    byte    id;
    word    start_address;
    word    end_address;
    word    execution_address;
};
typedef struct stBinHeader BinHeader;



class BinFile
{
    public:
        BinFile();
        virtual ~BinFile();

        const BinHeader *GetBinHeader();
        void ClearBin();

    protected:
        static const uint BIN_HEADER_SIZE = 7;
        BinHeader       *bin_header_;

        bool FillAndValidateBIN(void *source);

    private:
        static const byte BIN_ID = 0xFE;
};

#endif // BINFILE_H
