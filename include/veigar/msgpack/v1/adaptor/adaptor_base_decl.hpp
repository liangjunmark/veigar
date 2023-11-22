//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2016 KONDO Takatoshi
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef MSGPACK_V1_ADAPTOR_BASE_DECL_HPP
#define MSGPACK_V1_ADAPTOR_BASE_DECL_HPP

#include "veigar/msgpack/versioning.hpp"
#include "veigar/msgpack/object_fwd.hpp"
#include "veigar/msgpack/pack.hpp"

namespace veigar_msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v1) {
/// @endcond

template <typename Stream>
class packer;

namespace adaptor {

// Adaptor functors

template <typename T, typename Enabler = void>
struct convert;

template <typename T, typename Enabler = void>
struct pack;

template <typename T, typename Enabler = void>
struct object;

template <typename T, typename Enabler = void>
struct object_with_zone;

} // namespace adaptor

// operators

template <typename T>
typename veigar_msgpack::enable_if<
    !is_array<T>::value,
    veigar_msgpack::object const&
>::type
operator>> (veigar_msgpack::object const& o, T& v);
template <typename T, std::size_t N>
veigar_msgpack::object const& operator>> (veigar_msgpack::object const& o, T(&v)[N]);

template <typename Stream, typename T>
typename veigar_msgpack::enable_if<
    !is_array<T>::value,
    veigar_msgpack::packer<Stream>&
>::type
operator<< (veigar_msgpack::packer<Stream>& o, T const& v);
template <typename Stream, typename T, std::size_t N>
veigar_msgpack::packer<Stream>& operator<< (veigar_msgpack::packer<Stream>& o, const T(&v)[N]);

template <typename T>
typename veigar_msgpack::enable_if<
    !is_array<T>::value
>::type
operator<< (veigar_msgpack::object& o, T const& v);
template <typename T, std::size_t N>
void operator<< (veigar_msgpack::object& o, const T(&v)[N]);

template <typename T>
typename veigar_msgpack::enable_if<
    !is_array<T>::value
>::type
operator<< (veigar_msgpack::object::with_zone& o, T const& v);
template <typename T, std::size_t N>
void operator<< (veigar_msgpack::object::with_zone& o, const T(&v)[N]);

/// @cond
} // MSGPACK_API_VERSION_NAMESPACE(v1)
/// @endcond

} // namespace veigar_msgpack

#endif // MSGPACK_V1_ADAPTOR_BASE_DECL_HPP
