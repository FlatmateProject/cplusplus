/*
 * DEX: a library for out-of-core graph-based data storage and query processing.
 *
 * See the file LICENSE.txt for redistribution information.
 *
 * Copyright (c) 2005-2010 Sparsity Technologies, Barcelona (Spain)
 */

#ifndef _DEX_EDGETYPELOADER_H_
#define _DEX_EDGETYPELOADER_H_

/**
 * \addtogroup io
 * @{
 *
 * \file EdgeTypeLoader.h
 * \brief It contains the definition of EdgeTypeLoader class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */

#include "io/TypeLoader.h"


BEGIN_DEX_IO_NAMESPACE


/**
 * \brief EdgeTypeLoader class.
 *
 * Specific TypeLoader implementation for edge types.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT EdgeTypeLoader
: public TypeLoader
{
public:
    /**
     * \brief Creates a new instance.
     */
    EdgeTypeLoader()
    : TypeLoader()
    {
    }

    
    /**
     * \brief Creates a new instance.
     * \param rr [in] Input RowReader.
     * \param g [in] Graph.
     * \param t [in] Type identifier.
     * \param attrs [in] Attribute identifiers to be loaded.
     * \param attrsPos [in] Attribute positions (column index >=0).
     * to all listeners.
     * \param hPos [in] The position (index column) for the head value.
     * \param tPos [in] The position (index column) for the tail value.
     * \param hAttr [in] The attribute identifier for the head.
     * \param tAttr [in] The attribute identifier for the tail.
     */
    EdgeTypeLoader(RowReader & rr, dex::gdb::Graph & g, dex::gdb::type_t t,
                   dex::gdb::AttributeList &attrs,
                   dex::gdb::Int32List &attrsPos,
                   dex::gdb::int32_t hPos, dex::gdb::int32_t tPos,
                   dex::gdb::attr_t hAttr, dex::gdb::attr_t tAttr)
    : TypeLoader(rr, g, t, attrs, attrsPos)
    {
        SetHeadAttribute(hAttr);
        SetHeadPosition(hPos);
        SetTailAttribute(tAttr);
        SetTailPosition(tPos);
    }

    /**
     * \brief Destructor.
     */
    virtual ~EdgeTypeLoader()
    {
    }

    /**
     * \brief See TypeLoader::Run
     */
    void Run()
    throw(dex::gdb::IOException, dex::gdb::Error)
    {
        dex::io::TypeLoader::Run(ONE_PHASE, 1);
    }

    /**
     * \brief See TypeLoader::RunTwoPhases
     */
    void RunTwoPhases()
    throw(dex::gdb::IOException, dex::gdb::Error)
    {
        dex::io::TypeLoader::Run(TWO_PHASES, 1);
    }

    /**
     * \brief See TypeLoader::RunNPhases
     */
    void RunNPhases(dex::gdb::int32_t partitions)
    throw(dex::gdb::IOException, dex::gdb::Error)
    {
        dex::io::TypeLoader::Run(N_PHASES, partitions);
    }


    /**
     * \brief Sets the attribute that will be used to find the head of the edge.
     * This method is protected because only the Edge loaders should have it.
     * \param attr [in] Head Attribute
     */
    void SetHeadAttribute(dex::gdb::attr_t attr)
    {
        TypeLoader::SetHeadAttribute(attr);
    }
    /**
     * \brief Sets the position of the head attribute in the source data.
     * This method is protected because only the Edge loaders should have it.
     * \param pos [in] Head position
     */
    void SetHeadPosition(dex::gdb::int32_t pos)
    {
        TypeLoader::SetHeadPosition(pos);
    }
    /**
     * \brief Sets the attribute that will be used to find the tail of the edge.
     * This method is protected because only the Edge loaders should have it.
     * \param attr [in] Tail Attribute
     */
    void SetTailAttribute(dex::gdb::attr_t attr)
    {
        TypeLoader::SetTailAttribute(attr);
    }
    /**
     * \brief Sets the position of the tail attribute in the source data.
     * This method is protected because only the Edge loaders should have it.
     * \param pos [in] Tail position
     */
    void SetTailPosition(dex::gdb::int32_t pos)
    {
        TypeLoader::SetTailPosition(pos);
    }

};


END_DEX_IO_NAMESPACE

/** @} */
#endif // _DEX_EDGETYPELOADER_H_
