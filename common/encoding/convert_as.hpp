/// @file convert_as.hpp
/// @brief �����R�[�h�ϊ������֐� convert_as ��񋟂���w�b�_�B
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
    /// @brief �߂�l�̌^���w�肵�ĕ����R�[�h�ϊ����s���B
    /// @tparam ToString �ϊ���̕�����^�B��������K�v������B
    /// @tparam From �ϊ����̕����R�[�h��ʗ񋓒l�B
    /// @tparam To �ϊ���̕����R�[�h��ʗ񋓒l�B
    /// @tparam FromString �ϊ����̕�����^�B
    /// @param[in] from �ϊ����̕�����B
    /// @param[in] from_to_tag �ϊ��������R�[�h�ƕϊ��敶���R�[�h�������^�O�B
    /// @param[in] loc ���P�[���ݒ�B
    /// @return �ϊ���̕�����B
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

    /// @brief �߂�l�̌^���w�肵�ĕ����R�[�h�ϊ����s���B
    /// @tparam ToString �ϊ���̕�����^�B��������K�v������B
    /// @tparam From �ϊ����̕����R�[�h��ʗ񋓒l�B
    /// @tparam To �ϊ���̕����R�[�h��ʗ񋓒l�B
    /// @param[in] from �ϊ����̕�����B
    /// @param[in] from_to_tag �ϊ��������R�[�h�ƕϊ��敶���R�[�h�������^�O�B
    /// @param[in] loc ���P�[���ݒ�B
    /// @return �ϊ���̕�����B
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
