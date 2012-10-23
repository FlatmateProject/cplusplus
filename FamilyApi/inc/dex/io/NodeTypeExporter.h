/*
 * DEX: a library for out-of-core graph-based data storage and query processing.
 *
 * See the file LICENSE.txt for redistribution information.
 *
 * Copyright (c) 2005-2010 Sparsity Technologies, Barcelona (Spain)
 */

#ifndef _DEX_NODETYPEEXPORTER_H_
#define _DEX_NODETYPEEXPORTER_H_

/**
 * \addtogroup io
 * @{
 *
 * \file NodeTypeExporter.h
 * \brief It contains the definition of NodeTypeExporter class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */

#include "io/TypeExporter.h"


BEGIN_DEX_IO_NAMESPACE


/**
 * \brief NodeTypeExporter class.
 *
 * Specific TypeExporter implementation for node types.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT NodeTypeExporter
: public TypeExporter
{
public:
    
    /**
     * \brief Creates a new instance.
     */
    NodeTypeExporter()
    : TypeExporter()
    {
    }

    /**
     * \brief Creates a new instance.
     * \param rw [in] Output RowWriter.
     * \param g [in] Graph.
     * \param t [in] Type identifier.
     * \param attrs [in] Attribute identifiers to be exported.
     */
    NodeTypeExporter(RowWriter & rw, dex::gdb::Graph & g, dex::gdb::type_t t,
                     dex::gdb::AttributeList & attrs)
    : TypeExporter(rw, g, t, attrs)
    {
    }

    /**
     * \brief Destructor.
     */
    virtual ~NodeTypeExporter()
    {
    }

    /**
     * \brief See TypeExporter::Run
     */
    void Run() throw(dex::gdb::IOException, dex::gdb::Error)
    {
        dex::io::TypeExporter::RunProcess();
    }

};


END_DEX_IO_NAMESPACE

/** @} */
#endif // _DEX_NODETYPEEXPORTER_H_
