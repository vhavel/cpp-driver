/* 
 * File:   cql_to_many_connections_per_host.hpp
 * Author: mc
 *
 * Created on September 16, 2013, 2:06 PM
 */

#ifndef CQL_TO_MANY_CONNECTIONS_PER_HOST_EXCEPTION_HPP_
#define	CQL_TO_MANY_CONNECTIONS_PER_HOST_EXCEPTION_HPP_

#include "cql/exceptions/cql_generic_exception.hpp"

namespace cql {
    class cql_to_many_connections_per_host_exception: public cql_generic_exception {
    public:
        cql_to_many_connections_per_host_exception()
            : cql_generic_exception("Maximum number of connections per host is reached") { }
    };
}

#endif	/* CQL_TO_MANY_CONNECTIONS_PER_HOST_EXCEPTION_HPP_ */

