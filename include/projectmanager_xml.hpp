/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   06-04-2013 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Load / save a project in a XML file
 **************************************************************/


#ifndef PROJECTMANAGERXML_H
#define PROJECTMANAGERXML_H

#include <wx/xml/xml.h>
#include <wx/textctrl.h>
//#include "process_data.h"
#include "labelslist.hpp"
//#include "main_dialog_base.hpp"

class ProjectManagerXML
{
    public:
        ProjectManagerXML(IDZ80MainBase *parent);

        bool IsSaved();
        bool HasName();
        void New();
        bool Save(const wxString &filename);
        bool Save();
        bool Open(const wxString &filename, const wxString &syslabels_path);
        void SetFilename(const wxString &filename);
        wxString GetFilename();

        void LogIt(const wxString &str);
        void SetLog(wxTextCtrl *set_log);


    protected:
        static const wxString PROJECT_EXTENSION;
        static const unsigned int PROJECT_FILE_VERSION;
        static const wxString ROOT_STR;
        static const wxString SECTION_CODEVIEWLINE_STR;
        static const wxString SECTION_DISASSEMBLED_STR;
        static const wxString SECTION_FILEPROPERTIES_STR;
        static const wxString SECTION_PROGRAMLABEL_STR;
        static const wxString SECTION_VARLABEL_STR;
        static const wxString SECTION_IOLABEL_STR;
        static const wxString SECTION_CONSTANTLABEL_STR;

        static const wxString SUBSECTION_FILENAME_STR;
        static const wxString SUBSECTION_FILEPATH_STR;
        static const wxString SUBSECTION_FILETYPE_STR;
        static const wxString SUBSECTION_CARTRIDGE_STR;
        static const wxString SUBSECTION_ADDRESS_STR;
        static const wxString SUBSECTION_LABEL_STR;
        static const wxString SUBSECTION_DATA_STR;
        static const wxString SUBSECTION_CODE_STR;
        static const wxString SUBSECTION_LINE_STR;

        static const wxString SUBSECTION_FILE_STR;

        static const wxString PROPERTY_FILENAME_STR;
        static const wxString ATTRIBUTE_FILEPATH_STR;
        static const wxString PROPERTY_FILETYPE_STR;
        static const wxString ATTRIBUTE_CARTRIDGE_STR;
        static const wxString ATTRIBUTE_ADDRESS_STR;
        static const wxString ATTRIBUTE_LABEL_STR;
        static const wxString ATTRIBUTE_DATA_STR;
        static const wxString ATTRIBUTE_CODE_STR;
        static const wxString ATTRIBUTE_LINE_STR;

        static const wxString ATTRIBUTE_VERSION_STR;
        static const wxString ATTRIBUTE_FILENAME_STR;
        static const wxString ATTRIBUTE_ORIGINALPATH_STR;
        static const wxString ATTRIBUTE_FILETYPE_STR;
        static const wxString ATTRIBUTE_STARTADDRESS_STR;
        static const wxString ATTRIBUTE_EXECUTIONADDRESS_STR;
        static const wxString ATTRIBUTE_ENDADDRESS_STR;
        static const wxString ATTRIBUTE_TOTALLINES_STR;
        static const wxString ATTRIBUTE_LABELUSERS_STR;

        static const wxString ATTRIBUTE_OPCODE_MNEMONIC_SIGNATURE_STR;
        static const wxString ATTRIBUTE_OPCODE_OFFSET_STR;
        static const wxString ATTRIBUTE_OPCODE_LENGTH_STR;
        static const wxString ATTRIBUTE_OPCODE_ARGUMENTS_STR;

        static const wxString ATTRIBUTE_ORG_STR;
        static const wxString ATTRIBUTE_DASMITEM_STR;
        static const wxString ATTRIBUTE_LINEPROGRAMLABEL_STR;
        static const wxString ATTRIBUTE_LINEVARLABEL_STR;
        static const wxString ATTRIBUTE_COMMENT_STR;


    private:
        enum ReadExceptions
        {
            RE_OPCODE_CONVERSION_FAIL,
            RE_OPCODE_LENGTH_NOT_FOUND,
            RE_OPCODE_MNEMONIC_SIGNATURE_NOT_FOUND,
            RE_OPCODE_OFFSET_NOT_FOUND
        };
        struct ExceptionData
        {
            ReadExceptions  code;
            int line;
        };

        bool    saved, named;
        wxString    project_filename;
        IDZ80MainBase *main_;

        wxTextCtrl  *log;

        bool readHeader(wxXmlDocument &doc);
        bool readFileProperties(wxXmlDocument &doc);
        void readLabel(wxXmlDocument &doc, LabelListCtrl *current_label, const wxString &labelstr);
        void readDisassembled(wxXmlDocument &doc);
        void readCodeLine(wxXmlDocument &doc);

        bool fill_Disassembled(wxXmlNode *datanode);
        bool fillCodeViewLine(wxXmlNode *datanode);

        wxXmlNode *findSection(wxXmlNode *firstchild, const wxString &section);

        void linkLabels(wxArrayInt *label_users);

        void writeHeader(wxXmlDocument &doc);
        void writeFileProperties(wxXmlDocument &doc);
        void writeLabel(wxXmlDocument &doc, LabelListCtrl *current_label, const wxString &section_name);
        void writeDisassembled(wxXmlDocument &doc);
        void writeCodeLine(wxXmlDocument &doc);

        wxString getArgumentAsString(DisassembledItem *d_item);
        void setArgumentFromString(const wxString &arg_str, DisassembledItem *d_item);

        void PrintReadDisassebledException(ExceptionData exceptiondata);
};

#endif // PROJECTMANAGERXML_H
