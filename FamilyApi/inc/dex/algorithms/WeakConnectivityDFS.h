/*
 * DEX: a library for out-of-core graph-based data storage and query processing.
 *
 * See the file LICENSE.txt for redistribution information.
 *
 * Copyright (c) 2005-2010 Sparsity Technologies, Barcelona (Spain)
 */

#ifndef _DEX_WEAKCONNECTIVITYDFS_H_
#define _DEX_WEAKCONNECTIVITYDFS_H_

/**
 * \addtogroup algorithms
 * @{
 *
 * \file WeakConnectivityDFS.h
 * \brief It contains the definition of WeakConnectivityDFS class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */


#include "algorithms/WeakConnectivity.h"


BEGIN_DEX_ALGORITHMS_NAMESPACE


/**
 * \brief WeakConnectivityDFS class.
 * 
 * This class can be used to solve the problem of finding weakly connected
 * components in an <b> undirected </b> graph or in a directed graph which will
 * be considered as an undirected one.
 *
 * It consists in finding components where every pair <CODE>(u,v)</CODE>
 * of nodes contained in it has a path from <CODE>u</CODE> to <CODE>v</CODE> and
 * from <CODE>v</CODE> to <CODE>u</CODE>.  This implementation is based on the
 * Depth-First Search (DFS) algorithm.
 *
 * It is possible to set some restrictions after constructing a new instance
 * of this class and before running it in order to limit the results. Those
 * restrictions are:
 *
 * (i) The set of edge types which will be navigated through
 *     while traversing the graph. The method is
 *     {@link #AddEdgeType} for adding one edge type and
 *     {@link #AddAllEdgeTypes} for adding all edge types.
 *
 * (ii) The set of node types which will be navigated through
 *      while traversing the graph. The method is {@link #AddNodeType}
 *      for adding one node type and {@link #AddAllNodeTypes} for adding all
 *      node types.
 *
 * For each instance of this class, it is only allowed to run the algorithm once.
 *
 * After the execution, we can retrieve the results stored in an instance of the
 * class {@link ConnectedComponents} using the {@link #getConnectedComponents()}
 * method.
 *
 * Please, see {@link edu.upc.dama.dex.algorithms} for further information related
 * to how to use this class.
 *
 * NOTE: It is required to indicate the set of edge types and node types before
 * running this algorithm.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT WeakConnectivityDFS : public WeakConnectivity
{
public:

    /**
     * Creates a new instance of {@link WeakConnectivityDFS}.
     * After creating this instance is required to indicate the set of edge types
     * and the set of node types which will be navigated through while traversing
     * the graph in order to find the weak connected components.
     * \param s [in] Session to get the graph from and calculate the connectivity
     */
    WeakConnectivityDFS(dex::gdb::Session &s);

    /**
     * \brief Destructor.
     */
    virtual ~WeakConnectivityDFS();
    
    /**
     * \brief Executes the algorithm.
     */
    void Run();

private:

    /**
     * \brief Calculate the connected component of a node.
     */
    void ComputeDFS(dex::gdb::oid_t idNode);

};


END_DEX_ALGORITHMS_NAMESPACE

/** @} */
#endif // _DEX_WEAKCONNECTIVITYDFS_H_
