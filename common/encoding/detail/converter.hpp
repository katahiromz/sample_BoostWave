/// @file converter.hpp
/// @brief 文字コード変換処理関数のヘルパクラス converter を提供するヘッダ。
/// @author ruche

#pragma once
#ifndef ENCODING_DETAIL_CONVERTER_HPP
#define ENCODING_DETAIL_CONVERTER_HPP

#include <encoding/charcode.hpp>
#include <encoding/detail/narrow.hpp>
#include <encoding/detail/utf8.hpp>
#include <type_traits>
#include <locale>
#include <utility>
#include <iterator>

namespace encoding { namespace detail
{
    /// @brief 文字コード変換を行うファンクタクラス。
    /// @tparam From 変換元の文字コード種別列挙値。
    /// @tparam To 変換先の文字コード種別列挙値。
    /// @tparam FromString 変換元の文字列型。
    /// @tparam ToString 変換先の文字列型。
    template<charcode From, charcode To, class FromString, class ToString, class = void>
    struct converter;

    /// @brief 特殊化された converter クラスの処理結果の型を提供するクラス。
    /// @tparam ToString 変換先の文字列型。
    template<class ToString>
    struct converter_result
    {
        /// 戻り値の型。
        typedef typename std::remove_reference<ToString>::type type;
    };

    /// 同一の文字コード間での変換を行うファンクタクラス。
    template<charcode C, class FromString, class ToString>
    struct converter<
        C,
        C,
        FromString,
        ToString,
        typename std::enable_if<
            std::is_convertible<
                FromString,
                typename converter_result<ToString>::type>::value>::type>
    {
        /// 戻り値の型。
        typedef typename converter_result<ToString>::type result_type;

        result_type operator()(FromString&& from, const std::locale&) const
        {
            // result_type に変換可能なのでそのまま返す
            return from;
        }
    };

    /// 同一の文字コード間での変換を行うファンクタクラス。
    template<charcode C, class FromString, class ToString>
    struct converter<
        C,
        C,
        FromString,
        ToString,
        typename std::enable_if<
            !std::is_convertible<
                FromString,
                typename converter_result<ToString>::type>::value>::type>
    {
        /// 戻り値の型。
        typedef typename converter_result<ToString>::type result_type;

        result_type operator()(FromString&& from, const std::locale&) const
        {
            return result_type(std::begin(from), std::end(from));
        }
    };

    /// マルチバイトからワイドへの文字コード変換を行うファンクタクラス。
    template<class FromString, class ToString>
    struct converter<charcode::narrow, charcode::wide, FromString, ToString>
    {
        /// 戻り値の型。
        typedef typename converter_result<ToString>::type result_type;

        result_type operator()(FromString&& from, const std::locale& loc) const
        {
            return narrow::convert<result_type>(std::forward<FromString>(from), loc);
        }
    };

    /// ワイドからマルチバイトへの文字コード変換を行うファンクタクラス。
    template<class FromString, class ToString>
    struct converter<charcode::wide, charcode::narrow, FromString, ToString>
    {
        /// 戻り値の型。
        typedef typename converter_result<ToString>::type result_type;

        result_type operator()(FromString&& from, const std::locale& loc) const
        {
            return narrow::convert<result_type>(std::forward<FromString>(from), loc);
        }
    };

    /// マルチバイトからUTF-8への文字コード変換を行うファンクタクラス。
    template<class FromString, class ToString>
    struct converter<charcode::narrow, charcode::utf8, FromString, ToString>
    {
        /// 戻り値の型。
        typedef typename converter_result<ToString>::type result_type;

        /// ワイド文字列型。
        typedef typename charcode_traits<charcode::wide>::string_type wide_string_type;

        result_type operator()(FromString&& from, const std::locale& loc) const
        {
            return
                utf8::convert<result_type>(
                    narrow::convert<wide_string_type>(
                        std::forward<FromString>(from),
                        loc));
        }
    };

    /// UTF-8からマルチバイトへの文字コード変換を行うファンクタクラス。
    template<class FromString, class ToString>
    struct converter<charcode::utf8, charcode::narrow, FromString, ToString>
    {
        /// 戻り値の型。
        typedef typename converter_result<ToString>::type result_type;

        /// ワイド文字列型。
        typedef typename charcode_traits<charcode::wide>::string_type wide_string_type;

        result_type operator()(FromString&& from, const std::locale& loc) const
        {
            return
                narrow::convert<result_type>(
                    utf8::convert<wide_string_type>(std::forward<FromString>(from)),
                    loc);
        }
    };

    /// ワイドからUTF-8への文字コード変換を行うファンクタクラス。
    template<class FromString, class ToString>
    struct converter<charcode::wide, charcode::utf8, FromString, ToString>
    {
        /// 戻り値の型。
        typedef typename converter_result<ToString>::type result_type;

        result_type operator()(FromString&& from, const std::locale&) const
        {
            return utf8::convert<result_type>(std::forward<FromString>(from));
        }
    };

    /// UTF-8からワイドへの文字コード変換を行うファンクタクラス。
    template<class FromString, class ToString>
    struct converter<charcode::utf8, charcode::wide, FromString, ToString>
    {
        /// 戻り値の型。
        typedef typename converter_result<ToString>::type result_type;

        result_type operator()(FromString&& from, const std::locale&) const
        {
            return utf8::convert<result_type>(std::forward<FromString>(from));
        }
    };
}}

#endif // ENCODING_DETAIL_CONVERTER_HPP
