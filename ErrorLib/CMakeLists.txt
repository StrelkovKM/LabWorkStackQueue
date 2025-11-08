file(GLOB hdrs "*.hpp*")

file(GLOB srcs "*.cpp")

add_library(${errorlib} STATIC ${srcs} ${hdrs})

target_link_libraries(${errorlib} ${stringlib})