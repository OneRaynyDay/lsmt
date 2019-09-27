#ifndef LSMT_HPP
#define LSMT_HPP

#include <sstable.hpp>
#include <libcuckoo/cuckoohash_map.hh>
#include <shared_mutex>
#include <iostream>
#include <filesystem>

namespace lsmt{

/**
 * Background
 * ----------
 * LSM Trees are important data structures for KV stores. The tree structure is a bit of a misnomer,
 * as it's actually a composition of SSTables in disk and containers in memory (For information on SSTables, check out sstable.hpp).
 *
 * We can specify levels that SSTables live in and LSM trees will be responsible for managing these sstables,
 * churning them or moving them down the hierarchy when necessary.
 *
 * The general architecture will look like:
 * In-memory data structure
 * Tier 1 SSTable
 * Tier 2 SSTable
 * ...
 * Tier N SSTable
 *
 * Each one of these tiers should be in increasing latency for read and writes. The idea is to keep the
 * memory access as fast as possible for writes, as any incoming write will hit memory first, and encounter a
 * stop-the-world merge and downgrade of sstables into different memory hierarchies.
 *
 * Concurrency model
 * -----------------
 * In general, we want the concurrency model to be: SINGLE writer MULTIPLE readers.
 * 
 * This implies a few things:
 * - Our in-memory structure must be threadsafe. SINGLE writer implies there will be no write-races, but
 *   we need to protect reads from reading stale data. This means we need shared locks for reading and unique lock for writing,
 *   which implies a read/write lock paradigm.
 * - SSTables are meant to be immutable, and so reading them in parallel will be okay.
 * - TODO: Consider when merge and serialization is happening.
 *
 * Some in-memory data structures used in consideration are:
 * - std::set with non-granular locking
 * - cuckoohash_map (concurrent)
 * - folly::AtomicHashMap (concurrent)
 * - tbb::concurrent_hash_map (concurrent)
 * - folly::AtomicSkipList (concurrent)
 */


// We want the following information about each sstable. 
// Future additions to their information could contain information about compaction, whether we want a bloom filter, etc.
// int - max size of sstables
// std::filesystem::path - where to write them (mounts of different disk types)
using sstable_metadata = std::pair<int, std::filesystem::path>;

// To ensure correct locking behavior, we use read/write locks.
using lock = std::shared_mutex;
using write_lock = std::unique_lock<lock>;
using read_lock = std::shared_lock<lock>;

static constexpr std::size_t MAX_MEM = 1000000; // In-memory data structure will be 1 MB at most.

template <typename K, typename V, std::size_t lsmtree_levels>
class lsmtree {
    // This is a rough estimate of how much
    static constexpr std::size_t MAX_ENTRIES = MAX_MEM / (sizeof(K) + sizeof(V));

    // Map type is currently using cuckoo hashmaps
    using map_t = cuckoohash_map<K,V>;

public:
    // For an LSM Tree, we want:
    // sstables - Metadata required for tiers of sstables
    // tombstone - Special data of type V that users are not allowed to use as inserts (responsibility to user),
    //             and is denoted as a deleted record.
    lsmtree(const std::array<sstable_metadata, lsmtree_levels>& sstables, const V& tombstone) {

    }

    // Getters
    // -------

    const int size() {
        return _map.size();
    }

    // Returns true if there are too many elements inside the in-memory data structure and it needs to be flushed to disk.
    const bool full() {
        return size() >= MAX_ENTRIES;
    }
    
    // We want to insert into an LSM Tree.
    // To insert, simply add into the in-memory container for most cases. This means doing a write-lock.
    // TODO
    void insert(const K& key, const V& value) {
        // If map is full, then we need to lock all inserts until the result is written to disk.
        {
            // Note: The reason we don't use for example cuckoo hashmap's lock_tbl() directly is
            // because we may want to swap out implementation and so it's best we have our own lock.
            write_lock wlock(_lock);
            if(full()) {
                // TODO: Write to disk.
                std::cout << "Clearing now..." << std::endl; 
                // Scrap the current map
                _map.clear();
            }
        }

        // Insert into map (which may be cleared just now)
        _map.insert(key, value);
    }

    // We want to delete from an LSM Tree.
    // To delete, we actually apply an insert but with a tombstone, so essentially calling the above insert with tombstone.
    // TODO

    // We want to read from an LSM Tree.
    // To read, we have to do a read-lock, and check to see if the data exists. If not, then we need to
    // scan subsequent SSTables until we find the entry.
    // TODO
private:
    // In-memory data structure that is append & update-only.
    map_t _map;

    // Lock for writes when the map is full.
    lock _lock;
};

} // namespace lsmt

#endif
