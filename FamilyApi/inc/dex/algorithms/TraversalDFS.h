/*
 * DEX: a library for out-of-core graph-based data storage and query processing.
 *
 * See the file LICENSE.txt for redistribution information.
 *
 * Copyright (c) 2005-2010 Sparsity Technologies, Barcelona (Spain)
 */

#ifndef _DEX_TRAVERSALDFS_H_
#define _DEX_TRAVERSALDFS_H_

/**
 * \addtogroup algorithms
 * @{
 *
 * \file TraversalDFS.h
 * \brief It contains the definition of TraversalDFS class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */


#include "algorithms/Traversal.h"
#include <stack>


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
 * \brief Depth-First Search (DFS) implementation of Traversal.
 *
 * Starting from a source or root node, it visits as far as possible along
 * each branch before backtracking.
 * 
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT TraversalDFS 
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
     * \brief Returns the depth of the current node.
     *
     * That is, it returns the depth of the node returned in the last call to
     * Next().
     * \return The depth of the current node.
     */
    virtual dex::gdb::int32_t GetCurrentDepth() const;

    /**
     * \brief Destructor
     */
    virtual ~TraversalDFS();
    
    /**
     * \brief Creates a new instance.
     * \param s [in] Session to get the graph from and perform traversal.
     * \param node [in] Node to start traversal from.
     */
    TraversalDFS(dex::gdb::Session &s, dex::gdb::oid_t node);

private:
    
    /**
     * \brief Non implemented to avoid use of copy constructor.
     */
    TraversalDFS(const TraversalDFS & trav);
    /**
     * \brief Non implemented to avoid use of empty constructor.
     */
    TraversalDFS();
    

    /**
     * \brief Store neighbors information.
     */
    class NeighborsInfo
    {
    public:
        dex::gdb::Objects *objs;
        dex::gdb::ObjectsIterator *it;
        dex::gdb::int32_t depth;

        NeighborsInfo(dex::gdb::Objects *o, dex::gdb::int32_t d);
        ~NeighborsInfo();
    };

    /**
     * The stack which contains all the node collections that hasn't been expanded yet.
     */
    std::stack< NeighborsInfo* > stack;

    /**
     * The collection of node identifers which has all the identifiers of
     * those nodes which have not been exploded yet.
     */
    dex::gdb::Objects * nodesVisited;

    /**
     * \brief The current node
     */
    dex::gdb::int64_t idNodePrevious;
    /**
     * \brief the depth of idNodePrevious node
     */
    dex::gdb::int32_t previousDepth;

    /**
     * \brief the depth of the last node returned by the Next method.
     */
    dex::gdb::int32_t currentLevel;

    /**
     * \brief 
     */
    dex::gdb::oid_t ComputeNext();
    /**
     * \brief 
     */
    void PrepareStack();
    /**
     * \brief
     */
    void VisitNeighbors();
};


END_DEX_ALGORITHMS_NAMESPACE

/** @} */
#endif // _DEX_TRAVERSALDFS_H_
