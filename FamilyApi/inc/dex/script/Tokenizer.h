/*
 * DEX: a library for out-of-core graph-based data storage and query processing.
 *
 * See the file LICENSE.txt for redistribution information.
 *
 * Copyright (c) 2005-2010 Sparsity Technologies, Barcelona (Spain)
 */


#ifndef _DEX_TOKENIZER_H_
#define _DEX_TOKENIZER_H_


/**
 * \addtogroup script
 * @{
 *
 * \file Tokenizer.h
 * \brief It contains the declaration of Tokenizer class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */


#include "gdb/common.h"

/*
 * Forward declaration of dexcore classes.
 */
namespace dexcore
{
    class FileReader;
    class String;
}


BEGIN_DEX_SCRIPT_NAMESPACE


/*
 * Forward declaration.
 */
class Tokenizer;


/**
 * \brief Token class.
 *
 * Stores information of a token such as its value and its position
 * (line and column) into the stream.
 *
 * For those token corresponding to an SCRIPT_PARSER_TK_STRING, the
 * value is stored as an unicode string, thus GetString should be used.
 * For those token corresponding to a SCRIPT_PARSER_TK_NUMBER, the
 * value is stored as a 64-bit integer number, thus GetNumber should be used.
 */
class Token
{
    friend class Tokenizer; // to set private fields
    
private:
    
    /**
     * \brief Unicode string value.
     */
    std::wstring str;
    /**
     * \brief 64-bit integer number.
     */
    dex::gdb::int64_t num;
    /**
     * \brief Line number in range [0..L).
     */
    dex::gdb::int32_t line;
    /**
     * \brief Column number in range [0..C).
     */
    dex::gdb::int32_t column;
    
    /**
     * \brief Non implemented to avoid use of copy constructor.
     */
    Token(const Token &tkn);
    
    /**
     * \brief Non implemented to avoid use of assignment operator.
     */
    Token & operator =(const Token &tkn);
    
public:
    
    /**
     * \brief Creates a new instance.
     *
     * Empty string and all values to 0.
     */
    Token();
    
    /**
     * \brief Gets the value.
     * \return The unicode string value.
     */
    const std::wstring & GetString() const
    {
        return str;
    }
    
    /**
     * \brief Gets the number.
     * \return The 64-bit integer number.
     */
    const dex::gdb::int64_t GetNumber() const
    {
        return num;
    }
    
    /**
     * \brief Gets the line.
     * \return The line.
     */
    dex::gdb::int32_t GetLine() const
    {
        return line;
    }    
    
    /**
     * \brief Gets the column.
     * \return The column.
     */
    dex::gdb::int32_t GetColumn() const
    {
        return column;
    }    
};


/**
 * \brief Tokenizer class.
 *
 * Reads from a stream until there are no more tokens.
 *
 * See Next method.
 */
class Tokenizer
{
private:
    
    /**
     * \brief Input file Reader.
     */
    dexcore::FileReader *fileReader;
    /**
     * \brief Current line in range [0..L).
     */
    dex::gdb::int32_t line;
    /**
     * \brief Current column in range [0..C).
     */
    dex::gdb::int32_t column;

    /**
     * \brief Non implemented to avoid use of copy constructor.
     */
    Tokenizer(const Token &tknzr);
    
    /**
     * \brief Non implemented to avoid use of assignment operator.
     */
    Tokenizer & operator =(const Tokenizer &tknzr);
    
public:
    
    /**
     * \brief Indicates tokenize process failed.
     *
     * It can be returned by Next method.
     */
    static const dex::gdb::int32_t SCRIPT_PARSER_TK_ERROR;
    /**
     * \brief Indicates tokenize process ended.
     *
     * It can be returned by Next method.
     */
    static const dex::gdb::int32_t SCRIPT_PARSER_TK_END;
    
    /**
     * \brief Creates a new instance.
     * \param strm [in] Input unicode stream. It must be opened and ready
     * to be read.
     */
    Tokenizer(dexcore::FileReader *fr);
    
    /**
     * \brief Destructor.
     */
    virtual ~Tokenizer()
    {
    }
    
    /**
     * \brief Gets the next token.
     * \param token [out] Updates the given token.
     * \return It returns a token identifier (see Parser.h) or
     * SCRIPT_PARSER_TK_ERROR when the tokenize process fails
     * or SCRIPT_PARSER_TK_END when the tokenize process ends.
     */
    dex::gdb::int32_t Next(Token & token);

private:

    /**
     * \brief Gets the next char from the stream and 
     * updates line and column values.
     */
    dex::gdb::uchar_t ReadChar();
    
    /**
     * \brief Unicode character for 'end of stream'.
     */
    static const dex::gdb::uchar_t UCHAR_ENDSTRM;
    /**
     * \brief Unicode character for 'end of line'.
     */
    static const dex::gdb::uchar_t UCHAR_ENDL;
    /**
     * \brief Unicode character for '.
     */
    static const dex::gdb::uchar_t UCHAR_STR;
    /**
     * \brief Unicode character for ".
     */
    static const dex::gdb::uchar_t UCHAR_QUOTED;
    /**
     * \brief Unicode character for 'starting a comment line'.
     */
    static const dex::gdb::uchar_t UCHAR_COMMENT;

    /**
     * \brief Gets if the given character is a separator o not.
     *
     * Separators: whitespace, tab, '\r' and '\n'.
     * \param ch [in] Unicode character.
     * \param TRUE if the given character is a separator, FALSE otherwise.
     */
    dex::gdb::bool_t IsSeparator(dex::gdb::uchar_t wch) const;
    
    /**
     * \brief Gets if the given character is an 'special char' or not.
     *
     * Special chars: 
     * SCRIPT_PARSER_TK_COMMA ',', 
     * SCRIPT_PARSER_TK_PAREN_OPEN '(', 
     * SCRIPT_PARSER_TK_PAREN_CLOSE ')',
     * SCRIPT_PARSER_TK_DOT '.',
     * SCRIPT_PARSER_TK_ASTERISK '*',
     * SCRIPT_PARSER_TK_EQUAL '='.
     * \param wch [in] Unicode character.
     * \param tkId [out] If given (not-NULL) and just when returning TRUE
     * it is set to its corresponding token identifier (Parser.h).
     * \return TRUE if wch is an special char, FALSE otherwise.
     */
    dex::gdb::bool_t IsSpecialChar(dex::gdb::uchar_t wch, dex::gdb::int32_t * tkId) const;
    
    /**
     * \brief Gets if the given character is a digit [0..9] or not.
     * \param wch [in] Unicode character.
     * \param digit [out] If given (not-NULL) and just when returning TRUE
     * it is set to its corresponding digit value.
     * \return TRUE if wch is a digit [0..9], FALSE otherwise.
     */
    dex::gdb::bool_t IsDigit(dex::gdb::uchar_t wch, dex::gdb::int32_t * digit) const;
};


END_DEX_SCRIPT_NAMESPACE


/** @} */
#endif // _DEX_TOKENIZER_H_
