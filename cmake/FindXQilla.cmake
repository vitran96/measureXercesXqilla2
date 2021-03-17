find_path(XQilla_INCLUDE_DIR
    NAMES xqilla/xqilla-simple.hpp
    DOC "XQilla include directory")
mark_as_advanced(XQilla_INCLUDE_DIR)

find_library(XQilla_LIBRARY
    NAMES "xqilla"
          "xqilla${XQilla_MAYBE_DEBUG_SUFFIX}"
          "xqilla${XQilla_EXPECTED_VERSION}"
          "xqilla${XQilla_EXPECTED_VERSION}${XQilla_MAYBE_DEBUG_SUFFIX}"
    DOC "XQilla libraries")
mark_as_advanced(XQilla_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(XQilla
    FOUND_VAR XQilla_FOUND
    REQUIRED_VARS XQilla_LIBRARY
                  XQilla_INCLUDE_DIR
    FAIL_MESSAGE "Failed to find XQilla")

if (XQilla_FOUND)
    set(XQilla_INCLUDE_DIRS ${XQilla_INCLUDE_DIR})
    set(XQilla_LIBRARIES ${XQilla_LIBRARY})

    if (NOT TARGET XQilla::XQilla)
        add_library(XQilla::XQilla UNKNOWN IMPORTED)
        if (XQilla_INCLUDE_DIRS)
            set_target_properties(XQilla::XQilla PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${XQilla_INCLUDE_DIRS}")
        endif ()
        if (EXISTS "${XQilla_LIBRARY}")
            set_target_properties(XQilla::XQilla PROPERTIES
                IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
                IMPORTED_LOCATION "${XQilla_LIBRARY}")
        endif ()
    endif ()
endif ()
