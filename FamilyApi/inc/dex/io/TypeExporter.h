/*
 * DEX: a library for out-of-core graph-based data storage and query processing.
 *
 * See the file LICENSE.txt for redistribution information.
 *
 * Copyright (c) 2005-2010 Sparsity Technologies, Barcelona (Spain)
 */

#ifndef _DEX_TYPEEXPORTER_H_
#define _DEX_TYPEEXPORTER_H_

/**
 * \addtogroup io
 * @{
 *
 * \file TypeExporter.h
 * \brief It contains the definition of TypeExporter classes.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */

#include "io/RowWriter.h"
#include "gdb/Exception.h"
#include "gdb/Graph.h"
#include <vector>


BEGIN_DEX_IO_NAMESPACE


/**
 * \brief Provides information about the progress of an type export process.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT TypeExporterEvent 
{
    friend class TypeExporter; // to call constructor and set values
    
private:
    
    /**
     * \brief Object type identifier.
     */
    dex::gdb::type_t type;
    /**
     * \brief Current number of objects exported.
     */
    dex::gdb::int64_t count;
    /**
     * \brief Total number of objects exported.
     */
    dex::gdb::int64_t total;
    /**
     * \brief Last event or not.
     */
    dex::gdb::bool_t last;

    /**
     * \brief Creates a new instance.
     */
    TypeExporterEvent()
    {
    }
    
public:
    
    /**
     * \brief Destructor.
     */
    virtual ~TypeExporterEvent()
    {
    }
    
    /**
     * \brief Gets the type identifier.
     * \return The type identifier.
     */
    dex::gdb::type_t GetTypeId() const 
    {
        return type;
    }
    
    /**
     * \brief Gets the current number of objects exported.
     * \return The current number of objects exported.
     */
    dex::gdb::int64_t GetCount() const
    {
        return count;
    }
    
    /**
     * \brief Gets the total number of objects exported.
     * \return The total number of objects exported.
     */
    dex::gdb::int64_t GetTotal() const
    {
        return total;
    }
    
    /**
     * \brief Gets if this is the last event or not.
     * \return TRUE if this is the last event, FALSE otherwise.
     */
    dex::gdb::bool_t IsLast() const
    {
        return last;
    }
    
private:
    
    /**
     * \brief Non implemented to avoid use of copy constructor.
     */
    TypeExporterEvent(const TypeExporterEvent & tee);
    
    /**
     * \brief Non implemented to avoid use of assignment operator.
     */
    TypeExporterEvent & operator =(const TypeExporterEvent & tee);
};


/**
 * \brief Easy STL printing operator redefinition.
 */
std::wostream & operator << (std::wostream & wostrm, 
                             const TypeExporterEvent & ev);


/**
 * \brief Interface to be implemented to receive TypeExporterEvent events 
 * from a TypeExporter.
 * 
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT TypeExporterListener 
{
public:
    
    /**
     * \brief Method to be notified from a TypeExporter.
     * \param tee [in] Notified event.
     */
    virtual void NotifyEvent(const TypeExporterEvent & tee) = 0;

    /**
     * \brief Destructor.
     */
    virtual ~TypeExporterListener()
    {
    }
    
private:
    
    /**
     * \brief Non implemented to avoid use of copy constructor.
     */
    TypeExporterListener(const TypeExporterListener & tel);
    
    /**
     * \brief Non implemented to avoid use of assignment operator.
     */
    TypeExporterListener & operator =(const TypeExporterListener & tel);
    
protected:
    
    /**
     * Protected because none should instantiate a RowWriter. Just inherited
     * classes may use this empty constructor.
     */
    TypeExporterListener()
    {
    }
};


/**
 * \brief Base TypeExporter class.
 *
 * Base class to export a node or edge type from a graph using a RowWriter.
 * 
 * TypeExporterListener can be registered to receive information about
 * the progress of the export process by means of TypeExporterEvent.
 * 
 * The default frequency of notification to listeners is 100000.
 *
 * By default no header row is created.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT TypeExporter
{
private:
    
    /**
     * \brief Output RowWriter.
     */
    RowWriter * rowWriter;
    /**
     * \brief Object type identifier to be exported.
     */
    dex::gdb::type_t type;
    /**
     * \brief Type data of the type to be exported.
     */
    const dex::gdb::Type * tdata;
    /**
     * \brief Stores if the type to be exported corresponds to a node type or
     * not (edge type).
     */
    dex::gdb::bool_t isNodeType;
    /**
     * \brief Graph to be exported.
     */
    dex::gdb::Graph * graph;
    /**
     * \brief Frequency.
     *
     * An ExporterEvent is sent to all listeners when
     * frequency steps have been executed.
     */
    dex::gdb::int32_t frequency;
    /**
     * \brief List of listeners.
     */
    std::vector<TypeExporterListener *> listeners;
    /**
     * \brief If TRUE, then print attributes information on the first row.
     */
    dex::gdb::bool_t printHeader;
    /**
     * \brief Attribute identifiers to be exported.
     */
    std::vector<dex::gdb::attr_t> attributes;
    /**
     * \brief Attribute data for all the attribute identifiers to be exported.
     */
    std::vector<const dex::gdb::Attribute *> attributesData;
    /**
     * \brief In case of export an edge type, the position (index column)
     * for the head value, -1 otherwise.
     */
    dex::gdb::int32_t headPos;
    /**
     * \brief In case of export an edge type, the position (index column)
     * for the tail value, -1 otherwise.
     */
    dex::gdb::int32_t tailPos;
    /**
     * \brief In case of export an edge type, the attribute identifier
     * to get the value to be dumped for the head, InvalidAttribute otherwise.
     */
    dex::gdb::attr_t headAttr;
    /**
     * \brief In case of export and edge, the attribute data corresponding to
     * headAttr, NULL otherwise.
     */
    const dex::gdb::Attribute * headAttrData;
    /**
     * \brief In case of export an edge type, the attribute identifier
     * to get the value to be dumped for the tail, InvalidAttribute otherwise.
     */
    dex::gdb::attr_t tailAttr;
    /**
     * \brief In case of export and edge, the attribute data corresponding to
     * tailAttr, NULL otherwise.
     */
    const dex::gdb::Attribute * tailAttrData;
    
    /**
     * \brief Default Frequency.
     */
    static dex::gdb::int32_t DefaultFrequency;
    /**
     * \brief Default Frequency.
     */
    static dex::gdb::bool_t DefaultHeader;


protected:
    
    /**
     * \brief Creates a new instance.
     */
    TypeExporter();

    /**
     * \brief Creates a new instance with the minimum common required arguments.
     * \param rw [in] Output RowWriter.
     * \param g [in] Graph.
     * \param t [in] Type identifier.
     * \param attrs [in] Attribute identifiers to be exported.
     */
    TypeExporter(RowWriter & rw, dex::gdb::Graph & g, dex::gdb::type_t t,
                 dex::gdb::AttributeList & attrs);

    /**
     * \brief Checks that all the required settings are ready to run.
     * \return Returns true if all the settings are ready.
     */
    dex::gdb::bool_t CanRun();
    
    /**
     * \brief Notifies progress to all registered listeners.
     * \param ev [in] Progress event to be notified.
     */
    void NotifyListeners(const TypeExporterEvent & ev);
    
    /**
     * \brief Runs export process.
     * \exception IOException If bad things happen writting to the RowWriter.
     */
    void RunProcess() 
    throw(dex::gdb::IOException, dex::gdb::Error);

    /**
     * \brief Sets the attribute that will be used to get the value to be dumped
     * for the head of the edge.
     * This method is protected because only the Edge exporters should have it.
     * \param attr [in] Head Attribute
     */
    void SetHeadAttribute(dex::gdb::attr_t attr);
    /**
     * \brief Sets the position (index column) of the head attribute in the exported data.
     * This method is protected because only the Edge exporters should have it.
     * \param pos [in] Head position
     */
    void SetHeadPosition(dex::gdb::int32_t pos);
    /**
     * \brief Sets the attribute that will be used to get the value to be dumped
     * for the tail of the edge.
     * This method is protected because only the Edge exporters should have it.
     * \param attr [in] Tail Attribute
     */
    void SetTailAttribute(dex::gdb::attr_t attr);
    /**
     * \brief Sets the position (index column) of the tail attribute in the exported data.
     * This method is protected because only the Edge exporters should have it.
     * \param pos [in] Tail position
     */
    void SetTailPosition(dex::gdb::int32_t pos);
    
public:
    
    /**
     * \brief Destructor.
     */
    virtual ~TypeExporter();
    
    /**
     * \brief Registers a new listener.
     * \param tel [in] TypeExporterListener to be registered.
     */
    void Register(TypeExporterListener & tel);

    /**
     * \brief Runs export process.
     * \exception IOException If bad things happen writing to the RowWriter.
     */
    virtual void Run() 
    throw(dex::gdb::IOException, dex::gdb::Error) = 0;

    /**
     * \brief Sets the output data destination.
     * \param rr [in] Input RowWriter.
     */
    void SetRowWriter(RowWriter & rw);

    /**
     * \brief Sets the graph that will be exported.
     * \param g [in] Graph.
     */
    void SetGraph(dex::gdb::Graph & g);

    /**
     * \brief Sets the type to be exported.
     * \param t [in] Type identifier.
     */
    void SetType(dex::gdb::type_t t);

    /**
     * \brief Sets the list of Attributes.
     * \param attrs [in] Attribute identifiers to be exported
     */
    void SetAttributes(dex::gdb::AttributeList &attrs);

    /**
     * \brief Sets the frequency of listener notification.
     * freq [in] Frequency in number of rows managed to notify progress to all listeners
     */
    void SetFrequency(dex::gdb::int32_t freq);

    /**
     * \brief Sets the presence of a header row.
     * \param header [in] If TRUE, a header row is dumped with the name of
     * the attributes.
     */
    void SetHeader(dex::gdb::bool_t header);
};


END_DEX_IO_NAMESPACE

/** @} */
#endif // _DEX_TYPEEXPORTER_H_
