/// @file narrow.hpp
/// @brief マルチバイトとワイド間の文字コード変換処理を提供するヘッダ。
/// @author ruche

#pragma once
#ifndef ENCODING_DETAIL_NARROW_HPP
#define ENCODING_DETAIL_NARROW_HPP

#include <encoding/charcode.hpp>
#include <type_traits>
#include <locale>
#include <iterator>
#include <utility>

namespace encoding { namespace detail { namespace narrow
{
    /// マルチバイト文字型。
    typedef charcode_traits<charcode::narrow>::value_type narrow_char_t;

    /// ワイド文字型。
    typedef charcode_traits<charcode::wide>::value_type wide_char_t;

    /// 文字コード変換に用いる codecvt 型。
    typedef std::codecvt<wide_char_t, narrow_char_t, std::mbstate_t> codecvt_t;

    /// @brief コンバータクラス。
    /// @tparam FromChar 変換元の文字型。
    /// @tparam ToChar 変換先の文字型。
    /// @tparam ToAlloc 変換先のアロケータ型。
    template<class FromChar, class ToChar, class ToAlloc>
    struct converter;

    /// @brief マルチバイトからワイドへのコンバータクラス。
    /// @tparam ToAlloc 変換先のアロケータ型。
    template<class ToAlloc>
    struct converter<narrow_char_t, wide_char_t, ToAlloc>
    {
        /// wstring_convert 型。
        typedef std::wstring_convert<codecvt_t, wide_char_t, ToAlloc> type;

        /// 文字コード変換結果型。
        typedef typename type::wide_string result_type;

        /// 文字コードを変換する。
        static result_type convert(
            const codecvt_t& cvt,
            const narrow_char_t* first,
            const narrow_char_t* last)
        {
            return type(&cvt).from_bytes(first, last);
        }
    };

    /// @brief ワイドからマルチバイトへのコンバータクラス。
    /// @tparam ToAlloc 変換先のアロケータ型。
    template<class ToAlloc>
    struct converter<wide_char_t, narrow_char_t, ToAlloc>
    {
        /// wstring_convert 型。
        typedef
            std::wstring_convert<
                codecvt_t,
                wide_char_t,
                std::allocator<wide_char_t>,
                ToAlloc>
            type;

        /// 文字コード変換結果型。
        typedef typename type::byte_string result_type;

        /// 文字コードを変換する。
        static result_type convert(
            const codecvt_t& cvt,
            const wide_char_t* first,
            const wide_char_t* last)
        {
            return type(&cvt).to_bytes(first, last);
        }
    };

    /// @brief コンバート処理関数の特性を提供するクラス。
    /// @tparam FromString 変換元の文字列型。
    /// @tparam ToString 変換先の文字列型。
    template<class FromString, class ToString>
    struct convert_traits
    {
        /// 変換元の文字列型。
        typedef typename std::remove_reference<FromString>::type from_string_type;

        /// 変換先の文字列型。
        typedef typename std::remove_reference<ToString>::type to_string_type;

        /// 変換元の文字型。
        typedef typename from_string_type::value_type from_char_type;

        /// 変換先の文字型。
        typedef typename to_string_type::value_type to_char_type;

        /// 変換先のアロケータ型。
        typedef typename to_string_type::allocator_type to_allocator_type;

        /// コンバータクラス型。
        typedef converter<from_char_type, to_char_type, to_allocator_type> converter_t;

        /// コンバート結果を to_string_type に変換可能ならば true 。
        static const bool result_convertible =
            std::is_convertible<converter_t::result_type, to_string_type>::value;
    };

    /// マルチバイトとワイド間で文字コード変換を行う。
    template<class ToString, class FromString>
    inline typename std::enable_if<
        convert_traits<FromString, ToString>::result_convertible,
        typename convert_traits<FromString, ToString>::to_string_type>::type
    convert(FromString&& from, const std::locale& loc)
    {
        // 戻り値は to_string_type に変換可能なのでそのまま返す
        return
            convert_traits<FromString, ToString>::converter_t::convert(
                std::use_facet<codecvt_t>(loc),
                from.data(),
                from.data() + from.size());
    }

    /// マルチバイトとワイド間で文字コード変換を行う。
    template<class ToString, class FromString>
    typename std::enable_if<
        !convert_traits<FromString, ToString>::result_convertible,
        typename convert_traits<FromString, ToString>::to_string_type>::type
    convert(FromString&& from, const std::locale& loc)
    {
        typedef convert_traits<FromString, ToString> traits_t;

        auto to =
            convert<traits_t::converter_t::result_type>(
                std::forward<FromString>(from),
                loc);

        return traits_t::to_string_type(std::begin(to), std::end(to));
    }
}}}

#endif // ENCODING_DETAIL_NARROW_HPP
