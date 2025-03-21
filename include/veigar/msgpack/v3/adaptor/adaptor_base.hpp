#ifndef __VEIGAR_MSGPACK_V3_ADAPTOR_ADAPTOR_BASE_HPP // Add by msgpack.py
#define __VEIGAR_MSGPACK_V3_ADAPTOR_ADAPTOR_BASE_HPP
//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2015-2016 KONDO Takatoshi
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef MSGPACK_V3_ADAPTOR_BASE_HPP
#define MSGPACK_V3_ADAPTOR_BASE_HPP

#include "veigar/msgpack/v3/adaptor/adaptor_base_decl.hpp"

namespace veigar_msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v3) {
/// @endcond


namespace adaptor {

// Adaptor functors

template <typename T, typename Enabler>
struct convert : v2::adaptor::convert<T, Enabler> {
};

template <typename T, typename Enabler>
struct pack : v2::adaptor::pack<T, Enabler> {
};

template <typename T, typename Enabler>
struct object
    : v2::adaptor::object<
        T,
        typename veigar_msgpack::enable_if<
            !veigar_msgpack::is_same<T, std::string>::value &&
            !veigar_msgpack::is_array<T>::value,
            Enabler
        >::type
      > {
};

template <typename T, typename Enabler>
struct object_with_zone : v2::adaptor::object_with_zone<T, Enabler> {
};

} // namespace adaptor

/// @cond
} // MSGPACK_API_VERSION_NAMESPACE(v3)
/// @endcond

} // namespace veigar_msgpack


#endif // MSGPACK_V3_ADAPTOR_BASE_HPP

#endif // !__VEIGAR_MSGPACK_V3_ADAPTOR_ADAPTOR_BASE_HPP