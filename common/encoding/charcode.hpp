/// @file charcode.hpp
/// @brief �����R�[�h��ʗ񋓃N���X charcode ��񋟂���w�b�_�B
/// @author ruche

#pragma once
#ifndef ENCODING_CHARCODE_HPP
#define ENCODING_CHARCODE_HPP

#include <encoding/detail/has_value_type.hpp>
#include <type_traits>
#include <string>

namespace encoding
{
    /// �����R�[�h��ʗ񋓃N���X�B
    enum class charcode
    {
        /// �}���`�o�C�g�B
        narrow,

        /// ���C�h�BWindows�ł�UTF-16LE�B
        wide,

        /// UTF-8�B
        utf8,
    };

    /// �����R�[�h��ʗ񋓒l�̓�����񋟂���N���X�B
    template<charcode C>
    struct charcode_traits;

    /// ���ꉻ���ꂽ charcode_traits �N���X�̊��N���X�^�B
    template<class TChar>
    struct charcode_traits_base
    {
        /// �����^�B
        typedef TChar value_type;

        /// ����̕�����^�B
        typedef std::basic_string<value_type> string_type;
    };

    /// �����R�[�h��ʗ񋓒l charcode::narrow �̓�����񋟂���N���X�B
    template<>
    struct charcode_traits<charcode::narrow> : public charcode_traits_base<char>
    {
    };

    /// �����R�[�h��ʗ񋓒l charcode::wide �̓�����񋟂���N���X�B
    template<>
    struct charcode_traits<charcode::wide> : public charcode_traits_base<wchar_t>
    {
    };

    /// �����R�[�h��ʗ񋓒l charcode::utf8 �̓�����񋟂���N���X�B
    template<>
    struct charcode_traits<charcode::utf8> : public charcode_traits_base<char>
    {
    };

    /// @brief �w�肵�������R�[�h��ʂ��i�[�\�ȕ�����^�ł��邩�ۂ��𒲂ׂ�B
    /// @tparam C �����R�[�h��ʗ񋓒l�B
    /// @tparam String ������^�B�Q�ƌ^�̏ꍇ�� remove_reference �����B
    template<charcode C, class String, class = void>
    class is_charcode_string : public std::false_type
    {
    };

    /// @brief �w�肵�������R�[�h��ʂ��i�[�\�ȕ�����^�ł��邩�ۂ��𒲂ׂ�B
    /// @tparam C �����R�[�h��ʗ񋓒l�B
    /// @tparam String ������^�B�Q�ƌ^�̏ꍇ�� remove_reference �����B
    ///
    /// charcode_traits<C>::value_type �� String::value_type �̌^�����������
    /// �ÓI�����o�ϐ� value �� true �ƂȂ�B
    /// ����ȊO�̏ꍇ�A value �� false �ƂȂ�B
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
