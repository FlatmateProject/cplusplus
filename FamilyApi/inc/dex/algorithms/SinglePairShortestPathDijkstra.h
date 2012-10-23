/*
 * DEX: a library for out-of-core graph-based data storage and query processing.
 *
 * See the file LICENSE.txt for redistribution information.
 *
 * Copyright (c) 2005-2010 Sparsity Technologies, Barcelona (Spain)
 */

#ifndef _DEX_SINGLEPAIRSHORTESTPATHDIJKSTRA_H_
#define _DEX_SINGLEPAIRSHORTESTPATHDIJKSTRA_H_

/**
 * \addtogroup algorithms
 * @{
 *
 * \file SinglePairShortestPathDijkstra.h
 * \brief It contains the definition of SinglePairShortestPathDijkstra class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */


#include "algorithms/SinglePairShortestPath.h"


BEGIN_DEX_ALGORITHMS_NAMESPACE


/**
 * \brief SinglePairShortestPathDijkstra class.
 * 
 * It solves the single-pair shortest path problem using a Dijkstra-based
 * implementation.
 * 
 * It is a weighted algorithm, so it takes into account the cost of the edges 
 * to compute a minimum-cost shortest path. That is, the user may set
 * for each edge type which attribute should be used to retrieve the cost
 * of the edge. If no attribute is given for an edge type, this will assume
 * the edge has a fixed cost (the default is 1). Only numerical attribute can
 * be set as weight attributes (that is Long, Integer or Double attributes
 * are allowed).
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT SinglePairShortestPathDijkstra 
: public SinglePairShortestPath
{
public:
    
    /**
     * \brief Destructor.
     */
    virtual ~SinglePairShortestPathDijkstra();
    
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
     * The cost is the sum of the weights of the edges in the shortest path.
     * \return The cost of the shortest path.
     */
    virtual dex::gdb::double64_t GetCost();
    
    /**
     * \brief Creates a new instance.
     * \param s [in] Session to get the graph from and perform traversal.
     * \param src [in] Source node.
     * \param dst [dst] Destination node.
     */
    SinglePairShortestPathDijkstra(dex::gdb::Session &s,
        dex::gdb::oid_t src, dex::gdb::oid_t dst);

    /**
     * \brief Allows for traversing edges of the given type using the given
     * attribute as the weight.
     * \param t [in] Edge type.
     * \param d [in] Edge direction.
     * \param attr [in] Attribute to be used as the weight. It must be a
     * global attribute or an attribute of the given edge type.
     */
    virtual void AddWeightedEdgeType(dex::gdb::type_t t,
                    dex::gdb::EdgesDirection d, dex::gdb::attr_t attr);

    /**
     * \brief Sets the weight assigned to the unweighted edges.
     * All the edges from the types added without an explicit weight attribute will get this weight.
     * The default weight for this edges is 1.
     */
    virtual void SetUnweightedEdgeCost(dex::gdb::double64_t weight);

private:
    
    /**
     * \brief Weights of the edge types.
     */
    std::map<dex::gdb::type_t, dex::gdb::attr_t> edgeWeights;
    /**
     * \brief Weight (cost) for those edges which does not have
     * a weight attribute set.
     */
    dex::gdb::double64_t defaultWeight;

    
    class FibonacciHeap
    {
    public:
        struct Node
        {
            dex::gdb::int64_t data;
            Node * child;
            Node * left;
            Node * parent;
            Node * right;
            dex::gdb::bool_t mark;
            dex::gdb::double64_t key;
            dex::gdb::int32_t degree;
            dex::gdb::int32_t level;
            Node(dex::gdb::int64_t d, dex::gdb::double64_t k, dex::gdb::int32_t l);
            virtual ~Node();
        private:
            Node();
            Node(const Node & node);
        };
        Node * minNode;
        FibonacciHeap();
        virtual ~FibonacciHeap();
        dex::gdb::bool_t IsEmpty() const;
        void Clear();
        void DecreaseKey(Node * x, dex::gdb::double64_t k);
        //void Delete(Node * x); // never used!
        void Insert(Node * x, dex::gdb::double64_t k);
        Node * Min();
        Node * RemoveMin();
        dex::gdb::int32_t Size();
        //FibonacciHeap(FibonacciHeap * h1, FibonacciHeap * h2); // never used! // union
    protected:
        void CascadingCut(Node * x);
        void Consolidate();
        void Cut(Node * x, Node * y);
        void Link(Node * y, Node * x);
    private:
        static const dex::gdb::double64_t OneOverLogPhi;
        static const dex::gdb::double64_t DoubleNegativeInfity;
        dex::gdb::int32_t nNodes;
    };
    
    /**
     * The queue which contains all the nodes that hasn't been expanded yet.
     */
    FibonacciHeap queue;
    /**
     * The collection which contains all the node identifiers of those nodes that
     * have been already visited (expanded).
     */
    dex::gdb::Objects * visitedSet;
    /**
     * The number of levels between the source node and the first node of the
     * queue.
     */
    dex::gdb::int32_t levelNodeFirstQueue;
    /**
     * The distance between the source node and the first node of the queue.
     */
    dex::gdb::double64_t distNodeFirstQueue;
    std::map<dex::gdb::int64_t,FibonacciHeap::Node*> nodesInQueue;
    /**
     * Attribute identifier which represents its predecesor node.
     */
    dex::gdb::attr_t pred_attr;
    /**
     * Attribute identifier which represents the distance from the source node
     * to the current node.
     */
    dex::gdb::attr_t dist_attr;
    
    /**
     * \brief Non implemented to avoid use of copy constructor.
     */
    SinglePairShortestPathDijkstra(const SinglePairShortestPathDijkstra & trav);
    /**
     * \brief Non implemented to avoid use of empty constructor.
     */
    SinglePairShortestPathDijkstra();    
    
    /**
     * \brief Adds all the information about the previous node of a given nodes, the
     * distance between the given node and the source node and, finally, adds
     * the given node to the queue that stores the nodes that hasn't been
     * exploded yet.
     * \param idNode The given node which needs to be added some information.
     * \param dist The distance between the given node and the source node.
     * \param level The level of the given node.
     * \param idNodePred The previous node of the given node in the BFS
     * exploration.
     */
    void AddInfoToNode(dex::gdb::oid_t idNode, dex::gdb::double64_t dist, 
            dex::gdb::int32_t level, dex::gdb::oid_t idNodePred);
    /** 
     * \brief Solves the single-pair shortest path problem with unweighted edges
     * by implementing the Dijkstra's algorithm
     * \return TRUE if a shortest path has been found, FALSE otherwise.
     */
    dex::gdb::bool_t Dijkstra();

    dex::gdb::double64_t GetDistance(dex::gdb::oid_t idNode);
    dex::gdb::oid_t GetEdge(dex::gdb::oid_t idTail, dex::gdb::oid_t idHead,
                        dex::gdb::type_t edgetype, 
                        dex::gdb::EdgesDirection navigation);
    dex::gdb::oid_t GetEdgeMinimumWeight(dex::gdb::oid_t idNode, 
                        dex::gdb::oid_t idNodePred);
    dex::gdb::oid_t GetPredecessor(dex::gdb::oid_t idNode);
    dex::gdb::double64_t GetWeight(dex::gdb::oid_t idEdge, 
                            dex::gdb::attr_t attrtype);
    void RelaxNeighbor(dex::gdb::oid_t idNode,
                            dex::gdb::oid_t idNeighbor, 
                            dex::gdb::double64_t neighborWeight);
    void RelaxNeighbors(dex::gdb::oid_t idNode);
    /**
     * \brief After computing the shortest path, it's necessary to find out
     * the path and store each node identifier in an array.
     */
    void StoreShortestPath();
};


END_DEX_ALGORITHMS_NAMESPACE

/** @} */
#endif // _DEX_SINGLEPAIRSHORTESTPATHDIJKSTRA_H_
