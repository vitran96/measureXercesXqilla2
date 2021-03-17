# *_EXPECTED_VERSION_* must be updated when updating the 3rd lib library

if (NOT TARGET XQilla::XQilla AND
    NOT ${XQilla_ROOT} STREQUAL "" AND
    EXISTS ${XQilla_ROOT})

    LIST(APPEND CMAKE_PREFIX_PATH ${XQilla_ROOT})

    set(XQilla_EXPECTED_VERSION_MAJOR "2")
    set(XQilla_EXPECTED_VERSION_MINOR "3") # Currently this is only for Windows
    set(XQilla_EXPECTED_VERSION "${XQilla_EXPECTED_VERSION_MAJOR}${XQilla_EXPECTED_VERSION_MINOR}")

    set(XQilla_MAYBE_DEBUG_SUFFIX "d")
endif ()

find_package(XQilla REQUIRED)

file(GLOB_RECURSE XQilla_BIN
    "${XQilla_ROOT}/lib/xqilla*.dll" "${XQilla_ROOT}/lib/libxqilla.so.*.*.*")
