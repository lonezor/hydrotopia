set(CMAKE_CXX_STANDARD 20)

add_executable(full_spectrum_led_service
    main.cpp
)

target_include_directories(full_spectrum_led_service
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

install(TARGETS full_spectrum_led_service)
