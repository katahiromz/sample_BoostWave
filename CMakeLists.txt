cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
#===========================================================
# CMakeLists.txt for opencv_tools
#===========================================================

# 共通モジュール設定
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(common)

# ビルド構成設定
setup_config_types()

# プロジェクト名設定
project("sample_BoostWave")

# サブプロジェクト追加
add_subdirectory("01_Basic")
add_subdirectory("02_SjisBasic")
add_subdirectory("03_IgnoreDirectiveHook")
add_subdirectory("04_OutputHookInfos")
add_subdirectory("05_MakeIncludeTree")

#===========================================================
# EOF
