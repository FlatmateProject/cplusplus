/*
 * DEX: a library for out-of-core graph-based data storage and query processing.
 *
 * See the file LICENSE.txt for redistribution information.
 *
 * Copyright (c) 2005-2010 Sparsity Technologies, Barcelona (Spain)
 */

#ifndef _DEX_STRONGCONNECTIVITYGABOW_H_
#define _DEX_STRONGCONNECTIVITYGABOW_H_

/**
 * \addtogroup algorithms
 * @{
 *
 * \file StrongConnectivityGabow.h
 * \brief It contains the definition of StrongConnectivityGabow class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */


#include "algorithms/StrongConnectivity.h"
#include <stack>

BEGIN_DEX_ALGORITHMS_NAMESPACE


/**
 * This class can be used to solve the problem of finding strongly connected
 * components in a <b> directed </b> graph.
 *
 * It consists in finding components where every pair (u,v) of nodes
 * contained in it has a path from u to v using the specified direction for each
 * edge type.  This implementation is based on the Gabow algorithm.
 *
 * It is possible to set some restrictions after constructing a new instance
 * of this class and before running it in order to limit the results. Those
 * restrictions are:
 *
 * (i) The set of edge types which will be navigated through
 *     while traversing the graph. The method is
 *     {@link #AddEdgeType} for adding one edge type and {@link #AddAllEdgeTypes}
 *     for adding all edge types  with a specified direction.
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
class DEX_EXPORT StrongConnectivityGabow : public StrongConnectivity
{
public:

    /**
     * Creates a new instance of {@link StrongConnectivityGabow}.
     * After creating this instance is required to indicate the set of edge types
     * and the set of node types which will be navigated through while traversing
     * the graph in order to find the strong connected components.
     * \param s [in] Session to get the graph from and calculate the connectivity
     */
    StrongConnectivityGabow(dex::gdb::Session &s);

    /**
     * \brief Destructor.
     */
    virtual ~StrongConnectivityGabow();
    
    /**
     * \brief Executes the algorithm.
     */
    void Run();

private:

    class InfoNode
    {
    private:
        /** Node to be exploded in the next iterations. */
        dex::gdb::oid_t idNod;
        /** The node before the current one. */
        dex::gdb::oid_t idNodPred;
        /** Boolean to know if the node must be poped. */
        dex::gdb::bool_t isNeededToBePop;

    public:
        InfoNode(dex::gdb::oid_t idNode, dex::gdb::oid_t idNodePred, dex::gdb::bool_t isNeededToBePoped)
        {
            idNod = idNode;
            idNodPred = idNodePred;
            isNeededToBePop = isNeededToBePoped;
        }

        inline dex::gdb::oid_t GetIdNode()
        {
            return idNod;
        }

        inline dex::gdb::oid_t GetIdNodPred()
        {
            return idNodPred;
        }

        inline dex::gdb::bool_t IsNeededTobePoped()
        {
            return isNeededToBePop;
        }
    };

    dex::gdb::Objects* nodesStored;
    dex::gdb::Objects* nodesVisited;
    std::stack<dex::gdb::oid_t> stack1;
    std::stack<dex::gdb::oid_t> stack2;
    std::stack<InfoNode*> infoStack;
    /**
     * \brief An auxiliary transient attribute.
     */
    dex::gdb::attr_t indexAttribute;
    dex::gdb::int32_t index;


    void Gabow(dex::gdb::oid_t idNode);
    void SetInfoToNodeNotVisited(dex::gdb::oid_t idNodePoped, dex::gdb::oid_t idNodePredPoped);
    void SetIndex(dex::gdb::oid_t idNode, dex::gdb::int32_t index);
    dex::gdb::int32_t GetIndex(dex::gdb::oid_t idNode);
    void StoreStronglyConnectedComponent(dex::gdb::oid_t idNode);
    void VisitNeighborsOfAType(dex::gdb::oid_t idNode, dex::gdb::type_t edgetype, dex::gdb::EdgesDirection edgeDirection);
};


END_DEX_ALGORITHMS_NAMESPACE

/** @} */
#endif // _DEX_STRONGCONNECTIVITYGABOW_H_
