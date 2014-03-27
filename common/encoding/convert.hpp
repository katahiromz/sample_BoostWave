/// @file convert.hpp
/// @brief 文字コード変換処理関数 convert を提供するヘッダ。
/// @author ruche

#pragma once
#ifndef ENCODING_CONVERT_HPP
#define ENCODING_CONVERT_HPP

#include <encoding/convert_as.hpp>
#include <encoding/charcode.hpp>
#include <encoding/convert_from_to.hpp>
#include <type_traits>
#include <locale>
#include <utility>

namespace encoding
{
    /// @brief 文字コード変換を行う。
    /// @tparam From 変換元の文字コード種別列挙値。
    /// @tparam To 変換先の文字コード種別列挙値。
    /// @tparam FromString 変換元の文字列型。
    /// @param[in] from 変換元の文字列。
    /// @param[in] from_to_tag 変換元文字コードと変換先文字コードを示すタグ。
    /// @param[in] loc ロケール設定。
    /// @return 変換後の文字列。
    template<charcode From, charcode To, class FromString>
    inline auto convert(
        FromString&& from,
        convert_from_to<From, To> from_to_tag = convert_from_to<From, To>(),
        const std::locale& loc = std::locale(""))
        ->
        typename std::enable_if<
            is_charcode_string<From, FromString>::value,
            typename charcode_traits<To>::string_type>::type
    {
        return
            convert_as<typename charcode_traits<To>::string_type>(
                std::forward<FromString>(from),
                from_to_tag,
                loc);
    }

    /// @brief 文字コード変換を行う。
    /// @tparam From 変換元の文字コード種別列挙値。
    /// @tparam To 変換先の文字コード種別列挙値。
    /// @param[in] from 変換元の文字列。
    /// @param[in] from_to_tag 変換元文字コードと変換先文字コードを示すタグ。
    /// @param[in] loc ロケール設定。
    /// @return 変換後の文字列。
    template<charcode From, charcode To>
    inline typename charcode_traits<To>::string_type convert(
        const typename charcode_traits<From>::value_type* from,
        convert_from_to<From, To> from_to_tag = convert_from_to<From, To>(),
        const std::locale& loc = std::locale(""))
    {
        return
            convert(
                typename charcode_traits<From>::string_type(from),
                from_to_tag,
                loc);
    }
}

#endif // ENCODING_CONVERT_HPP
