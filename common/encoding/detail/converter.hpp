/// @file converter.hpp
/// @brief �����R�[�h�ϊ������֐��̃w���p�N���X converter ��񋟂���w�b�_�B
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
    /// @brief �����R�[�h�ϊ����s���t�@���N�^�N���X�B
    /// @tparam From �ϊ����̕����R�[�h��ʗ񋓒l�B
    /// @tparam To �ϊ���̕����R�[�h��ʗ񋓒l�B
    /// @tparam FromString �ϊ����̕�����^�B
    /// @tparam ToString �ϊ���̕�����^�B
    template<charcode From, charcode To, class FromString, class ToString, class = void>
    struct converter;

    /// @brief ���ꉻ���ꂽ converter �N���X�̏������ʂ̌^��񋟂���N���X�B
    /// @tparam ToString �ϊ���̕�����^�B
    template<class ToString>
    struct converter_result
    {
        /// �߂�l�̌^�B
        typedef typename std::remove_reference<ToString>::type type;
    };

    /// ����̕����R�[�h�Ԃł̕ϊ����s���t�@���N�^�N���X�B
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
        /// �߂�l�̌^�B
        typedef typename converter_result<ToString>::type result_type;

        result_type operator()(FromString&& from, const std::locale&) const
        {
            // result_type �ɕϊ��\�Ȃ̂ł��̂܂ܕԂ�
            return from;
        }
    };

    /// ����̕����R�[�h�Ԃł̕ϊ����s���t�@���N�^�N���X�B
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
        /// �߂�l�̌^�B
        typedef typename converter_result<ToString>::type result_type;

        result_type operator()(FromString&& from, const std::locale&) const
        {
            return result_type(std::begin(from), std::end(from));
        }
    };

    /// �}���`�o�C�g���烏�C�h�ւ̕����R�[�h�ϊ����s���t�@���N�^�N���X�B
    template<class FromString, class ToString>
    struct converter<charcode::narrow, charcode::wide, FromString, ToString>
    {
        /// �߂�l�̌^�B
        typedef typename converter_result<ToString>::type result_type;

        result_type operator()(FromString&& from, const std::locale& loc) const
        {
            return narrow::convert<result_type>(std::forward<FromString>(from), loc);
        }
    };

    /// ���C�h����}���`�o�C�g�ւ̕����R�[�h�ϊ����s���t�@���N�^�N���X�B
    template<class FromString, class ToString>
    struct converter<charcode::wide, charcode::narrow, FromString, ToString>
    {
        /// �߂�l�̌^�B
        typedef typename converter_result<ToString>::type result_type;

        result_type operator()(FromString&& from, const std::locale& loc) const
        {
            return narrow::convert<result_type>(std::forward<FromString>(from), loc);
        }
    };

    /// �}���`�o�C�g����UTF-8�ւ̕����R�[�h�ϊ����s���t�@���N�^�N���X�B
    template<class FromString, class ToString>
    struct converter<charcode::narrow, charcode::utf8, FromString, ToString>
    {
        /// �߂�l�̌^�B
        typedef typename converter_result<ToString>::type result_type;

        /// ���C�h������^�B
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

    /// UTF-8����}���`�o�C�g�ւ̕����R�[�h�ϊ����s���t�@���N�^�N���X�B
    template<class FromString, class ToString>
    struct converter<charcode::utf8, charcode::narrow, FromString, ToString>
    {
        /// �߂�l�̌^�B
        typedef typename converter_result<ToString>::type result_type;

        /// ���C�h������^�B
        typedef typename charcode_traits<charcode::wide>::string_type wide_string_type;

        result_type operator()(FromString&& from, const std::locale& loc) const
        {
            return
                narrow::convert<result_type>(
                    utf8::convert<wide_string_type>(std::forward<FromString>(from)),
                    loc);
        }
    };

    /// ���C�h����UTF-8�ւ̕����R�[�h�ϊ����s���t�@���N�^�N���X�B
    template<class FromString, class ToString>
    struct converter<charcode::wide, charcode::utf8, FromString, ToString>
    {
        /// �߂�l�̌^�B
        typedef typename converter_result<ToString>::type result_type;

        result_type operator()(FromString&& from, const std::locale&) const
        {
            return utf8::convert<result_type>(std::forward<FromString>(from));
        }
    };

    /// UTF-8���烏�C�h�ւ̕����R�[�h�ϊ����s���t�@���N�^�N���X�B
    template<class FromString, class ToString>
    struct converter<charcode::utf8, charcode::wide, FromString, ToString>
    {
        /// �߂�l�̌^�B
        typedef typename converter_result<ToString>::type result_type;

        result_type operator()(FromString&& from, const std::locale&) const
        {
            return utf8::convert<result_type>(std::forward<FromString>(from));
        }
    };
}}

#endif // ENCODING_DETAIL_CONVERTER_HPP
