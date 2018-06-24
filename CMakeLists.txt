# CMakeLists.txt

# Copyright (C) 2018 Yufei Yuan

# This code is released under the libbpg license.
# For conditions of distribution and use, see the disclaimer
# and license in README.

set(CMAKE_CONFIGURATION_TYPES "Release;Debug")

project(libbpg)
cmake_minimum_required(VERSION 3.2)

set(LIBBPG_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

set(LIBBPG_MAJOR 0)
set(LIBBPG_MINOR 9)
set(LIBBPG_RELEASE 8)
set(LIBBPG_NAME bpg)
set(LIBBPG_VERSION ${LIBBPG_MAJOR}.${LIBBPG_MINOR}.${LIBBPG_RELEASE})

include(GNUInstallDirs)

# Command line options
option(LIBBPG_SHARED "Build shared lib" ON)
option(LIBBPG_STATIC "Build static lib" ON)
option(LIBBPG_INSTALL_PREFIX "Installation prefix" "${CMAKE_INSTALL_PREFIX}")

# Many more configuration options could be added here
option(LIBBPG_EMCC "Build JavaScript decoder with Emscripten" OFF)
option(LIBBPG_X265 "Build with X265 for encoder" ON)
option(LIBBPG_JCTVC "Build with HM reference encoder" OFF)
option(LIBBPG_BPGVIEW "Build bpgview utility" ON)
option(LIBBPG_HIGH_BIT_DEPTH "Build with bit depths > 12" OFF)

set(CPACK_PACKAGE_VENDOR "LibBPG")
set(CPACK_RESOURCE_FILE_LICENSE "${LIBBPG_BASE_DIR}/README")
set(CPACK_MONOLITHIC_INSTALL OFF)
if (WIN32)
  set(CPACK_GENERATOR "ZIP")
else()
  set(CPACK_GENERATOR "TGZ")
endif()

# Remove this to get package names that are formatted as
# ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_SYSTEM_NAME}.
set(CPACK_PACKAGE_FILE_NAME "libbpg" CACHE STRING "Name of package created by distrib target")
include(CPack)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set(CMAKE_OBJECT_PATH_MAX 260)
  message("Windows: setting CMAKE_OBJECT_PATH_MAX to ${CMAKE_OBJECT_PATH_MAX}")
endif()

set(CMAKE_MACOSX_RPATH ON)

# to distinguish between debug and release lib
set(CMAKE_DEBUG_POSTFIX "d")

# Export all symbols
SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Set warnings globally
option(WARNINGS_AS_ERRORS "Treat warnings as errors" ON)
if (WARNINGS_AS_ERRORS)
    message(STATUS "WARNINGS_AS_ERRORS enabled")
else()
    message(STATUS "WARNINGS_AS_ERRORS disabled")
endif()

# Our source files
set(LIBBPG_PUBLIC_HEADERS
    libbpg.h
)

set(LIBBPG_PRIVATE_HEADERS
    config.h
    libavcodec/avcodec.h
    libavcodec/bswapdsp.h
    libavcodec/bytestream.h
    libavcodec/cabac_functions.h
    libavcodec/cabac.h
    libavcodec/cabac_tablegen.h
    libavcodec/get_bits.h
    libavcodec/golomb.h
    libavcodec/hevcdsp.h
    libavcodec/hevc.h
    libavcodec/hevcpred.h
    libavcodec/internal.h
    libavcodec/mathops.h
    libavcodec/old_codec_ids.h
    libavcodec/put_bits.h
    libavcodec/rnd_avg.h
    libavcodec/thread.h
    libavcodec/version.h
    libavcodec/videodsp.h
    libavutil/buffer.h
    libavutil/buffer_internal.h
    libavutil/frame.h
    libavutil/internal.h
    libavutil/intmath.h
    libavutil/log.h
    libavutil/md5.h
    libavutil/mem.h
    libavutil/pixdesc.h
)

set(LIBBPG_SOURCES
    ${LIBBPG_PUBLIC_HEADERS}
    ${LIBBPG_PRIVATE_HEADERS}
    libavcodec/cabac.c
    libavcodec/golomb.c
    libavcodec/hevc.c
    libavcodec/hevc_cabac.c
    libavcodec/hevcdsp.c
    libavcodec/hevc_filter.c
    libavcodec/hevc_mvs.c
    libavcodec/hevcpred.c
    libavcodec/hevc_ps.c
    libavcodec/hevc_refs.c
    libavcodec/hevc_sei.c
    libavcodec/utils.c
    libavcodec/videodsp.c
    libavutil/buffer.c
    libavutil/frame.c
    libavutil/log2_tab.c
    libavutil/md5.c
    libavutil/mem.c
    libavutil/pixdesc.c
    libbpg.c
)

set(LIBBPG_JCTVC_SOURCES
    jctvc/program_options_lite.cpp
    jctvc/TAppEncCfg.cpp
    jctvc/TAppEncTop.cpp
    jctvc/libmd5/libmd5.c
    jctvc/TLibCommon/ContextModel3DBuffer.cpp
    jctvc/TLibCommon/ContextModel.cpp
    jctvc/TLibCommon/Debug.cpp
    jctvc/TLibCommon/SEI.cpp
    jctvc/TLibCommon/TComBitStream.cpp
    jctvc/TLibCommon/TComCABACTables.cpp
    jctvc/TLibCommon/TComChromaFormat.cpp
    jctvc/TLibCommon/TComDataCU.cpp
    jctvc/TLibCommon/TComInterpolationFilter.cpp
    jctvc/TLibCommon/TComLoopFilter.cpp
    jctvc/TLibCommon/TComMotionInfo.cpp
    jctvc/TLibCommon/TComPattern.cpp
    jctvc/TLibCommon/TComPic.cpp
    jctvc/TLibCommon/TComPicSym.cpp
    jctvc/TLibCommon/TComPicYuv.cpp
    jctvc/TLibCommon/TComPicYuvMD5.cpp
    jctvc/TLibCommon/TComPrediction.cpp
    jctvc/TLibCommon/TComRdCost.cpp
    jctvc/TLibCommon/TComRdCostWeightPrediction.cpp
    jctvc/TLibCommon/TComRom.cpp
    jctvc/TLibCommon/TComSampleAdaptiveOffset.cpp
    jctvc/TLibCommon/TComSlice.cpp
    jctvc/TLibCommon/TComTrQuant.cpp
    jctvc/TLibCommon/TComTU.cpp
    jctvc/TLibCommon/TComWeightPrediction.cpp
    jctvc/TLibCommon/TComYuv.cpp
    jctvc/TLibEncoder/NALwrite.cpp
    jctvc/TLibEncoder/SEIwrite.cpp
    jctvc/TLibEncoder/SyntaxElementWriter.cpp
    jctvc/TLibEncoder/TEncAnalyze.cpp
    jctvc/TLibEncoder/TEncBinCoderCABACCounter.cpp
    jctvc/TLibEncoder/TEncBinCoderCABAC.cpp
    jctvc/TLibEncoder/TEncCavlc.cpp
    jctvc/TLibEncoder/TEncCu.cpp
    jctvc/TLibEncoder/TEncEntropy.cpp
    jctvc/TLibEncoder/TEncGOP.cpp
    jctvc/TLibEncoder/TEncPic.cpp
    jctvc/TLibEncoder/TEncPreanalyzer.cpp
    jctvc/TLibEncoder/TEncRateCtrl.cpp
    jctvc/TLibEncoder/TEncSampleAdaptiveOffset.cpp
    jctvc/TLibEncoder/TEncSbac.cpp
    jctvc/TLibEncoder/TEncSearch.cpp
    jctvc/TLibEncoder/TEncSlice.cpp
    jctvc/TLibEncoder/TEncTop.cpp
    jctvc/TLibEncoder/WeightPredAnalysis.cpp
    jctvc/TLibVideoIO/TVideoIOYuv.cpp
)

set(LIBBPG_BPGDEC_SOURCES
    bpgdec.c
)

if(LIBBPG_X265)
    set(LIBBPG_BPGENC_SOURCES
        bpgenc.h
        x265/source/x265.h
        bpgenc.c
        x265_glue.c
    )
else()
    set(LIBBPG_BPGENC_SOURCES
        bpgenc.h
        bpgenc.c
        jctvc_glue.cpp
    )
endif()

set(LIBBPG_BPGVIEW_SOURCES
    bpgview.c
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(NOT MSVC)
    add_definitions(-D_FILE_OFFSET_BITS=64
                    -D_LARGEFILE_SOURCE
                    -D_REENTRANT
                    -DCONFIG_BPG_VERSION="${LIBBPG_VERSION}")

    add_compile_options(-Os
                        -Wall
                        -MMD
                        -fno-asynchronous-unwind-tables
                        -fdata-sections
                        -ffunction-sections
                        -fno-math-errno
                        -fno-signed-zeros
                        -fno-tree-vectorize
                        -fomit-frame-pointer)

    if(WARNINGS_AS_ERRORS)
        add_compile_options(-Werror
                            -Wno-switch
                            -Wno-logical-not-parentheses
                            -Wno-maybe-uninitialized
                            -Wno-dangling-else
                            -Wno-parentheses
                            -Wno-unused-variable
                            -Wno-unused-function
                            -Wno-unused-but-set-variable
                            -Wno-misleading-indentation
                            -Wno-sign-compare
                            -Wno-format-truncation
                            -Wno-shift-negative-value
                            -Wno-implicit-fallthrough
                            -Wno-unused-label)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-pointer-sign")
    endif()

    if(LIBBPG_HIGH_BIT_DEPTH)
        add_definitions(-DRExt__HIGH_BIT_DEPTH_SUPPORT)
    endif()

    if(APPLE)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-dead_strip")
    else()
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")
    endif()
else()
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)

    add_compile_options(/W3)
    add_compile_options(/wd4018)  # disable "signed/unsigned mismatch"
    add_compile_options(/wd4503)  # disable "decorated name length exceeded, name was truncated"
    add_compile_options(/wd4267)  # disable "conversion from 'size_t' to 'int', possible loss of data"
    add_compile_options(/wd4800)  # forcing value to bool 'true' or 'false' (performance warning)
    if(WARNINGS_AS_ERRORS)
        add_compile_options(/WX)
    endif()
endif()

unset(BPG_LIB_TARGETS)

if(LIBBPG_SHARED)
    add_library(bpg SHARED ${LIBBPG_SOURCES})
    set(BPG_LIB_TARGETS bpg)
    set_target_properties(bpg PROPERTIES OUTPUT_NAME ${LIBBPG_NAME})
    if(MSVC)
        set_target_properties(bpg PROPERTIES PREFIX "lib")
        set_target_properties(bpg PROPERTIES IMPORT_PREFIX "lib")
    endif()
    target_compile_definitions(bpg PUBLIC
                               -D_ISOC99_SOURCE
                               -D_POSIX_C_SOURCE=200112
                               -D_XOPEN_SOURCE=600
                               -DHAVE_AV_CONFIG_H
                               -D_GNU_SOURCE=1
                               -DUSE_VAR_BIT_DEPTH
                               -DUSE_PRED)
    target_compile_options(bpg PUBLIC -std=c99)
endif()

if(LIBBPG_STATIC)
    add_library(bpg_static STATIC ${LIBBPG_SOURCES})
    set_target_properties(bpg_static PROPERTIES OUTPUT_NAME ${LIBBPG_NAME}
                          CLEAN_DIRECT_OUTPUT 1)
    list(APPEND BPG_LIB_TARGETS bpg_static)
    if(MSVC)
        set_target_properties(bpg_static PROPERTIES PREFIX "lib")
    endif()
    target_compile_definitions(bpg_static PUBLIC
                               -D_ISOC99_SOURCE
                               -D_POSIX_C_SOURCE=200112
                               -D_XOPEN_SOURCE=600
                               -DHAVE_AV_CONFIG_H
                               -D_GNU_SOURCE=1
                               -DUSE_VAR_BIT_DEPTH
                               -DUSE_PRED)
    target_compile_options(bpg_static PUBLIC -std=c99)
endif()

if(LIBBPG_X265)
    set(x265_12bit "${CMAKE_CURRENT_BINARY_DIR}/x265.out/12bit/libx265.a")

    add_custom_command(OUTPUT ${x265_12bit}
                       COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/x265.out/12bit"
                       COMMAND ${CMAKE_COMMAND} -E chdir "${CMAKE_CURRENT_BINARY_DIR}/x265.out/12bit" ${CMAKE_COMMAND} "${CMAKE_CURRENT_SOURCE_DIR}/x265/source" ARGS "-DHIGH_BIT_DEPTH=ON" "-DEXPORT_C_API=OFF" "-DENABLE_SHARED=OFF" "-DENABLE_CLI=OFF" "-DMAIN12=ON"
                       COMMAND ${CMAKE_COMMAND} --build "${CMAKE_CURRENT_BINARY_DIR}/x265.out/12bit"
                       COMMENT "Build 12-bit x265 library"
                       VERBATIM
                       COMMAND_EXPAND_LISTS)
    add_custom_target(libx265_12bit ALL DEPENDS ${x265_12bit})

    set(x265_10bit "${CMAKE_CURRENT_BINARY_DIR}/x265.out/10bit/libx265.a")

    add_custom_command(OUTPUT ${x265_10bit}
                       COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/x265.out/10bit"
                       COMMAND ${CMAKE_COMMAND} -E chdir "${CMAKE_CURRENT_BINARY_DIR}/x265.out/10bit" ${CMAKE_COMMAND} "${CMAKE_CURRENT_SOURCE_DIR}/x265/source" ARGS "-DHIGH_BIT_DEPTH=ON" "-DEXPORT_C_API=OFF" "-DENABLE_SHARED=OFF" "-DENABLE_CLI=OFF" "-DMAIN10=ON"
                       COMMAND ${CMAKE_COMMAND} --build "${CMAKE_CURRENT_BINARY_DIR}/x265.out/10bit"
                       DEPENDS ${x265_12bit}
                       COMMENT "Build 10-bit x265 library"
                       VERBATIM
                       COMMAND_EXPAND_LISTS)
    add_custom_target(libx265_10bit ALL DEPENDS ${x265_10bit})

    set(x265_8bit "${CMAKE_CURRENT_BINARY_DIR}/x265.out/8bit/libx265.a")

    add_custom_command(OUTPUT ${x265_8bit}
                       COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/x265.out/8bit"
                       COMMAND ${CMAKE_COMMAND} -E chdir "${CMAKE_CURRENT_BINARY_DIR}/x265.out/8bit" ${CMAKE_COMMAND} "${CMAKE_CURRENT_SOURCE_DIR}/x265/source" ARGS "-DLINKED_10BIT=ON" "-DLINKED_12BIT=ON" "-DENABLE_SHARED=OFF" "-DENABLE_CLI=OFF"
                       COMMAND ${CMAKE_COMMAND} --build "${CMAKE_CURRENT_BINARY_DIR}/x265.out/8bit"
                       DEPENDS ${x265_10bit}
                       COMMENT "Build 8-bit x265 library"
                       VERBATIM
                       COMMAND_EXPAND_LISTS)
    add_custom_target(libx265_8bit ALL DEPENDS ${x265_8bit})
endif()

if(LIBBPG_JCTVC)
    add_library(jctvc STATIC ${LIBBPG_JCTVC_SOURCES})
    target_compile_options(jctvc PRIVATE -Wno-sign-compare)
    target_include_directories(jctvc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/jctvc)
endif()

add_executable(bpgdec ${LIBBPG_BPGDEC_SOURCES})
if(MSVC)
    target_link_libraries(bpgdec bpg png z)
else()
    if(NOT APPLE)
        target_link_libraries(bpgdec rt)
    endif()
    target_link_libraries(bpgdec bpg png z m pthread)
endif()

add_executable(bpgenc ${LIBBPG_BPGENC_SOURCES})
if(LIBBPG_JCTVC)
    if(MSVC)
        target_link_libraries(bpgenc jctvc png jpeg z)
    else()
        if(NOT APPLE)
            target_link_libraries(bpgenc rt)
        endif()
        target_link_libraries(bpgenc jctvc png jpeg z m pthread)
    endif()
    target_compile_options(bpgenc PRIVATE -Wno-unused-but-set_variable)
    target_compile_definitions(bpgenc PRIVATE -DUSE_JCTVC)
    target_include_directories(bpgenc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/jctvc)
else()
    if(MSVC)
        target_link_libraries(bpgenc png jpeg z)
    else()
        if(NOT APPLE)
            target_link_libraries(bpgenc rt)
        endif()
        target_link_libraries(bpgenc ${x265_8bit} ${x265_12bit} ${x265_10bit} png jpeg z m pthread stdc++)
    endif()
    target_compile_definitions(bpgenc PRIVATE -DUSE_X265)
    target_include_directories(bpgenc PRIVATE
                               ${CMAKE_CURRENT_SOURCE_DIR}/x265/source
                               ${CMAKE_CURRENT_BINARY_DIR}/x265.out/8bit)
    add_dependencies(bpgenc libx265_8bit)
endif()

if(LIBBPG_BPGVIEW)
    add_executable(bpgview ${LIBBPG_BPGVIEW_SOURCES})
    if(MSVC)
        target_link_libraries(bpgview bpg mingw32 SDLmain SDL_image SDL mwindows)
    else()
        if(NOT APPLE)
            target_link_libraries(bpgview rt)
        endif()
        target_link_libraries(bpgview bpg m pthread SDL_image SDL)
    endif()
endif()

# ------------------------------------------------
# install

install(TARGETS bpgdec bpgenc
        RUNTIME DESTINATION bin)

if(NOT WIN32)
  set(CPACK_PACKAGE_EXT "tar.gz")
  set(DISTRIB_PACKAGE_EXT "tgz")
else()
  set(CPACK_PACKAGE_EXT "zip")
  set(DISTRIB_PACKAGE_EXT ${CPACK_PACKAGE_EXT})
endif()

