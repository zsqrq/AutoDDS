/*
@file
Defines `boost::hana::experimental::type_name`.

Copyright Louis Dionne 2013-2022
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXPERIMENTAL_TYPE_NAME_HPP
#define BOOST_HANA_EXPERIMENTAL_TYPE_NAME_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/string.hpp>

#include <cstddef>
#include <utility>


namespace boost { namespace hana { namespace experimental {
    namespace detail {
        struct cstring {
            char const* ptr;
            std::size_t length;
        };

        // Note: We substract the null terminator from the string sizes below.
        template <typename T>
        constexpr auto type_name_impl2() {
        #if defined(BOOST_HANA_CONFIG_CLANG)
            constexpr char const* pretty_function = __PRETTY_FUNCTION__;
            constexpr std::size_t total_size = sizeof(__PRETTY_FUNCTION__) - 1;
            constexpr std::size_t prefix_size = sizeof("auto boost::hana::experimental::detail::type_name_impl2() [T = ") - 1;
            constexpr std::size_t suffix_size = sizeof("]") - 1;
        #elif defined(BOOST_HANA_CONFIG_GCC)
            constexpr char const* pretty_function = __PRETTY_FUNCTION__;
            constexpr std::size_t total_size = sizeof(__PRETTY_FUNCTION__) - 1;
            constexpr std::size_t prefix_size = sizeof("constexpr auto boost::hana::experimental::detail::type_name_impl2() [with T = ") - 1;
            constexpr std::size_t suffix_size = sizeof("]") - 1;
        #else
            #error "No support for this compiler."
        #endif

            cstring s{pretty_function + prefix_size, total_size - prefix_size - suffix_size};
            return s;
        }

        template <typename T, std::size_t ...i>
        auto type_name_impl1(std::index_sequence<i...>) {
            constexpr auto name = detail::type_name_impl2<T>();
            return hana::string<*(name.ptr + i)...>{};
        }
    } // end namespace detail

    //! @ingroup group-experimental
    //! Returns a `hana::string` representing the name of the given type, at
    //! compile-time.
    //!
    //! This only works on Clang (and apparently MSVC, but Hana does not work
    //! there as of writing this). Original idea taken from
    //! https://github.com/Manu343726/ctti.
    template <typename T>
    auto type_name() {
        constexpr auto name = detail::type_name_impl2<T>();
        return detail::type_name_impl1<T>(std::make_index_sequence<name.length>{});
    }
} }} // end namespace boost::hana

#endif // !BOOST_HANA_EXPERIMENTAL_TYPE_NAME_HPP
