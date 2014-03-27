/// @file convert.hpp
/// @brief �����R�[�h�ϊ������֐� convert ��񋟂���w�b�_�B
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
    /// @brief �����R�[�h�ϊ����s���B
    /// @tparam From �ϊ����̕����R�[�h��ʗ񋓒l�B
    /// @tparam To �ϊ���̕����R�[�h��ʗ񋓒l�B
    /// @tparam FromString �ϊ����̕�����^�B
    /// @param[in] from �ϊ����̕�����B
    /// @param[in] from_to_tag �ϊ��������R�[�h�ƕϊ��敶���R�[�h�������^�O�B
    /// @param[in] loc ���P�[���ݒ�B
    /// @return �ϊ���̕�����B
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

    /// @brief �����R�[�h�ϊ����s���B
    /// @tparam From �ϊ����̕����R�[�h��ʗ񋓒l�B
    /// @tparam To �ϊ���̕����R�[�h��ʗ񋓒l�B
    /// @param[in] from �ϊ����̕�����B
    /// @param[in] from_to_tag �ϊ��������R�[�h�ƕϊ��敶���R�[�h�������^�O�B
    /// @param[in] loc ���P�[���ݒ�B
    /// @return �ϊ���̕�����B
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
