/*
 * DEX: a library for out-of-core graph-based data storage and query processing.
 *
 * See the file LICENSE.txt for redistribution information.
 *
 * Copyright (c) 2005-2010 Sparsity Technologies, Barcelona (Spain)
 */

#ifndef _DEX_ROWWRITER_H_
#define _DEX_ROWWRITER_H_

/**
 * \addtogroup io
 * @{
 *
 * \file RowWriter.h
 * \brief It contains the definition of RowWriter interface.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */

#include "gdb/common.h"
#include "gdb/Exception.h"
#include "gdb/Graph_data.h"


BEGIN_DEX_IO_NAMESPACE


/**
 * \brief RowWriter interface.
 *
 * Common interface for those writers which dump the data from an string array.
 *
 * It works as follows: perform as many write operations as necessary and call
 * close once at the end. Once close is called no more write operations can
 * be executed.
 * 
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT RowWriter
{
public:
    
    /**
     * \brief Writes the next row.
     * \param row [in] Row of data.
     * \exception IOException If bad things happen during the write.
     */
    virtual void Write(dex::gdb::StringList & row)
    throw(dex::gdb::IOException, dex::gdb::Error) = 0;
    
    /**
     * \brief Closes the writer.
     * \exception IOException If the close fails.
     */
    virtual void Close()
    throw(dex::gdb::IOException, dex::gdb::Error) = 0;

    /**
     * \brief Destructor.
     */
    virtual ~RowWriter()
    {
    }
    
private:
    
    /**
     * \brief Non implemented to avoid use of copy constructor.
     */
    RowWriter(const RowWriter & rw);
    
    /**
     * \brief Non implemented to avoid use of assignment operator.
     */
    RowWriter & operator =(const RowWriter & rw);
    
protected:
    
    /**
     * \brief Empty constructor.
     * 
     * Protected because no one should instantiate a RowWriter. Just inherited
     * classes can use this empty constructor.
     */
    RowWriter()
    {
    }
};


END_DEX_IO_NAMESPACE

/** @} */
#endif // _DEX_ROWWRITER_H_
