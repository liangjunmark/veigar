#ifndef __VEIGAR_MSGPACK_V3_ADAPTOR_NIL_DECL_HPP // Add by msgpack.py
#define __VEIGAR_MSGPACK_V3_ADAPTOR_NIL_DECL_HPP
//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2008-2016 FURUHASHI Sadayuki and KONDO Takatoshi
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef MSGPACK_V3_TYPE_NIL_DECL_HPP
#define MSGPACK_V3_TYPE_NIL_DECL_HPP

#include "veigar/msgpack/v2/adaptor/nil_decl.hpp"

namespace veigar_msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v3) {
/// @endcond

namespace type {

using v2::type::nil_t;

#if defined(MSGPACK_USE_LEGACY_NIL)

typedef nil_t nil;

#endif // defined(MSGPACK_USE_LEGACY_NIL)

using v2::type::operator<;
using v2::type::operator==;

}  // namespace type

/// @cond
}  // MSGPACK_API_VERSION_NAMESPACE(v3)
/// @endcond

}  // namespace veigar_msgpack

#endif // MSGPACK_V3_TYPE_NIL_DECL_HPP

#endif // !__VEIGAR_MSGPACK_V3_ADAPTOR_NIL_DECL_HPP