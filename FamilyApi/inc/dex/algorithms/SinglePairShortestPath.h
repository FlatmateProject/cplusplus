/*
 * DEX: a library for out-of-core graph-based data storage and query processing.
 *
 * See the file LICENSE.txt for redistribution information.
 *
 * Copyright (c) 2005-2010 Sparsity Technologies, Barcelona (Spain)
 */

#ifndef _DEX_SINGLEPAIRSHORTESTPATH_H_
#define _DEX_SINGLEPAIRSHORTESTPATH_H_

/**
 * \addtogroup algorithms
 * @{
 *
 * \file SinglePairShortestPath.h
 * \brief It contains the definition of SinglePairShortestPath class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */


#include "algorithms/ShortestPath.h"


/*
 * Forward declaration of dex classes.
 */
namespace dex
{
    namespace gdb
    {
        class OIDList;
    }
}


BEGIN_DEX_ALGORITHMS_NAMESPACE


/**
 * \brief SinglePairShortestPath class.
 * 
 * Classes implementing this abstract class solve the
 * shortest path problem in a graph from a given source node and to
 * a given destination node.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT SinglePairShortestPath 
: public ShortestPath
{
public:
    
    /** 
     * Gets the shortest path between the source node and the destination node
     * as an ordered set of nodes.
     * \return Ordered set of node identifiers.
     */
    virtual dex::gdb::OIDList * GetPathAsNodes() = 0;
    
    /** 
     * Gets the shortest path between the source node and the destination node
     * as an ordered set of edges.
     * \return Ordered set of edge identifiers.
     */
    virtual dex::gdb::OIDList * GetPathAsEdges() = 0;
    
    /** 
     * Gets the cost of the shortest path.
     *
     * The cost for unweighted algorithms is the number of hops of the
     * shortest path. For weighted algoritms, the cost is the sum of
     * the costs of the edges of the shortest path.
     * \return The cost of the shortest path.
     */
    virtual dex::gdb::double64_t GetCost() = 0;
    
    /**
     * Returns TRUE If a path exists or FALSE otherwise.
     */
    virtual dex::gdb::bool_t Exists();

    /**
     * \brief Destructor.
     */
    virtual ~SinglePairShortestPath();
    
protected:
    
    /**
     * \brief Creates a new instance.
     * \param s [in] Session to get the graph from and perform traversal.
     * \param src [in] Source node.
     * \param dst [dst] Destination node.
     */
    SinglePairShortestPath(dex::gdb::Session &s,
        dex::gdb::oid_t src, dex::gdb::oid_t dst);

    /**
     * \brief Source node.
     */
    dex::gdb::oid_t source;
    
    /**
     * \brief Destination node.
     */
    dex::gdb::oid_t destination;
    
    /**
     * \brief Ordered set of node identifiers representing the shortest path.
     */
    dex::gdb::OIDList * pathAsNodes;

    /**
     * \brief Ordered set of edge identifiers representing the shortest path.
     *
     * May be computed lazily when requested from the pathAsNodes.
     */
    dex::gdb::OIDList * pathAsEdges;


private:
    
    /**
     * \brief Non implemented to avoid use of copy constructor.
     */
    SinglePairShortestPath(const SinglePairShortestPath & trav);
    /**
     * \brief Non implemented to avoid use of empty constructor.
     */
    SinglePairShortestPath();    
};


END_DEX_ALGORITHMS_NAMESPACE

/** @} */
#endif // _DEX_SINGLEPAIRSHORTESTPATH_H_
