%name Parser
%token_prefix SCRIPT_PARSER_TK_

%include {
#include "script/Tokenizer.h"
#include "script/ScriptParser.h"
#include "io/CSVReader.h"
#include "io/NodeTypeLoader.h"
#include "io/EdgeTypeLoader.h"
#include "gdb/Graph.h"
    
namespace dex { namespace script {
    
    /*
     * Structure to store edge definitions.
     * Used when creating an edge type (CREATE EDGE).
     */
    struct EdgeDefinition
    {
    public:
        std::wstring * name;
        dex::gdb::bool_t directed;
        
        EdgeDefinition()
        : name(new std::wstring())
        , directed(false)
        {
        }
        
        virtual ~EdgeDefinition()
        {
            if ((std::wstring *)NULL != name) delete name;
        }
    };
    
    /*
     * Structure to store attribute definitions.
     * Used when creating a node or edge type (CREATE NODE or CREATE EDGE).
     */
    struct AttributeDefinition
    {
    public:
        std::wstring * name;
        dex::gdb::DataType dt;
        dex::gdb::AttributeKind kind;
        
        AttributeDefinition()
        : name(new std::wstring())
        , dt(dex::gdb::Boolean)
        , kind(dex::gdb::Basic)
        {
        }
        
        virtual ~AttributeDefinition()
        {
            if ((std::wstring *)NULL != name) delete name;
        }
    };
    
    /*
     * Structure to store column definitions.
     * Used when loading a node or edge type (LOAD NODES|EDGES INTO X COLUMNS ...).
     */
    struct ColumnDefinition
    {
    public:
        std::wstring * name;
        std::wstring * alias;
        dex::gdb::attr_t attr;
        
        ColumnDefinition()
        : name(new std::wstring())
        , alias(new std::wstring())
        , attr(dex::gdb::Attribute::InvalidAttribute)
        {
        }
        
        virtual ~ColumnDefinition()
        {
            if ((std::wstring *)NULL != name) delete name;
            if ((std::wstring *)NULL != alias) delete alias;
        }
    };
    
    /*
     * Structure to store fields properties.
     * Used when loading a node or edge type (FIELDS TERMINATED X ENCLOSED Y ALLOW MULTILINE)
     * If fields are NULL then they have not been set.
     */
    struct FieldsDefinition
    {
    public:
        dex::gdb::uchar_t * terminated;
        dex::gdb::uchar_t * enclosed;
        dex::gdb::bool_t * multiline;
        dex::gdb::int32_t * maxExtralines;
        
        FieldsDefinition()
        : terminated((dex::gdb::uchar_t *)NULL)
        , enclosed((dex::gdb::uchar_t *)NULL)
        , multiline((dex::gdb::bool_t *)NULL)
        , maxExtralines((dex::gdb::int32_t *)NULL)
        {
        }
        
        virtual ~FieldsDefinition()
        {
            if ((dex::gdb::uchar_t *)NULL != terminated) delete terminated;
            if ((dex::gdb::uchar_t *)NULL != enclosed) delete enclosed;
            if ((dex::gdb::bool_t *)NULL != multiline) delete multiline;
            if ((dex::gdb::int32_t *)NULL != maxExtralines) delete maxExtralines;
        }
    };
    
    struct WhereDefinition
    {
    public:
        std::wstring * tailName;
        std::wstring * tailType;
        std::wstring * tailAttr;
        
        std::wstring * headName;
        std::wstring * headType;
        std::wstring * headAttr;
        
        WhereDefinition()
        : tailName((std::wstring *)NULL)
        , tailType((std::wstring *)NULL)
        , tailAttr((std::wstring *)NULL)
        , headName((std::wstring *)NULL)
        , headType((std::wstring *)NULL)
        , headAttr((std::wstring *)NULL)
        {
        }
        
        virtual ~WhereDefinition()
        {
            if ((std::wstring *)NULL != tailName) delete tailName;
            if ((std::wstring *)NULL != tailType) delete tailType;
            if ((std::wstring *)NULL != tailAttr) delete tailAttr;
            if ((std::wstring *)NULL != headName) delete headName;
            if ((std::wstring *)NULL != headType) delete headType;
            if ((std::wstring *)NULL != headAttr) delete headAttr;
        }
    };
    
    /**
     * This structure is filled in as follows:
     *
     * mode rows                        --> *phases = 1 & partitions = NULL
     * mode columns                     --> *phases = 2 & partitions = NULL
     * mode columns split               --> *phases = 3 & partitions = NULL
     * mode columns split partitions X  --> *phases = 3 & *partitons = X
     */
    struct ModeDefinition
    {
    public:
        dex::gdb::int32_t * phases;
        dex::gdb::int32_t * partitions;
        
        ModeDefinition()
        : phases((dex::gdb::int32_t *)NULL)
        , partitions((dex::gdb::int32_t *)NULL)
        {
        }
        
        virtual ~ModeDefinition()
        {
            if ((dex::gdb::int32_t *)NULL != phases) delete phases;
            if ((dex::gdb::int32_t *)NULL != partitions) delete partitions;
        }
    };
    
    /**
     * \brief Implementation of TypeLoaderListener for the script.
     */
    class ScriptTypeLoaderListener : public dex::io::TypeLoaderListener
    {
    private:
        
        std::wostream & outstrm;
        
    public:
        
        ScriptTypeLoaderListener(std::wostream & ostrm)
        : outstrm(ostrm)
        {
        }
        
        void NotifyEvent(const dex::io::TypeLoaderEvent & ev)
        {
            outstrm << ev << std::endl;
        }        
    };
    
}} // end nampespace dex::script

#define SCRIPT_LISTENER_FREQ 100000
#define GRAPH spdata->GetGraph()
#define OUT_STRM if ((std::wostream *)NULL != spdata->GetOutputStream()) *(spdata->GetOutputStream())
#define ERR_STRM if ((std::wostream *)NULL != spdata->GetErrorStream()) *(spdata->GetErrorStream())
}

%extra_argument { dex::script::ScriptParserData * spdata }
%token_type { dex::script::Token * }
%token_destructor { delete $$; }
%type openORcreate { dex::gdb::bool_t } // OPEN --> true, CREATE --> false
%type nodeORedge { dex::gdb::bool_t } // NODE --> true, EDGE --> false
%type nodesORedges { dex::gdb::bool_t } // NODES --> true, EDGES --> false
%type attributedef { dex::script::AttributeDefinition * }
%type datatype { dex::gdb::DataType }
%type index_kind { enum dex::gdb::AttributeKind }
%type attributedef_list { std::vector<dex::script::AttributeDefinition *> * }
%type attributedef_list_nopar { std::vector<dex::script::AttributeDefinition *> * }
%type directedORundirectedEdge { dex::script::EdgeDefinition * }
%type restrictedEdge { dex::script::EdgeDefinition * }
%type materialize_neighbors { dex::gdb::bool_t }
%type from { dex::gdb::int64_t * } // if NULL it has not been set
%type max { dex::gdb::int64_t * } // if NULL it has not been set
%type columndef_list { std::vector<dex::script::ColumnDefinition *> * }
%type columndef { dex::script::ColumnDefinition * }
%type ignore { std::vector<std::wstring *> * }
%type ignorecols_list { std::vector<std::wstring *> * }
%type where { dex::script::WhereDefinition * } // as optional, it could be NULL
%type fields { dex::script::FieldsDefinition * }
%type enclosed { dex::script::FieldsDefinition * }
%type multiline { dex::gdb::int32_t }
%type mode { dex::script::ModeDefinition * }
%type localeORnolocale { std::wstring * } // as optional, it could be NULL

%parse_accept {
    if (dex::script::Parsing == yypParser->spdata->GetState())
    {
        yypParser->spdata->SetState(dex::script::Parsed);
    }
}

%parse_failure {
    yypParser->spdata->SetState(dex::script::ParserFailure);
}

%syntax_error {
    yypParser->spdata->SetState(dex::script::SyntaxError);
}

%stack_overflow {
    yypParser->spdata->SetState(dex::script::StackOverflow);
}
    
%start_symbol main

////////////////////////////////////////////////////////////////////////////////
    
main ::= command_list.


command_list ::= command_list command.


command_list ::= .


//    (CREATE|OPEN) GDB alias INTO filename 

command ::= openORcreate(open) GDB STRING(tknAlias) INTO STRING(tknPath).
{
    while (spdata->Execute()) // we'll break the while at the end or in case of error
    {
        if (!spdata->CloseStorage())
        {
            ERR_STRM << dex::script::ExecutionError 
            << L" Previous gdb cannot be closed" << std::endl;
            spdata->SetState(dex::script::ExecutionError);
            break;
        }
        
        if (!spdata->OpenStorage(tknPath->GetString(), tknAlias->GetString(), open))
        {
            ERR_STRM << dex::script::ExecutionError 
            << L" The gdb '" << tknAlias->GetString() 
            << L"' cannot be " << (open?L"opened":L"created") << std::endl;
            spdata->SetState(dex::script::ExecutionError);
        }
        else 
        {
            OUT_STRM << dex::script::Parsing 
            << (open?L" OPEN":L" CREATE") << L" GDB '" << tknAlias->GetString() << L"' INTO '" 
            << tknPath->GetString() << L"'" << std::endl;
        }
        
        break;
    }
}


openORcreate(b) ::= OPEN.
{
    b = true;
}


openORcreate(b) ::= CREATE.
{
    b = false;
}


//    CREATE NODE node_type_name "("
//      [attribute_name (INTEGER|LONG|DOUBLE|STRING|BOOLEAN|TIMESTAMP|TEXT) [INDEXED|UNIQUE|BASIC], ...]
//    ")"

command ::= CREATE NODE STRING(tknName) attributedef_list(attrDefs).
{
    while (spdata->Execute()) // we'll break the while at the end or in case of error
    {
        if (!spdata->StorageIsOpened())
        {
            ERR_STRM << dex::script::SyntaxError 
            << L" OPEN or CREATE is required" << std::endl;
            spdata->SetState(dex::script::SyntaxError);
            break;
        }
        
        dex::gdb::type_t type = dex::gdb::Type::InvalidType;
        try 
        {
            type = GRAPH->NewNodeType(tknName->GetString());
        }
        catch (dex::gdb::Error err) 
        {
            std::wstring wstr(err.Message().begin(), err.Message().end());
            ERR_STRM << dex::script::ExecutionError 
            << L" " << wstr << std::endl;
            spdata->SetState(dex::script::ExecutionError);
        }
        if (!spdata->IsNoErrorState()) break;
        
        DEX_ASSERT(dex::gdb::Type::InvalidType != type);
        OUT_STRM << dex::script::Parsing 
        << L" CREATE NODE '" << tknName->GetString() << L"'" 
        << std::endl;
        
        for (std::vector<dex::script::AttributeDefinition *>::iterator it = attrDefs->begin();
             (it != attrDefs->end()) && spdata->IsNoErrorState(); it++)
        {
            dex::script::AttributeDefinition * adef = *it;
            dex::gdb::attr_t attr = dex::gdb::Attribute::InvalidAttribute;
            try 
            {
                attr = GRAPH->NewAttribute(type, *(adef->name), 
                                           adef->dt, adef->kind);
            }
            catch (dex::gdb::Error err) 
            {
                std::wstring wstr(err.Message().begin(), err.Message().end());
                ERR_STRM << dex::script::ExecutionError 
                << L" " << wstr << std::endl;
                spdata->SetState(dex::script::ExecutionError);
            }
            if (spdata->IsNoErrorState())
            {
                OUT_STRM << dex::script::Parsing 
                << L" CREATE ATTRIBUTE '" << tknName->GetString() 
                << L"'.'" << *(adef->name) << L"' " 
                << adef->dt << L" "
                << adef->kind
                << std::endl;
            }
        }
        
        break;
    }
}


attributedef_list(attrDefs) ::= .
{
    attrDefs = new std::vector<dex::script::AttributeDefinition *>();
}
        
attributedef_list(attrDefs) ::= PAREN_OPEN attributedef_list_nopar(attrDefs2) PAREN_CLOSE.
{
    attrDefs = attrDefs2;
}


attributedef_list_nopar(attrDefs) ::= attributedef_list_nopar(attrDefs2) COMMA attributedef(attrDef).
{
    attrDefs = attrDefs2;
    attrDefs->push_back(attrDef);
}
    
    
attributedef_list_nopar(attrDefs) ::= attributedef(attrDef).
{
    attrDefs = new std::vector<dex::script::AttributeDefinition *>();
    attrDefs->push_back(attrDef);
}
    
    
attributedef_list_nopar(attrDefs) ::= .
{
    attrDefs = new std::vector<dex::script::AttributeDefinition *>();
}
        
    
attributedef(attrDef) ::= STRING(tknName) datatype(thedatatype) index_kind(thekind).
{
    attrDef = new dex::script::AttributeDefinition();
    attrDef->name->assign(tknName->GetString());
    attrDef->dt = thedatatype;
    attrDef->kind = thekind;
}


datatype(thedatatype) ::= BOOLEAN.
{
    thedatatype = dex::gdb::Boolean;
}


datatype(thedatatype) ::= INTEGER.
{
    thedatatype = dex::gdb::Integer;
}


datatype(thedatatype) ::= LONG.
{
    thedatatype = dex::gdb::Long;
}


datatype(thedatatype) ::= DOUBLE.
{
    thedatatype = dex::gdb::Double;
}


datatype(thedatatype) ::= OID.
{
    thedatatype = dex::gdb::OID;
}


datatype(thedatatype) ::= TIMESTAMP.
{
    thedatatype = dex::gdb::Timestamp;
}


datatype(thedatatype) ::= TEXT.
{
    thedatatype = dex::gdb::Text;
}


datatype(thedatatype) ::= STRING.
{
    thedatatype = dex::gdb::String;
}

        
index_kind(kind) ::= .
{
    kind = dex::gdb::Basic; 
}


index_kind(kind) ::= BASIC.
{
    kind = dex::gdb::Basic; 
}


index_kind(kind) ::= INDEXED.
{
    kind = dex::gdb::Indexed; 
}


index_kind(kind) ::= UNIQUE.
{
    kind = dex::gdb::Unique; 
}


// Rule for RESTRICTED edges
//    CREATE [DIRECTED] EDGE edge_type_name
//    FROM node_type_name TO node_type_name 
//     ["(" attribute_name (INTEGER|LONG|DOUBLE|STRING|BOOLEAN|TIMESTAMP|TEXT)[, ...]")"]
//     [MATERIALIZE NEIGHBORS]"
//
command ::= restrictedEdge(edgeDef) STRING(tknTail) TO STRING(tknHead) attributedef_list(attrDefs) materialize_neighbors(neigh).
{
    while (spdata->Execute()) // we'll break the while at the end or in case of error
    {
        if (!spdata->StorageIsOpened())
        {
            ERR_STRM << dex::script::SyntaxError 
            << L" OPEN or CREATE is required" << std::endl;
            spdata->SetState(dex::script::SyntaxError);
            break;
        }
        
        dex::gdb::type_t type = dex::gdb::Type::InvalidType;
        try 
        {
            dex::gdb::type_t tailT = GRAPH->FindType(tknTail->GetString());
            if ((dex::gdb::Type::InvalidType == tailT) 
                || (dex::gdb::Type::GlobalType == tailT))
            {
                ERR_STRM << dex::script::ExecutionError 
                << L"Given tail type '" << tknTail->GetString() 
                << L"' does not exist" << std::endl;
                spdata->SetState(dex::script::ExecutionError);
            }
            else 
            {
                dex::gdb::type_t headT = GRAPH->FindType(tknHead->GetString());
                if ((dex::gdb::Type::InvalidType == headT) 
                    || (dex::gdb::Type::GlobalType == headT))
                {
                    ERR_STRM << dex::script::ExecutionError 
                    << L"Given head type '" << tknHead->GetString() 
                    << L"' does not exist" << std::endl;
                    spdata->SetState(dex::script::ExecutionError);
                }
                else 
                {
                    type = GRAPH->NewRestrictedEdgeType(*edgeDef->name, 
                                                        tailT, headT, neigh);
                }
            }
        }
        catch (dex::gdb::Error err) 
        {
            std::wstring wstr(err.Message().begin(), err.Message().end());
            ERR_STRM << dex::script::ExecutionError 
            << L" " << wstr << std::endl;
            spdata->SetState(dex::script::ExecutionError);
        }
        if (!spdata->IsNoErrorState()) break;

        DEX_ASSERT(dex::gdb::Type::InvalidType != type);
        OUT_STRM << dex::script::Parsing 
        << L" CREATE EDGE '" << *edgeDef->name << L"'" 
        << L" from '" << tknTail->GetString() << L"' to '" << tknHead->GetString() << L"'" 
        << std::endl;
        for (std::vector<dex::script::AttributeDefinition *>::iterator it = attrDefs->begin();
             (it != attrDefs->end()) && spdata->IsNoErrorState(); it++)
        {
            dex::script::AttributeDefinition * adef = *it;
            dex::gdb::attr_t attr = dex::gdb::Attribute::InvalidAttribute;
            try 
            {
                attr = GRAPH->NewAttribute(type, *(adef->name), 
                                           adef->dt, adef->kind);
            }
            catch (dex::gdb::Error err) 
            {
                std::wstring wstr(err.Message().begin(), err.Message().end());
                ERR_STRM << dex::script::ExecutionError 
                << L" " << wstr << std::endl;
                spdata->SetState(dex::script::ExecutionError);
            }
            if (spdata->IsNoErrorState())
            {
                DEX_ASSERT(dex::gdb::Attribute::InvalidAttribute != attr);
                OUT_STRM << dex::script::Parsing 
                << L" CREATE ATTRIBUTE '" << *edgeDef->name << L"'.'" 
                << *(adef->name) << L"' " 
                << adef->dt << L" " 
                << adef->kind << std::endl;
            }
        }
        
        break;
    }
}


// Rule for NOT restricted edges
//    CREATE [DIRECTED|UNDIRECTED] EDGE edge_type_name
//     ["(" attribute_name (INTEGER|LONG|DOUBLE|STRING|BOOLEAN|TIMESTAMP|TEXT)[, ...]")"]
//     [MATERIALIZE NEIGHBORS]"
command ::= directedORundirectedEdge(edgeDef) attributedef_list(attrDefs) materialize_neighbors(neigh).
{
    while (spdata->Execute()) // we'll break the while at the end or in case of error
    {
        if (!spdata->StorageIsOpened())
        {
            ERR_STRM << dex::script::SyntaxError 
            << L" OPEN or CREATE is required" << std::endl;
            spdata->SetState(dex::script::SyntaxError);
            break;
        }
        
        dex::gdb::type_t type = dex::gdb::Type::InvalidType;
        try 
        {
            type = GRAPH->NewEdgeType(*edgeDef->name, edgeDef->directed, neigh);
        }
        catch (dex::gdb::Error err) 
        {
            std::wstring wstr(err.Message().begin(), err.Message().end());
            ERR_STRM << dex::script::ExecutionError 
            << L" " << wstr << std::endl;
            spdata->SetState(dex::script::ExecutionError);
        }
        if (!spdata->IsNoErrorState()) break;
        
        DEX_ASSERT(dex::gdb::Type::InvalidType != type);
        OUT_STRM << dex::script::Parsing 
        << L" CREATE " << (edgeDef->directed?L"DIRECTED":L"UNDIRECTED") << L" EDGE '" 
        << *edgeDef->name << L"' " << std::endl;
        for (std::vector<dex::script::AttributeDefinition *>::iterator it = attrDefs->begin();
             (it != attrDefs->end()) && spdata->IsNoErrorState(); it++)
        {
            dex::script::AttributeDefinition * adef = *it;
            dex::gdb::attr_t attr = dex::gdb::Attribute::InvalidAttribute;
            try 
            {
                attr = GRAPH->NewAttribute(type, *(adef->name), 
                                           adef->dt, adef->kind);
            }
            catch (dex::gdb::Error err) 
            {
                std::wstring wstr(err.Message().begin(), err.Message().end());
                ERR_STRM << dex::script::ExecutionError 
                << L" " << wstr << std::endl;
                spdata->SetState(dex::script::ExecutionError);
            }
            if (spdata->IsNoErrorState())
            {
                DEX_ASSERT(dex::gdb::Attribute::InvalidAttribute != attr);
                OUT_STRM << dex::script::Parsing 
                << L" CREATE ATTRIBUTE '" << *edgeDef->name
                << L"'.'" << *(adef->name) << L"' " 
                << adef->dt << L" " 
                << adef->kind << std::endl;
            }
        }
        
        break;
    }
}

        
materialize_neighbors(neigh) ::= .
{
    neigh = false;
}
    
    
materialize_neighbors(neigh) ::= MATERIALIZE NEIGHBORS.
{
    neigh = true;
}
    
    


restrictedEdge(edgeDef) ::= CREATE EDGE STRING(tknName) FROM.
{
    edgeDef = new dex::script::EdgeDefinition();
    edgeDef->name->assign(tknName->GetString());
    edgeDef->directed = true;
}

restrictedEdge(edgeDef) ::= CREATE DIRECTED EDGE STRING(tknName) FROM.
{
    edgeDef = new dex::script::EdgeDefinition();
    edgeDef->name->assign(tknName->GetString());
    edgeDef->directed = true;
}


directedORundirectedEdge(edgeDef) ::= CREATE EDGE STRING(tknName).
{
    edgeDef = new dex::script::EdgeDefinition();
    edgeDef->name->assign(tknName->GetString());
    edgeDef->directed = true;
}


directedORundirectedEdge(edgeDef) ::= CREATE DIRECTED EDGE STRING(tknName).
{
    edgeDef = new dex::script::EdgeDefinition();
    edgeDef->name->assign(tknName->GetString());
    edgeDef->directed = true;
}


directedORundirectedEdge(edgeDef) ::= CREATE UNDIRECTED EDGE STRING(tknName).
{
    edgeDef = new dex::script::EdgeDefinition();
    edgeDef->name->assign(tknName->GetString());
    edgeDef->directed = false;
}



command ::= DROP nodeORedge(isnode) STRING(tknName).
{
    while (spdata->Execute()) // we'll break the while at the end or in case of error
    {
        if (!spdata->StorageIsOpened())
        {
            ERR_STRM << dex::script::SyntaxError 
            << L" OPEN or CREATE is required" << std::endl;
            spdata->SetState(dex::script::SyntaxError);
            break;
        }
        
        try 
        {
            dex::gdb::type_t type = GRAPH->FindType(tknName->GetString());
            if (dex::gdb::Type::InvalidType == type)
            {
                ERR_STRM << dex::script::ExecutionError 
                << L"Give type '" << tknName->GetString() 
                << L"' does not exist" << std::endl;
                spdata->SetState(dex::script::ExecutionError);
            }
            else 
            {
                dex::gdb::Type * tdata = GRAPH->GetType(type);
                if (isnode && dex::gdb::Node != tdata->GetObjectType())
                {
                    ERR_STRM << dex::script::ExecutionError 
                    << L"Give type '" << tknName->GetString() 
                    << L"' is not a node type" << std::endl;
                    spdata->SetState(dex::script::ExecutionError);
                    delete tdata;
                }
                else if (!isnode && dex::gdb::Edge != tdata->GetObjectType())
                {
                    ERR_STRM << dex::script::ExecutionError 
                    << L"Give type '" << tknName->GetString() 
                    << L"' is not an edge type" << std::endl;
                    spdata->SetState(dex::script::ExecutionError);
                    delete tdata;
                }
                else 
                {
                    delete tdata;
                    GRAPH->RemoveType(type);
                }
            }
        }
        catch (dex::gdb::Error err) 
        {
            std::wstring wstr(err.Message().begin(), err.Message().end());
            ERR_STRM << dex::script::ExecutionError 
            << L" " << wstr << std::endl;
            spdata->SetState(dex::script::ExecutionError);
        }
        if (spdata->IsNoErrorState())
        {
            OUT_STRM << dex::script::Parsing 
            << L" DROP " << (isnode?L"NODE":L"EDGE") << L" '" << tknName->GetString() << L"'" 
            << std::endl;
        }
        
        break;
    }
}


nodeORedge(b) ::= NODE.
{
    b = true;
}


nodeORedge(b) ::= EDGE.
{
    b = false;
}
        
        
command ::= DROP ATTRIBUTE STRING(tknTypeName) DOT STRING(tknAttrName).
{
    while (spdata->Execute()) // we'll break the while at the end or in case of error
    {
        if (!spdata->StorageIsOpened())
        {
            ERR_STRM << dex::script::SyntaxError 
            << L" OPEN or CREATE is required" << std::endl;
            spdata->SetState(dex::script::SyntaxError);
            break;
        }
        
        try 
        {
            dex::gdb::type_t type = GRAPH->FindType(tknTypeName->GetString());
            if (dex::gdb::Type::InvalidType == type)
            {
                ERR_STRM << dex::script::ExecutionError 
                << L"Give type '" << tknTypeName->GetString() 
                << L"' does not exist" << std::endl;
                spdata->SetState(dex::script::ExecutionError);
            }
            dex::gdb::attr_t attr = GRAPH->FindAttribute(type, tknAttrName->GetString());
            if (dex::gdb::Attribute::InvalidAttribute == attr)
            {
                ERR_STRM << dex::script::ExecutionError 
                << L"Given attribute '" << tknTypeName->GetString() << L"'."
                << tknAttrName->GetString() << L"' does not exist" << std::endl;
                spdata->SetState(dex::script::ExecutionError);
            }
            GRAPH->RemoveAttribute(attr);
        }
        catch (dex::gdb::Error err) 
        {
            std::wstring wstr(err.Message().begin(), err.Message().end());
            ERR_STRM << dex::script::ExecutionError 
            << L" " << wstr << std::endl;
            spdata->SetState(dex::script::ExecutionError);
        }
        if (spdata->IsNoErrorState())
        {
            OUT_STRM << dex::script::Parsing 
            << L" DROP ATTRIBUTE '" << tknTypeName->GetString() << L"'.'"
            << tknAttrName->GetString() << L"'" << std::endl;
        }
        
        break;
    }
}
    

command ::= INDEX STRING(tknTypeName) DOT STRING(tknAttrName) index_kind(thekind).
{
    while (spdata->Execute()) // we'll break the while at the end or in case of error
    {
        if (!spdata->StorageIsOpened())
        {
            ERR_STRM << dex::script::SyntaxError 
            << L" OPEN or CREATE is required" << std::endl;
            spdata->SetState(dex::script::SyntaxError);
            break;
        }
        
        try 
        {
            dex::gdb::type_t type = GRAPH->FindType(tknTypeName->GetString());
            if (dex::gdb::Type::InvalidType == type)
            {
                ERR_STRM << dex::script::ExecutionError 
                << L"Give type '" << tknTypeName->GetString() 
                << L"' does not exist" << std::endl;
                spdata->SetState(dex::script::ExecutionError);
            }
            dex::gdb::attr_t attr = GRAPH->FindAttribute(type, tknAttrName->GetString());
            if (dex::gdb::Attribute::InvalidAttribute == attr)
            {
                ERR_STRM << dex::script::ExecutionError 
                << L"Given attribute '" << tknTypeName->GetString() << L"'."
                << tknAttrName->GetString() << L"' does not exist" << std::endl;
                spdata->SetState(dex::script::ExecutionError);
            }
            GRAPH->IndexAttribute(attr, thekind);
        }
        catch (dex::gdb::Error err) 
        {
            std::wstring wstr(err.Message().begin(), err.Message().end());
            ERR_STRM << dex::script::ExecutionError 
            << L" " << wstr << std::endl;
            spdata->SetState(dex::script::ExecutionError);
        }
        if (spdata->IsNoErrorState())
        {
            OUT_STRM << dex::script::Parsing 
            << L" INDEX ATTRIBUTE '" << tknTypeName->GetString() << L"'.'"
            << tknAttrName->GetString() << L" " 
            << thekind << std::endl;
        }
        
        break;
    }
}


//    LOAD (NODES|EDGES) file_name
//    [LOCALE locale_name]
//    COLUMNS attribute_name [alias_name], ...
//    INTO node_type_name
//    [IGNORE (attribute_name|alias_name), ....]
//    WHERE TAIL name = type.attr HEAD name = type.attr   <-- this just when loading EDGES
//    [FIELDS
//     [TERMINATED char]
//     [ENCLOSED char]
//     [ALLOW_MULTILINE [maxExtralines]]]
//    [FROM num]
//    [MAX num]
//    [MODE (ROWS|COLUMNS [SPLIT [PARTITIONS num]])]


command ::= LOAD nodesORedges(isnode) STRING(tknFilePath)
            localeORnolocale(localeData)
            COLUMNS columndef_list(colDefs)
            INTO STRING(tknTypeName)
            ignore(ignoreCols)
            where(whereDef)
            fields(fieldsDef)
            from(f)
            max(m)
            mode(modeDef).
{
    while (spdata->Execute()) // we'll break the while at the end or in case of error
    {
        if (!spdata->StorageIsOpened())
        {
            ERR_STRM << dex::script::SyntaxError 
            << L" OPEN or CREATE is required" << std::endl;
            spdata->SetState(dex::script::SyntaxError);
            break;
        }
        
        std::wstringstream wout;
        wout << dex::script::Parsing << L" LOAD " 
        << (isnode?L"NODES ":L"EDGES ") << tknFilePath->GetString();

        if (!localeData->empty())
        {
            wout << L" LOCALE " << *localeData;
        }
        
        wout << L" COLUMNS ";
        if (0 == colDefs->size())
        {
            ERR_STRM << dex::script::SyntaxError 
            << L" At least one COLUMN must be specified" << std::endl;
            spdata->SetState(dex::script::SyntaxError);
            break;
        }
        for (std::vector<dex::script::ColumnDefinition *>::iterator it = colDefs->begin();
             it != colDefs->end(); it++)
        {
            wout << *((*it)->name) << L"[" << *((*it)->alias) << L"] ";
        }
        
        dex::gdb::type_t t = dex::gdb::Type::InvalidType;
        wout << L"\t INTO " << tknTypeName->GetString();
        // Check given type exists and all given attributes (columns) 
        // belong to the given type
        
        t = GRAPH->FindType(tknTypeName->GetString());
        if (dex::gdb::Type::InvalidType == t)
        {
            ERR_STRM << dex::script::ExecutionError << L" Given type '" 
            << tknTypeName->GetString() << L"' does not exist" << std::endl;
            spdata->SetState(dex::script::ExecutionError);
            break;
        }

        dex::gdb::Type * tdata = GRAPH->GetType(t);
        dex::gdb::bool_t b = (dex::gdb::Node == tdata->GetObjectType());
        delete tdata;
        if (isnode != b)
        {
            ERR_STRM << dex::script::SyntaxError << L" Given type '" 
            << tknTypeName->GetString() << L"' is not a " 
            << (isnode?L"node":L"edge") << L" type" << std::endl;
            spdata->SetState(dex::script::SyntaxError);
            break;
        }
        for (std::vector<dex::script::ColumnDefinition *>::iterator it = colDefs->begin();
             it != colDefs->end(); it++)
        {
            std::wstring & attrname = *((*it)->name);
            if (attrname.compare(L"*") == 0)
            {
                (*it)->attr = dex::gdb::Attribute::InvalidAttribute; // mark it to be ignored
            }
            else 
            {
                dex::gdb::attr_t attr = GRAPH->FindAttribute(t, *((*it)->name));
                if (dex::gdb::Attribute::InvalidAttribute != attr)
                {
                    (*it)->attr = attr; // we set this now!
                }
            }
        }
        
        DEX_ASSERT(dex::gdb::Type::InvalidType != t);
        // ASSERT: for all colDefs, its attr != dex::gdb::Attribute::InvalidAttribute
        
        if (((std::vector<std::wstring *> *) NULL != ignoreCols)
            && (ignoreCols->size() > 0))
        {
            wout << L" IGNORE ";
            for (std::vector<std::wstring *>::iterator it = ignoreCols->begin();
                 spdata->IsNoErrorState() && (it != ignoreCols->end()); it++)
            {
                wout << *(*it) << L" ";
                std::wstring & ignorecolname = *(*it);
                dex::gdb::bool_t found = false;
                for (std::vector<dex::script::ColumnDefinition *>::iterator it = colDefs->begin();
                     it != colDefs->end(); it++)
                {
                    if ((ignorecolname.compare(*((*it)->alias)) == 0)
                        || (ignorecolname.compare(*((*it)->name)) == 0))
                    {
                        found = true;
                        // then just 'mark' them into colDefs to be ignored
                        (*it)->attr = dex::gdb::Attribute::InvalidAttribute;
                    }
                }
                if (!found)
                {
                    ERR_STRM << dex::script::SyntaxError 
                    << L" Given column '" << ignorecolname 
                    << L"' to be ignored does not exist" << std::endl;
                    spdata->SetState(dex::script::SyntaxError);
                }
            }
            if (!spdata->IsNoErrorState()) break;
        }
        
        if (((dex::script::WhereDefinition *) NULL == whereDef) && !isnode)
        {
            ERR_STRM << dex::script::SyntaxError 
            << L" WHERE clause must be specified when loading edges" << std::endl;
            spdata->SetState(dex::script::SyntaxError);
            break;
        }
        else if (((dex::script::WhereDefinition *) NULL != whereDef) && isnode)
        {
            ERR_STRM << dex::script::SyntaxError 
            << L" WHERE clause cannot be specified when loading nodes" << std::endl;
            spdata->SetState(dex::script::SyntaxError);
            break;
        }
        
        dex::gdb::int32_t hPos = -1, tPos = -1;
        dex::gdb::attr_t hAttr = dex::gdb::Attribute::InvalidAttribute, tAttr = dex::gdb::Attribute::InvalidAttribute;
        if (((dex::script::WhereDefinition *) NULL != whereDef))
        {
            wout << L" WHERE TAIL " 
            << *(whereDef->tailName) << L"=" << *(whereDef->tailType) << L"." << *(whereDef->tailAttr)
            << L" HEAD " 
            << *(whereDef->headName) << L"=" << *(whereDef->headType) << L"." << *(whereDef->headAttr);
            
            dex::gdb::int32_t pos = 0;
            for (std::vector<dex::script::ColumnDefinition *>::iterator it = colDefs->begin();
                 (it != colDefs->end()) && ((-1 == hPos) || (-1 == tPos)); it++, pos++)
            {
                if (-1 == tPos
                    && 
                    ((whereDef->tailName->compare(*((*it)->alias)) == 0) 
                     || (whereDef->tailName->compare(*((*it)->name)) == 0)))
                {
                    tPos = pos;
                }
                
                if (-1 == hPos
                    && 
                    ((whereDef->headName->compare(*((*it)->alias)) == 0) 
                     || (whereDef->headName->compare(*((*it)->name)) == 0)))
                {
                    hPos = pos;
                }
            }
            
            if (-1 == tPos)
            {
                ERR_STRM << dex::script::SyntaxError
                << L" [WHERE TAIL] Given name '" << *(whereDef->tailName) << L"' does not reference any column" << std::endl;
                spdata->SetState(dex::script::SyntaxError);
                break;
            }
            
            if (-1 == hPos)
            {
                ERR_STRM << dex::script::SyntaxError
                << L" [WHERE HEAD] Given name '" << *(whereDef->headName) 
                << L"' does not reference any column" << std::endl;
                spdata->SetState(dex::script::SyntaxError);
                break;
            }
            
            dex::gdb::type_t aux = GRAPH->FindType(*(whereDef->tailType));
            if (dex::gdb::Type::InvalidType == aux)
            {
                ERR_STRM << dex::script::SyntaxError
                << L" [WHERE TAIL] Given type '" << *(whereDef->tailType) 
                << L"' does not exist" << std::endl;
                spdata->SetState(dex::script::SyntaxError);
                break;
            }
            tAttr = GRAPH->FindAttribute(aux, *(whereDef->tailAttr));
            if (dex::gdb::Attribute::InvalidAttribute == tAttr)
            {
                ERR_STRM << dex::script::SyntaxError
                << L" [WHERE TAIL] Given attribute '" << *(whereDef->tailAttr) 
                << L"' does not exist" << std::endl;
                spdata->SetState(dex::script::SyntaxError);
                break;
            }
            aux = GRAPH->FindType(*(whereDef->headType));
            if (dex::gdb::Type::InvalidType == aux)
            {
                ERR_STRM << dex::script::SyntaxError
                << L" [WHERE HEAD] Given type '" << *(whereDef->headType) 
                << L"' does not exist" << std::endl;
                spdata->SetState(dex::script::SyntaxError);
                break;
            }
            hAttr = GRAPH->FindAttribute(aux, *(whereDef->headAttr));
            if (dex::gdb::Attribute::InvalidAttribute == hAttr)
            {
                ERR_STRM << dex::script::SyntaxError
                << L" [WHERE HEAD] Given attribute '" << *(whereDef->headAttr) 
                << L"' does not exist" << std::endl;
                spdata->SetState(dex::script::SyntaxError);
                break;
            }
        }
            
        if ((dex::gdb::uchar_t *)NULL != fieldsDef->terminated)
        {
            wout << L" FIELDS TERMINATED " << *(fieldsDef->terminated);
        }
        if ((dex::gdb::uchar_t *)NULL != fieldsDef->enclosed)
        {
            wout << L" ENCLOSED " << *(fieldsDef->enclosed);
        }
        if (((dex::gdb::bool_t *)NULL != fieldsDef->multiline) && *(fieldsDef->multiline))
        {
            wout << L" ALLOW MULTILINE";
            if ((dex::gdb::int32_t *)NULL != fieldsDef->maxExtralines)
            {
                wout << L" " << *fieldsDef->maxExtralines;
            }
        }
        
        if ((dex::gdb::int64_t *)NULL != f)
        {
            wout << L" FROM " << *f;
        }
        
        if ((dex::gdb::int64_t *)NULL != m)
        {
            wout << L" MAX " << *m;
        }
        
        if ((dex::script::ModeDefinition *)NULL == modeDef)
        {
            wout << L" MODE ROWS";
        }
        else
        {
            if (1 == *(modeDef->phases)) wout << L" MODE ROWS";
            else if (2 == *(modeDef->phases)) wout << L" MODE COLUMNS";
            else if (3 == *(modeDef->phases))
            {
                wout << L" MODE COLUMNS SPLIT";
                if ((dex::gdb::int32_t *)NULL != modeDef->partitions) wout << L" PARTITIONS " << *(modeDef->partitions);
            }
            else DEX_PERMANENT_ASSERT(false); // it does not make sense!
        }


        OUT_STRM << wout.str() << std::endl;
        //
        // Configure reader
        //
        dex::io::CSVReader reader;
        try
        {
            // Separator ----------------------------------------------
            if ((dex::gdb::uchar_t *)NULL != fieldsDef->terminated)
            {
                std::wstring separator(1, *(fieldsDef->terminated));
                reader.SetSeparator(separator);
            }
            // Quotes  ------------------------------------------------
            if ((dex::gdb::uchar_t *)NULL != fieldsDef->enclosed)
            {
                std::wstring quotes(1, *(fieldsDef->enclosed));
                reader.SetQuotes(quotes);
            }
            // Multilines ---------------------------------------------
            dex::gdb::bool_t allowMultilines = ((dex::gdb::bool_t *)NULL != fieldsDef->multiline ? *(fieldsDef->multiline) : false);
            //If allowMultilines is true, fieldsDef->maxExtralines should exist. 
            dex::gdb::int32_t maxMultilines = ((dex::gdb::int32_t *)NULL != fieldsDef->maxExtralines ? *(fieldsDef->maxExtralines) : 0);
            if (allowMultilines)
                reader.SetMultilines(maxMultilines);
            else
                reader.SetSingleLine();
            // StartLine ----------------------------------------------
            if ((dex::gdb::int64_t *)NULL != f)
                reader.SetStartLine(*f);
            // NumLines -----------------------------------------------
            if ((dex::gdb::int64_t *)NULL != m)
                reader.SetNumLines(*m); // 0 means unlimited
            // Locale -------------------------------------------------
            reader.SetLocale(*localeData);
        }
        catch (dex::gdb::Error ex) 
        {
            ERR_STRM << dex::script::ExecutionError << L" Error in file '" 
            << tknFilePath->GetString() << "' settings: "
            << ex.Message().c_str() << std::endl;
            spdata->SetState(dex::script::ExecutionError);
        }
        if (!spdata->IsNoErrorState()) break;  
        
        
        //
        // Configure loader
        //
        dex::gdb::AttributeList aList;
        dex::gdb::Int32List posList;
        dex::gdb::int32_t i = 0;
        for (std::vector<dex::script::ColumnDefinition *>::iterator it = colDefs->begin();
             it != colDefs->end(); it++, i++)
        {
            if (dex::gdb::Attribute::InvalidAttribute != (*it)->attr)
            {
                aList.Add((*it)->attr);
                posList.Add(i);
            }
        }
        
        // Open reader
        try 
        {
            reader.Open(tknFilePath->GetString());
        }
        catch (dex::gdb::IOException ex) 
        {
            ERR_STRM << dex::script::ExecutionError << L" Given file '" 
            << tknFilePath->GetString() << "' cannot be opened" << std::endl;
            spdata->SetState(dex::script::ExecutionError);
        }
        if (!spdata->IsNoErrorState()) break;
        
        // Loader
        dex::io::TypeLoader * loader = (dex::io::TypeLoader *)NULL;
        if (isnode)
        {
            loader = new dex::io::NodeTypeLoader(reader, *(GRAPH), t, aList, posList);
        }
        else
        {
            loader = new dex::io::EdgeTypeLoader(reader, *(GRAPH), t, aList, posList, hPos, tPos, hAttr, tAttr);
        }
        loader->SetLocale(*localeData);
        loader->SetFrequency(SCRIPT_LISTENER_FREQ);
        loader->SetTimestampFormat(spdata->GetTimestampFormat());
        
        delete localeData;
        
        std::wstringstream errorFile;
        errorFile << tknTypeName->GetString() << L"-errors.log";
        loader->SetLogError(errorFile.str());
        dex::script::ScriptTypeLoaderListener * listener = (dex::script::ScriptTypeLoaderListener *)NULL;
        if ((std::wostream *)NULL != spdata->GetOutputStream()) 
        {
            listener = new dex::script::ScriptTypeLoaderListener(*(spdata->GetOutputStream()));
            loader->Register(*listener);
        }
        
        try 
        {
            if ((dex::script::ModeDefinition *)NULL == modeDef)
            {
                loader->Run();
            }
            else
            {
                if (1 == *(modeDef->phases))
                {
                    loader->Run();
                }
                else if (2 == *(modeDef->phases))
                {
                    loader->RunTwoPhases();
                }
                else if (3 == *(modeDef->phases))
                {
                    dex::gdb::int32_t parts = ((dex::gdb::int32_t*)NULL == modeDef->partitions ? 1 : *(modeDef->partitions));
                    loader->RunNPhases(parts);
                }
                else 
                {
                    DEX_PERMANENT_ASSERT(false); // it does not make sense!
                }
            }
        }
        catch (dex::gdb::Exception ex) 
        {
            std::wstring wstr(ex.Message().begin(), ex.Message().end());
            ERR_STRM << dex::script::ExecutionError << L" Loading failed because of: " << wstr << std::endl;
            spdata->SetState(dex::script::ExecutionError);
        }
        if ((dex::script::ScriptTypeLoaderListener *)NULL != listener) delete listener;
        reader.Close();
            
        break;
    }
}


nodesORedges(b) ::= NODES.
{
    b = true;
}


nodesORedges(b) ::= EDGES.
{
    b = false;
}


localeORnolocale(localeData) ::= LOCALE STRING(tknLocaleName).
{
    localeData = (std::wstring*) new std::wstring(tknLocaleName->GetString());
}

localeORnolocale(localeData) ::= .
{
    localeData = (std::wstring*) new std::wstring();
}


columndef_list(colDefs) ::= columndef_list(colDefs2) COMMA columndef(coldef).
{
    colDefs = colDefs2;
    colDefs->push_back(coldef);
}


columndef_list(colDefs) ::= .
{
    colDefs = new std::vector<dex::script::ColumnDefinition *>();
}



columndef_list(colDefs) ::= columndef(coldef).
{
    colDefs = new std::vector<dex::script::ColumnDefinition *>();
    colDefs->push_back(coldef);
}


columndef(coldef) ::= ASTERISK(tknAsterisk).
{
    coldef = new dex::script::ColumnDefinition();
    coldef->name->assign(tknAsterisk->GetString());
    coldef->alias->assign(tknAsterisk->GetString());
}


columndef(coldef) ::= STRING(tknName).
{
    coldef = new dex::script::ColumnDefinition();
    coldef->name->assign(tknName->GetString());
    coldef->alias->assign(tknName->GetString());
}


columndef(coldef) ::= STRING(tknName) STRING(tknAlias).
{
    coldef = new dex::script::ColumnDefinition();
    coldef->name->assign(tknName->GetString());
    coldef->alias->assign(tknAlias->GetString());
}


ignore(cols) ::= IGNORE ignorecols_list(cols2).
{
    cols = cols2;
}


ignore(cols) ::= .
{
    cols = new std::vector<std::wstring *>();
}


ignorecols_list(cols) ::= ignorecols_list(cols2) COMMA STRING(tknColName).
{
    cols = cols2;
    cols->push_back(new std::wstring(tknColName->GetString()));
}


ignorecols_list(cols) ::= STRING(tknColName).
{
    cols = new std::vector<std::wstring *>();
    cols->push_back(new std::wstring(tknColName->GetString()));
}


where(whereDef) ::= .
{
    whereDef = (dex::script::WhereDefinition *)NULL;
}


where(whereDef) ::= WHERE 
                    TAIL STRING(tknTailName) EQUAL STRING(tknTailType) DOT STRING(tknTailAttr)
                    HEAD STRING(tknHeadName) EQUAL STRING(tknHeadType) DOT STRING(tknHeadAttr).
{
    whereDef = new dex::script::WhereDefinition();
    whereDef->tailName = new std::wstring(tknTailName->GetString());
    whereDef->tailType = new std::wstring(tknTailType->GetString());
    whereDef->tailAttr = new std::wstring(tknTailAttr->GetString());
    whereDef->headName = new std::wstring(tknHeadName->GetString());
    whereDef->headType = new std::wstring(tknHeadType->GetString());
    whereDef->headAttr = new std::wstring(tknHeadAttr->GetString());
}


fields(fieldsDef) ::= FIELDS TERMINATED STRING(tknTerminated) enclosed(fieldsDef2) multiline(extraMultilines).
{
    fieldsDef = fieldsDef2;            
    if (1 == tknTerminated->GetString().size())
    {
        fieldsDef->terminated = new dex::gdb::uchar_t;
        *(fieldsDef->terminated) = tknTerminated->GetString().at(0);
    }
    else if (0 == tknTerminated->GetString().compare(L"\\t")) // A two cahr string representing one tab
    {
        fieldsDef->terminated = new dex::gdb::uchar_t;
        *(fieldsDef->terminated) = L'\t';
    } 
    else // TODO [sgomez] Is this really required?
    {
        ERR_STRM << dex::script::SyntaxError 
        << L" Terminated character must be quoted and 1-char length" << std::endl;
        spdata->SetState(dex::script::SyntaxError);
    }
    if (extraMultilines >= 0)
    {
        fieldsDef->multiline = new dex::gdb::bool_t;
        *(fieldsDef->multiline) = true;
        fieldsDef->maxExtralines = new dex::gdb::int32_t;
        *fieldsDef->maxExtralines = extraMultilines;
    }
}


fields(fieldsDef) ::= .
{
    fieldsDef = new dex::script::FieldsDefinition();
}

enclosed(fieldsDef) ::= ENCLOSED STRING(tknEnclosed) .
{
    fieldsDef = new dex::script::FieldsDefinition();
    if (1 == tknEnclosed->GetString().size())
    {
        fieldsDef->enclosed = new dex::gdb::uchar_t;
        *(fieldsDef->enclosed) = tknEnclosed->GetString().at(0);
    }
    else if (0 == tknEnclosed->GetString().compare(L"\\t")) // A two cahr string representing one tab
    {
        fieldsDef->enclosed = new dex::gdb::uchar_t;
        *(fieldsDef->enclosed) = L'\t';
    }
    else // TODO [sgomez] Is this really required?
    {
        ERR_STRM << dex::script::SyntaxError
        << L" Enclosed character must be quoted and 1-char length" << std::endl;
        spdata->SetState(dex::script::SyntaxError);
    }
}

enclosed(fieldsDef) ::= .
{
    fieldsDef = new dex::script::FieldsDefinition();
}


multiline(extraMultilines) ::= ALLOW MULTILINE NUMBER(tknMaxExtraLines).
{
    extraMultilines = (dex::gdb::int32_t)tknMaxExtraLines->GetNumber();
}

multiline(extraMultilines) ::= ALLOW MULTILINE.
{
    // The DEFAULT number of extra lines.
    extraMultilines = (dex::gdb::int32_t) 100;
}

multiline(extraMultilines) ::= .
{
    extraMultilines = (dex::gdb::int32_t)-1;
}

from(f) ::= .
{
    f = (dex::gdb::int64_t *)NULL;
}


from(f) ::= FROM NUMBER(tknFrom).
{
    f = new dex::gdb::int64_t;
    *f = tknFrom->GetNumber();
}


max(m) ::= .
{
    m = (dex::gdb::int64_t *)NULL;
}


max(m) ::= MAX NUMBER(tknMax).
{
    m = new dex::gdb::int64_t;
    *m = tknMax->GetNumber();
}


mode(modeDef) ::= .
{
    modeDef = (dex::script::ModeDefinition *)NULL;
}


mode(modeDef) ::= MODE ROWS.
{
    modeDef = new dex::script::ModeDefinition();
    modeDef->phases = new dex::gdb::int32_t;
    *(modeDef->phases) = (dex::gdb::int32_t)1;
}


mode(modeDef) ::= MODE COLUMNS.
{
    modeDef = new dex::script::ModeDefinition();
    modeDef->phases = new dex::gdb::int32_t;
    *(modeDef->phases) = (dex::gdb::int32_t)2;
}


mode(modeDef) ::= MODE COLUMNS SPLIT.
{
    modeDef = new dex::script::ModeDefinition();
    modeDef->phases = new dex::gdb::int32_t;
    *(modeDef->phases) = (dex::gdb::int32_t)3;
}


mode(modeDef) ::= MODE COLUMNS SPLIT PARTITIONS NUMBER(tknPartitions).
{
    modeDef = new dex::script::ModeDefinition();
    modeDef->phases = new dex::gdb::int32_t;
    *(modeDef->phases) = (dex::gdb::int32_t)3;
    modeDef->partitions = new dex::gdb::int32_t;
    *(modeDef->partitions) = (dex::gdb::int32_t)tknPartitions->GetNumber();
}



command ::= SET TIMESTAMP FORMAT STRING(tknTimestampFormat).
{
    while (spdata->Execute()) // we'll break the while at the end or in case of error
    {
        if (!spdata->StorageIsOpened())
        {
            ERR_STRM << dex::script::SyntaxError 
            << L" OPEN or CREATE is required" << std::endl;
            spdata->SetState(dex::script::SyntaxError);
            break;
        }
        
        try 
        {
            // Keep the Timestamp format to use it in all the loads
            spdata->SetTimestampFormat(tknTimestampFormat->GetString());
        }
        catch (dex::gdb::Error err) 
        {
            std::wstring wstr(err.Message().begin(), err.Message().end());
            ERR_STRM << dex::script::ExecutionError 
            << L" " << wstr << std::endl;
            spdata->SetState(dex::script::ExecutionError);
        }
        if (spdata->IsNoErrorState())
        {
            OUT_STRM << dex::script::Parsing 
            << L" SET TIMESTAMP FORMAT '" << tknTimestampFormat->GetString() << L"'" << std::endl;
        }
        
        break;
    }
}
