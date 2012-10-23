/*
 * DEX: a library for out-of-core graph-based data storage and query processing.
 *
 * See the file LICENSE.txt for redistribution information.
 *
 * Copyright (c) 2005-2010 Sparsity Technologies, Barcelona (Spain)
 */

#ifndef _DEX_STRONGCONNECTIVITY_H_
#define _DEX_STRONGCONNECTIVITY_H_

/**
 * \addtogroup algorithms
 * @{
 *
 * \file StrongConnectivity.h
 * \brief It contains the definition of StrongConnectivity class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */

#include "algorithms/Connectivity.h"


BEGIN_DEX_ALGORITHMS_NAMESPACE

/**
 * \brief StrongConnectivity class.
 * 
 * Any class implementing this abstract class can be used to solve the problem
 * of finding strongly connected components in a <b> directed </b> graph.
 *
 * It consists in finding components where every pair (u,v) of nodes
 * contained in it has a path from u to v using the specified direction for each
 * edge type.
 *
 * It is possible to set some restrictions after constructing a new instance
 * of this class and before running it in order to limit the results. Those
 * restrictions are:
 *
 * (i) The set of edge types which will be navigated through
 *     while traversing the graph. The method is
 *     {@link #AddEdgeType} for adding one edge type and {@link #AddAllEdgeTypes}
 *     for adding all edge types with a specified direction.
 *
 * (ii) The set of node types which will be navigated through
 *      while traversing the graph. The method is
 *      {@link #AddNodeType} for adding one node type and {@link #AddAllNodeTypes}
 *      for adding all node types.
 *
 * For each instance of this class, it is only allowed to run the algorithm once.
 *
 * After the execution, we can retrieve the results stored in an instance of the
 * class {@link ConnectedComponents} using the {@link #GetConnectedComponents()}
 * method.
 *
 * NOTE: It is required to indicate the set of edge types and node types before
 * running this algorithm.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT StrongConnectivity : public Connectivity
{
public:

    /**
     * \brief Destructor.
     */
    virtual ~StrongConnectivity();

    /**
     * \brief Allows connectivity through edges of the given type.
     * \param t [in] Edge type.
     * \param d [in] Edge direction.
     */
    virtual void AddEdgeType(dex::gdb::type_t t, dex::gdb::EdgesDirection d);

    /**
     * \brief Allows connectivity through all edge types of the graph.
     * \param d [in] Edge direction.
     */
    virtual void AddAllEdgeTypes(dex::gdb::EdgesDirection d);

protected:

    /**
     * Creates a new instance of {@link StrongConnectivity}.
     *
     * \param s [in] Session to get the graph from and calculate the connectivity
     */
    StrongConnectivity(dex::gdb::Session &s);

private:

    /**
     * \brief Non implemented to avoid use of copy constructor.
     */
    StrongConnectivity(const StrongConnectivity & wc);
    /**
     * \brief Non implemented to avoid use of empty constructor.
     */
    StrongConnectivity();
};


END_DEX_ALGORITHMS_NAMESPACE

/** @} */
#endif // _DEX_STRONGCONNECTIVITY_H_
