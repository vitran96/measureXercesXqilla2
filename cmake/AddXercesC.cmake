# *_EXPECTED_VERSION_* must be updated when updating the 3rd lib library

if (NOT TARGET XercesC::XercesC AND
    NOT ${XercesC_ROOT} STREQUAL "" AND
    EXISTS ${XercesC_ROOT})

    LIST(APPEND CMAKE_PREFIX_PATH ${XercesC_ROOT})

    set(XercesC_EXPECTED_VERSION_MAJOR "3")
    set(XercesC_EXPECTED_VERSION_MINOR "1")
    set(XercesC_EXPECTED_VERSION "${XercesC_EXPECTED_VERSION_MAJOR}.${XercesC_EXPECTED_VERSION_MINOR}")

    set(XercesC_MAYBE_DEBUG_SUFFIX "D")
endif ()

find_package(XercesC ${XercesC_EXPECTED_VERSION} REQUIRED)

file(GLOB_RECURSE XercesC_BIN
    FOLLOW_SYMLINKS
    "${XercesC_ROOT}/bin/xerces-c_*_*.dll" "${XercesC_ROOT}/lib/libxerces-c-*.*.so")
