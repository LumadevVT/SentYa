set(CROC_BINARY_VERSION "v10.3.1")

set(CROC_BINARY_EXT "tar.gz")

if (LINUX)
    set(CROC_BINARY_OS "Linux")
    set(CROC_BINARY_EXT "tar.gz")
elseif (APPLE)
    set(CROC_BINARY_OS "macOS")
    set(CROC_BINARY_EXT "tar.gz")
elseif (CYGWIN OR WIN32)
    set(CROC_BINARY_OS "Windows")
    set(CROC_BINARY_EXT "zip")
else ()
    message(FATAL_ERROR "Unsupported device, create a pull request to add support")
endif ()

set(CROC_URL "https://github.com/schollz/croc/releases/download")

FetchContent_Declare(
        CrocBinary
        URL "${CROC_URL}/${CROC_BINARY_VERSION}/croc_${CROC_BINARY_VERSION}_${CROC_BINARY_OS}-64bit.${CROC_BINARY_EXT}"
        SOURCE_DIR "${CMAKE_BINARY_DIR}/binaries/"
        DOWNLOAD_EXTRACT_TIMESTAMP true
)

FetchContent_MakeAvailable(CrocBinary)
