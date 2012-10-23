/*
 * DEX: a library for out-of-core graph-based data storage and query processing.
 *
 * See the file LICENSE.txt for redistribution information.
 *
 * Copyright (c) 2005-2010 Sparsity Technologies, Barcelona (Spain)
 */

#ifndef _DEX_OBJECTSITERATOR_H_
#define _DEX_OBJECTSITERATOR_H_

/**
 * \addtogroup gdb
 * @{
 *
 * \file ObjectsIterator.h
 * \brief It contains the definition of ObjectsIterator class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */

#include "gdb/Handler.h"

/*
 * Forward declaration of dexcore classes.
 */
namespace dexcore
{
    // this does not work, so ObjecstIterator can not manage with Handler class!!
    //class Objects::Iterator; 
}

BEGIN_DEX_GDB_NAMESPACE

/*
 * Forward declaration of dex classes.
 */
class Objects;


/**
 * \brief ObjectsIterator class.
 *
 * Iterator to traverse all the object identifier into a Objects instance.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT ObjectsIterator
//: private Handler<dexcore::Objects::Iterator>
{
    friend class Objects; // to access private constructor
    
public:
    
    /**
     * \brief Destructor.
     */
    virtual ~ObjectsIterator();
    
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
    oid_t Next();
    
private:
    
    /**
     * \brief Creates a new instance.
     * \param core_objs_it [in] Handled dexcore Objects::Iterator reference.
     * \param objs [in] Parent Objects reference.
     */
    ObjectsIterator(void * core_objs_it, Objects * objs);
    
    /**
     * \brief Non implemented to avoid use of copy constructor.
     */
    ObjectsIterator(const ObjectsIterator & objsIt);

    /**
     * \brief Non implemented to avoid use of assignment operator.
     */
    ObjectsIterator & operator =(const ObjectsIterator & objsIt);
    
    /**
     * \brief Parent Objects reference.
     */
    Objects * objs;
    
    /**
     * \brief Handled dexcore Objects::Iterator instance.
     *
     * This is handled as a void * because forward declaration of
     * the nested Objects::Iterator class does not work.
     */
    void * core_objs_it;

    /**
     * \brief Invalidates the ObjectIterator and deletes the dexcore instance.
     * It's used when the parent Object is deleted whel it still has ObjectsIterator
     * open.
     */
    void Invalidate();
};


END_DEX_GDB_NAMESPACE

/** @} */
#endif // _DEX_OBJECTSITERATOR_H_
