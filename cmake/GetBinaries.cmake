include(FetchContent)

option(USE_CROC_BACKEND "Use croc backend for SentYa" ON)
option(USE_SENDME_BACKEND "Use sendme backend for SentYa" OFF)

if (USE_CROC_BACKEND)
    include(cmake/DownloadCroc.cmake)
endif ()

if (USE_SENDME_BACKEND)
    include(cmake/DownloadSendme.cmake)
endif ()
