/*
 * DEX: a library for out-of-core graph-based data storage and query processing.
 *
 * See the file LICENSE.txt for redistribution information.
 *
 * Copyright (c) 2005-2010 Sparsity Technologies, Barcelona (Spain)
 */

#ifndef _DEX_SESSION_H_
#define _DEX_SESSION_H_

/**
 * \addtogroup gdb
 * @{
 *
 * \file Session.h
 * \brief It contains the definition of Session class.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */

#include "gdb/Handler.h"

/*
 * Forward declaration of dexcore classes.
 */
namespace dexcore
{
    class Session;
}

BEGIN_DEX_GDB_NAMESPACE

/*
 * Forward declaration of dex classes.
 */
class Database;
class Graph;
class Objects;


/**
 * \brief Session class.
 *
 * A Session is a stateful period of activity of a user with the Database.

 * All the manipulation of a Database must be enclosed into a Session.
 * A Session can be initiated from a Database instance and allows for
 * getting a Graph instance which represents the persistent graph 
 * (the graph database).
 *
 * Also, temporary data is associated to the Session, thus when a Session
 * is closed, all the temporary data associated to the Session
 * is removed too. Objects or Values instances or even session attributes 
 * are an example of temporary data.
 *
 * Moreover, a Session is exclusive for a thread, thus if it is shared
 * among threads results may be fatal or unexpected.
 *
 * <b>Transactions</b>
 *
 * A Session allows for enclosing a set of graph operations into
 * a transaction. A transaction defines a granurality level
 * for concurrent execution of Sessions. The explicit use of transactions
 * may improve the performance of concurrent execution of Sessions.
 *
 * All operations within a transaction are considered an execution unit.
 * By default, if no transactions are defined by the user, all operations
 * behave as autocommit, that is a transaction is created just before
 * each method and closed when the method finishes.
 *
 * For the moment, transactions have a partial support of the ACID properties.
 *
 * <i>Transaction type</i>
 *
 * There are two types of transactions: Read or Shared, and Write or Exclusive.
 * DEX's concurrency model is based in a N-readers 1-writer model, thus
 * read transactions can be executed concurrently whereas write transactions
 * are executed exclusively.
 *
 * The type of a transaction is defined for the operations it contains.
 * Initially, a transaction starts as a read transaction and just when
 * a method which updates the persistent graph database is found it 
 * automatically becomes a write transaction. Obviously, to become a write
 * transaction this must wait until all other read transactions have finished.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT Session
: private Handler<dexcore::Session>
{
    friend class Database; // to access private constructor
    friend class Graph; // to access dexcore handled reference
    friend class Objects; // to access dexcore handled reference
    friend class ObjectsIterator;  // to access dexcore handled reference
    friend class Values; // to access dexcore handled reference
    friend class ValuesIterator; // to access dexcore handled reference
    friend class TextStream; // to access dexcore handled reference
    
public:
    
    /**
     * \brief Destructor.
     */
    virtual ~Session();
    
    /**
     * \brief Gets the Graph instance.
     * \return The Graph instance.
     */
    Graph * GetGraph();
    
    /**
     * \brief Creates a new Objects instance.
     * \return The new Objects instance.
     */
    Objects * NewObjects();
    
    /**
     * \brief Begins a transaction.
     */
    void Begin();
    
    /**
     * \brief Commits a transaction.
     */
    void Commit();
    
private:
    
    /**
     * \brief Creates a new instance.
     * \param core_sess [in] Handled dexcore Session reference.
     * \param db [in] Parent Database reference.
     */
    Session(dexcore::Session * core_sess, Database * db);
    
    /**
     * \brief Non implemented to avoid use of copy constructor.
     */
    Session(const Session & session);

    /**
     * \brief Non implemented to avoid use of asignment operator.
     */
    Session & operator =(const Session & session);
    
    /**
     * \brief Parent Database reference.
     */
    Database * db;
};

END_DEX_GDB_NAMESPACE

/** @} */
#endif // _DEX_SESSION_H_
