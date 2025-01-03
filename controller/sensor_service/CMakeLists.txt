set(CMAKE_CXX_STANDARD 20)

add_executable(hydro_sensor_service
    data_processor.cpp
    gnuplot.cpp
    main.cpp
)

target_include_directories(hydro_sensor_service
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

install(TARGETS hydro_sensor_service)
