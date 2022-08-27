find_package(Threads REQUIRED)

add_executable(hydroctrl
    common/channel_type.cpp
    common/channel_type.hpp
    common/channel/channel.cpp
    common/channel/channel.hpp
    common/channel/subsystem/core/rotary_alarm_light_channel.cpp
    common/channel/subsystem/core/rotary_alarm_light_channel.hpp
    common/channel/subsystem/main/full_spectrum_light_channel.cpp
    common/channel/subsystem/main/full_spectrum_light_channel.hpp
    common/channel/subsystem/main/ventilation_fan_channel.cpp
    common/channel/subsystem/main/ventilation_fan_channel.hpp
    common/channel/subsystem/main/water_pump_channel.cpp
    common/channel/subsystem/main/water_pump_channel.hpp
    common/configuration.hpp
    common/error_code.cpp
    common/error_code.hpp
    common/event.cpp
    common/event.hpp
    common/io_monitor.cpp
    common/io_monitor.hpp
    common/log.cpp
    common/log.hpp
    common/network/socket.cpp
    common/network/socket.hpp
    common/power_consumption.cpp
    common/power_consumption.hpp
    common/relay_module/relay_module.cpp
    common/relay_module/relay_module.hpp
    common/string_processing/regex.cpp
    common/string_processing/regex.hpp
    common/subsystem.cpp
    common/subsystem.hpp
    common/system_clock.cpp
    common/system_clock.hpp
    common/system/system.hpp
    common/task_scheduler/data_types.hpp
    common/task_scheduler/task_scheduler_interface.hpp
    common/task_scheduler/task_scheduler.cpp
    common/task_scheduler/task_scheduler.hpp
    common/task_scheduler/task.hpp
    common/unit/temperature.cpp
    common/unit/temperature.hpp
    common/ventilation_fan.cpp
    common/ventilation_fan.hpp
    controller/controller.cpp
    controller/controller.hpp
    main.cpp
    user_interface/console_user_interface/cli.cpp
    user_interface/console_user_interface/cli.hpp
    user_interface/socket_user_interface/request_handler.cpp
    user_interface/socket_user_interface/request_handler.hpp
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
