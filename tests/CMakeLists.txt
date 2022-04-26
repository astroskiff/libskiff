
link_directories(${CPPUTEST_LIBRARIES})

find_package(libutil REQUIRED)

include_directories(
  ${CPPUTEST_INCLUDE_DIRS}
  ${LIBUTIL_INCLUDE_DIRS}
)

add_executable(libskiff_unit_tests
        ${SOURCES}
        floating_point.cpp
        instruction_generator.cpp
        main.cpp)


target_link_libraries(libskiff_unit_tests
        ${CPPUTEST_LDFLAGS}
        libutil)

add_custom_command(TARGET libskiff_unit_tests COMMAND ./libskiff_unit_tests POST_BUILD)
