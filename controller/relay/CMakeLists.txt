add_subdirectory(16_channels)
add_subdirectory(32_channels)

add_executable(rc_ctrl_service
    service.c
)

set_property(TARGET rc_ctrl_service PROPERTY C_STANDARD 99)

set_property(TARGET rc_ctrl_service PROPERTY POSITION_INDEPENDENT_CODE TRUE)

target_include_directories(
    rc_ctrl_service
    PRIVATE
    16_channels/lib/include/rc
    #32_channels/lib/include/rc
    )

target_link_libraries(rc_ctrl_service
    PRIVATE
    relay_controller::relay_16
    #relay_controller::relay_32
)

target_compile_definitions(
    rc_ctrl_service
    PRIVATE
    RELAY_MODULE_16_CHANNELS
    #RELAY_MODULE_32_CHANNELS
)

install(
    TARGETS
    rc_ctrl_service
)
