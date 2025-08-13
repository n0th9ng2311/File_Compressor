cmake_minimum_required(VERSION 3.30)
project(file_comp)

set(CMAKE_CXX_STANDARD 20)

# Set runtime library path
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_executable(file_comp
        src/main.cpp
        src/file_comp.cpp
        hdr/file_comp.h
        hdr/common.h
        hdr/all_comp_algo.h
        comp_algo/bmp_/bmp_comp.cpp
        comp_algo/bmp_/bmp_comp.h
        comp_algo/csv_/csv_comp.cpp
        comp_algo/csv_/csv_comp.h
        comp_algo/xml_/xml_comp.cpp
        comp_algo/xml_/xml_comp.h
        comp_algo/iso_/iso_comp.cpp
        comp_algo/iso_/iso_comp.h
        comp_algo/jpg_/jpg_comp.cpp
        comp_algo/jpg_/jpg_comp.h
        comp_algo/log_/log_comp.cpp
        comp_algo/log_/log_comp.h
        comp_algo/png_/png_comp.cpp
        comp_algo/png_/png_comp.h
        comp_algo/psd_/psd_comp.cpp
        comp_algo/psd_/psd_comp.h
        comp_algo/tar_/tar_comp.cpp
        comp_algo/tar_/tar_comp.h
        comp_algo/tiff_/tiff_comp.cpp
        comp_algo/tiff_/tiff_comp.h
        comp_algo/txt_/txt_comp.cpp
        comp_algo/txt_/txt_comp.h
        comp_algo/vhd_/vhd_comp.cpp
        comp_algo/vhd_/vhd_comp.h
        comp_algo/aiff_/aiff_comp.cpp
        comp_algo/aiff_/aiff_comp.h
        comp_algo/wav_/wav_comp.h
        comp_algo/wav_/wav_comp.cpp
)

# Find packages
find_package(ZLIB REQUIRED)

# Add subdirectories
add_subdirectory(external/audio-adpcm-cpp)
add_subdirectory(external/lame)
add_subdirectory(external/audioFile)


# Link libraries
target_link_libraries(file_comp
        PRIVATE
        mp3lame
        adpcm
        ZLIB::ZLIB
        AudioFile
)

# Include directories
target_include_directories(file_comp
        PRIVATE
        external/lame/lame-3.100/include
        external/audio-adpcm-cpp/include
        external/audioFile
)

# Copy DLLs to output directory (Windows only)
if(WIN32)
    # Find and copy LAME DLLs
    find_file(LAME_DLL
            NAMES libmp3lame.dll libmp3lame-0.dll mp3lame.dll
            PATHS ${CMAKE_BINARY_DIR}/external/lame
            ${CMAKE_BINARY_DIR}/external/lame/Debug
            ${CMAKE_BINARY_DIR}/external/lame/Release
    )

    if(LAME_DLL)
        add_custom_command(TARGET file_comp POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${LAME_DLL} $<TARGET_FILE_DIR:file_comp>
        )
    endif()

    # Copy ZLIB DLLs if they exist
    find_file(ZLIB_DLL
            NAMES zlib1.dll zlibwapi.dll
            PATHS ${ZLIB_ROOT}/bin
            ${CMAKE_BINARY_DIR}
    )

    if(ZLIB_DLL)
        add_custom_command(TARGET file_comp POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${ZLIB_DLL} $<TARGET_FILE_DIR:file_comp>
        )
    endif()
endif()
