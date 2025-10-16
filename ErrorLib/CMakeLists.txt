file(GLOB hdrs "*.h*")

file(GLOB srcs "*.cpp")

add_library(${errorlib} STATIC ${srcs} ${hdrs})

target_link_libraries(${errorlib} ${stringlib})