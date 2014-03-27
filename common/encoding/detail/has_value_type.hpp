/// @file has_value_type.hpp
/// @brief ����^ value_type �������ۂ���񋟂���N���X has_value_type �̃w�b�_�B
/// @author ruche

#pragma once
#ifndef ENCODING_DETAIL_HAS_VALUE_TYPE_HPP
#define ENCODING_DETAIL_HAS_VALUE_TYPE_HPP

#include <type_traits>

namespace encoding { namespace detail
{
    /// has_value_type �N���X�̎����p�N���X�B
    template<class T>
    class has_value_type_impl
    {
    private:
        template<class U>
        static std::true_type check(const typename U::value_type*);

        template<class>
        static std::false_type check(...);

    public:
        /// �^ T ������^ value_type �����Ȃ�� true_type �B
        /// �^ T ������^ value_type �������Ȃ��Ȃ�� false_type �B
        typedef decltype(check<T>(nullptr)) type;
    };

    /// �^ T ������^ value_type �������ۂ���񋟂���N���X�B
    template<class T>
    class has_value_type : public has_value_type_impl<T>::type
    {
    };
}}

#endif // ENCODING_DETAIL_HAS_VALUE_TYPE_HPP
