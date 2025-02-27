#ifndef __VEIGAR_MSGPACK_V2_PARSE_RETURN_HPP // Add by msgpack.py
#define __VEIGAR_MSGPACK_V2_PARSE_RETURN_HPP
//
// MessagePack for C++ deserializing routine
//
// Copyright (C) 2008-2016 FURUHASHI Sadayuki and KONDO Takatoshi
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef MSGPACK_V2_PARSE_RETURN_HPP
#define MSGPACK_V2_PARSE_RETURN_HPP

#include "veigar/msgpack/v1/parse_return.hpp"

namespace veigar_msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v2) {
/// @endcond


// for internal use
typedef enum {
    PARSE_SUCCESS              = v1::PARSE_SUCCESS,
    PARSE_EXTRA_BYTES          = v1::PARSE_EXTRA_BYTES,
    PARSE_CONTINUE             = v1::PARSE_CONTINUE,
    PARSE_PARSE_ERROR          = v1::PARSE_PARSE_ERROR,
    PARSE_STOP_VISITOR         = -2
} parse_return;

/// @cond
}  // MSGPACK_API_VERSION_NAMESPACE(v2)
/// @endcond

}  // namespace veigar_msgpack

#endif // MSGPACK_V2_PARSE_RETURN_HPP

#endif // !__VEIGAR_MSGPACK_V2_PARSE_RETURN_HPP