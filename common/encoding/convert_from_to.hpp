/// @file convert_from_to.hpp
/// @brief �����R�[�h�ϊ���ʃ^�O�N���X convert_from_to ��񋟂���w�b�_�B
/// @author ruche

#pragma once
#ifndef ENCODING_CONVERT_FROM_TO_HPP
#define ENCODING_CONVERT_FROM_TO_HPP

#include <encoding/charcode.hpp>

namespace encoding
{
    /// �����R�[�h�ϊ���ʃ^�O�N���X�B
    template<charcode From, charcode To>
    struct convert_from_to { };

#define ENCODING_PP_DEFINE_CONVERT_FROM_TO(F,T)                         \
    typedef convert_from_to<charcode::F, charcode::T> F##_to_##T##_t;   \
    static const F##_to_##T##_t F##_to_##T

    // ��`�ςݕ����R�[�h�ϊ����
    ENCODING_PP_DEFINE_CONVERT_FROM_TO(narrow,wide);    // narrow_to_wide
    ENCODING_PP_DEFINE_CONVERT_FROM_TO(wide,narrow);    // wide_to_narrow
    ENCODING_PP_DEFINE_CONVERT_FROM_TO(narrow,utf8);    // narrow_to_utf8
    ENCODING_PP_DEFINE_CONVERT_FROM_TO(utf8,narrow);    // utf8_to_narrow
    ENCODING_PP_DEFINE_CONVERT_FROM_TO(wide,utf8);      // wide_to_utf8
    ENCODING_PP_DEFINE_CONVERT_FROM_TO(utf8,wide);      // utf8_to_wide

#undef ENCODING_PP_DEFINE_CONVERT_TAG
}

#endif // ENCODING_CONVERT_FROM_TO_HPP
