set(SENDME_BINARY_VERSION "v0.31.0")

if (LINUX)
    set(SENDME_BINARY_OS "linux")
    set(SENDME_BINARY_EXT "tar.gz")
elseif (APPLE)
    set(SENDME_BINARY_OS "darwin")
    set(SENDME_BINARY_EXT "tar.gz")
elseif (CYGWIN OR WIN32)
    set(SENDME_BINARY_OS "windows")
    set(SENDME_BINARY_EXT "zip")
else ()
    message(FATAL_ERROR "Unsupported device, create a pull request to add support")
endif ()

set(SENDME_URL "https://github.com/n0-computer/sendme/releases/download")

FetchContent_Declare(
        SendMeBinary
        URL "${SENDME_URL}/${SENDME_BINARY_VERSION}/sendme-${SENDME_BINARY_VERSION}-${SENDME_BINARY_OS}-x86_64.${SENDME_BINARY_EXT}"
        SOURCE_DIR "${CMAKE_BINARY_DIR}/binaries/"
        DOWNLOAD_EXTRACT_TIMESTAMP true
)

FetchContent_MakeAvailable(SendMeBinary)
