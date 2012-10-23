/*
 * DEX: a library for out-of-core graph-based data storage and query processing.
 *
 * See the file LICENSE.txt for redistribution information.
 *
 * Copyright (c) 2005-2010 Sparsity Technologies, Barcelona (Spain)
 */


#ifndef _DEX_SCRIPTPARSER_H_
#define _DEX_SCRIPTPARSER_H_


/**
 * \addtogroup script
 * @{
 *
 * \file ScriptParser.h
 * \brief It contains the declaration of ScriptParser class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */


#include "gdb/common.h"
#include "gdb/Exception.h"


/*
 * Forward declaration of dexcore classes.
 */
namespace dexcore
{
    class FileReader;
}

/*
 * Forward declaration.
 */
namespace dex 
{
    namespace gdb 
    {
        class Dex;
        class Database;
        class Session;
        class Graph;
    }
}


BEGIN_DEX_SCRIPT_NAMESPACE


/**
 * \brief ScriptParser state.
 */
enum ScriptParserState
{
    /**
     * \brief Initial and 'there is no problem for the moment' state.
     */
    Parsing,
    /**
     * \brief Finished successfully state.
     */
    Parsed,
    /**
     * \brief Error case.
     */
    SyntaxError,
    /**
     * \brief Error case.
     */
    ParserFailure,
    /**
     * \brief Error case.
     */
    StackOverflow,
    /**
     * \brief Error case.
     */
    TokenizerError,
    /**
     * \brief Error case when executing generated code.
     */
    ExecutionError
};


/**
 * \brief Easy STL printing operator redefinition.
 *
 * It allows to do: ... << dex::script::SyntaxError << ...
 */
std::wostream & operator << (std::wostream & wostrm, 
                             const enum ScriptParserState & state);


/**
 * \brief Extra data to be shared between the Parser (Parser.cpp) 
 * and the ScriptParser.
 */
class ScriptParserData
{
private:
    
    /**
     * \brief Dex instance.
     */
    dex::gdb::Dex * dex;
    /**
     * \brief Database instance.
     */
    dex::gdb::Database * db;
    /**
     * \brief Session instance.
     */
    dex::gdb::Session * sess;
    /**
     * \brief Graph instance.
     */
    dex::gdb::Graph * graph;
    /**
     * \brief State.
     */
    ScriptParserState state;
    /**
     * \brief Output stream.
     */
    std::wostream * ostrm;
    /**
     * \brief Error output stream.
     */
    std::wostream * estrm;
    /**
     * \brief If TRUE parsed script will be executed,
     * if FALSE it will be just parsed.
     */
    dex::gdb::bool_t exec;
    

    /**
     * \brief The current timestamp format
     */
    std::wstring timestampFormat;

    /**
     * \brief Non implemented to avoid use of copy constructor.
     */
    ScriptParserData(const ScriptParserData &spd);
    
    /**
     * \brief Non implemented to avoid use of assignment operator.
     */
    ScriptParserData & operator =(const ScriptParserData &spd);

public:
    
    /**
     * \brief Creates a new instance.
     *
     * Initial state: Parsing.
     * \param execute [in] If TRUE parsed script will be executed,
     * if FALSE it will be just parsed.
     * \param outStrm [in] Output stream. If NULL no output is done.
     * \param errStrm [in] Error output stream. If NULL no error output is done.
     */
    ScriptParserData(dex::gdb::bool_t execute,
                     std::wostream * outStrm, std::wostream * errStrm);
    
    /**
     * \brief Destructor.
     *
     * Closes the storage if it is opened.
     */
    virtual ~ScriptParserData();
    
    /**
     * \brief Gets if the parsed script must be executed or not.
     * \return If TRUE parsed script will be executed,
     * if FALSE it will be just parsed.
     */
    dex::gdb::bool_t Execute() const
    {
        return exec;
    }
    
    /**
     * \brief Gets the output stream.
     * \return A reference to the output stream.
     */
    std::wostream * GetOutputStream()
    {
        return ostrm;
    }
    
    /**
     * \brief Gets the error output stream.
     * \return A reference to the output stream.
     */
    std::wostream * GetErrorStream()
    {
        return estrm;
    }
    
    /**
     * \brief Opens the storage.
     *
     * It initializes Dex, Database, Session and Graph instances.
     *
     * \param path [in] Path of the Dex database file.
     * \param alias [in] Alias of the Dex database.
     * \param create [in] If TRUE, it opens an existing Dex database, 
     * if FALSE it creates a new one one. Also, when opening it validates 
     * the given alias matches the real one.
     * \return TRUE if ok, FALSE in case of ERROR.
     */
    dex::gdb::bool_t OpenStorage(const std::wstring & path, 
                                 const std::wstring & alias, 
                                 dex::gdb::bool_t open);
    
    /**
     * \brief Closes the storage.
     *
     * It closes Dex, Database, Session and Graph instances.
     */
    dex::gdb::bool_t CloseStorage();

    /**
     * \brief Gets if the storage is opened.
     * \return TRUE if the storage is opened, FALSE otherwise.
     */
    dex::gdb::bool_t StorageIsOpened();
    
    /**
     * \brief Gets the Graph.
     * \pre Storage must be opened.
     */
    dex::gdb::Graph * GetGraph()
    {
        return graph;
    }
    
    /**
     * \brief Gets the state of the parser.
     * \return The state of the parser.
     */
    ScriptParserState GetState() const
    {
        return state;
    }
    
    /**
     * \brief Sets the state of the parser.
     * \param st [in] The state of the parser.
     */
    void SetState(ScriptParserState st)
    {
        state = st;
    }
    
    /**
     * \brief Gets if the current state is not an error state.
     *
     * See ScriptParserState.
     *
     * Error states: all but Parsing and Parsed
     */
    dex::gdb::bool_t IsNoErrorState() const
    {
        return ((Parsing == state) 
                || (Parsed == state));
    }


    /**
     * \brief Gets the current timestamp format.
     * \return The current timestamp format string.
     */
    const std::wstring & GetTimestampFormat() const
    {
        return timestampFormat;
    }

    /**
     * \brief Sets the current timestamp format.
     * \param format [in] The new timestamp format string
     */
    void SetTimestampFormat(const std::wstring &format)
    {
        timestampFormat = format;
    }
};


/**
 * \brief ScriptParser.
 *
 * The ScriptParser can create schemas and load data from a set of commands
 * in a dex script.
 *
 * A DEX script contains an ordered list of commands. ScriptParser will execute 
 * each one of them in order. Commands will create schemas, 
 * define nodes and edges, and load data into a previous defined DEX schema. 
 *
 * There are six main commands: (i) database creation 'create dbgraph': creates 
 * a new empty schema into a DEX database, and sets this database to 
 * perform the following operatinos; (ii) database usage 'use dbgraph': opens 
 * an existing DEX database and sets this database to perform the following 
 * operations; (iii) node type creation 'create node': creates a node type 
 * into the database; (iv) edge type creation 'create edge': creates an edge 
 * type into the database; (v) node data load 'load nodes': loads an CSV file 
 * into the database; (vi) edge data load 'load edges': loads an CSV file into 
 * the database.
 *
 * -- Schema definition --
 * 
 * This creates a DEX graph database:
 *
 * CREATE (GDB|DBGRAPH) alias INTO filename 
 * 
 * where alias is the name of the graph database to be created and filename
 * corresponds to the path to store the dex database.
 *
 * Instead of creating a new database, you can set an existing one as the
 * operation database of the script:
 *
 * USE (GDB|DBGRAPH) alias INTO filename
 *
 * All following commands will be performed on the last created or used graph
 * database.
 *
 * This creates a node type:
 *
 * CREATE NODE node_type_name "("
 * [attribute_name (INTEGER|LONG|DOUBLE|STRING|BOOLEAN|TIMESTAMP|TEXT) 
 * [INDEXED|UNIQUE|BASIC], ...] ")"
 * 
 * and this an edge type:
 *
 * CREATE [UNDIRECTED] EDGE edge_type_name 
 * [FROM node_type_name TO node_type_name]
 * "(" [attribute_name (INT|LONG|DOUBLE|STRING|BOOLEAN|TIMESTAMP|TEXT), ...]
 * ") [MATERIALIZE NEIGHBORS]"
 *
 * Here there are some examples:
 * 
 * create gdb EXAMPLE into 'ex.dex'
 *
 * use gdb WIKIPEDIA into 'wikipedia.dex'
 *
 * create node TITLES (ID int unique, 'TEXT' string, NLC string, TITLE string indexed)
 *
 * create node IMAGES (ID int unique, NLC string, FILENAME string indexed)
 *
 * create edge REFS (NLC string, "TEXT" string, TYPE string)
 *
 * create edge IMGS
 *
 * create dbgraph FAMILY into 'family.dex'
 *
 * create node PERSON (NAME string indexed, ID int unique, YEAR int)
 *
 * create node DOG (NAME string indexed, YEAR int)
 *
 * create edge CHILD from PERSON to PERSON (YEAR int)
 *
 * create undirected edge MARRIED from PERSON to PERSON (YEAR int) materialize neighbors
 *
 * create edge PET from PERSON to DOG () materialize neighbors
 *
 * create gdb CARMODEL into 'cars.dex'
 *
 * create node PERSON (NAME string, ID int unique, YEAR int)
 *
 * create node CAR (MODEL string, ID int, OWNER int indexed)
 *
 * Note you may quote name identifiers in order to be able to use reserved words.
 *
 * -- Data node load --
 *
 * Load nodes command creates nodes and sets attributes values for nodes imported 
 * from a CSV. For each CSV row a new node is created.
 *
 * This is the command:
 *
 * LOAD NODES file_name [LOCALE locale_name] COLUMNS attribute_name [alias_name], ...
 * INTO node_type_name [IGNORE (attribute_name|alias_name), ....]
 * [FIELDS [TERMINATED char] [ENCLOSED char] [ALLOW_MULTILINE [maxExtralines]]]
 * [FROM num] [MAX num] [MODE (ROWS|COLUMNS [SPLIT [PARTITIONS num]])]
 * 
 * Here there are some examples:
 *
 * load nodes 'titles.csv' columns ID, 'TEXT', NLC, TITLE into TITLES
 *
 * load nodes 'images.csv' columns ID, NLC, FILENAME into IMAGES from 2 max 10000 mode columns
 *
 * load nodes 'people.csv' locale 'en_US.utf8' columns *, DNI, NAME, AGE, *, ADDRESS into PEOPLE mode row
 * 
 * -- Data edge load --
 *
 * Load edges command creates edges between existing nodes and sets attributes 
 * values for those edges imported from a CSV. For each CSV row a new edge is 
 * created.
 *
 * LOAD EDGES file_name [LOCALE locale_name] COLUMNS attribute_name [alias_name], ...
 * INTO node_type_name [IGNORE (attribute_name|alias_name), ....]
 * WHERE TAIL (attribute_name|alias_name) = node_type_name.attribute_name
 * HEAD (attribute_name|alias_name) = node_type_name.attribute_name
 * [FIELDS [TERMINATED char] [ENCLOSED char] [ALLOW_MULTILINE [maxExtralines]]]
 * [FROM num] [MAX num] [MODE (ROWS|COLUMNS [SPLIT [PARTITIONS num]])]
 * 
 * Tail node is defined by tail property, it looks for the node where attribute
 * value is the same than the node of an specific name with the same value at 
 * specific attribute name. In the same way, head node is defined by head 
 * property.
 *
 * Here there are some examples:
 *
 * load edges 'references.csv' columns NLC, 'TEXT', TYPE, FROM F, TO T
 * into REFS ignore F, T where tail F = TITLES.ID head T = TITLES.ID
 * mode columns split partitions 3
 *
 * load edges 'imagesReferences.csv' locale 'es_ES.iso88591' columns From, To into IMGS
 * ignore From, To where tail From = TITLES.ID HEAD To = IMAGES.ID
 * mode rows
 *
 * load edges 'calls.gz' columns From, To, Time, Long into CALLS
 * ignore From, To where tail From = PEOPLE.DNI head To = PEOPLE.DNI
 * mode columns
 * 
 *
 * -- Schema update --
 *
 * Schema update commands allows for updating the schema of a graph database. 
 * Nowadays it is possible to remove node or edge types or attributes. 
 * The node attribute indexing can also be modified.
 *
 * DROP (NODE|EDGE|ATTRIBUTE) name
 *
 * INDEX node_type_name.attribute_name [INDEXED|UNIQUE|BASIC]
 *
 * Examples:
 *
 * drop edge REFS
 * 
 * drop node 'TITLES'
 *
 * drop attribute PEOPLE.DNI
 *
 * index PEOPLE.NAME indexed
 *
 * index CAR.ID unique
 *
 * -- Timestamp Format --
 *
 * The timestamp format can be set with the command:
 *
 * SET TIMESTAMP FORMAT timestamp_format_string
 *
 * After this command, all timestamps data are loaded with the
 * format specified.
 *
 * Valid format fields:
 *
 *     yyyy -> Year
 *
 *     yy -> Year without century
 *
 *     MM -> Month [1..12]
 *
 *     dd -> Day of month [1..31]
 *
 *     hh -> Hour [0..23]
 *
 *     mm -> Minute [0..59]
 *
 *     ss -> Second [0..59]
 *
 *     SSS -> Millisecond [0..999]
 *
 * Default formats accepted when this command is not present:
 *
 *     "yyyy-MM-dd hh:mm:ss.SSS"
 *
 *     "yyyy-MM-dd hh:mm:ss"
 *
 *     "yyyy-MM-dd"
 *
 * -------
 */
class DEX_EXPORT ScriptParser
{
private:
    
    /**
     * \brief Output stream.
     *
     * If NULL, standard output will be used.
     */
    std::wostream * outStrm;
    
    /**
     * \brief Error stream.
     *
     * If NULL, standard error output will be used.
     */
    std::wostream * errStrm;
    
    /**
     * \brief Non implemented to avoid use of copy constructor.
     */
    ScriptParser(const ScriptParser &sp);
    
    /**
     * \brief Non implemented to avoid use of assignment operator.
     */
    ScriptParser & operator =(const ScriptParser &sp);

public:

    /**
     * \brief Constructor.
     */
    ScriptParser();
    
    /**
     * \brief Destructor.
     */
    virtual ~ScriptParser();
    
    /**
     * \brief Sets the output log.
     *
     * If not set, output log corresponds to standard output.
     * \param path [in] Path of the output log.
     * \exception IOException If bad things happen opening the file.
     */
    void SetOutputLog(const std::wstring & path)
    throw(dex::gdb::IOException);
    
    /**
     * \brief Sets the error log.
     *
     * If not set, error log corresponds to standard error output.
     * \param path [in] Path of the error log.
     * \exception IOException If bad things happen opening the file.
     */
    void SetErrorLog(const std::wstring & path)
    throw(dex::gdb::IOException);

    /**
     * \brief Parser the given input stream.
     * \param fileReader [in] Input file reader.
     * \param execute [in] If TRUE the script is executed, if FALSE
     * it is just parsed.
     * \return TRUE if ok, FALSE in case of error.
     */
    dex::gdb::bool_t Parse(dexcore::FileReader *fileReader, dex::gdb::bool_t execute);
    
    /**
     * \brief Parses the given input file.
     * \param path [in] Input file path.
     * \param execute [in] If TRUE the script is executed, if FALSE
     * it is just parsed.
     * \param localeStr [in] The locale string for reading the input file. See CSVReader.
     * \return TRUE if ok, FALSE in case of error.
     * \exception IOException If bad things happen opening the file.
     */
    dex::gdb::bool_t Parse(const std::wstring & path, dex::gdb::bool_t execute, const std::wstring &localeStr)
    throw(dex::gdb::IOException);

    /**
     * \brief Writes an script with the schema definition for the given database.
     * \param path [in] Filename of the script to be writen.
     * \param db [in] Database.
     * \exception IOException If bad things happen opening or writing the file.
     */
    static void GenerateSchemaScript(const std::wstring & path, dex::gdb::Database & db)
    throw(dex::gdb::IOException);
};


END_DEX_SCRIPT_NAMESPACE


/** @} */
#endif // _DEX_SCRIPTPARSER_H_
