/*
 * DEX: a library for out-of-core graph-based data storage and query processing.
 *
 * See the file LICENSE.txt for redistribution information.
 *
 * Copyright (c) 2005-2010 Sparsity Technologies, Barcelona (Spain)
 */

#ifndef _DEX_EXCEPTION_H_
#define _DEX_EXCEPTION_H_

/**
 * \addtogroup gdb
 * @{
 *
 * \file Exception.h
 * \brief It contains a hierarchy of exceptions.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */

#include "gdb/common.h"

BEGIN_DEX_GDB_NAMESPACE

/*
 *
 * EXCEPTIONS
 *
 */


/**
 * \brief Exception class.
 *
 * This is the superclass of those exceptions that can be thrown during
 * the normal execution of a program.
 *
 * It is expected all methods declare all Exception (or subclasses)  
 * they throw.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT Exception
{
protected:
    
    /**
     * \brief Message of the exception.
     */
    std::string message;
    
public:
    
    /**
     * \brief Creates a new instance.
     */
    Exception();
    
    /**
     * \brief Creates a new instance.
     * \param mess [in] Message of the exception.
     */
    Exception(const std::string & mess);
    
    /**
     * \brief Gets the message of the exception.
	 *
	 * It should be called GetMessage but this is not possible
	 * because of a macro defined in Windows.
     * \return The message of the exception.
     */
    const std::string & Message() const;
    
    /**
     * \brief Destructor.
     */
    virtual ~Exception();
};


/**
 * \brief IO exception class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT IOException
: public Exception
{
public:
    
    /**
     * \brief Creates a new instance.
     */
    IOException();
    
    /**
     * \brief Creates a new instance.
     * \param mess [in] Message of the exception.
     */
    IOException(const std::string & mess);
    
    /**
     * \brief Destructor.
     */
    virtual ~IOException();
};


/**
 * \brief File not found exception class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT FileNotFoundException
: public IOException
{
public:
    
    /**
     * \brief Creates a new instance.
     */
    FileNotFoundException();
    
    /**
     * \brief Creates a new instance.
     * \param mess [in] Message of the exception.
     */
    FileNotFoundException(const std::string & mess);
    
    /**
     * \brief Destructor.
     */
    virtual ~FileNotFoundException();
};


/**
 * \brief No such element exception class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT NoSuchElementException
: public Exception
{
public:
    
    /**
     * \brief Creates a new instance.
     */
    NoSuchElementException();
    
    /**
     * \brief Creates a new instance.
     * \param mess [in] Message of the exception.
     */
    NoSuchElementException(const std::string & mess);
    
    /**
     * \brief Destructor.
     */
    virtual ~NoSuchElementException();
};


/*
 *
 * ERRORS
 *
 */


/**
 * \brief Error class.
 *
 * An Error corresponds to an unexpected and unpredictable exception.
 * 
 * As all methods can throw an Error at any moment, it is not expected 
 * they declare the Error (or subclasses) they may throw.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT Error
: public Exception
{
    
public:
    
    /**
     * \brief Creates a new Error instance from a dexcore error code.
     * \param coreErrorCode [in] Dexcore error code.
     * \return Depending on the given dexcore error, this may return an 
     * Error instance or an specific Error subclass instance.
     */
    static Error NewError(int32_t coreErrorCode);
    
    /**
     * \brief Creates a new instance.
     */
    Error();
    
    /**
     * \brief Creates a new instance.
     * \param mess [in] Message of the exception.
     */
    Error(const std::string & mess);
    
    /**
     * \brief Destructor.
     */
    virtual ~Error();
};


/**
 * \brief System error class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT SystemError
: public Error
{
public:
    
    /**
     * \brief Creates a new instance.
     */
    SystemError();
    
    /**
     * \brief Creates a new instance.
     * \param mess [in] Message of the exception.
     */
    SystemError(const std::string & mess);
    
    /**
     * \brief Destructor.
     */
    virtual ~SystemError();
};


/**
 * \brief Application error class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT AppError
: public Error
{
public:
    
    /**
     * \brief Creates a new instance.
     */
    AppError();
    
    /**
     * \brief Creates a new instance.
     * \param mess [in] Message of the exception.
     */
    AppError(const std::string & mess);
    
    /**
     * \brief Destructor.
     */
    virtual ~AppError();
};


/**
 * \brief Wrong argument error class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT WrongArgumentError
: public AppError
{
public:
    
    /**
     * \brief Creates a new instance.
     */
    WrongArgumentError();
    
    /**
     * \brief Creates a new instance.
     * \param mess [in] Message of the exception.
     */
    WrongArgumentError(const std::string & mess);
    
    /**
     * \brief Destructor.
     */
    virtual ~WrongArgumentError();
};


/**
 * \brief IO error class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT IOError
: public SystemError
{
public:
    
    /**
     * \brief Creates a new instance.
     */
    IOError();
    
    /**
     * \brief Creates a new instance.
     * \param mess [in] Message of the exception.
     */
    IOError(const std::string & mess);
    
    /**
     * \brief Destructor.
     */
    virtual ~IOError();
};


/**
 * \brief License error class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT LicenseError
: public AppError
{
public:
    
    /**
     * \brief Creates a new instance.
     */
    LicenseError();
    
    /**
     * \brief Creates a new instance.
     * \param mess [in] Message of the exception.
     */
    LicenseError(const std::string & mess);
    
    /**
     * \brief Destructor.
     */
    virtual ~LicenseError();
};


/**
 * \brief Unsupported operation error class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT UnsupportedOperationError
: public AppError
{
public:
    
    /**
     * \brief Creates a new instance.
     */
    UnsupportedOperationError();
    
    /**
     * \brief Creates a new instance.
     * \param mess [in] Message of the exception.
     */
    UnsupportedOperationError(const std::string & mess);
    
    /**
     * \brief Destructor.
     */
    virtual ~UnsupportedOperationError();
};


END_DEX_GDB_NAMESPACE

/** @} */
#endif // _DEX_EXCEPTION_H_
