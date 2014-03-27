cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
#===========================================================
# CMakeLists.txt for common library
#===========================================================

# 共通モジュール設定
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
include(common)

# プロジェクト名
set(PROJ_NAME "02_SjisBasic")

# プロジェクト種別
set(PROJ_TYPE "program")

# ソースのルートディレクトリパス
set(PROJ_SRC_DIRS "src" "../common")

# インクルードパス
set(PROJ_INCLUDES "../common")

#===========================================================

# 共通設定
message("-- setup : ${PROJ_NAME}")
setup_common()
message("-- setup end")

#===========================================================
# EOF
