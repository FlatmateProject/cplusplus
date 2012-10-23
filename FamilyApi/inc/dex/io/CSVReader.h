/*
 * DEX: a library for out-of-core graph-based data storage and query processing.
 *
 * See the file LICENSE.txt for redistribution information.
 *
 * Copyright (c) 2005-2010 Sparsity Technologies, Barcelona (Spain)
 */

#ifndef _DEX_CSVREADER_H_
#define _DEX_CSVREADER_H_

/**
 * \addtogroup io
 * @{
 *
 * \file CSVReader.h
 * \brief It contains the definition of CSVReader class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */

#include "io/RowReader.h"

/*
 * Forward declaration of dexcore classes.
 */
namespace dexcore
{
    class FileReader;
    class String;
    class Locale;
}


BEGIN_DEX_IO_NAMESPACE


/**
 * \brief CSVReader interface.
 *
 * A very simple CSV reader.
 *
 * It works as any other RowReader, but open must be called once before
 * the first read operation.
 *
 * Using the format <a href="http://tools.ietf.org/html/rfc4180">RFC 4180</a>.<br>
 * Except: leading and trailing spaces, adjacent to CSV separator character,
 * are trimmed.<br>
 *
 * You can use your own separators and quote characters.
 * By default the separator is the comma (,) and the quote character
 * is the double quotes (").
 *
 * Fields with multiple lines can be allowed (and the maximum lines specified),
 * but the default is a single line.
 *
 * The locale string can be used to set the language, country and the file
 * encoding. The format must be "[language_territory][.codeset]". But
 * only the file encoding is being used in the current version.
 *
 * The languages supported are: "en_US", "es_ES" and "ca_ES". <br>
 * The file encodings supported are: "utf8" and "iso88591".
 *
 * For example:
 *
 * To don't change the default locales, use an empty string: "".<br>
 * To read a file in utf8 with the default language settings use ".utf8".<br>
 * To read a file in iso88591 with English language use: "en_US.iso88591".<br>
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT CSVReader
: public RowReader
{
private:
    
    /**
     * Output file path.
     */
    std::wstring file;
    /**
     * \brief The locale of the file.
     */
    dexcore::Locale* locale;
    /**
     * Character used to separate the values.
     */
    dex::gdb::uchar_t separator;
    /**
     * Character used to quote strings.
     */
    dex::gdb::uchar_t quote;    
    /**
     * Allow multiline fields or not.
     */
    dex::gdb::bool_t allowMultiline;
    /**
     * Maximum number of extra lines for each multiline column.
     */
    dex::gdb::int32_t maxExtraLines;
    /**
     * Number of lines from the beginning to be ignored.
     */
    dex::gdb::int32_t skipLines;
    /**
     * Number of lines to be read (0 == unlimited).
     */
    dex::gdb::int32_t maxLines;
    /**
     * Input stream.
     */
    dexcore::FileReader *fileReader;

    /**
     * Buffer to store the current line.
     */
    dexcore::String *tempString;


    /**
     * Default field separator character.
     */
    static const dex::gdb::uchar_t defaultSeparator;
    /**
     * Default field quote character.
     */
    static const dex::gdb::uchar_t defaultQuote;

public:

    /**
     * Constructs CSVReader.
     *
     */
    CSVReader();

    /**
     * Sets the character used to separate fields in the file.
     * \param sep [in] Separator character.
     */
    void SetSeparator(const std::wstring & sep)
    throw (dex::gdb::Error);

    /**
     * Sets the character used to quote fields.
     * \param quotes [in] Quote character.
     */
    void SetQuotes(const std::wstring & quotes)
    throw (dex::gdb::Error);

    /**
     * \brief Allows the use of fields with more than one line.
     * \param numExtralines [in] Maximum number of extra lines
     *        for each column (0==unlimited, N==N+1 total rows).
     */
    void SetMultilines(dex::gdb::int32_t numExtralines);

    /**
     * \brief Only allows single line fields.
     */
    void SetSingleLine();

    /**
     * \brief Sets the number of lines to be skiped from the beginning.
     * \param startLine [in] The line number to skip for start reading
     */
    void SetStartLine(dex::gdb::int32_t startLine);

    /**
     * \brief Used to limit the number of lines that will be read.
     * \param numLines [in] The maximum number of lines to read (0 == unlimited)
     */
    void SetNumLines(dex::gdb::int32_t numLines);

    /**
     * \brief Sets the locale that will be used to read the file.
     * \param localeStr [in] The locale string for the file encoding.
     */
    void SetLocale(const std::wstring &localeStr);

    /**
     * \brief Opens the source file path.
     * \param f [in] CSV file path.
     * \exception IOException If bad things happen opening the file.
     */
    void Open(const std::wstring & f)
    throw(dex::gdb::IOException);

    /**
     * \brief Moves the reader to the beginning.
     * <p>
     * Restarts the reader.
     * @return <code>true</code> if the reader can be restarted,
     * <code>false</code> otherwise.
     * \exception IOException If bad things happen during the restart.
     */
    dex::gdb::bool_t Reset()
    throw(dex::gdb::IOException);
    
    /**
     * \brief Reads the next row as a string array.
     * \param row [out] A string list with each comma-separated element as
     * a separate entry.
     * \return Returns true if a row had been read or false otherwise.
     * \exception IOException If bad things happen during the read.
     */
    dex::gdb::bool_t Read(dex::gdb::StringList &row)
    throw(dex::gdb::IOException);

    /**
     * \brief The row number for the current row.
     * @return The current row number; 0 if there is no current row.
     * \exception IOException If it fails.
     */
    dex::gdb::int32_t GetRow()
    throw(dex::gdb::IOException);
    
    /**
     * \brief Closes the reader.
     * \exception IOException If the close fails.
     */
    void Close()
    throw(dex::gdb::IOException);
    
    /**
     * \brief Destructor.
     */
    virtual ~CSVReader();

private:

    /**
     * \brief Non implemented to avoid use of copy constructor.
     */
    CSVReader(const CSVReader & csvr);

    /**
     * \brief Non implemented to avoid use of assignment operator.
     */
    CSVReader & operator =(const CSVReader & csvr);

    /**
     * \brief The number of lines that had been readed.
     */
    dex::gdb::int32_t linesReaded;

    /**
     * \brief False if the header lines have to be skiped.
     */
    dex::gdb::bool_t linesSkiped;

    /**
     * Reads the next line from the file.
     * @return The next line from the file without trailing newline
     * @throws IOException If bad things happen during the read
     */
    dex::gdb::bool_t GetNextLine(std::wstring &nextLine)
    throw(dex::gdb::IOException);

    /**
     * Reads the a single line from the file. This does not skip header lines.
     * @return The next line from the file without trailing newline
     * @throws IOException If bad things happen during the read
     */
    dex::gdb::bool_t ReadALine(std::wstring &nextLine)
    throw(dex::gdb::IOException);

    /**
     * Parses an incoming String and returns an array of elements.
     * @param nextLine The string to parse
     * @return The comma-tokenized list of elements, or null if nextLine is null
     * @throws IOException If bad things happen during the read
     */
    dex::gdb::bool_t ParseLine(std::wstring &nextLine, dex::gdb::StringList &row);
    /**
     * Trim the leading and trailing spaces adjacent to CSV separator character
     * and remove quoted field characters.
     * @param sb The string field on CSV format
     * @return The cleaned string.
     */
    std::wstring CleanExpression(std::wstring sb);
};


END_DEX_GDB_NAMESPACE

/** @} */
#endif // _DEX_CSVREADER_H_
