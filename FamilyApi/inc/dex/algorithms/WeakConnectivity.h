/*
 * DEX: a library for out-of-core graph-based data storage and query processing.
 *
 * See the file LICENSE.txt for redistribution information.
 *
 * Copyright (c) 2005-2010 Sparsity Technologies, Barcelona (Spain)
 */

#ifndef _DEX_WEAKCONNECTIVITY_H_
#define _DEX_WEAKCONNECTIVITY_H_

/**
 * \addtogroup algorithms
 * @{
 *
 * \file WeakConnectivity.h
 * \brief It contains the definition of WeakConnectivity class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */

#include "algorithms/Connectivity.h"


BEGIN_DEX_ALGORITHMS_NAMESPACE

/**
 * \brief WeakConnectivity class.
 * 
 * Any class implementing this abstract class can be used to solve the problem
 * of finding weakly connected components in an <b> undirected </b> graph or in
 * a directed graph which will be considered as an undirected one.
 *
 * It consists in finding components where every pair <CODE>(u,v)</CODE>
 * of nodes contained in it has a path from <CODE>u</CODE> to <CODE>v</CODE> and
 * from <CODE>v</CODE> to <CODE>u</CODE>.
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
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT WeakConnectivity : public Connectivity
{
public:

    /**
     * \brief Destructor.
     */
    virtual ~WeakConnectivity();

    /**
     * \brief Allows connectivity through edges of the given type.
     * In a weak connectivity the edges can be used in {@link #Any} direction.
     * \param t [in] Edge type.
     * \param d [in] Edge direction.
     */
    virtual void AddEdgeType(dex::gdb::type_t t);

    /**
     * \brief Allows connectivity through all edge types of the graph.
     * In a weak connectivity the edges can be used in {@link #Any} direction.
     * \param d [in] Edge direction.
     */
    virtual void AddAllEdgeTypes();


protected:

    /**
     * Creates a new instance of {@link WeakConnectivity}.
     *
     * \param s [in] Session to get the graph from and calculate the connectivity
     */
    WeakConnectivity(dex::gdb::Session &s);

private:

    /**
     * \brief Non implemented to avoid use of copy constructor.
     */
    WeakConnectivity(const WeakConnectivity & wc);
    /**
     * \brief Non implemented to avoid use of empty constructor.
     */
    WeakConnectivity();
};


END_DEX_ALGORITHMS_NAMESPACE

/** @} */
#endif // _DEX_WEAKCONNECTIVITY_H_
