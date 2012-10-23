/*
 * DEX: a library for out-of-core graph-based data storage and query processing.
 *
 * See the file LICENSE.txt for redistribution information.
 *
 * Copyright (c) 2005-2010 Sparsity Technologies, Barcelona (Spain)
 */

#ifndef _DEX_VALUES_H_
#define _DEX_VALUES_H_

/**
 * \addtogroup gdb
 * @{
 *
 * \file Values.h
 * \brief It contains the definition of Values class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */

#include "gdb/Handler.h"

/*
 * Forward declaration of dexcore classes.
 */
namespace dexcore
{
    class Values;
}

BEGIN_DEX_GDB_NAMESPACE

/*
 * Forward declaration of dex classes.
 */
class Session;
class ValuesIterator;
class Graph;


/**
 * \brief Value set class.
 *
 * This is a set of Value instances, that is there is no duplicated elements.
 *
 * Use a ValuesIterator to traverse all the elements into the set.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT Values
: private Handler<dexcore::Values>
{
    friend class Graph; // to access private constructor
    
public:
    
    /**
     * \brief Destructor.
     */
    virtual ~Values();
    
    /**
     * \brief Gets the number of elements into the collection.
     * \return The number of elements into the collection.
     */
    int64_t Count();
    
    /**
     * \brief Gets a ValuesIterator.
     * \return ValuesIterator instance.
     * \param order [in] Ascending or descending order.
     */
    ValuesIterator * Iterator(Order order);

private:
    
    /**
     * \brief Creates a new instance.
     * \param core_values [in] Dexcore Values reference.
     * \param s [in] Session reference.
     * \param g [in] Graph reference.
     * \param a [in] Dex attribute identifier.
     */
    Values(dexcore::Values * core_values, Session * s, Graph * g, attr_t a);
    
    /**
     * \brief Non implemented to avoid use of copy constructor.
     */
    Values(const Values & values);

    /**
     * \brief Non implemented to avoid use of assignment operator.
     */
    Values & operator =(const Values & values);
    
    /**
     * \brief Session reference.
     */
    Session * sess;

    /**
     * \brief Graph reference.
     */
    Graph * graph;
    
    /**
     * \brief Dex attribute identifier.
     */
    attr_t attr;
};


END_DEX_GDB_NAMESPACE

/** @} */
#endif // _DEX_VALUES_H_
