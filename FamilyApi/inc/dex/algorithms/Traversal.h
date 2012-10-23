/*
 * DEX: a library for out-of-core graph-based data storage and query processing.
 *
 * See the file LICENSE.txt for redistribution information.
 *
 * Copyright (c) 2005-2010 Sparsity Technologies, Barcelona (Spain)
 */

#ifndef _DEX_TRAVERSAL_H_
#define _DEX_TRAVERSAL_H_

/**
 * \addtogroup algorithms
 * @{
 *
 * \file Traversal.h
 * \brief It contains the definition of Traversal class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */


#include "gdb/common.h"
#include <map>
#include <vector>


/*
 * Forward declaration of dex classes.
 */
namespace dex
{
    namespace gdb
    {
        class Session;
        class Graph;
        class Objects;
    }
}


BEGIN_DEX_ALGORITHMS_NAMESPACE


/**
 * \brief Traversal class.
 * 
 * Any class implementing this abstract class can be used to traverse a graph.
 *
 * The user must set which edge types and node types can be used for the 
 * traversal. For the edge types, the user may specify how to navigate them: 
 *
 * (i) Just navigate through out-going edges from the current node.
 *
 * (ii) Just navigate through in-going edges from the current node.
 *
 * (iii) Navigate through both in-going and out-going edges from
 * the current node.
 *
 * Once the instance has been created and the allowed node and edge types
 * has been set, it can be used as an iterator, retrieving the next object
 * identifier of the traversal until there are no more.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT Traversal
{
public:
    
    /**
     * \brief Allows for traversing edges of the given type.
     * \param t [in] Edge type.
     * \param d [in] Edge direction.
     */
    virtual void AddEdgeType(dex::gdb::type_t t, dex::gdb::EdgesDirection d);
    
    /**
     * \brief Allows for traversing all edge types of the graph.
     * \param d [in] Edge direction.
     */
    virtual void AddAllEdgeTypes(dex::gdb::EdgesDirection d);
    
    /**
     * \brief Allows for traversing nodes of the given type.
     */
    virtual void AddNodeType(dex::gdb::type_t t);
    
    /**
     * \brief Allows for traversing all node types of the graph.
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
     * \brief Gets the next object of the traversal.
     * \return A node or edge identifier.
     */
    virtual dex::gdb::oid_t Next() = 0;
    
    /**
     * \brief Gets if there are more objects to be traversed.
     * \return TRUE if there are more objects, FALSE otherwise.
     */
    virtual dex::gdb::bool_t HasNext() = 0;
    
    /**
     * \brief Returns the depth of the current node.
     *
     * That is, it returns the depth of the node returned in the last
     * call to Next().
     * \return The depth of the current node.
     */
    virtual dex::gdb::int32_t GetCurrentDepth() const = 0;

    /**
     * \brief Sets the maximum hops restriction.
     *
     * All paths longer than the maximum hops restriction will be ignored.
     * \param m [in] The maximum  hops restriction.
     * It must be positive or zero. Zero, the default value, means unlimited.
     */
    virtual void SetMaximumHops(dex::gdb::int32_t maxhops);

    /**
     * \brief Destructor.
     */
    virtual ~Traversal();
    
protected:
    
    /**
     * \brief Creates a new instance.
     * \param s [in] Session to get the graph from and perform traversal.
     * \param node [in] Node to start traversal from.
     */
    Traversal(dex::gdb::Session &s, dex::gdb::oid_t node);
    
    /**
     * \brief Session.
     */
    dex::gdb::Session * sess;
    /**
     * \brief Graph.
     */
    dex::gdb::Graph * graph;
    /**
     * \brief Source node of the traversal.
     */
    dex::gdb::oid_t src;
    /**
     * \brief Allowed edge types.
     */
    std::map<dex::gdb::type_t, dex::gdb::EdgesDirection> edgeTypes;
    /**
     * \brief Allowed node types.
     */
    std::vector<dex::gdb::type_t> nodeTypes;
    /**
     * \brief Maximum number of hops allowed.
     */
    dex::gdb::int32_t maxHops;
    /**
     * \brief The set of excluded nodes.
     */
    dex::gdb::Objects *excludedNodes;
    /**
     * \brief The set of excluded edges.
     */
    dex::gdb::Objects *excludedEdges;

    
    /**
     * Check that edges had been added.
     */
    void AssertAddedEdges();
    /**
     * Check that nodes had been added.
     */
    void AssertAddedNodes();
    /**
     * Check that the given edge type is valid
     */
    void AssertEdgeType(dex::gdb::type_t edgetype);
    /**
     * Check that the given node type is valid
     */
    void AssertNodeType(dex::gdb::type_t nodetype);
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
    Traversal(const Traversal & trav);
    /**
     * \brief Non implemented to avoid use of empty constructor.
     */
    Traversal();    
};


END_DEX_ALGORITHMS_NAMESPACE

/** @} */
#endif // _DEX_TRAVERSAL_H_
