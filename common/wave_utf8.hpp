#pragma once

#include <encoding/convert.hpp>
#include <boost/wave/language_support.hpp>
#include <boost/wave/cpp_exceptions.hpp>
#include <utility>
#include <string>
#include <fstream>
#include <exception>

namespace wave_utf8
{
    /// UTF-8�������W�������R�[�h�ɕϊ�
    template<class TString>
    std::string convertFromUtf8(TString&& utf8Text)
    {
        return encoding::convert(std::forward<TString>(utf8Text), encoding::utf8_to_narrow);
    }

    /// �W�������R�[�h�̃t�@�C����UTF-8������Ƃ��ēǂݍ���
    std::string readFileToUtf8(const char* filePath)
    {
        // �t�@�C�����J��
        std::ifstream fs(filePath);
        if (!fs)
        {
            std::string msg = "Cannot open file '";
            msg += (filePath == nullptr) ? "(nullptr)" : filePath;
            msg += "'.";
            throw std::exception(msg.c_str());
        }

        // �t�@�C���ǂݍ���
        fs.unsetf(std::ios::skipws);
        std::string text(
            std::istreambuf_iterator<char>(fs.rdbuf()),
            std::istreambuf_iterator<char>());

        // UTF-8������ɕϊ����ĕԂ�
        return encoding::convert(std::move(text), encoding::narrow_to_utf8);
    }

    /// �\�[�X�t�@�C���ǂݍ��݂̃|���V�[�N���X
    /// boost::wave::iteration_context_policies::load_file_to_string �N���X����ɍ쐬
    class Utf8InputPolicy
    {
    public:
        template<typename IterContextT>
        class inner
        {
        public:
            template<typename PositionT>
            static void init_iterators(
                IterContextT& iterContext,
                const PositionT& pos,
                boost::wave::language_support language)
            {
                try
                {
                    iterContext.code = readFileToUtf8(iterContext.filename.c_str());
                }
                catch (const std::exception&)
                {
                    BOOST_WAVE_THROW_CTX(
                        iterContext.ctx,
                        boost::wave::preprocess_exception,
                        bad_include_file,
                        iterContext.filename.c_str(),
                        pos);
                    return;
                }

                typedef typename IterContextT::iterator_type iterator_type;
                iterContext.first =
                    iterator_type(
                        iterContext.code.begin(),
                        iterContext.code.end(),
                        PositionT(iterContext.filename),
                        language);
                iterContext.last = iterator_type();
            }

        private:
            std::string code;
        };
    };
}
