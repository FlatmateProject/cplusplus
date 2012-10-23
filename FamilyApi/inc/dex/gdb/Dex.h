/*
 * DEX: a library for out-of-core graph-based data storage and query processing.
 *
 * See the file LICENSE.txt for redistribution information.
 *
 * Copyright (c) 2005-2010 Sparsity Technologies, Barcelona (Spain)
 */

#ifndef _DEX_DEX_H_
#define _DEX_DEX_H_

/**
 * \addtogroup gdb
 * @{
 *
 * \file Dex.h
 * \brief It contains the declaration of Dex and DexConfig classes.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */

#include "gdb/Handler.h"
#include <vector>

/*
 * Forward declaration of dexcore classes.
 */
namespace dexcore
{
    class Properties;
    class DEX;
    class LogConsumer;
}

BEGIN_DEX_GDB_NAMESPACE

/*
 * Forward declaration of dex classes.
 */
class Dex;
class Database;


/**
 * \brief Dex properties file.
 *
 * This class is implemented as a singleton, so all public methods are
 * static.
 *
 * It allows for getting the property values stored in a properties file. 
 * A properties file is a file where there is one line per property.
 * A property is defined by a key and a value as follows: key=value
 *
 * By default, this loads properties from the file './dex.cfg'.
 * The user may choose to load a different file by calling the method Load().
 *
 * If the default properties file or the one loaded by the user 
 * do not exist, then this behaves as loading an empty properties file.
 */
class DEX_EXPORT DexProperties
{
private:

    /**
     * \brief Non implemented to avoid use of empty constructor.
     */
    DexProperties();
    
    /**
     * \brief Non implemented to avoid use of copy constructor.
     */
    DexProperties(const DexProperties &props);
    
    /**
     * \brief Non implemented to avoid use of assignment operator.
     */
    DexProperties & operator =(const DexProperties &props);
    
    /**
     * \brief Loaded properties instance.
     */
    static dexcore::Properties * props;
    
    /**
     * \brief Default properties file to load in case none is given by the user.
     */
    static const std::wstring DefaultPropertiesFile;
    
    /**
     * \brief Auxiliary string to return string properties.
     *
     * IMPORTANT: This may be a race condition in a concurrent scenario. 
     * But DexProperties should never be used in a concurrent scenario.
     */
    static std::wstring aux;

public:
    
    /**
     * \brief Loads properties from the given file path.
     * \param path [in] File path to load properties from.
     */
    static void Load(const std::wstring & path);
    
    /**
     * Gets a property.
     * \param key [in] The name of the property to lookup.
     * \param def [in] Default value to be returned in case there is no
     * property with the name key.
     * \return The value of the property, or def if the key is not found.
     */
    static const std::wstring & Get(const std::wstring & key, const std::wstring & def);
    
    /**
     * Gets a property as an integer.
     * \param key [in] The name of the property to lookup.
     * \param def [in] Default value to be returned in case there is no
     * property with the name key.
     * \return The property value, or def if the key is not found or in 
     * case or error.
     */
    static int32_t GetInteger(const std::wstring & key, int32_t def);

    /**
     * Gets a property as a boolean.
     * \param key [in] The name of the property to lookup.
     * \param def [in] Default value to be returned in case there is no
     * property with the name key.
     * \return The property value, or def if the key is not found or in 
     * case or error.
     */
    static bool_t GetBoolean(const std::wstring & key, bool_t def);
};


/**
 * \brief Dex configuration class.
 *
 * If not specified, 0 means unlimited which is the maximum available.
 * For the pools that's the total cache size.
 * For the cache unlimited means nearly all the physical memory of the computer.
 * 
 * For each field, there is a default value. This value can be overrided
 * with values from a properties file (see DexProperties class). Also, 
 * this settings can be overrided calling a specific setter.
 *
 * For each field, it is shown its default value and the property to override
 * this value:
 *
 * Extent size: 4KB ('dex.storage.extentsize' at DexProperties).
 * 
 * Pages per extent: 1 page ('dex.storage.extentpages' at DexProperties).
 * 
 * Pool frame size: 1 extent ('dex.io.pool.frame.size' at DexProperties).
 *
 * Minimum size for the persistent pool: 64 frames ('dex.io.pool.persistent.minsize' at DexProperties).
 *
 * Maximum size for the persistent pool: 0 frames ('dex.io.pool.persistent.maxsize' at DexProperties).
 *
 * Minimum size for the temporary pool: 16 frames ('dex.io.pool.temporal.minsize' at DexProperties).
 *
 * Maximum size for the temporary pool: 0 frames ('dex.io.pool.temporal.maxsize' at DexProperties).
 *
 * Maximum size for the cache (all pools): 0 MB ('dex.io.cache.maxsize' at DexProperties).
 *
 * License code: "" ('dex.license' at DexProperties). No license code means evaluation license.
 *
 * Log level: LogLevel::Info ('dex.log.level' at DexProperties).
 *
 * Log file: "dex.log" ('dex.log.file' at DexProperties).
 *
 * Cache statistics: false (disabled) ('dex.cache.statistics' at DexProperties).
 *
 * Cache statistics log file: "statistics.log" ('dex.cache.statisticsFile' at DexProperties).
 *
 * Cache statistics snapshot time: 1000 msecs ('dex.cache.statisticsSnapshotTime' at DexProperties).
 * 
 * Recovery enabled: false ('dex.io.recovery' at DexProperties).
 * 
 * Recovery log file: "" ('dex.io.recovery.logfile' at DexProperties).
 * 
 * Recovery cache max size: 1MB ('dex.io.recovery.cachesize' at DexProperties).
 * 
 * Recovery checkpoint time: 60 seconds ('dex.io.recovery.checkpointTime' at DexProperties).
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT DexConfig
{
private:
    /**
     * \brief Size of the storage extent (in KB).
     */
    int32_t extentSize;
    
    /**
     * \brief Number of pages per extent.
     */
    int32_t extentPages;
    
    /**
     * \brief Size of a pool frame in number of extents.
     */
    int32_t poolFrameSize;
    
    /**
     * \brief Minimum size for the persistent pool in number of frames.
     */
    int32_t poolPersistentMinSize;
    
    /**
     * \brief Maximum size for the persistent pool in number of frames.
     */
    int32_t poolPersistentMaxSize;
    
    /**
     * \brief Minimum size for the temporary pool in number of frames.
     */
    int32_t poolTemporaryMinSize;
    
    /**
     * \brief Maximum size for the temporary pool in number of frames.
     */
    int32_t poolTemporaryMaxSize;
    
    /**
     * \brief Maximum size for the cache (all pools) in MB.
     */
    int32_t cacheMaxSize;
    
    /**
     * \brief License code.
     */
    std::wstring license;
    
    /**
     * \brief Log file.
     */
    std::wstring logFile;
    
    /**
     * \brief LogLevel.
     */
    LogLevel logLevel;
    
    /**
     * \brief Cache statistics enabled or not.
     */
    bool_t cacheStats;
    
    /**
     * \brief Cache statistics log file.
     *
     * Useless if cache statistics are disabled.
     */
    std::wstring cacheStatsFile;
    
    /**
     * \brief Cache statistics snapshot time in miliseconds.
     *
     * Useless if cache statistics are disabled.
     */
    int32_t cacheStatsSnapshotTime;    
    
    /**
     * \brief Recovery enabled or not.
     */
    bool_t recoveryEnabled;
    
    /**
     * \brief Recovery log file.
     */
    std::wstring recoveryLogFile;
    
    /**
     * \brief Maximum size for the recovery log cache in extents.
     */
    int32_t recoveryCacheMaxSize;
    
    /**
     * \brief Delay time (in seconds) between automatic checkpoints.
     */
    int32_t recoveryCheckpointTime;
    
    /**
     * \brief Non implemented to avoid use of copy constructor.
     */
    DexConfig(const DexConfig &cfg);
    
    /**
     * \brief Non implemented to avoid use of assignment operator.
     */
    DexConfig & operator =(const DexConfig &cfg);
    
public:
    
    /**
     * \brief Creates a new instance.
     *
     * Values are set with default values.
     */
    DexConfig();
    
    /**
     * \brief Gets the size of a extent.
     * \return The size of a extent in KB.
     */
    int32_t GetExtentSize() const
    {
        return extentSize;
    }
    
    /**
     * \brief Sets the size of a pool frame in number of extents.
     * \param v [in] The size of a extent size in KB.
     * An extent can have a size between 4KB and 64KB, and it must be a power of 2.
     */
    void SetExtentSize(int32_t v)
    {
        if ((v != 4) && (v != 8) && (v != 16) && (v != 32) && (v != 64))
            throw WrongArgumentError("The extent size must be a power of 2 between 4KB and 64KB.");
        extentSize = v;
    }
    
    /**
     * \brief Gets the number of pages per extent.
     * \return The number of pages per extent.
     */
    int32_t GetExtentPages() const
    {
        return extentPages;
    }
    
    /**
     * \brief Sets the number of pages per extent.
     * \param v [in] The number of pages.
     * It must be at least 1 page and the page size must be greater than or equal to 4KB.
     */
    void SetExtentPages(int32_t v)
    {
        if (v < 1)
            throw WrongArgumentError("The number of pages must be at least 1 per extent.");
        if (v > extentSize/4)
            throw WrongArgumentError("Too many pages per extent. The page size must be greater than or equal to 4KB.");
        extentPages = v;
    }
    
    /**
     * \brief Gets the size of a pool frame in number of extents.
     * \return The size of a pool frame in number of extents.
     */
    int32_t GetPoolFrameSize() const
    {
        return poolFrameSize;
    }
    
    /**
     * \brief Sets the size of a pool frame in number of extents.
     * \param v [in] The size of a pool frame in number of extents.
     * It must be non-negative.
     */
    void SetPoolFrameSize(int32_t v)
    {
        if (0 > v) throw WrongArgumentError("Expected a non-negative value");
        poolFrameSize = v;
    }
    
    /**
     * \brief Gets the minimum size for the persistent pool in number of frames.
     * \return The minimum size for the persistent pool in number of frames.
     */
    int32_t GetPoolPersistentMinSize() const
    {
        return poolPersistentMinSize;
    }
    
    /**
     * \brief Sets the minimum size for the persistent pool in number of frames.
     * \param v [in] The minimum size for the persistent pool in number of frames.
     * It must be non-negative.
     */
    void SetPoolPersistentMinSize(int32_t v)
    {
        if (0 > v) throw WrongArgumentError("Expected a non-negative value");
        poolPersistentMinSize = v;
    }
    
    /**
     * \brief Gets the maximum size for the persistent pool in number of frames.
     * \return The maximum size for the persistent pool in number of frames.
     */
    int32_t GetPoolPersistentMaxSize() const
    {
        return poolPersistentMaxSize;
    }
    
    /**
     * \brief Sets the maximum size for the persistent pool in number of frames.
     * \param v [in] The maximum size for the persistent pool in number of frames.
     * It must be non-negative.
     */
    void SetPoolPersistentMaxSize(int32_t v)
    {
        if (0 > v) throw WrongArgumentError("Expected a non-negative value");
        poolPersistentMaxSize = v;
    }
    
    /**
     * \brief Gets the minimum size for the temporary pool in number of frames.
     * \return The minimum size for the temporary pool in number of frames.
     */
    int32_t GetPoolTemporaryMinSize() const
    {
        return poolTemporaryMinSize;
    }
    
    /**
     * \brief Sets the minimum size for the temporary pool in number of frames.
     * \param v [in] The minimum size for the temporary pool in number of frames.
     * It must be non-negative.
     */
    void SetPoolTemporaryMinSize(int32_t v)
    {
        if (0 > v) throw WrongArgumentError("Expected a non-negative value");
        poolTemporaryMinSize = v;
    }
    
    /**
     * \brief Gets the maximum size for the temporary pool in number of frames.
     * \return The maximum size for the temporary pool in number of frames.
     */
    int32_t GetPoolTemporaryMaxSize() const
    {
        return poolTemporaryMaxSize;
    }
    
    /**
     * \brief Sets the maximum size for the temporary pool in number of frames.
     * \param v [in] The maximum size for the temporary pool in number of frames.
     * It must be non-negative.
     */
    void SetPoolTemporaryMaxSize(int32_t v)
    {
        if (0 > v) throw WrongArgumentError("Expected a non-negative value");
        poolTemporaryMaxSize = v;
    }
    
    /**
     * \brief Gets the maximum size for the cache (all pools) in MB.
     * \return The maximum size for the cache (all pools) in MB.
     */
    int32_t GetCacheMaxSize() const
    {
        return cacheMaxSize;
    }
    
    /**
     * \brief Sets the maximum size for the cache (all pools) in MB.
     * \param v [in] The maximum size for the cache (all pools) in MB.
     * It must be non-negative.
     */
    void SetCacheMaxSize(int32_t v)
    {
        if (0 > v) throw WrongArgumentError("Expected a non-negative value");
        cacheMaxSize = v;
    }
    
    /**
     * \brief Gets the license code.
     * \return The license code.
     */
    const std::wstring & GetLicense() const
    {
        return license;
    }
    
    /**
     * \brief Sets the license code.
     * \param v [in] The license code.
     */
    void SetLicense(const std::wstring &v) 
    {
        license = v;
    }
    
    /**
     * \brief Gets the log file.
     * \return The log file.
     */
    const std::wstring & GetLogFile() const
    {
        return logFile;
    }
    
    /**
     * \brief Sets the log file.
     * \param v [in] The log file.
     */
    void SetLogFile(const std::wstring & v)
    {
        logFile = v;
    }
    
    /**
     * \brief Gets the log level.
     * \return The LogLevel.
     */
    LogLevel GetLogLevel() const
    {
        return logLevel;
    }
    
    /**
     * \brief Sets the log level.
     * \param v [in] The LogLevel.
     */
    void SetLogLevel(LogLevel v)
    {
        logLevel = v;
    }
    
    /**
     * \brief Gets whether cache statistics are enabled or disabled.
     * \return TRUE if cache statistics are enabled, FALSE otherwise.
     */
    bool_t GetCacheStatisticsEnabled() const
    {
        return cacheStats;
    }
    
    /**
     * \brief Enables or disables cache statistics.
     * \param v [in] If TRUE this enables cache statistics, if FALSE this
     * disables cache statistics.
     */
    void SetCacheStatisticsEnabled(bool_t v)
    {
        cacheStats = v;
    }
    
    /**
     * \brief Gets the cache statistics log file.
     *
     * Useless if cache statistics are disabled.
     * \return The cache statistics log file.
     */
    const std::wstring & GetCacheStatisticsFile() const
    {
        return cacheStatsFile;
    }
    
    /**
     * \brief Sets the cache statistics log file.
     *
     * Useless if cache statistics are disabled.
     * \param v [in] The cache statistics log file.
     */
    void SetCacheStatisticsFile(const std::wstring & v)
    {
        cacheStatsFile = v;
    }
    
    /**
     * \brief Gets the cache statistics snapshot time in miliseconds.
     * 
     * Useless if cache statistics are disabled.
     * \return The cache statistics snapshot time in miliseconds.
     */
    int32_t GetCacheStatisticsSnapshotTime() const
    {
        return cacheStatsSnapshotTime;
    }
    
    /**
     * \brief Sets the cache statistics snapshot time.
     *
     * Useless if cache statistics are disabled.
     * \param v [in] The cache statistics snapshot time in miliseconds.
     */
    void SetCacheStatisticsSnapshotTime(int32_t v)
    {
        if (0 >= v) throw WrongArgumentError("Expected a positive value");
        cacheStatsSnapshotTime = v;
    }

    /**
     * \brief Gets whether the recovery is enabled or disabled.
     * \return TRUE if the recovery is enabled, FALSE otherwise.
     */
    bool_t GetRecoveryEnabled() const
    {
        return recoveryEnabled;
    }
    
    /**
     * \brief Enables or disables the recovery.
     * \param v [in] If TRUE this enables the recovery, if FALSE then
     * disables it.
     */
    void SetRecoveryEnabled(bool_t v)
    {
        recoveryEnabled = v;
    }
    
    /**
     * \brief Gets the recovery log file.
     * \return The recovery log file.
     */
    const std::wstring & GetRecoveryLogFile() const
    {
        return recoveryLogFile;
    }
    
    /**
     * \brief Sets the recovery log file.
     * \param v [in] The recovery log file.
     * Left it empty for the default log file (same as <database_file_name>.log)
     */
    void SetRecoveryLogFile(const std::wstring & v)
    {
        recoveryLogFile = v;
    }
    
    /**
     * \brief Gets the maximum size for the recovery log cache in extents.
     * \return The maximum size for the recovery log cache in extents.
     */
    int32_t GetRecoveryCacheMaxSize() const
    {
        return recoveryCacheMaxSize;
    }
    
    /**
     * \brief Sets the maximum size for the recovery log cache in extents.
     * \param v [in] The maximum size for the recovery log cache in extents.
     * A 0 sets the default value (extents up to 1MB).
     */
    void SetRecoveryCacheMaxSize(int32_t v)
    {
        if (0 > v) throw WrongArgumentError("Expected a non-negative value");
        recoveryCacheMaxSize = v;
    }
    
    /**
     * \brief Gets the delay time (in seconds) between automatic checkpoints.
     * \return The delay time (in seconds) between automatic checkpoints.
     */
    int32_t GetRecoveryCheckpointTime() const
    {
        return recoveryCheckpointTime;
    }
    
    /**
     * \brief Sets the delay time (in seconds) between automatic checkpoints.
     * \param v [in] The delay time (in seconds) between automatic checkpoints.
     * A 0 forces a checkpoint after each commited transaction.
     */
    void SetRecoveryCheckpointTime(int32_t v)
    {
        if (0 > v) throw WrongArgumentError("Expected a non-negative value");
        recoveryCheckpointTime = v;
    }
    
};


/**
 * \brief Dex class.
 *
 * All Dex programs must have one single Dex instance to manage one
 * or more Database instances. 
 *
 * This class allows for the creation of new Databases or open an existing one.
 *
 * \author Sparsity Technologies http://www.sparsity-technologies.com
 */
class DEX_EXPORT Dex
: private Handler<dexcore::DEX>
{
public:
    
    /**
     * \brief Dex version.
     */
    static const std::wstring Version;
    
    /**
     * \brief Creates a new instance.
     * \param config [in] Dex configuration.
     */
    Dex(const DexConfig & config);
    
    /**
     * \brief Destructor.
     */
    virtual ~Dex();
    
    /**
     * \brief Creates a new Database instance.
     * \param path [in] Database storage file.
     * \param alias [in] Database alias name.
     * \return A Database instance.
     * \exception FileNotFoundException If the given file cannot be created.
     */
    Database * Create(const std::wstring & path, const std::wstring & alias)
    throw(dex::gdb::FileNotFoundException, dex::gdb::Error);
    
    /**
     * \brief Opens an existing Database instance.
     * \param path [in] Database storage file.
     * \param read [in] If TRUE, open Database in read-only mode.
     * \return A Database instance.
     * \exception FileNotFoundException If the given file does not exist.
     */
    Database * Open(const std::wstring & path, bool_t read) 
    throw(dex::gdb::FileNotFoundException, dex::gdb::Error);
    
    /**
     * \brief Restores a Database from a backup file. See Graph::Backup.
     * \param path [in] Database storage file.
     * \param backupFile [in] The Backup file to be restored.
     * \return A Database instance.
     * \exception FileNotFoundException If the given file cannot be created, or the exported data file does not exists.
     */
    Database * Restore(const std::wstring & path, const std::wstring & backupFile)
    throw(dex::gdb::FileNotFoundException, dex::gdb::Error);

private:
    
    /**
     * \brief Stores dexcore LogConsumer reference.
     *
     * For the moment, just one is used (the default one), but it is ready
     * for multiple LogConsumer references.
     */
    std::vector<dexcore::LogConsumer *> logConsumers;
    
    /**
     * \brief Non implemented to avoid use of copy constructor.
     */
    Dex(const Dex & d);
    
    /**
     * \brief Non implemented to avoid use of assignment operator.
     */
    Dex & operator =(const Dex & d);
};


END_DEX_GDB_NAMESPACE

/** @} */
#endif // _DEX_DEX_H_
