/// @file charcode.hpp
/// @brief 文字コード種別列挙クラス charcode を提供するヘッダ。
/// @author ruche

#pragma once
#ifndef ENCODING_CHARCODE_HPP
#define ENCODING_CHARCODE_HPP

#include <encoding/detail/has_value_type.hpp>
#include <type_traits>
#include <string>

namespace encoding
{
    /// 文字コード種別列挙クラス。
    enum class charcode
    {
        /// マルチバイト。
        narrow,

        /// ワイド。WindowsではUTF-16LE。
        wide,

        /// UTF-8。
        utf8,
    };

    /// 文字コード種別列挙値の特性を提供するクラス。
    template<charcode C>
    struct charcode_traits;

    /// 特殊化された charcode_traits クラスの基底クラス型。
    template<class TChar>
    struct charcode_traits_base
    {
        /// 文字型。
        typedef TChar value_type;

        /// 既定の文字列型。
        typedef std::basic_string<value_type> string_type;
    };

    /// 文字コード種別列挙値 charcode::narrow の特性を提供するクラス。
    template<>
    struct charcode_traits<charcode::narrow> : public charcode_traits_base<char>
    {
    };

    /// 文字コード種別列挙値 charcode::wide の特性を提供するクラス。
    template<>
    struct charcode_traits<charcode::wide> : public charcode_traits_base<wchar_t>
    {
    };

    /// 文字コード種別列挙値 charcode::utf8 の特性を提供するクラス。
    template<>
    struct charcode_traits<charcode::utf8> : public charcode_traits_base<char>
    {
    };

    /// @brief 指定した文字コード種別を格納可能な文字列型であるか否かを調べる。
    /// @tparam C 文字コード種別列挙値。
    /// @tparam String 文字列型。参照型の場合は remove_reference される。
    template<charcode C, class String, class = void>
    class is_charcode_string : public std::false_type
    {
    };

    /// @brief 指定した文字コード種別を格納可能な文字列型であるか否かを調べる。
    /// @tparam C 文字コード種別列挙値。
    /// @tparam String 文字列型。参照型の場合は remove_reference される。
    ///
    /// charcode_traits<C>::value_type と String::value_type の型が等しければ
    /// 静的メンバ変数 value は true となる。
    /// それ以外の場合、 value は false となる。
    template<charcode C, class String>
    class is_charcode_string<
        C,
        String,
        typename std::enable_if<
            detail::has_value_type<
                typename std::remove_reference<String>::type>::value>::type>
        :
        public std::is_same<
            typename charcode_traits<C>::value_type,
            typename std::remove_reference<String>::type::value_type>
    {
    };
}

#endif // ENCODING_CHARCODE_HPP
