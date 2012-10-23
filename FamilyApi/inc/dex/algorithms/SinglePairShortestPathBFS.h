/*
 * DEX: a library for out-of-core graph-based data storage and query processing.
 *
 * See the file LICENSE.txt for redistribution information.
 *
 * Copyright (c) 2005-2010 Sparsity Technologies, Barcelona (Spain)
 */

#ifndef _DEX_SINGLEPAIRSHORTESTPATHBFS_H_
#define _DEX_SINGLEPAIRSHORTESTPATHBFS_H_

/**
 * \addtogroup algorithms
 * @{
 *
 * \file SinglePairShortestPathBFS.h
 * \brief It contains the definition of SinglePairShortestPathBFS class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */


#include "algorithms/SinglePairShortestPath.h"


BEGIN_DEX_ALGORITHMS_NAMESPACE


/**
 * \brief SinglePairShortestPathBFS class.
 * 
 * It solves the single-pair shortest path problem using a BFS-based
 * implementation.
 * 
 * It is a unweighted algorithm, that is it assumes all edges have the same
 * cost.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT SinglePairShortestPathBFS 
: public SinglePairShortestPath
{
public:
    
    /**
     * \brief Destructor.
     */
    virtual ~SinglePairShortestPathBFS();
    
    /**
     * \brief Executes the algorithm.
     */
    virtual void Run();
    
    /**
     * Gets the shortest path between the source node and the destination node
     * as an ordered set of nodes.
     * \return Ordered set of node identifiers.
     */
    virtual dex::gdb::OIDList * GetPathAsNodes();
    
    /**
     * Gets the shortest path between the source node and the destination node
     * as an ordered set of edges.
     * \return Ordered set of edge identifiers.
     */
    virtual dex::gdb::OIDList * GetPathAsEdges();
    
    /**
     * Gets the cost of the shortest path.
     *
     * The cost is the number of hops of the shortest path.
     * \return The cost of the shortest path.
     */
    virtual dex::gdb::double64_t GetCost();
    
    /**
     * \brief Creates a new instance.
     * \param s [in] Session to get the graph from and perform traversal.
     * \param src [in] Source node.
     * \param dst [dst] Destination node.
     */
    SinglePairShortestPathBFS(dex::gdb::Session &s,
        dex::gdb::oid_t src, dex::gdb::oid_t dst);

private:
    /**
     * \brief Non implemented to avoid use of copy constructor.
     */
    SinglePairShortestPathBFS(const SinglePairShortestPathBFS & trav);
    /**
     * \brief Non implemented to avoid use of empty constructor.
     */
    SinglePairShortestPathBFS();    

    /**
     * Find an edge from the given set that had not been excluded.
     * @return Returns the edge identifier or InvalidOID.
     */
    dex::gdb::oid_t FindAValidEdge(dex::gdb::Objects *edges);
    /**
     * Find an edge from the given edge type and direction between
     * the nodes specified that had not been excluded.
     * @return Returns the edge identifier or InvalidOID.
     */
    dex::gdb::oid_t GetAValidEdge( dex::gdb::type_t edgeT,
            dex::gdb::EdgesDirection dir, dex::gdb::oid_t predNode,
            dex::gdb::oid_t current);

};


END_DEX_ALGORITHMS_NAMESPACE

/** @} */
#endif // _DEX_SINGLEPAIRSHORTESTPATHBFS_H_
