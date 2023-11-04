find_package(Threads REQUIRED)

add_executable(hydroctrl
    common/channel_type.cpp
    common/channel/channel.cpp
    common/channel/subsystem/core/rotary_alarm_light_channel.cpp
    common/channel/subsystem/main/full_spectrum_light_channel.cpp
    common/channel/subsystem/main/step_down_voltage_transformer_channel.cpp
    common/channel/subsystem/main/ventilation_fan_channel.cpp
    common/channel/subsystem/main/water_pump_channel.cpp
    common/channel/subsystem/main/wind_simulation_fan_channel.cpp
    common/error_code.cpp
    common/event.cpp
    common/io_monitor.cpp
    common/log.cpp
    common/network/socket.cpp
    common/power_consumption.cpp
    common/relay_module/relay_module.cpp
    common/string_processing/regex.cpp
    common/subsystem.cpp
    common/system_clock.cpp
    common/task_scheduler/task_scheduler.cpp
    common/unit/temperature.cpp
    common/ventilation_fan.cpp
    controller/controller.cpp
    main.cpp
    user_interface/console_user_interface/cli.cpp
    user_interface/socket_user_interface/request_handler.cpp
)


# Enable when running on real HW
list(APPEND hydroctrl_flags
#   HC_RELAY_MODULE_ENABLED
#   HC_SERIAL_CONSOLE_ENABLED
)

list(APPEND hydroctrl_libs
   CppLinuxSerial
)

target_compile_definitions(hydroctrl
    PRIVATE
    ${hydroctrl_flags}
)

target_link_libraries(hydroctrl
    PRIVATE
        rc_relay
        Threads::Threads
        ${hydroctrl_libs}
)

target_include_directories(hydroctrl
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

install(TARGETS hydroctrl)
