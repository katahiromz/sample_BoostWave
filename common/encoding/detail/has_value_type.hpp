/// @file has_value_type.hpp
/// @brief 内包型 value_type を持つか否かを提供するクラス has_value_type のヘッダ。
/// @author ruche

#pragma once
#ifndef ENCODING_DETAIL_HAS_VALUE_TYPE_HPP
#define ENCODING_DETAIL_HAS_VALUE_TYPE_HPP

#include <type_traits>

namespace encoding { namespace detail
{
    /// has_value_type クラスの実装用クラス。
    template<class T>
    class has_value_type_impl
    {
    private:
        template<class U>
        static std::true_type check(const typename U::value_type*);

        template<class>
        static std::false_type check(...);

    public:
        /// 型 T が内包型 value_type を持つならば true_type 。
        /// 型 T が内包型 value_type を持たないならば false_type 。
        typedef decltype(check<T>(nullptr)) type;
    };

    /// 型 T が内包型 value_type を持つか否かを提供するクラス。
    template<class T>
    class has_value_type : public has_value_type_impl<T>::type
    {
    };
}}

#endif // ENCODING_DETAIL_HAS_VALUE_TYPE_HPP
