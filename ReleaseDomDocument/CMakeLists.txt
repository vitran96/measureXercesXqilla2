project (ReleaseDomDocument)

set(headers
    "main.hpp"
)

set(srcs
    "main.cpp"
)

add_executable(${PROJECT_NAME} ${headers} ${srcs})
target_link_libraries(${PROJECT_NAME}
    XercesC::XercesC
    XQilla::XQilla
)
