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

#include "ProcessData.h"


class ProjectManagerXML
{
    public:
        ProjectManagerXML(ProcessData *process_data);
/*
        virtual ~ProjectManagerXML();
*/
        bool IsSaved();
        bool HasName();
        void New();
        bool Save(const wxString &filename);
        bool Save();
        bool Open(const wxString &filename);
        void SetFilename(const wxString &filename);
        wxString GetFilename();

        void LogIt(const wxString &str);
        void SetLog(wxTextCtrl *set_log);

    protected:
        static const wxString PROJECT_EXTENSION;
        static const uint PROJECT_FILE_VERSION;
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
        static const wxString SUBSECTION_OPCODE_STR;
        static const wxString SUBSECTION_LINE_STR;

        static const wxString ATTRIBUTE_VERSION_STR;
        static const wxString ATTRIBUTE_FILENAME_STR;
        static const wxString ATTRIBUTE_ORIGINALPATH_STR;
        static const wxString ATTRIBUTE_FILETYPE_STR;
        static const wxString ATTRIBUTE_STARTADDRESS_STR;
        static const wxString ATTRIBUTE_EXECUTIONADDRESS_STR;
        static const wxString ATTRIBUTE_ENDADDRESS_STR;
        static const wxString ATTRIBUTE_TOTALLINES_STR;
        static const wxString ATTRIBUTE_LABELUSERS_STR;
        static const wxString ATTRIBUTE_OPCODETYPE_STR;
        static const wxString ATTRIBUTE_OPCODELENGTH_STR;
        static const wxString ATTRIBUTE_OPCODEOFFSET_STR;
        static const wxString ATTRIBUTE_OPCODE_STR;
        static const wxString ATTRIBUTE_ARGUMENTNUM_STR;
        static const wxString ATTRIBUTE_ARGUMENTSIZE_STR;
        static const wxString ATTRIBUTE_ARGUMENTS_STR;
        static const wxString ATTRIBUTE_ORG_STR;
        static const wxString ATTRIBUTE_DASMITEM_STR;
        static const wxString ATTRIBUTE_LINEPROGRAMLABEL_STR;
        static const wxString ATTRIBUTE_LINEVARLABEL_STR;
        static const wxString ATTRIBUTE_COMMENT_STR;


    private:
        bool    saved, named;
        wxString    project_filename;
        ProcessData *process;
        CodeViewLine    *CodeViewLines;

        wxTextCtrl  *log;

        bool readHeader(wxXmlDocument &doc);
        bool readFileProperties(wxXmlDocument &doc);
        void readLabel(wxXmlDocument &doc, LabelListCtrl *current_label, const wxString &labelstr);
        void readDasmData(wxXmlDocument &doc);
        void readCodeLine(wxXmlDocument &doc);

        bool fillDasmData(wxXmlNode *datanode);
        bool fillCodeViewLine(wxXmlNode *datanode);

        wxXmlNode *findSection(wxXmlNode *firstchild, const wxString &section);

        void linkLabels(wxArrayInt *label_users);

        void writeHeader(wxXmlDocument &doc);
        void writeFileProperties(wxXmlDocument &doc);
        void writeLabel(wxXmlDocument &doc, LabelListCtrl *current_label, const wxString &section_name);
        void writeDasmData(wxXmlDocument &doc);
        void writeCodeLine(wxXmlDocument &doc);
};

#endif // PROJECTMANAGERXML_H
