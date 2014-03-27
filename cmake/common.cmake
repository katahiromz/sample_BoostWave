cmake_policy(PUSH)
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
#===========================================================
# cmake common module
#===========================================================

#---------------------------------------
# 定数定義
#---------------------------------------

# プロジェクトに含めるファイルの拡張子
set(SRC_EXTENDS   "c;cc;cpp;cxx")
set(EXTRA_EXTENDS "h;hpp;hxx;inc;def")

# コンパイラに追加設定するオプション
if(MSVC)
    # boost::wave のコードで出てしまうW4警告は無視
    set(ADDITIONAL_COMPILE_FLAGS "/MP /W4 /WX /wd4100 /wd4245 /wd4512 /wd4706 /wd4996")
    set(ADDITIONAL_COMPILE_FLAGS_RELEASE "/Ox /Oi /Ot /Oy")
else()
    set(ADDITIONAL_COMPILE_FLAGS "")
    set(ADDITIONAL_COMPILE_FLAGS_RELEASE "")
endif()

#---------------------------------------
# 変数名とその内容を表示する。
#---------------------------------------
macro(show_variable V)
    message("${V}=${${V}}")
endmacro()

#---------------------------------------
# VC++ビルド構成の設定を行う。
# project コマンドよりも前に呼び出すこと。
#---------------------------------------
macro(setup_config_types)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
endmacro()

#---------------------------------------
# 共通設定を行う。
# 
# 事前に以下の変数の設定が必要。
#  - PROJ_NAME     -- プロジェクト名
#  - PROJ_SRC_DIRS -- ソースのルートディレクトリパスのリスト
#  - PROJ_INCLUDES -- インクルードパスのリスト
#
# 以下の変数は未設定ならば既定値が設定される。
#  - PROJ_TYPE     -- プロジェクトの種別
#  - PROJ_BASE_DIR -- プロジェクトのルートディレクトリパス
#
# PROJ_TYPE には下記のいずれかの値を指定する。
#  - "program" -- 実行プログラム(既定値)
#  - "static"  -- スタティックライブラリ
#  - "shared"  -- 共有ライブラリ(DLL)
#---------------------------------------
macro(setup_common)
    # 必要な変数を設定
    if(NOT PROJ_TYPE)
        set(PROJ_TYPE "program")
    endif()
    if(NOT PROJ_BASE_DIR)
        set(PROJ_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
    endif()

    # ビルド構成設定
    setup_config_types()

    # プロジェクト設定
    project("${PROJ_NAME}")

    # ソースファイルリスト作成
    set(SRC_FILES "")
    foreach(SRC_DIR ${PROJ_SRC_DIRS})
        # ファイルをリストアップ
        set(FILES_ON_DIR "")

        # ソース以外のファイルを先に追加
        foreach(EXTRA_EXT ${EXTRA_EXTENDS})
            file(GLOB_RECURSE TEMP_SRC_FILES
                "${PROJ_BASE_DIR}/${SRC_DIR}/*.${EXTRA_EXT}")
            list(APPEND FILES_ON_DIR ${TEMP_SRC_FILES})
        endforeach()

        # コンパイル対象外のプロジェクトアイテムにする
        set_source_files_properties(${FILES_ON_DIR} PROPERTIES HEADER_FILE_ONLY on)

        # ソースファイルを追加
        foreach(SRC_EXT ${SRC_EXTENDS})
            file(GLOB_RECURSE TEMP_SRC_FILES
                "${PROJ_BASE_DIR}/${SRC_DIR}/*.${SRC_EXT}")
            list(APPEND FILES_ON_DIR ${TEMP_SRC_FILES})
        endforeach()

        # ファイルリストに追加
        list(APPEND SRC_FILES ${FILES_ON_DIR})
    endforeach()

    # 全ファイルに対する処理
    foreach(SRC_FILE ${SRC_FILES})
        # ディレクトリの相対パス取得
        file(RELATIVE_PATH REL_PATH "${PROJ_BASE_DIR}" "${SRC_FILE}")
        string(REGEX REPLACE "/[^/]+$" "" REL_PATH "${REL_PATH}")

        # ファイルの属するプロジェクト階層を設定(主にVC++用)
        string(REPLACE "/" "\\" PROJ_TREE "${REL_PATH}")
        source_group("${PROJ_TREE}" FILES "${SRC_FILE}")

        # ソースファイル名出力
        message("INPUT: ${SRC_FILE}")
    endforeach()

    # 作成設定
    if("${PROJ_TYPE}" STREQUAL "static")
        add_library("${PROJ_NAME}" STATIC ${SRC_FILES})
    elseif("${PROJ_TYPE}" STREQUAL "shared")
        add_library("${PROJ_NAME}" SHARED ${SRC_FILES})
    else()
        add_executable("${PROJ_NAME}" ${SRC_FILES})
    endif()

    # デバッグ版のポストフィックス設定
    set_target_properties("${PROJ_NAME}" PROPERTIES DEBUG_POSTFIX "_d")

    # インクルードパス追加
    foreach(INC_PATH ${PROJ_INCLUDES})
        include_directories("${PROJ_BASE_DIR}/${INC_PATH}")
    endforeach()

    # コンパイルオプションの設定
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} ${ADDITIONAL_COMPILE_FLAGS}")
    set(CMAKE_CXX_FLAGS_RELEASE
        "${CMAKE_CXX_FLAGS_RELEASE} ${ADDITIONAL_COMPILE_FLAGS_RELEASE}")
    set(CMAKE_C_FLAGS
        "${CMAKE_C_FLAGS} ${ADDITIONAL_COMPILE_FLAGS}")
    set(CMAKE_C_FLAGS_RELEASE
        "${CMAKE_C_FLAGS_RELEASE} ${ADDITIONAL_COMPILE_FLAGS_RELEASE}")
endmacro()

#===========================================================
cmake_policy(POP)
# EOF
