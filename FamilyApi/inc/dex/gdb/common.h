/*
 * DEX: a library for out-of-core graph-based data storage and query processing.
 *
 * See the file LICENSE.txt for redistribution information.
 *
 * Copyright (c) 2005-2010 Sparsity Technologies, Barcelona (Spain)
 */

#ifndef _DEX_COMMON_H_
#define _DEX_COMMON_H_

/**
 * \addtogroup gdb
 * @{
 *
 * \file common.h
 * \brief It contains common includes and definitions as well as set basic
 * data types and enumerations.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */


#ifdef _DEX_DEBUG
#  include <assert.h>
#  define DEX_DEBUG
#  include <assert.h>
#  define DEX_ASSERT(_cond) assert(_cond)
#  define DEX_PERMANENT_ASSERT(_cond) DEX_ASSERT(_cond)
#else // !_DEX_DEBUG
#  define DEX_ASSERT(_cond)
#  define DEX_PERMANENT_ASSERT(_cond) if (!(_cond)) { abort(); }
#endif // _DEX_DEBUG

/*
 * Common system includes.
 */
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>


/**
 * \brief Begining macro for the dex namespace.
 */
#define BEGIN_DEX_NAMESPACE namespace dex {
/**
 * \brief Ending macro for the dex namespace.
 */
#define END_DEX_NAMESPACE }
/**
 * \brief Begining macro for the dex::gdb namespace.
 */
#define BEGIN_DEX_GDB_NAMESPACE BEGIN_DEX_NAMESPACE namespace gdb {
/**
 * \brief Ending macro for the dex::gdb namespace.
 */
#define END_DEX_GDB_NAMESPACE END_DEX_NAMESPACE }
/**
 * \brief Begining macro for the dex::io namespace.
 */
#define BEGIN_DEX_IO_NAMESPACE BEGIN_DEX_NAMESPACE namespace io {
/**
 * \brief Ending macro for the dex::io namespace.
 */
#define END_DEX_IO_NAMESPACE END_DEX_NAMESPACE }
/**
 * \brief Begining macro for the dex::script namespace.
 */
#define BEGIN_DEX_SCRIPT_NAMESPACE BEGIN_DEX_NAMESPACE namespace script {
/**
 * \brief Ending macro for the dex::script namespace.
 */
#define END_DEX_SCRIPT_NAMESPACE END_DEX_NAMESPACE }
/**
 * \brief Begining macro for the dex::algorithms namespace.
 */
#define BEGIN_DEX_ALGORITHMS_NAMESPACE BEGIN_DEX_NAMESPACE namespace algorithms {
/**
 * \brief Ending macro for the dex::algorithms namespace.
 */
#define END_DEX_ALGORITHMS_NAMESPACE END_DEX_NAMESPACE }


#ifdef WIN32
#  ifdef dex_EXPORTS // automatically defined by cmake when linking a dynamic library
#    define DEX_EXPORT __declspec(dllexport)
#  else
#    define DEX_EXPORT __declspec(dllimport)
#  endif
#else
#  define DEX_EXPORT
#endif


BEGIN_DEX_GDB_NAMESPACE    

/**
 * \brief Byte type.
 */
typedef unsigned char byte_t;
/**
 * \brief Boolean type.
 */
typedef bool bool_t;
/**
 * \brief Character type.
 */
typedef char char_t;
/**
 * \brief Unicode character type.
 */
typedef wchar_t uchar_t;
/**
 * \brief 32-bit signed integer type.
 */
typedef signed int int32_t;
/**
 * \brief 64-bit signed integer type.
 */
typedef signed long long int64_t;
/**
 * \brief 64-bit double type.
 */
typedef double double64_t;
/**
 * \brief Graph node or edge type type.
 */
typedef int32_t type_t;
/**
 * \brief Graph attribute type.
 */
typedef int32_t attr_t;
/**
 * \brief Graph oid type.
 */
typedef int64_t oid_t;
/**
 * \brief Color codified as RGB 32-bit int.
 *
 * Bits 24-31 are alpha, 16-23 are red, 8-15 are green, 0-7 are blue.
 */
typedef int32_t ColorRGB;


/**
 * \brief Object type enumeration.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
enum ObjectType
{
    /**
     * \brief Node object type.
     */
    Node,
    /**
     * \brief Edge object type.
     */
    Edge
};


/**
 * \brief Data type enumeration.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
enum DataType
{
    /**
     * \brief Boolean data type.
     */
    Boolean,
    
    /**
     * \brief 32-bit signed integer data type.
     */
    Integer,
    
    /**
     * \brief 64-bit signed integer data type.
     */
    Long,
    
    /**
     * \brief 64-bit signed double data type.
     */
    Double,
    
    /**
     * \brief Distance from Epoch (UTC) time in milliseconds precision.
     */
    Timestamp,
    
    /**
     * \brief Unicode string data type.
     *
     * 2048 characters maximum length.
     */
    String,
    
    /**
     * \brief Large unicode character object data type.
     * \see TextStream
     */
    Text,
    
    /**
     * \brief Object identifier data type (oid_t).
     */
    OID
};

/**
 * \brief Easy STL printing operator redefinition.
 *
 * It allows to do: ... << dex::gdb::String << ...
 */
std::wostream & operator << (std::wostream & wostrm, 
                             const enum DataType & dt);

/**
 * \brief Condition operators enumeration.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
enum Condition
{
    /**
     * \brief Equal condition (==).
     *
     * Null values can be used together with this condition to retrieve all objects
     * having a null value for an attribute.
     */
    Equal,
    
    /**
     * \brief Greater or equal condition (>=).
     *
     * Null values cannot be used together with this condition.
     */
    GreaterEqual,
    
    /**
     * \brief Greater than condition (>).
     *
     * Null values cannot be used together with this condition.
     */
    GreaterThan,
    
    /**
     * \brief Less or equal condition (<=).
     *
     * Null values cannot be used together with this condition.
     */
    LessEqual,
    
    /**
     * \brief Less than condition (<).
     *
     * Null values cannot be used together with this condition.
     */
    LessThan,
    
    /**
     * \brief Not equal condition (!=).
     *
     * Null values can be used together with this condition to retrieve all objects
     * having a non-null value for an attribute.
     */
    NotEqual,
    
    /**
     * \brief Substring condition.
     *
     * Null values cannot be used together with this condition.
     *
     * This condition can just be used together with String values. It allows for searching
     * substrings (case sensitive). Ex:
     * <pre>
     * 'AAABBBCCCD' Like 'BBB'   returns <code>true</code>
     * 'AAABBBCCCD' Like 'bbb'   returns <code>false</code>
     * 'AAABBBCCCD' Like 'E'     returns <code>false</code>
     * </pre>
     */
    Like,
    
    /**
     * \brief Substring (no case sensitive) condition.
     *
     * Null values cannot be used together with this condition.
     *
     * This condition can just be used together with String values. It allows for searching
     * substrings (no case sensitive). Ex:
     * <pre>
     * 'AAABBBCCCD' LikeNoCase 'BBB'   returns <code>true</code>
     * 'AAABBBCCCD' LikeNoCase 'bbb'   returns <code>true</code>
     * 'AAABBBCCCD' LikeNoCase 'E'     returns <code>false</code>
     * </pre>
     */
    LikeNoCase,
    
    /**
     * \brief In range operator condition ([x,y]).
     *
     * Null values cannot be used together with this condition.
     */
    Between,
    
    /**
     * \brief Regular expression condition.
     *
     * Null values cannot be used together with this condition.
     *
     * This is condition can just be used together with String values. It allows for searching
     * objects matching a given regular expression.
     *
     * It is case sensitive.
     *
     * Regular expression format conforms most of the POSIX
     * Extended Regular Expressions. 
     *
     * Implemented operators are:
     *
     * letter: expects that letter.
     *         Example: "a" true if it includes an a.
     *
     * E1E2: two or more consecutive expressions are concatenated.
     *       Example: "b" true if it includes a substring ab.
     *
     * E1|E2: expects one of both expressions.
     *        Example: "cat|dog" true if it includes cat or dog.
     *
     * (E): groups an expression.
     *      Example: "r(u|a)n" true if it includes run or ran.
     *
     * E{m,M}: an expression is repeated from a minimimum times, m, to a
     *         maximum times, M.
     *         If maximum, M, is zero, then unlimited number of repetitions
     *         is accepted.
     *         Example: "ab{1,3}" true if includes ab, abb or abbb.
     *
     * E* E+ E?: equivalent to: E{0,0} E{1,0} E{0,1}.
     *
     * [letters]: equivalent to: (letter1|letter2|...|lettern).
     *            Example: "[abc]" true if there is any of a or b or c.
     *
     * [^letters]: expects none of those letters.
     *             Example: "[^abc]" false if there is any of a or b or c.
     *
     * .: accepts any letter, equivalent to [^]
     *
     * ^E: expects an expression at the beginning.
     *     Example: "^a" true if it starts with a.
     *
     * E$: expects an expression at the end.
     *     Example: "a$" true if it ends with a.
     *
     *
     * Some simple examples:
     *
     * 'AAABBBCCCD' ERE 'A+B*C+'   returns <code>true</code>
     *
     * 'AAACCCD'    ERE 'B*C+'     returns <code>true</code>
     *
     * 'AAACCCD'    ERE 'B+C+'     returns <code>false</code>
     *
     * 'AAACCCD'    ERE '^A[^]*D$'  returns <code>true</code>
     *
     * 'AAACCCD'    ERE 'B*C+$'    returns <code>false</code>
     */
    RegExp
};


/**
 * \brief Order enumeration.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
enum Order
{
    /**
     * \brief From lower to higher.
     */
    Ascendent,

    /**
     * \brief From higher to lower.
     */
    Descendent
};


/**
 * \brief Edges direction enumeration.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
enum EdgesDirection
{
    /**
     * \brief In-going edges.
     */
    Ingoing,
    
    /**
     * \brief Out-going edges.
     */
    Outgoing,
    
    /**
     * \brief In-going or out-going edges.
     */
    Any
};


/**
 * \brief Attribute kind enumeration.
 *
 * Basic attributes cannot be used for query operations (just get and set
 * attribute values) whereas Indexed and Unique are able to.
 * Unique attributes works as a primary key, that is two objects cannot have the
 * same value for an attribute (but NULL).
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
enum AttributeKind
{
    /**
     * \brief Basic attribute kind.
     */
    Basic,
    /**
     * \brief Indexed attribute kind.
     */
    Indexed,
    /**
     * \brief Unique attribute kind.
     */
    Unique
};


/**
 * \brief Easy STL printing operator redefinition.
 *
 * It allows to do: ... << dex::gdb::Basic << ...
 */
std::wostream & operator << (std::wostream & wostrm, 
                             const enum AttributeKind & ak);


/**
 * \brief Log level enumeration.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
enum LogLevel 
{
    /**
     * \brief Disable log.
     */
    Off,
    
    /**
     * \brief Severe log level.
     *
     * This is the lower log level, just critical errors are shown.
     */
    Severe,
    
    /**
     * \brief Warning log level.
     */
    Warning,
    
    /**
     * \brief Info log level.
     */
    Info,
    
    /**
     * \brief Config log level.
     */
    Config,
    
    /**
     * \brief Fine log level.
     *
     * This is the higher and finest log level, everything is dumped to the log.
     */
    Fine,
    
    /**
     * \brief Debug log level.
     *
     * This is for Dex development purposes and just works on debug versions 
     * of the library.
     */
    Debug
};


/**
 * \brief Export type.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
enum ExportType
{
    /**
     * \brief Export to Graphviz format.
     * 
     * Graphviz home page: http://www.graphviz.org
     */
    Graphviz,
    
    /**
     * \brief Export to GraphML format.
     * 
     * GraphML home page: http://graphml.graphdrawing.org/
     */
    GraphML,
    
    /**
     * \brief Export to YGRAPHML format.
     * 
     * It is an GraphML format extended with a set of yWorks 
     * ("http://www.yworks.com") extensions. Thus, it allows for the
     * visualization of the exported graph with the public 
     * yEd visualization tool ("http://www.yworks.com/products/yed").
     */
    YGraphML
};


/**
 * \brief Node shape.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
enum NodeShape 
{
    /**
     * Box shape.
     */
    Box,
    
    /**
     * Round shape.
     */
    Round
};


/**
 * \brief Platform usage statistics.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT PlatformStatistics 
{
    friend class Platform; // to set private fields
    
private:
    
    /**
     * \brief Numver of CPUs.
     */
    int32_t nCPUs;
    
    /**
     * \brief Time in microseconds (since epoch).
     */
    int64_t realTime;
    
    /**
     * \brief CPU user time in microseconds.
     */
    int64_t userTime;
    
    /**
     * \brief CPU system time in microseconds.
     */
    int64_t sysTime;
    
    /**
     * \brief Physical memory in KB.
     */
    int64_t totalMem;
    
    /**
     * \brief Free physical memory in KB.
     */
    int64_t availMem;
    
    /**
     * \brief Non implemented to avoid use of copy constructor.
     */
    PlatformStatistics(const PlatformStatistics &stats);
    
    /**
     * \brief Non implemented to avoid use of assignment operator.
     */
    PlatformStatistics & operator =(const PlatformStatistics &stats);
    
public:
    
    /**
     * \brief Creates a new instance setting all values to 0.
     */
    PlatformStatistics()
    : nCPUs((int32_t)0)
    , realTime((int64_t)0)
    , userTime((int64_t)0)
    , sysTime((int64_t)0)
    , totalMem((int64_t)0)
    , availMem((int64_t)0)
    {
    }
    
    /**
     * \brief Gets the number of CPUs.
     * \return The number of CPUs.
     */
    int32_t GetNumCPUs() const
    {
        return nCPUs;
    }
    
    /**
     * \brief Gets time in microseconds (since epoch).
     * \return Time in microseconds (since epoch).
     */
    int64_t GetRealTime() const
    {
        return realTime;
    }
    
    /**
     * \brief Gets CPU user time.
     * \return CPU user time.
     */
    int64_t GetUserTime() const
    {
        return userTime;
    }
    
    /**
     * \brief Gets CPU system time.
     * \return CPU system time.
     */
    int64_t GetSystemTime() const
    {
        return sysTime;
    }
    
    /**
     * \brief Gets physical memory size in Bytes.
     * \return Physical memory size in Bytes.
     */
    int64_t GetTotalMem() const
    {
        return totalMem;
    }    
    
    /**
     * \brief Gets avialable (free) memory size in Bytes.
     * \return Avialable (free) memory size in Bytes.
     */
    int64_t GetAvailableMem() const
    {
        return availMem;
    }    
};


/**
 * \brief Platform class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT Platform
{
public:
    
    /**
     * \brief Gets platform usage statistics.
     * \param stats [in|out] This updates the given PlatformStatistics.
     */
    static void GetStatistics(PlatformStatistics & stats);
};


END_DEX_GDB_NAMESPACE

/** @} */
#endif // _DEX_COMMON_H_
