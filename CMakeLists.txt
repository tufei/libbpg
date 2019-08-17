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
set(LIBBPG_DEC_NAME bpg_dec)
set(LIBBPG_ENC_NAME bpg_enc)
set(LIBBPG_VERSION ${LIBBPG_MAJOR}.${LIBBPG_MINOR}.${LIBBPG_RELEASE})

include(GNUInstallDirs)

# Command line options
option(LIBBPG_SHARED "Build shared lib" OFF)
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

# Add the include paths and link dependencies for bpgenc/bpgdec.
function(bpg_image_io TARGET)
  foreach(PKG PNG JPEG)
    find_package(${PKG} QUIET)
    if(${PKG}_FOUND)
      target_compile_definitions(${TARGET} PRIVATE ${${PKG}_DEFINITIONS})
      target_include_directories(${TARGET} PRIVATE ${${PKG}_INCLUDE_DIRS})
      target_link_libraries(${TARGET} PRIVATE ${${PKG}_LIBRARIES})
      message(STATUS "${PKG} found for ${TARGET}: include path ${${PKG}_INCLUDE_DIRS}, link path ${${PKG}_LIBRARIES}")
      message(STATUS "${PKG} include path ${${PKG}_INCLUDE_DIRS}")
      message(STATUS "${PKG} link path ${${PKG}_LIBRARIES}")
    else()
        message(SEND_ERROR "${PKG} not found for ${TARGET}")
    endif()
  endforeach()
endfunction()

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
set(LIBBPG_PUBLIC_HEADERS libbpg.h bpg.h)

set(LIBBPG_DEC_PUBLIC_HEADERS
    ${LIBBPG_PUBLIC_HEADERS}
    bpg_decoder.h
)

set(LIBBPG_ENC_PUBLIC_HEADERS
    ${LIBBPG_PUBLIC_HEADERS}
    bpg_encoder.h
)

set(LIBBPG_DEC_PRIVATE_HEADERS
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

set(LIBBPG_DEC_SOURCES
    ${LIBBPG_DEC_PUBLIC_HEADERS}
    ${LIBBPG_DEC_PRIVATE_HEADERS}
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

set(LIBBPG_ENC_SOURCES
    ${LIBBPG_ENC_PUBLIC_HEADERS}
    bpg_encoder.c
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
    set(LIBBPG_ENC_SOURCES
        ${LIBBPG_ENC_SOURCES}
        bpg_encoder_priv.h
        x265/source/x265.h
        x265_glue.c)
else()
    set(LIBBPG_ENC_SOURCES
        ${LIBBPG_ENC_SOURCES}
        bpg_encoder_priv.h
        jctvc_glue.cpp)
endif()

set(LIBBPG_BPGENC_SOURCES
    bpgenc.h
    bpgenc.c
)

set(LIBBPG_BPGVIEW_SOURCES
    bpgview.c
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(NOT MSVC)
    add_definitions(-D_FILE_OFFSET_BITS=64
                    -D_LARGEFILE_SOURCE
                    -D_REENTRANT
                    -D_BPG_API
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
    message(STATUS "Selected ${CMAKE_GENERATOR} generator")
    message(STATUS "Selected ${CMAKE_VS_PLATFORM_NAME} platform")
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE
                    -D_BPG_API
                    -DCONFIG_BPG_VERSION="${LIBBPG_VERSION}")

    add_compile_options(/W3)
    add_compile_options(/wd4018)  # disable "signed/unsigned mismatch"
    add_compile_options(/wd4047)  # disable "'void *' differs in levels of indirection from 'int'"
    add_compile_options(/wd4081)  # disable "expected 'newline'; found ';'"
    add_compile_options(/wd4101)  # disable "unreferenced local variable"
    add_compile_options(/wd4116)  # disable "unnamed type definition in parentheses"
    add_compile_options(/wd4146)  # disable "unary minus operator applied to unsigned type, result still unsigned"
    add_compile_options(/wd4244)  # disable "possible loss of data"
    add_compile_options(/wd4267)  # disable "conversion from 'size_t' to 'int', possible loss of data"
    add_compile_options(/wd4334)  # disable "result of 32-bit shift implicitly converted to 64 bits"
    add_compile_options(/wd4503)  # disable "decorated name length exceeded, name was truncated"
    add_compile_options(/wd4800)  # forcing value to bool 'true' or 'false' (performance warning)
    if(WARNINGS_AS_ERRORS)
        add_compile_options(/WX)
    endif()

    set(ZLIB_LIBRARY "C:\\Program Files\\zlib\\lib\\zlibstaticd.lib" CACHE FILEPATH "Zlib library path" FORCE)
    set(ZLIB_INCLUDE_DIR "C:\\Program Files\\zlib\\include" CACHE PATH "Zlib include path" FORCE)
    set(JPEG_LIBRARY "C:\\Program Files\\libjpeg\\lib\\jpeg.lib" CACHE FILEPATH "JPEG library path" FORCE)
    set(JPEG_INCLUDE_DIR "C:\\Program Files\\libjpeg\\include" CACHE PATH "JPEG include path" FORCE)
    set(PNG_LIBRARY "C:\\Program Files\\libpng\\lib\\libpng16_staticd.lib" CACHE FILEPATH "PNG library path" FORCE)
    set(PNG_PNG_INCLUDE_DIR "C:\\Program Files\\libpng\\include" CACHE PATH "PNG include path" FORCE)
endif()

unset(BPG_DEC_LIB_TARGETS)
unset(BPG_ENC_LIB_TARGETS)

if(LIBBPG_SHARED)
    add_library(bpg_dec SHARED ${LIBBPG_DEC_SOURCES})
    set(BPG_DEC_LIB_TARGETS bpg_dec)
    set_target_properties(bpg_dec PROPERTIES OUTPUT_NAME ${LIBBPG_DEC_NAME})
    if(MSVC)
        set_target_properties(bpg_dec PROPERTIES PREFIX "lib")
        set_target_properties(bpg_dec PROPERTIES IMPORT_PREFIX "lib")
    else()
        target_compile_options(bpg_dec PUBLIC -std=c99)
    endif()
    target_compile_definitions(bpg_dec PUBLIC
                               -D_ISOC99_SOURCE
                               -D_POSIX_C_SOURCE=200112
                               -D_XOPEN_SOURCE=600
                               -DHAVE_AV_CONFIG_H
                               -D_GNU_SOURCE=1
                               -DUSE_VAR_BIT_DEPTH
                               -DUSE_PRED)
    set_target_properties(bpg_dec PROPERTIES PUBLIC_HEADER "${LIBBPG_DEC_PUBLIC_HEADERS}")
endif()

if(LIBBPG_STATIC)
    add_library(bpg_dec_static STATIC ${LIBBPG_DEC_SOURCES})
    set_target_properties(bpg_dec_static PROPERTIES OUTPUT_NAME ${LIBBPG_DEC_NAME}
                          CLEAN_DIRECT_OUTPUT 1)
    list(APPEND BPG_DEC_LIB_TARGETS bpg_dec_static)
    if(MSVC)
        set_target_properties(bpg_dec_static PROPERTIES PREFIX "lib")
    else()
        target_compile_options(bpg_dec_static PUBLIC -std=c99)
    endif()
    target_compile_definitions(bpg_dec_static PUBLIC
                               -D_ISOC99_SOURCE
                               -D_POSIX_C_SOURCE=200112
                               -D_XOPEN_SOURCE=600
                               -DHAVE_AV_CONFIG_H
                               -D_GNU_SOURCE=1
                               -DUSE_VAR_BIT_DEPTH
                               -DUSE_PRED)
    set_target_properties(bpg_dec_static PROPERTIES PUBLIC_HEADER "${LIBBPG_DEC_PUBLIC_HEADERS}")
endif()

if(LIBBPG_X265)
    if(MSVC)
        set(x265_12bit "${CMAKE_CURRENT_BINARY_DIR}/x265.out/12bit/${CMAKE_BUILD_TYPE}/x265-static.lib")
        add_custom_command(OUTPUT ${x265_12bit}
                           COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/x265.out/12bit"
                           COMMAND ${CMAKE_COMMAND} -E chdir "${CMAKE_CURRENT_BINARY_DIR}/x265.out/12bit" ${CMAKE_COMMAND} "${CMAKE_CURRENT_SOURCE_DIR}/x265/source" ARGS "-DHIGH_BIT_DEPTH=ON" "-DEXPORT_C_API=OFF" "-DENABLE_SHARED=OFF" "-DENABLE_CLI=OFF" "-DMAIN12=ON" "-G" "${CMAKE_GENERATOR}"
                           COMMAND ${CMAKE_COMMAND} --build "${CMAKE_CURRENT_BINARY_DIR}/x265.out/12bit"
                           COMMENT "Build 12-bit x265 library"
                           VERBATIM
                           COMMAND_EXPAND_LISTS)
        add_custom_target(libx265_12bit ALL DEPENDS ${x265_12bit})

        set(x265_10bit "${CMAKE_CURRENT_BINARY_DIR}/x265.out/10bit/${CMAKE_BUILD_TYPE}/x265-static.lib")
        add_custom_command(OUTPUT ${x265_10bit}
                           COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/x265.out/10bit"
                           COMMAND ${CMAKE_COMMAND} -E chdir "${CMAKE_CURRENT_BINARY_DIR}/x265.out/10bit" ${CMAKE_COMMAND} "${CMAKE_CURRENT_SOURCE_DIR}/x265/source" ARGS "-DHIGH_BIT_DEPTH=ON" "-DEXPORT_C_API=OFF" "-DENABLE_SHARED=OFF" "-DENABLE_CLI=OFF" "-DMAIN10=ON" "-G" "${CMAKE_GENERATOR}"
                           COMMAND ${CMAKE_COMMAND} --build "${CMAKE_CURRENT_BINARY_DIR}/x265.out/10bit"
                           DEPENDS ${x265_12bit}
                           COMMENT "Build 10-bit x265 library"
                           VERBATIM
                           COMMAND_EXPAND_LISTS)
        add_custom_target(libx265_10bit ALL DEPENDS ${x265_10bit})

        set(x265_8bit "${CMAKE_CURRENT_BINARY_DIR}/x265.out/8bit/${CMAKE_BUILD_TYPE}/x265-static.lib")
        add_custom_command(OUTPUT ${x265_8bit}
                           COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/x265.out/8bit"
                           COMMAND ${CMAKE_COMMAND} -E chdir "${CMAKE_CURRENT_BINARY_DIR}/x265.out/8bit" ${CMAKE_COMMAND} "${CMAKE_CURRENT_SOURCE_DIR}/x265/source" ARGS "-DLINKED_10BIT=ON" "-DLINKED_12BIT=ON" "-DENABLE_SHARED=OFF" "-DENABLE_CLI=OFF" "-G" "${CMAKE_GENERATOR}"
                           COMMAND ${CMAKE_COMMAND} --build "${CMAKE_CURRENT_BINARY_DIR}/x265.out/8bit"
                           DEPENDS ${x265_10bit}
                           COMMENT "Build 8-bit x265 library"
                           VERBATIM
                           COMMAND_EXPAND_LISTS)
        add_custom_target(libx265_8bit ALL DEPENDS ${x265_8bit})
    else()
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
endif()

if(LIBBPG_JCTVC)
    add_library(jctvc STATIC ${LIBBPG_JCTVC_SOURCES})
    if(NOT MSVC)
        target_compile_options(jctvc PRIVATE -Wno-sign-compare)
    endif()
    target_include_directories(jctvc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/jctvc)
endif()

if(LIBBPG_SHARED)
    add_library(bpg_enc SHARED ${LIBBPG_ENC_SOURCES})
    set(BPG_ENC_LIB_TARGETS bpg_enc)
    set_target_properties(bpg_enc PROPERTIES OUTPUT_NAME ${LIBBPG_ENC_NAME})
    if(MSVC)
        set_target_properties(bpg_enc PROPERTIES PREFIX "lib")
        set_target_properties(bpg_enc PROPERTIES IMPORT_PREFIX "lib")
    endif()
    set_target_properties(bpg_enc PROPERTIES PUBLIC_HEADER "${LIBBPG_ENC_PUBLIC_HEADERS}")

    if(LIBBPG_JCTVC)
        target_link_libraries(bpg_enc PRIVATE jctvc)
        if(NOT MSVC)
            target_compile_options(bpg_enc PRIVATE -Wno-unused-but-set-variable)
        endif()
        target_compile_definitions(bpg_enc PRIVATE -D_BPG_API -DUSE_JCTVC)
        target_include_directories(bpg_enc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/jctvc)
    else()
        if(NOT MSVC)
            target_link_libraries(bpg_enc PRIVATE ${x265_8bit} ${x265_12bit} ${x265_10bit} stdc++)
            target_compile_options(bpg_enc PUBLIC -std=c99)
        else()
            target_link_libraries(bpg_enc PRIVATE ${x265_8bit} ${x265_12bit} ${x265_10bit})
        endif()
        target_compile_definitions(bpg_enc PRIVATE -D_BPG_API -DUSE_X265)
        target_include_directories(bpg_enc PRIVATE
                                   ${CMAKE_CURRENT_SOURCE_DIR}/x265/source
                                   ${CMAKE_CURRENT_BINARY_DIR}/x265.out/8bit)
        add_dependencies(bpg_enc libx265_8bit)
    endif()
endif()

if(LIBBPG_STATIC)
    add_library(bpg_enc_static STATIC ${LIBBPG_ENC_SOURCES})
    set_target_properties(bpg_enc_static PROPERTIES OUTPUT_NAME ${LIBBPG_ENC_NAME}
                          CLEAN_DIRECT_OUTPUT 1)
    list(APPEND BPG_ENC_LIB_TARGETS bpg_enc_static)
    if(MSVC)
        set_target_properties(bpg_enc_static PROPERTIES PREFIX "lib")
    endif()
    set_target_properties(bpg_enc_static PROPERTIES PUBLIC_HEADER "${LIBBPG_ENC_PUBLIC_HEADERS}")

    if(LIBBPG_JCTVC)
        target_link_libraries(bpg_enc_static PRIVATE jctvc)
        if(NOT MSVC)
            target_compile_options(bpg_enc_static PRIVATE -Wno-unused-but-set_variable)
        endif()
        target_compile_definitions(bpg_enc_static PRIVATE -D_BPG_API -DUSE_JCTVC)
        target_include_directories(bpg_enc_static PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/jctvc)
    else()
        if(NOT MSVC)
            target_compile_options(bpg_enc_static PUBLIC -std=c99)
            target_link_libraries(bpg_enc_static PRIVATE ${x265_8bit} ${x265_12bit} ${x265_10bit} pthread m z stdc++)
        else()
            target_link_libraries(bpg_enc_static PRIVATE ${x265_8bit} ${x265_12bit} ${x265_10bit})
        endif()
        target_compile_definitions(bpg_enc_static PRIVATE -D_BPG_API -DUSE_X265)
        target_include_directories(bpg_enc_static PRIVATE
                                   ${CMAKE_CURRENT_SOURCE_DIR}/x265/source
                                   ${CMAKE_CURRENT_BINARY_DIR}/x265.out/8bit)
        add_dependencies(bpg_enc_static libx265_8bit)
    endif()
endif()

add_executable(bpgdec ${LIBBPG_BPGDEC_SOURCES})
if(MSVC)
    target_link_libraries(bpgdec PRIVATE bpg_dec_static)
else()
    if(NOT APPLE)
        target_link_libraries(bpgdec PRIVATE rt)
    endif()
    target_link_libraries(bpgdec PRIVATE bpg_dec_static m pthread)
endif()
bpg_image_io(bpgdec)

add_executable(bpgenc ${LIBBPG_BPGENC_SOURCES})
if(MSVC)
    target_link_libraries(bpgenc PRIVATE bpg_enc_static)
else()
    if(NOT APPLE)
        target_link_libraries(bpgenc PRIVATE rt)
    endif()
    target_link_libraries(bpgenc PRIVATE bpg_enc_static)
    target_compile_options(bpgenc PRIVATE -Wno-unused-but-set-variable)
endif()
bpg_image_io(bpgenc)

if(NOT MSVC)
    if(LIBBPG_BPGVIEW)
        add_executable(bpgview ${LIBBPG_BPGVIEW_SOURCES})
        if(NOT APPLE)
            target_link_libraries(bpgview rt)
        endif()
        target_link_libraries(bpgview bpg_dec_static m pthread SDL_image SDL)
    endif()
endif()

# ------------------------------------------------
# install

install(TARGETS bpgdec bpgenc bpg_enc_static bpg_dec_static
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include/${CMAKE_PROJECT_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
if(LIBBPG_JCTVC)
    install(TARGETS jctvc ARCHIVE DESTINATION lib)
else()
    if(MSVC)
        install(FILES ${x265_8bit} DESTINATION lib RENAME libx265_8bit.lib)
        install(FILES ${x265_10bit} DESTINATION lib RENAME libx265_10bit.lib)
        install(FILES ${x265_12bit} DESTINATION lib RENAME libx265_12bit.lib)
    else()
        install(FILES ${x265_8bit} DESTINATION lib RENAME libx265_8bit.a)
        install(FILES ${x265_10bit} DESTINATION lib RENAME libx265_10bit.a)
        install(FILES ${x265_12bit} DESTINATION lib RENAME libx265_12bit.a)
    endif()
endif()

if(NOT WIN32)
  set(CPACK_PACKAGE_EXT "tar.gz")
  set(DISTRIB_PACKAGE_EXT "tgz")
else()
  set(CPACK_PACKAGE_EXT "zip")
  set(DISTRIB_PACKAGE_EXT ${CPACK_PACKAGE_EXT})
endif()

