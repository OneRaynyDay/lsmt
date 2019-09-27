#ifndef LSMT_HPP
#define LSMT_HPP

#include <iostream>

namespace lsmt {

/**
 * SSTables are serialized chunks of K/V pairs sorted by K.
 * We want these tables to be sorted so it would be easy to merge them into larger SSTables.
 * 
 * There are a few functionalities we want:
 * 1. Read from an SSTable a specific K.
 * 2. Merge two SSTables.
 * 3. Turn an in-memory data structure into an SSTable.
 *
 * TODO: Question: do we consider these as objects with RAII properties or no?
 */ 
    


} // namespace lsmt

#endif
