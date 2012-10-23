/*
 * DEX: a library for out-of-core graph-based data storage and query processing.
 *
 * See the file LICENSE.txt for redistribution information.
 *
 * Copyright (c) 2005-2010 Sparsity Technologies, Barcelona (Spain)
 */

#ifndef _DEX_TRAVERSALBFS_H_
#define _DEX_TRAVERSALBFS_H_

/**
 * \addtogroup algorithms
 * @{
 *
 * \file TraversalBFS.h
 * \brief It contains the definition of TraversalBFS class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */


#include "algorithms/Traversal.h"
#include <queue>


/*
 * Forward declaration of dex classes.
 */
namespace dex
{
    namespace gdb
    {
        class Objects;
        class ObjectsIterator;
    }
}


BEGIN_DEX_ALGORITHMS_NAMESPACE


/**
 * \brief Breadth-First Search implementation of Traversal.
 *
 * Starting from a source node, it visits all its neighbors at distance 1,
 * then all its neighbors at distance 2, and so on.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT TraversalBFS 
: public Traversal
{
public:
    
    /**
     * \brief Gets the next object of the traversal.
     * \return A node or edge identifier.
     */
    virtual dex::gdb::oid_t Next();
    
    /**
     * \brief Gets if there are more objects to be traversed.
     * \return TRUE if there are more objects, FALSE otherwise.
     */
    virtual dex::gdb::bool_t HasNext();
    
    /**
     * \brief Destructor
     */
    virtual ~TraversalBFS();
    
    /** 
     * \brief Returns the depth of the current node.
     *
     * That is, it returns the depth of the node returned in the last call to 
     * Next().
     * \return The depth of the current node.
     */
    virtual dex::gdb::int32_t GetCurrentDepth() const;
    
    /**
     * \brief Creates a new instance.
     * \param s [in] Session to get the graph from and perform traversal.
     * \param node [in] Node to start traversal from.
     */
    TraversalBFS(dex::gdb::Session &s, dex::gdb::oid_t node);

private:
    
    /**
     * \brief Non implemented to avoid use of copy constructor.
     */
    TraversalBFS(const TraversalBFS & trav);
    /**
     * \brief Non implemented to avoid use of empty constructor.
     */
    TraversalBFS();
    
    /**
     * \brief Collection of nodes in the current level
     */
    dex::gdb::Objects * nodesCurrentLevel;
    /**
     * \brief Collection of nodes in the next level
     */
    dex::gdb::Objects * nodesNextLevel;
    /**
     * \brief Collection of visited nodes.
     */
    dex::gdb::Objects * visited;
    /**
     * \brief The number which indicates the level of the current explosion in the TraversalBFS.
     */
    dex::gdb::int32_t currentLevel;
    /**
     * \brief Iterator of the nodes in the current level
     */
    dex::gdb::ObjectsIterator * nodesCurrentLevelIt;
};


END_DEX_ALGORITHMS_NAMESPACE

/** @} */
#endif // _DEX_TRAVERSALBFS_H_
