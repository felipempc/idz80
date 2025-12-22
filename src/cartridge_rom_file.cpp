/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Controls Cartridge ROM files
 **************************************************************/



#include "cartridge_rom_file.hpp"
#include <cstring>      // for memcpy



const uint CartridgeRomFile::CARTRIDGE_HEADER_SIZE;
const word CartridgeRomFile::ID_CARTRIDGE_ROM;
const word CartridgeRomFile::ID_CARTRIDGE_SUBROM;
const word CartridgeRomFile::CARTRIDGE_PAGE_ADDRESS;
const word CartridgeRomFile::CARTRIDGE_LAST_ADDRESS;




CartridgeRomFile::CartridgeRomFile()
{
    m_cartridge_header = 0;
}



CartridgeRomFile::~CartridgeRomFile()
{
    ClearCartridgeInfo();
}



void CartridgeRomFile::ClearCartridgeInfo()
{
    if (m_cartridge_header)
        delete m_cartridge_header;

    m_cartridge_header = 0;
}



bool CartridgeRomFile::ValidateCartridge(void *source)
{
	bool	is_cartridge = false,
            cart_init = false,
            cart_text = false,
            cart_statement = false,
            cart_device = false;
    byte    *header;

    if (!m_cartridge_header)
        m_cartridge_header = new CartHeader;

    header = static_cast<byte *>(source);
    std::memcpy(m_cartridge_header, source, sizeof(CartHeader));
    m_cartridge_header->id = static_cast<word>((*header++ * 0x100 + *header) & 0xFFFF);



	if ((m_cartridge_header->id == ID_CARTRIDGE_ROM) || (m_cartridge_header->id == ID_CARTRIDGE_SUBROM))
	{
		cart_init = ((m_cartridge_header->init) && (m_cartridge_header->init > CARTRIDGE_PAGE_ADDRESS) &&
                    (m_cartridge_header->init < CARTRIDGE_LAST_ADDRESS));
        cart_statement = ((m_cartridge_header->statement) && (m_cartridge_header->statement > CARTRIDGE_PAGE_ADDRESS) &&
                        (m_cartridge_header->statement < CARTRIDGE_LAST_ADDRESS));
        cart_device = ((m_cartridge_header->device) && (m_cartridge_header->device > CARTRIDGE_PAGE_ADDRESS) &&
                    (m_cartridge_header->device < CARTRIDGE_LAST_ADDRESS));
        cart_text = ((m_cartridge_header->text) && (m_cartridge_header->text > CARTRIDGE_PAGE_ADDRESS) &&
                    (m_cartridge_header->text < CARTRIDGE_LAST_ADDRESS));

		is_cartridge = cart_init || cart_statement || cart_device || cart_text;
	}

    if (!is_cartridge)
    {
        delete m_cartridge_header;
        m_cartridge_header = 0;
    }


	return is_cartridge;
}



bool CartridgeRomFile::GetEntries(IntArray &entrylist)
{
    bool success = false;

    entrylist.clear();

    if (m_cartridge_header)
    {
        if (m_cartridge_header->init)
            entrylist.push_back(m_cartridge_header->init);
        if (m_cartridge_header->statement)
            entrylist.push_back(m_cartridge_header->statement);
        if (m_cartridge_header->device)
            entrylist.push_back(m_cartridge_header->device);
    }

    if (entrylist.size() > 0)
        success = true;

    return success;
}



const CartHeader *CartridgeRomFile::GetCartridgeHeader()
{
    return m_cartridge_header;
}



bool CartridgeRomFile::HasBasic()
{
    return (m_cartridge_header && m_cartridge_header->text);
}



bool CartridgeRomFile::PureBasic()
{
    return (m_cartridge_header && !m_cartridge_header->device && !m_cartridge_header->init &&
            !m_cartridge_header->statement && m_cartridge_header->text);
}



bool CartridgeRomFile::isCartridge()
{
    return (m_cartridge_header);
}


