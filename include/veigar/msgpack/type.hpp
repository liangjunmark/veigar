#ifndef __VEIGAR_MSGPACK_TYPE_HPP // Add by msgpack.py
#define __VEIGAR_MSGPACK_TYPE_HPP
#include "cpp_config.hpp"
#include "adaptor/array_ref.hpp"
#include "adaptor/bool.hpp"
#include "adaptor/carray.hpp"
#include "adaptor/char_ptr.hpp"
#include "adaptor/deque.hpp"
#include "adaptor/ext.hpp"
#include "adaptor/fixint.hpp"
#include "adaptor/float.hpp"
#include "adaptor/int.hpp"
#include "adaptor/complex.hpp"
#include "adaptor/list.hpp"
#include "adaptor/map.hpp"
#include "adaptor/nil.hpp"
#include "adaptor/pair.hpp"
#include "adaptor/raw.hpp"
#include "adaptor/v4raw.hpp"
#include "adaptor/set.hpp"
#include "adaptor/size_equal_only.hpp"
#include "adaptor/string.hpp"
#include "adaptor/vector.hpp"
#include "adaptor/vector_bool.hpp"
#include "adaptor/vector_char.hpp"
#include "adaptor/vector_unsigned_char.hpp"
#include "adaptor/wstring.hpp"
#include "adaptor/msgpack_tuple.hpp"
#include "adaptor/define.hpp"

#if defined(MSGPACK_USE_CPP03)

#include "adaptor/tr1/unordered_map.hpp"
#include "adaptor/tr1/unordered_set.hpp"

#else  // defined(MSGPACK_USE_CPP03)

#include "adaptor/cpp11/array.hpp"
#include "adaptor/cpp11/array_char.hpp"
#include "adaptor/cpp11/array_unsigned_char.hpp"

#include "adaptor/cpp11/chrono.hpp"

#include "adaptor/cpp11/forward_list.hpp"
#include "adaptor/cpp11/reference_wrapper.hpp"
#include "adaptor/cpp11/shared_ptr.hpp"
#include "adaptor/cpp11/timespec.hpp"
#include "adaptor/cpp11/tuple.hpp"
#include "adaptor/cpp11/unique_ptr.hpp"
#include "adaptor/cpp11/unordered_map.hpp"
#include "adaptor/cpp11/unordered_set.hpp"

#if MSGPACK_HAS_INCLUDE(<optional>)
#include "adaptor/cpp17/optional.hpp"
#endif // MSGPACK_HAS_INCLUDE(<optional>)

#if MSGPACK_HAS_INCLUDE(<string_view>)
#include "adaptor/cpp17/string_view.hpp"
#endif // MSGPACK_HAS_INCLUDE(<string_view>)

#include "adaptor/cpp17/array_byte.hpp"
#include "adaptor/cpp17/byte.hpp"
#include "adaptor/cpp17/carray_byte.hpp"
#include "adaptor/cpp17/vector_byte.hpp"

#if MSGPACK_HAS_INCLUDE(<variant>)
#include "adaptor/cpp17/variant.hpp"
#endif // MSGPACK_HAS_INCLUDE(<variant>)

#if MSGPACK_HAS_INCLUDE(<span>)
#include "adaptor/cpp20/span.hpp"
#endif // MSGPACK_HAS_INCLUDE(<span>)

#endif // defined(MSGPACK_USE_CPP03)

#if !defined(MSGPACK_NO_BOOST)
#include "adaptor/boost/fusion.hpp"
#include "adaptor/boost/msgpack_variant.hpp"
#include "adaptor/boost/optional.hpp"
#include "adaptor/boost/string_ref.hpp"
#include "adaptor/boost/string_view.hpp"
#endif // !defined(MSGPACK_NO_BOOST)

#endif // !__VEIGAR_MSGPACK_TYPE_HPP