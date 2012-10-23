/*
 * DEX: a library for out-of-core graph-based data storage and query processing.
 *
 * See the file LICENSE.txt for redistribution information.
 *
 * Copyright (c) 2005-2010 Sparsity Technologies, Barcelona (Spain)
 */

#ifndef _DEX_VALUESITERATOR_H_
#define _DEX_VALUESITERATOR_H_

/**
 * \addtogroup gdb
 * @{
 *
 * \file ValuesIterator.h
 * \brief It contains the definition of ValuesIterator class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */

#include "gdb/Handler.h"
#include "gdb/Value.h"

/*
 * Forward declaration of dexcore classes.
 */
namespace dexcore
{
    class Values;
    class Session;
}

BEGIN_DEX_GDB_NAMESPACE

/*
 * Forward declaration of dex classes.
 */
class Values;

/**
 * \brief ValuesIterator class.
 *
 * It allows for traversing all the elements into a Values instance.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT ValuesIterator
: private Handler<dexcore::Values>
{
    friend class Values; // to access private constructor
    
public:
    
    /**
     * \brief Destructor.
     */
    virtual ~ValuesIterator();
    
    /**
     * \brief Gets if there are more elements to traverse.
     * \return TRUE if there are more elements to traverse,
     * FALSE otherwise.
     */
    bool_t HasNext();
    
    /**
     * \brief Gets the next element to traverse.
     * \return The next element.
     */
    Value* Next();

private:
    
    /**
     * \brief Creates a new instance.
     * \param core_values [in] Dexcore Values reference.
     */
    ValuesIterator(dexcore::Values * core_values, dexcore::Session* s);
    
    /**
     * \brief Non implemented to avoid use of copy constructor.
     */
    ValuesIterator(const ValuesIterator & objs);

    /**
     * \brief Non implemented to avoid use of assignment operator.
     */
    ValuesIterator & operator =(const ValuesIterator & objs);
    
    /**
     * \brief Current session.
     */
    dexcore::Session* sess;
};


END_DEX_GDB_NAMESPACE

/** @} */
#endif // _DEX_VALUESITERATOR_H_
