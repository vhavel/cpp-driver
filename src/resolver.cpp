/*
  Copyright (c) DataStax, Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/


#include "resolver.hpp"

#include "address.hpp"
#include "dense_hash_map.hpp"
#include "scoped_lock.hpp"

using namespace datastax;
using namespace datastax::internal;
using namespace datastax::internal::core;

namespace  {

class ResolveCache : public DenseHashMap<Address, AddressVec> {
public:
  ResolveCache() {
    set_empty_key(Address::EMPTY_KEY);
  }
};
ResolveCache resolve_cache__;

class Mutex {
public:
  Mutex() { uv_mutex_init(&m_); }
  uv_mutex_t* get() { return &m_; }
private:
  uv_mutex_t m_;
};
Mutex resolve_cache_mutex__;
}


bool datastax::internal::core::Resolver::is_resolved(const Address& hostname, AddressVec& cached) {
  ScopedMutex l(resolve_cache_mutex__.get());
  ResolveCache::const_iterator it =  resolve_cache__.find(hostname);
  if (it != resolve_cache__.end()) {
    cached = it->second;
    return true;
  }
  return false;
}

void datastax::internal::core::Resolver::add_resolved(const Address& hostname, const AddressVec& addresses) {
  ScopedMutex l(resolve_cache_mutex__.get());
  resolve_cache__[hostname] = addresses;
}
