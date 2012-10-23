/*
 * DEX: a library for out-of-core graph-based data storage and query processing.
 *
 * See the file LICENSE.txt for redistribution information.
 *
 * Copyright (c) 2005-2010 Sparsity Technologies, Barcelona (Spain)
 */

#ifndef _DEX_CONNECTIVITY_H_
#define _DEX_CONNECTIVITY_H_

/**
 * \addtogroup algorithms
 * @{
 *
 * \file Connectivity.h
 * \brief It contains the definition of Connectivity class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */

#include "ConnectedComponents.h"
#include <map>
#include <vector>


BEGIN_DEX_ALGORITHMS_NAMESPACE


/**
 * \brief Connectivity class.
 * 
 * Any class implementing this abstract class can be used to solve
 * a problem related to graph connectivity as finding the strongly connected
 * components or finding the weakly connected components.
 *
 * Any component of a graph is called connected if there is a path between
 * every pair of nodes contained in this component.
 *
 * (i) <b>Strongly Connected Components</b>: This is the problem to find
 *              all the connected components in a <b>directed</b> graph.
 *
 * (ii) <b>Weakly Connected Components</b>: This is the problem to find
 *              all the connected components in an <b>undirected</b> graph.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT Connectivity
{
public:

    /**
     * \brief Destructor.
     */
    virtual ~Connectivity();

    /**
     * \brief Allows connectivity through nodes of the given type.
     */
    virtual void AddNodeType(dex::gdb::type_t t);

    /**
     * \brief Allows connectivity through all node types of the graph.
     */
    virtual void AddAllNodeTypes();

    /**
     * \brief Set which nodes can't be used.
     * This will replace any previously specified set of excluded nodes.
     * Should only be used to exclude the usage of specific nodes
     * from allowed node types because it's less efficient than
     * not allowing a node type.
     * \param nodes [in] A set of node identifiers that must be kept intact
     * until the destruction of the class.
     */
    virtual void ExcludeNodes(dex::gdb::Objects &nodes);

    /**
     * \brief Set which edges can't be used.
     * This will replace any previously specified set of excluded edges.
     * Should only be used to exclude the usage of specific edges
     * from allowed edge types because it's less efficient than
     * not allowing an edge type.
     * \param edges [in] A set of edge identifiers that must be kept intact
     * until the destruction of the class.
     */
    virtual void ExcludeEdges(dex::gdb::Objects &edges);

    /**
     * Returns the results generated by the execution of the algorithm. These
     * results contain information related to the connected components found
     * as the number of different components, the set of nodes contained in
     * each component or many other data.
     *
     * @return Returns an instance of the class {@link ConnectedComponents}
     * which contain information related to the connected components found.
     */
    ConnectedComponents* GetConnectedComponents();

    /** Runs the algorithm in order to find the connected components.
     * <p>
     * This method can be called only once.
     */
    virtual void Run() = 0;

    /**
     * Creates a new common attribute type for all node types in the graph
     * in order to store, persistently, the results related to the connected
     * components found while executing this algorithm.
     * <p>
     * Whenever the user wants to retrieve the results, even when the graph has
     * been closed and opened again, it is only necessary to create a new instance
     * of the class {@link ConnectedComponents} indicating the graph and the
     * name of the common attribute type which stores the results. This instance
     * will have all the information related to the connected components found in
     * the moment of the execution of the algorithm that stored this data.
     * <p>
     * It is possible to run the algorithm without specifying this parameter in
     * order to avoid materializing the results of the execution.
     *
     * @param attributeName [in] The name of the common attribute type for all node types
     * in the graph which will store persistently the results generated by the
     * execution of the algorithm.
     */
    void SetMaterializedAttribute(const std::wstring &attributeName);


protected:

    /**
     * Creates a new instance of {@link Connectivity}.
     *
     * \param s [in] Session to get the graph from and calculate the connectivity
     */
    Connectivity(dex::gdb::Session &s);

    /**
     * \brief Allows connectivity through edges of the given type.
     * \param t [in] Edge type.
     * \param d [in] Edge direction.
     */
    void AddEdgeType(dex::gdb::type_t t, dex::gdb::EdgesDirection d);

    /**
     * \brief Allows connectivity through all edge types of the graph.
     * \param d [in] Edge direction.
     */
    void AddAllEdgeTypes(dex::gdb::EdgesDirection d);

    /**
     * \brief Session.
     */
    dex::gdb::Session * sess;
    /**
     * \brief Graph.
     */
    dex::gdb::Graph * graph;

    /**
     * \brief A type definition to store allowed edge types.
     */
    typedef std::map<dex::gdb::type_t, dex::gdb::EdgesDirection> EdgeTypes_t;
    /**
     * \brief Allowed edge types.
     */
    EdgeTypes_t edgeTypes;
    /**
     * \brief A type definition to store allowed node types.
     */
    typedef std::vector<dex::gdb::type_t> NodeTypes_t;
    /**
     * \brief Allowed node types.
     */
    std::vector<dex::gdb::type_t> nodeTypes;

    /**
     * \brief common attribute where the connected component information is stored.
     */
    dex::gdb::attr_t attrComponent;
    /**
     * \brief name of the common attribute where the connected component information is stored.
     */
    std::wstring attrComponentName;

    /**
     * \brief Current component identifier.
     */
    dex::gdb::int64_t actualComponent;

    /**
     * \brief Identifiers of the nodes not visited.
     */
    dex::gdb::Objects *nodesNotVisited;
    /**
     * \brief Materialized results.
     */
    dex::gdb::bool_t matResults;
    /**
     * \brief True if the connectivity has been calculated.
     */
    dex::gdb::bool_t computed;
    /**
     * \brief The set of excluded nodes.
     */
    dex::gdb::Objects *excludedNodes;
    /**
     * \brief The set of excluded edges.
     */
    dex::gdb::Objects *excludedEdges;

    /**
     * \brief The calculated connectivity information.
     */
    ConnectedComponents *ccs;

    // -----------------------------------------------

    /**
     * Check that edges had been added.
     */
    void AssertAddedEdges();
    /**
     * Check that nodes had been added.
     */
    void AssertAddedNodes();
    /**
     * Check that the connectivity had not been calculated yet.
     */
    void AssertNotComputed();
    /**
     * Assigns the current component to the given node.
     */
    void SetConnectedComponent(dex::gdb::oid_t idNode);
    /**
     * Set all the selected nodes in nodesNotVisited.
     * That's all the nodes of the allowed node types but not the excluded ones.
     */
    void SetNodesNotVisited();
    /**
     * Check that the given attribute name is not already in use.
     */
    void AssertNotComponentAttribute(const std::wstring &attributeName);
    /**
     * Check that the connectivity had been calculated.
     */
    void AssertComputed();
    /**
     * Check that the given edge type is valid
     */
    void AssertEdgeType(dex::gdb::type_t edgetype);
    /**
     * Check that the given node type is valid
     */
    void AssertNodeType(dex::gdb::type_t nodetype);
    /**
     * Set a new persistent global attribute to store the connectivity information.
     */
    void CreateGlobalPersistentAttribute(const std::wstring &attributeName);
    /**
     * Set a new temporary global attribute to store the connectivity information.
     */
    void CreateGlobalTransientAttribute();
    /**
     * Removes the global attribute where the connectivity information is stored.
     */
    void RemoveGlobalAttribute();
    /**
     * Check if the given node has an allowed type.
     */
    dex::gdb::bool_t IsNodeTypeAllowed(dex::gdb::oid_t nodeId);
    /**
     * Check if the given node is forbidden.
     */
    dex::gdb::bool_t IsNodeExcluded(dex::gdb::oid_t node);
    /**
     * Check if the given edge is forbidden.
     */
    dex::gdb::bool_t IsEdgeExcluded(dex::gdb::oid_t edge);

private:

    /**
     * \brief Non implemented to avoid use of copy constructor.
     */
    Connectivity(const Connectivity & conn);
    /**
     * \brief Non implemented to avoid use of empty constructor.
     */
    Connectivity();
};


END_DEX_ALGORITHMS_NAMESPACE

/** @} */
#endif // _DEX_CONNECTIVITY_H_
