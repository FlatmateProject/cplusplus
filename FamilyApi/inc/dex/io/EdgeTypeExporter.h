/*
 * DEX: a library for out-of-core graph-based data storage and query processing.
 *
 * See the file LICENSE.txt for redistribution information.
 *
 * Copyright (c) 2005-2010 Sparsity Technologies, Barcelona (Spain)
 */

#ifndef _DEX_EDGETYPEEXPORTER_H_
#define _DEX_EDGETYPEEXPORTER_H_

/**
 * \addtogroup io
 * @{
 *
 * \file EdgeTypeExporter.h
 * \brief It contains the definition of EdgeTypeExporter class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */

#include "io/TypeExporter.h"


BEGIN_DEX_IO_NAMESPACE


/**
 * \brief EdgeTypeExporter class.
 *
 * Specific TypeExporter implementation for edge types.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT EdgeTypeExporter
: public TypeExporter
{
public:
    
    /**
     * \brief Creates a new instance.
     */
    EdgeTypeExporter()
    : TypeExporter()
    {
    }


    /**
     * \brief Creates a new instance.
     * \param rw [in] Output RowWriter.
     * \param g [in] Graph.
     * \param t [in] Type identifier.
     * \param attrs [in] Attribute identifiers to be exported.
     * \param hPos [in] The position (index column) for the head value.
     * \param tPos [in] The position (index column) for the tail value.
     * \param hAttr [in] The attribute identifier to get the value to be dumped 
     * for the head.
     * \param tAttr [in] The attribute identifier to get the value to be dumped 
     * for the tail.
     */
    EdgeTypeExporter(RowWriter & rw, dex::gdb::Graph & g, dex::gdb::type_t t,
                     dex::gdb::AttributeList & attrs, 
                     dex::gdb::int32_t hPos, dex::gdb::int32_t tPos,
                     dex::gdb::attr_t hAttr, dex::gdb::attr_t tAttr)
    : TypeExporter(rw, g, t, attrs)
    {
        SetHeadAttribute(hAttr);
        SetHeadPosition(hPos);
        SetTailAttribute(tAttr);
        SetTailPosition(tPos);
    }

    /**
     * \brief Destructor.
     */
    virtual ~EdgeTypeExporter()
    {
    }

    /**
     * \brief See TypeExporter::Run
     */
    void Run() throw(dex::gdb::IOException, dex::gdb::Error) 
    {
        dex::io::TypeExporter::RunProcess();
    }


    /**
     * \brief Sets the attribute that will be used to get the value to be dumped
     * for the head of the edge.
     * \param attr [in] Head Attribute
     */
    void SetHeadAttribute(dex::gdb::attr_t attr)
    {
        TypeExporter::SetHeadAttribute(attr);
    }

    /**
     * \brief Sets the position (index column) of the head attribute in the exported data.
     * \param pos [in] Head position
     */
    void SetHeadPosition(dex::gdb::int32_t pos)
    {
        TypeExporter::SetHeadPosition(pos);
    }

    /**
     * \brief Sets the attribute that will be used to get the value to be dumped
     * for the tail of the edge.
     * \param attr [in] Tail Attribute
     */
    void SetTailAttribute(dex::gdb::attr_t attr)
    {
        TypeExporter::SetTailAttribute(attr);
    }

    /**
     * \brief Sets the position (index column) of the tail attribute in the exported data.
     * \param pos [in] Tail position
     */
    void SetTailPosition(dex::gdb::int32_t pos)
    {
        TypeExporter::SetTailPosition(pos);
    }
};


END_DEX_IO_NAMESPACE

/** @} */
#endif // _DEX_EDGETYPEEXPORTER_H_
