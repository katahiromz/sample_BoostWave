/// @file convert_as.hpp
/// @brief 文字コード変換処理関数 convert_as を提供するヘッダ。
/// @author ruche

#pragma once
#ifndef ENCODING_CONVERT_AS_HPP
#define ENCODING_CONVERT_AS_HPP

#include <encoding/charcode.hpp>
#include <encoding/convert_from_to.hpp>
#include <encoding/detail/converter.hpp>
#include <type_traits>
#include <locale>
#include <utility>

namespace encoding
{
    /// @brief 戻り値の型を指定して文字コード変換を行う。
    /// @tparam ToString 変換先の文字列型。明示する必要がある。
    /// @tparam From 変換元の文字コード種別列挙値。
    /// @tparam To 変換先の文字コード種別列挙値。
    /// @tparam FromString 変換元の文字列型。
    /// @param[in] from 変換元の文字列。
    /// @param[in] from_to_tag 変換元文字コードと変換先文字コードを示すタグ。
    /// @param[in] loc ロケール設定。
    /// @return 変換後の文字列。
    template<class ToString, charcode From, charcode To, class FromString>
    inline auto convert_as(
        FromString&& from,
        convert_from_to<From, To> = convert_from_to<From, To>(),
        const std::locale& loc = std::locale(""))
        ->
        typename std::enable_if<
            is_charcode_string<From, FromString>::value &&
            is_charcode_string<To, ToString>::value,
            typename detail::converter_result<ToString>::type>::type
    {
        return
            detail::converter<From, To, FromString, ToString>()(
                std::forward<FromString>(from),
                loc);
    }

    /// @brief 戻り値の型を指定して文字コード変換を行う。
    /// @tparam ToString 変換先の文字列型。明示する必要がある。
    /// @tparam From 変換元の文字コード種別列挙値。
    /// @tparam To 変換先の文字コード種別列挙値。
    /// @param[in] from 変換元の文字列。
    /// @param[in] from_to_tag 変換元文字コードと変換先文字コードを示すタグ。
    /// @param[in] loc ロケール設定。
    /// @return 変換後の文字列。
    template<class ToString, charcode From, charcode To>
    inline auto convert_as(
        const typename charcode_traits<From>::value_type* from,
        convert_from_to<From, To> from_to_tag = convert_from_to<From, To>(),
        const std::locale& loc = std::locale(""))
        ->
        typename std::enable_if<
            is_charcode_string<To, ToString>::value,
            typename detail::converter_result<ToString>::type>::type
    {
        return
            convert_as<ToString>(
                typename charcode_traits<From>::string_type(from),
                from_to_tag,
                loc);
    }
}

#endif // ENCODING_CONVERT_AS_HPP
