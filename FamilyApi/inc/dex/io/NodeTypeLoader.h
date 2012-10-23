/*
 * DEX: a library for out-of-core graph-based data storage and query processing.
 *
 * See the file LICENSE.txt for redistribution information.
 *
 * Copyright (c) 2005-2010 Sparsity Technologies, Barcelona (Spain)
 */

#ifndef _DEX_NODETYPELOADER_H_
#define _DEX_NODETYPELOADER_H_

/**
 * \addtogroup io
 * @{
 *
 * \file NodeTypeLoader.h
 * \brief It contains the definition of NodeTypeLoader class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */

#include "io/TypeLoader.h"


BEGIN_DEX_IO_NAMESPACE


/**
 * \brief NodeTypeLoader class.
 *
 * Specific TypeLoader implementation for node types.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT NodeTypeLoader
: public TypeLoader
{
public:
    
    /**
     * \brief Creates a new instance.
     */
    NodeTypeLoader()
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
     */
    NodeTypeLoader(RowReader & rr, dex::gdb::Graph & g, dex::gdb::type_t t,
                   dex::gdb::AttributeList &attrs,
                   dex::gdb::Int32List &attrsPos)
    : TypeLoader(rr, g, t, attrs, attrsPos)
    {
    }

    /**
     * \brief Destructor.
     */
    virtual ~NodeTypeLoader()
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
};


END_DEX_IO_NAMESPACE

/** @} */
#endif // _DEX_NODETYPELOADER_H_
