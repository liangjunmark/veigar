#ifndef __VEIGAR_MSGPACK_PREPROCESSOR_REPETITION_DEDUCE_R_HPP // Add by msgpack.py
#define __VEIGAR_MSGPACK_PREPROCESSOR_REPETITION_DEDUCE_R_HPP
# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef MSGPACK_PREPROCESSOR_REPETITION_DEDUCE_R_HPP
# define MSGPACK_PREPROCESSOR_REPETITION_DEDUCE_R_HPP
#
# include <veigar/msgpack/preprocessor/detail/auto_rec.hpp>
# include <veigar/msgpack/preprocessor/repetition/for.hpp>
#
# /* MSGPACK_PP_DEDUCE_R */
#
# define MSGPACK_PP_DEDUCE_R() MSGPACK_PP_AUTO_REC(MSGPACK_PP_FOR_P, 256)
#
# endif

#endif // !__VEIGAR_MSGPACK_PREPROCESSOR_REPETITION_DEDUCE_R_HPP