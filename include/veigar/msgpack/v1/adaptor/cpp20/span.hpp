#ifndef __VEIGAR_MSGPACK_V1_ADAPTOR_CPP20_SPAN_HPP // Add by msgpack.py
#define __VEIGAR_MSGPACK_V1_ADAPTOR_CPP20_SPAN_HPP
//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2021 KONDO Takatoshi and Daniil Kovalev
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MSGPACK_V1_TYPE_CPP20_SPAN_HPP
#define MSGPACK_V1_TYPE_CPP20_SPAN_HPP

#include "veigar/msgpack/cpp_version.hpp"

// Some compilers still do not set the corresponding macro to 202002
#if MSGPACK_CPP_VERSION > 201703

#include "veigar/msgpack/versioning.hpp"
#include "veigar/msgpack/adaptor/adaptor_base.hpp"
#include "veigar/msgpack/object.hpp"
#include "veigar/msgpack/adaptor/check_container_size.hpp"
#include "veigar/msgpack/meta.hpp"

#include <span>
#include <cstring>
#include <cstddef>

namespace veigar_msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v1) {
/// @endcond

namespace adaptor {

#define MSGPACK_ADAPTOR_CONVERT_SPAN_BINARY(byte_t)                                                                 \
    template <>                                                                                                     \
    struct convert<std::span<const byte_t> > {                                                                      \
        veigar_msgpack::object const& operator()(veigar_msgpack::object const& o, std::span<const byte_t>& v) const {             \
            switch (o.type) {                                                                                       \
            case veigar_msgpack::type::BIN:                                                                                \
                v = std::span(reinterpret_cast<const byte_t*>(o.via.bin.ptr), o.via.bin.size);                      \
                break;                                                                                              \
            default:                                                                                                \
                throw veigar_msgpack::type_error();                                                                        \
                break;                                                                                              \
            }                                                                                                       \
            return o;                                                                                               \
        }                                                                                                           \
    };

#define MSGPACK_ADAPTOR_PACK_SPAN_BINARY(byte_t)                                                                    \
    template <>                                                                                                     \
    struct pack<std::span<byte_t> > {                                                                               \
        template <typename Stream>                                                                                  \
        veigar_msgpack::packer<Stream>& operator()(veigar_msgpack::packer<Stream>& o, const std::span<byte_t>& v) const {         \
            uint32_t size = checked_get_container_size(v.size());                                                   \
            o.pack_bin(size);                                                                                       \
            if (size != 0) {                                                                                        \
                o.pack_bin_body(reinterpret_cast<char const*>(v.data()), size);                                     \
            }                                                                                                       \
            return o;                                                                                               \
        }                                                                                                           \
    };

#define MSGPACK_ADAPTOR_OBJECT_SPAN_BINARY(byte_t)                                                                  \
    template <>                                                                                                     \
    struct object<std::span<byte_t> > {                                                                             \
        void operator()(veigar_msgpack::object& o, const std::span<byte_t>& v) const {                                     \
            uint32_t size = checked_get_container_size(v.size());                                                   \
            o.type = veigar_msgpack::type::BIN;                                                                            \
            if (size != 0) {                                                                                        \
                o.via.bin.ptr = reinterpret_cast<char const*>(v.data());                                            \
            }                                                                                                       \
            o.via.bin.size = size;                                                                                  \
        }                                                                                                           \
    };

#define MSGPACK_ADAPTOR_OBJECT_WITH_ZONE_SPAN_BINARY(byte_t)                                                        \
    template <>                                                                                                     \
    struct object_with_zone<std::span<byte_t> > {                                                                   \
        void operator()(veigar_msgpack::object::with_zone& o, const std::span<byte_t>& v) const {                          \
            uint32_t size = checked_get_container_size(v.size());                                                   \
            o.type = veigar_msgpack::type::BIN;                                                                            \
            o.via.bin.size = size;                                                                                  \
            if (size != 0) {                                                                                        \
                char* ptr = static_cast<char*>(o.zone.allocate_align(size, MSGPACK_ZONE_ALIGNOF(char)));            \
                o.via.bin.ptr = ptr;                                                                                \
                std::memcpy(ptr, v.data(), size);                                                                   \
            }                                                                                                       \
        }                                                                                                           \
    };

#define MSGPACK_ADAPTOR_SPAN_BINARY(byte_t)                                                                         \
    MSGPACK_ADAPTOR_CONVERT_SPAN_BINARY(byte_t)                                                                     \
    MSGPACK_ADAPTOR_PACK_SPAN_BINARY(byte_t)                                                                        \
    MSGPACK_ADAPTOR_PACK_SPAN_BINARY(const byte_t)                                                                  \
    MSGPACK_ADAPTOR_OBJECT_SPAN_BINARY(byte_t)                                                                      \
    MSGPACK_ADAPTOR_OBJECT_SPAN_BINARY(const byte_t)                                                                \
    MSGPACK_ADAPTOR_OBJECT_WITH_ZONE_SPAN_BINARY(byte_t)                                                            \
    MSGPACK_ADAPTOR_OBJECT_WITH_ZONE_SPAN_BINARY(const byte_t)

MSGPACK_ADAPTOR_SPAN_BINARY(char)
MSGPACK_ADAPTOR_SPAN_BINARY(unsigned char)
MSGPACK_ADAPTOR_SPAN_BINARY(std::byte)

#undef MSGPACK_ADAPTOR_SPAN_BINARY
#undef MSGPACK_ADAPTOR_OBJECT_WITH_ZONE_SPAN_BINARY
#undef MSGPACK_ADAPTOR_OBJECT_SPAN_BINARY
#undef MSGPACK_ADAPTOR_PACK_SPAN_BINARY
#undef MSGPACK_ADAPTOR_CONVERT_SPAN_BINARY

} // namespace adaptor

/// @cond
} // MSGPACK_API_VERSION_NAMESPACE(v1)
/// @endcond

} // namespace veigar_msgpack

#endif // MSGPACK_CPP_VERSION > 201703

#endif // MSGPACK_V1_TYPE_CPP11_ARRAY_HPP

#endif // !__VEIGAR_MSGPACK_V1_ADAPTOR_CPP20_SPAN_HPP