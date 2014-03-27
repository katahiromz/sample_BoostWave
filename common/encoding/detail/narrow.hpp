/// @file narrow.hpp
/// @brief �}���`�o�C�g�ƃ��C�h�Ԃ̕����R�[�h�ϊ�������񋟂���w�b�_�B
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
    /// �}���`�o�C�g�����^�B
    typedef charcode_traits<charcode::narrow>::value_type narrow_char_t;

    /// ���C�h�����^�B
    typedef charcode_traits<charcode::wide>::value_type wide_char_t;

    /// �����R�[�h�ϊ��ɗp���� codecvt �^�B
    typedef std::codecvt<wide_char_t, narrow_char_t, std::mbstate_t> codecvt_t;

    /// @brief �R���o�[�^�N���X�B
    /// @tparam FromChar �ϊ����̕����^�B
    /// @tparam ToChar �ϊ���̕����^�B
    /// @tparam ToAlloc �ϊ���̃A���P�[�^�^�B
    template<class FromChar, class ToChar, class ToAlloc>
    struct converter;

    /// @brief �}���`�o�C�g���烏�C�h�ւ̃R���o�[�^�N���X�B
    /// @tparam ToAlloc �ϊ���̃A���P�[�^�^�B
    template<class ToAlloc>
    struct converter<narrow_char_t, wide_char_t, ToAlloc>
    {
        /// wstring_convert �^�B
        typedef std::wstring_convert<codecvt_t, wide_char_t, ToAlloc> type;

        /// �����R�[�h�ϊ����ʌ^�B
        typedef typename type::wide_string result_type;

        /// �����R�[�h��ϊ�����B
        static result_type convert(
            const codecvt_t& cvt,
            const narrow_char_t* first,
            const narrow_char_t* last)
        {
            return type(&cvt).from_bytes(first, last);
        }
    };

    /// @brief ���C�h����}���`�o�C�g�ւ̃R���o�[�^�N���X�B
    /// @tparam ToAlloc �ϊ���̃A���P�[�^�^�B
    template<class ToAlloc>
    struct converter<wide_char_t, narrow_char_t, ToAlloc>
    {
        /// wstring_convert �^�B
        typedef
            std::wstring_convert<
                codecvt_t,
                wide_char_t,
                std::allocator<wide_char_t>,
                ToAlloc>
            type;

        /// �����R�[�h�ϊ����ʌ^�B
        typedef typename type::byte_string result_type;

        /// �����R�[�h��ϊ�����B
        static result_type convert(
            const codecvt_t& cvt,
            const wide_char_t* first,
            const wide_char_t* last)
        {
            return type(&cvt).to_bytes(first, last);
        }
    };

    /// @brief �R���o�[�g�����֐��̓�����񋟂���N���X�B
    /// @tparam FromString �ϊ����̕�����^�B
    /// @tparam ToString �ϊ���̕�����^�B
    template<class FromString, class ToString>
    struct convert_traits
    {
        /// �ϊ����̕�����^�B
        typedef typename std::remove_reference<FromString>::type from_string_type;

        /// �ϊ���̕�����^�B
        typedef typename std::remove_reference<ToString>::type to_string_type;

        /// �ϊ����̕����^�B
        typedef typename from_string_type::value_type from_char_type;

        /// �ϊ���̕����^�B
        typedef typename to_string_type::value_type to_char_type;

        /// �ϊ���̃A���P�[�^�^�B
        typedef typename to_string_type::allocator_type to_allocator_type;

        /// �R���o�[�^�N���X�^�B
        typedef converter<from_char_type, to_char_type, to_allocator_type> converter_t;

        /// �R���o�[�g���ʂ� to_string_type �ɕϊ��\�Ȃ�� true �B
        static const bool result_convertible =
            std::is_convertible<converter_t::result_type, to_string_type>::value;
    };

    /// �}���`�o�C�g�ƃ��C�h�Ԃŕ����R�[�h�ϊ����s���B
    template<class ToString, class FromString>
    inline typename std::enable_if<
        convert_traits<FromString, ToString>::result_convertible,
        typename convert_traits<FromString, ToString>::to_string_type>::type
    convert(FromString&& from, const std::locale& loc)
    {
        // �߂�l�� to_string_type �ɕϊ��\�Ȃ̂ł��̂܂ܕԂ�
        return
            convert_traits<FromString, ToString>::converter_t::convert(
                std::use_facet<codecvt_t>(loc),
                from.data(),
                from.data() + from.size());
    }

    /// �}���`�o�C�g�ƃ��C�h�Ԃŕ����R�[�h�ϊ����s���B
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
