/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Controls Cartridge ROM files
 **************************************************************/


#ifndef CARTRIDGEROMFILE_H
#define CARTRIDGEROMFILE_H

#include "idz80_basic_types.h"


struct stCartHeader
{
	word	id;
	word	init;
	word	statement;
	word	device;
	word	text;
};
typedef struct stCartHeader CartHeader;




class CartridgeRomFile
{
    public:
        CartridgeRomFile();
        virtual ~CartridgeRomFile();

        const CartHeader *GetCartridgeHeader();
        bool GetEntries(IntArray &entrylist);
        void ClearCartridgeInfo();
        bool HasBasic();
        bool PureBasic();
        bool isCartridge();

    protected:
        static const uint CARTRIDGE_HEADER_SIZE = 16;

        // Memory pages
        static const word MEMORY_PAGE_0_START = 0x0000;
        static const word MEMORY_PAGE_0_END = 0x3FFF;
        static const word MEMORY_PAGE_1_START = 0x4000;
        static const word MEMORY_PAGE_1_END = 0x7FFF;
        static const word MEMORY_PAGE_2_START = 0x8000;
        static const word MEMORY_PAGE_2_END = 0xBFFF;
        static const word MEMORY_PAGE_3_START = 0xC000;
        static const word MEMORY_PAGE_3_END = 0xFFFF;

		static const word CARTRIDGE_PAGE_ADDRESS = MEMORY_PAGE_1_START;
		static const word CARTRIDGE_LAST_ADDRESS = MEMORY_PAGE_3_START;

		bool ValidateCartridge(void *source);

    private:
		static const word ID_CARTRIDGE_ROM = 0x4142;
		static const word ID_CARTRIDGE_SUBROM = 0x4344;

        CartHeader		*m_cartridge_header;

};

#endif // CARTRIDGEROMFILE_H
