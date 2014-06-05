#pragma once

#include <boost/asio/ip/address.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "cassandra.h"

#include "cql_ccm_bridge_configuration.hpp"

namespace cql {

class cql_ccm_bridge_t;

} // namespace cql

/** Random, reusable tools for testing. */
namespace test_utils {

constexpr cass_duration_t ONE_MILLISECOND_IN_MICROS = 1000; // in microseconds
constexpr cass_duration_t ONE_SECOND_IN_MICROS = 1000 * ONE_MILLISECOND_IN_MICROS;

struct CassSessionDeleter {
    void operator()(CassSession* ptr) {
      CassFuture* future = cass_session_close(ptr);
      cass_future_wait(future);
      cass_future_free(future);
    }
};

struct CassFutureDeleter {
  void operator()(CassFuture* ptr) {
    cass_future_free(ptr);
  }
};

struct CassStatementDeleter {
    void operator()(CassStatement* ptr) {
      cass_statement_free(ptr);
    }
};

struct CassResultDeleter {
    void operator()(const CassResult* ptr) {
      cass_result_free(ptr);
    }
};

struct CassIteratorDeleter {
    void operator()(CassIterator* ptr) {
      cass_iterator_free(ptr);
    }
};

struct CassCollectionDeleter {
    void operator()(CassCollection* ptr) {
      cass_collection_free(ptr);
    }
};

struct CassPreparedDeleter {
    void operator()(const CassPrepared* ptr) {
      cass_prepared_free(ptr);
    }
};

typedef std::unique_ptr<CassSession, CassSessionDeleter> CassSessionPtr;
typedef std::unique_ptr<CassFuture, CassFutureDeleter> CassFuturePtr;
typedef std::unique_ptr<CassStatement, CassStatementDeleter> CassStatementPtr;
typedef std::unique_ptr<const CassResult, CassResultDeleter> CassResultPtr;
typedef std::unique_ptr<CassIterator, CassIteratorDeleter> CassIteratorPtr;
typedef std::unique_ptr<CassCollection, CassCollectionDeleter> CassCollectionPtr;
typedef std::unique_ptr<const CassPrepared, CassPreparedDeleter> CassPreparedPtr;

template<class T>
struct Value;

template<>
struct Value<cass_int32_t> {
    static CassError bind(CassStatement* statement, cass_size_t index, cass_int32_t value) {
      return cass_statement_bind_int32(statement, index, value);
    }

    static CassError append(CassCollection* collection, cass_int32_t value) {
      return cass_collection_append_int32(collection, value);
    }

    static CassError get(const CassValue* value, cass_int32_t* output) {
      return cass_value_get_int32(value, output);
    }

    static bool equal(cass_int32_t a, cass_int32_t b) {
      return a == b;
    }

    static cass_int32_t min_value() {
      return std::numeric_limits<cass_int32_t>::min();
    }

    static cass_int32_t max_value() {
      return std::numeric_limits<cass_int32_t>::max();
    }
};

template<>
struct Value<cass_int64_t> {
    static CassError bind(CassStatement* statement, cass_size_t index, cass_int64_t value) {
      return cass_statement_bind_int64(statement, index, value);
    }

    static CassError append(CassCollection* collection, cass_int64_t value) {
      return cass_collection_append_int64(collection, value);
    }

    static CassError get(const CassValue* value, cass_int64_t* output) {
      return cass_value_get_int64(value, output);
    }

    static bool equal(cass_int64_t a, cass_int64_t b) {
      return a == b;
    }

    static cass_int64_t min_value() {
      return std::numeric_limits<cass_int64_t>::min();
    }

    static cass_int64_t max_value() {
      return std::numeric_limits<cass_int64_t>::max();
    }
};

template<>
struct Value<cass_float_t> {
    static CassError bind(CassStatement* statement, cass_size_t index, cass_float_t value) {
      return cass_statement_bind_float(statement, index, value);
    }

    static CassError append(CassCollection* collection, cass_float_t value) {
      return cass_collection_append_float(collection, value);
    }

    static CassError get(const CassValue* value, cass_float_t* output) {
      return cass_value_get_float(value, output);
    }

    static bool equal(cass_float_t a, cass_float_t b) {
      return a == b;
    }

    static cass_float_t min_value() {
      return std::numeric_limits<cass_float_t>::min();
    }

    static cass_float_t max_value() {
      return std::numeric_limits<cass_float_t>::max();
    }
};

template<>
struct Value<cass_double_t> {
    static CassError bind(CassStatement* statement, cass_size_t index, cass_double_t value) {
      return cass_statement_bind_double(statement, index, value);
    }

    static CassError append(CassCollection* collection, cass_double_t value) {
      return cass_collection_append_double(collection, value);
    }

    static CassError get(const CassValue* value, cass_double_t* output) {
      return cass_value_get_double(value, output);
    }

    static bool equal(cass_double_t a, cass_double_t b) {
      return a == b;
    }

    static cass_double_t min_value() {
      return std::numeric_limits<cass_double_t>::min();
    }

    static cass_double_t max_value() {
      return std::numeric_limits<cass_double_t>::max();
    }
};

template<>
struct Value<cass_bool_t> {
    static CassError bind(CassStatement* statement, cass_size_t index, cass_bool_t value) {
      return cass_statement_bind_bool(statement, index, value);
    }

    static CassError append(CassCollection* collection, cass_bool_t value) {
      return cass_collection_append_bool(collection, value);
    }

    static CassError get(const CassValue* value, cass_bool_t* output) {
      return cass_value_get_bool(value, output);
    }

    static bool equal(cass_bool_t a, cass_bool_t b) {
      return a == b;
    }
};

template<>
struct Value<CassString> {
    static CassError bind(CassStatement* statement, cass_size_t index, CassString value) {
      return cass_statement_bind_string(statement, index, value);
    }

    static CassError append(CassCollection* collection, CassString value) {
      return cass_collection_append_string(collection, value);
    }

    static CassError get(const CassValue* value, CassString* output) {
      return cass_value_get_string(value, output);
    }

    static bool equal(CassString a, CassString b) {
      if(a.length != b.length) {
        return false;
      }
      return strncmp(a.data, b.data, a.length) == 0;
    }
};

template<>
struct Value<CassBytes> {
    static CassError bind(CassStatement* statement, cass_size_t index, CassBytes value) {
      return cass_statement_bind_bytes(statement, index, value);
    }

    static CassError append(CassCollection* collection, CassBytes value) {
      return cass_collection_append_bytes(collection, value);
    }

    static CassError get(const CassValue* value, CassBytes* output) {
      return cass_value_get_bytes(value, output);
    }

    static bool equal(CassBytes a, CassBytes b) {
      if(a.size != b.size) {
        return false;
      }
      return memcmp(a.data, b.data, a.size) == 0;
    }
};

template<>
struct Value<CassInet> {
    static CassError bind(CassStatement* statement, cass_size_t index, CassInet value) {
      return cass_statement_bind_inet(statement, index, value);
    }

    static CassError append(CassCollection* collection, CassInet value) {
      return cass_collection_append_inet(collection, value);
    }

    static CassError get(const CassValue* value, CassInet* output) {
      return cass_value_get_inet(value, output);
    }

    static bool equal(CassInet a, CassInet b) {
      if(a.address_length != b.address_length) {
        return false;
      }
      return memcmp(a.address, b.address, a.address_length) == 0;
    }

    static CassInet min_value() {
      CassInet value;
      value.address_length = 16;
      memset(value.address, 0x0, sizeof(value.address));
      return value;
    }

    static CassInet max_value() {
      CassInet value;
      value.address_length = 16;
      memset(value.address, 0xF, sizeof(value.address));
      return value;
    }
};

template<>
struct Value<CassUuid> {
    static CassError bind(CassStatement* statement, cass_size_t index, CassUuid value) {
      return cass_statement_bind_uuid(statement, index, value);
    }

    static CassError append(CassCollection* collection, CassUuid value) {
      return cass_collection_append_uuid(collection, value);
    }

    static CassError get(const CassValue* value, CassUuid* output) {
      return cass_value_get_uuid(value, *output);
    }

    static bool equal(CassUuid a, CassUuid b) {
      return memcmp(a, b, sizeof(CassUuid)) == 0;
    }
};

template<>
struct Value<CassDecimal> {
    static CassError bind(CassStatement* statement, cass_size_t index, CassDecimal value) {
      return cass_statement_bind_decimal(statement, index, value);
    }

    static CassError append(CassCollection* collection, CassDecimal value) {
      return cass_collection_append_decimal(collection, value);
    }

    static CassError get(const CassValue* value, CassDecimal* output) {
      return cass_value_get_decimal(value, output);
    }

    static bool equal(CassDecimal a, CassDecimal b) {
      if(a.scale != b.scale) {
        return false;
      }
      if(a.varint.size != b.varint.size) {
        return false;
      }
      return memcmp(a.varint.data, b.varint.data, a.varint.size) == 0;
    }
};

// Simple wrapper to allow CassUuid to be added to STL containers
struct Uuid {
    CassUuid uuid;
    operator cass_uint8_t*() { return uuid; }
};

template<>
struct Value<Uuid> {
    static CassError bind(CassStatement* statement, cass_size_t index, Uuid value) {
      return cass_statement_bind_uuid(statement, index, value.uuid);
    }

    static CassError append(CassCollection* collection, Uuid value) {
      return cass_collection_append_uuid(collection, value.uuid);
    }

    static CassError get(const CassValue* value, Uuid* output) {
      return cass_value_get_uuid(value, output->uuid);
    }

    static bool equal(Uuid a, Uuid b) {
      return memcmp(a.uuid, b.uuid, sizeof(CassUuid)) == 0;
    }

    static Uuid min_value() {
      Uuid value;
      memset(value.uuid, 0x0, sizeof(value.uuid));
      return value;
    }

    static Uuid max_value() {
      Uuid value;
      memset(value.uuid, 0xF, sizeof(value.uuid));
      return value;
    }
};

inline bool operator==(Uuid a, Uuid b) {
  return test_utils::Value<Uuid>::equal(a, b);
}

inline bool operator<(Uuid a, Uuid b) {
  return memcmp(a.uuid, b.uuid, sizeof(CassUuid)) < 0;
}


/** The following class cannot be used as a kernel of test fixture because of
    parametrized ctor. Derive from it to use it in your tests.
 */
struct MultipleNodesTest {
    MultipleNodesTest(int numberOfNodesDC1, int numberOfNodesDC2);
    virtual ~MultipleNodesTest();

    boost::shared_ptr<cql::cql_ccm_bridge_t> ccm;
    const cql::cql_ccm_bridge_configuration_t& conf;
    CassCluster* cluster;
};

struct SingleSessionTest : MultipleNodesTest {
  SingleSessionTest(int numberOfNodesDC1, int numberOfNodesDC2);
  virtual ~SingleSessionTest();

  CassSession* session;
};

const char* get_value_type(CassValueType type);

void execute_query(CassSession* session,
                   const std::string& query,
                   CassResultPtr* result = nullptr,
                   CassConsistency consistency = CASS_CONSISTENCY_ONE);

void wait_and_check_error(CassFuture* future, cass_duration_t timeout = 10 * ONE_SECOND_IN_MICROS);

inline CassBytes bytes_from_string(const char* str) {
  return cass_bytes_init(reinterpret_cast<const cass_uint8_t*>(str), strlen(str));
}

inline CassInet inet_v4_from_int(int32_t address) {
  CassInet inet;
  memcpy(inet.address, &address, sizeof(int32_t));
  inet.address_length = sizeof(int32_t);
  return inet;
}

inline CassDecimal decimal_from_scale_and_bytes(cass_int32_t scale, CassBytes bytes) {
  CassDecimal decimal;
  decimal.scale = scale;
  decimal.varint = bytes;
  return decimal;
}

inline Uuid generate_time_uuid() {
  Uuid uuid;
  cass_uuid_generate_time(uuid.uuid);
  return uuid;
}

inline Uuid generate_random_uuid() {
  Uuid uuid;
  cass_uuid_generate_random(uuid.uuid);
  return uuid;
}

inline std::string generate_unique_str() {
  Uuid uuid;
  cass_uuid_generate_time(uuid.uuid);
  char buffer[CASS_UUID_STRING_LENGTH];
  cass_uuid_string(uuid, buffer);
  return boost::replace_all_copy(std::string(buffer), "-", "");
}

std::string string_from_time_point(std::chrono::system_clock::time_point time);
std::string string_from_uuid(CassUuid uuid);

constexpr const char* CREATE_TABLE_ALL_TYPES =
    "CREATE TABLE %s ("
    "id uuid PRIMARY KEY,"
    "text_sample text,"
    "int_sample int,"
    "bigint_sample bigint,"
    "float_sample float,"
    "double_sample double,"
    "decimal_sample decimal,"
    "blob_sample blob,"
    "boolean_sample boolean,"
    "timestamp_sample timestamp,"
    "inet_sample inet);";

constexpr const char* CREATE_TABLE_TIME_SERIES =
    "CREATE TABLE %s ("
    "id uuid,"
    "event_time timestamp,"
    "text_sample text,"
    "int_sample int,"
    "bigint_sample bigint,"
    "float_sample float,"
    "double_sample double,"
    "decimal_sample decimal,"
    "blob_sample blob,"
    "boolean_sample boolean,"
    "timestamp_sample timestamp,"
    "inet_sample inet,"
    "PRIMARY KEY(id, event_time));";

extern const std::string CREATE_KEYSPACE_SIMPLE_FORMAT;
extern const std::string CREATE_KEYSPACE_GENERIC_FORMAT;
extern const std::string SIMPLE_KEYSPACE;
extern const std::string SIMPLE_TABLE;
extern const std::string CREATE_TABLE_SIMPLE_FORMAT;
extern const std::string INSERT_FORMAT;
extern const std::string SELECT_ALL_FORMAT;
extern const std::string SELECT_WHERE_FORMAT;
extern const std::string lorem_ipsum;

} // End of namespace test_utils

inline bool operator==(CassString a, CassString b) {
  return test_utils::Value<CassString>::equal(a, b);
}

inline bool operator<(CassString a, CassString b) {
  if(a.length > b.length) {
    return false;
  } else if(a.length < b.length) {
    return true;
  }
  return strncmp(a.data, b.data, a.length) < 0;
}

inline bool operator==(CassBytes a, CassBytes b) {
  return test_utils::Value<CassBytes>::equal(a, b);
}

inline bool operator<(CassBytes a, CassBytes b) {
  if(a.size > b.size) {
    return false;
  } else if(a.size < b.size) {
    return true;
  }
  return memcmp(a.data, b.data, a.size) < 0;
}

inline bool operator==(CassInet a, CassInet b) {
  return test_utils::Value<CassInet>::equal(a, b);
}

inline bool operator<(CassInet a, CassInet b) {
  if(a.address_length > b.address_length) {
    return false;
  } else if(a.address_length < b.address_length) {
    return true;
  }
  return memcmp(a.address, b.address, a.address_length) < 0;
}

inline bool operator==(CassDecimal a, CassDecimal b) {
  return test_utils::Value<CassDecimal>::equal(a, b);
}

inline bool operator<(CassDecimal a, CassDecimal b) {
  // TODO: This might not be exactly correct, but should work
  // for testing
  if(a.scale > b.scale) {
    return false;
  } else if(a.scale < b.scale) {
    return true;
  }
  if(a.varint.size > b.varint.size) {
    return false;
  } else if(a.varint.size < b.varint.size) {
    return true;
  }
  return memcmp(a.varint.data, b.varint.data, a.varint.size) < 0;
}
