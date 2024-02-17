add_executable(rc_ctrl_16
    src/main.c
)

set_property(TARGET rc_ctrl_16 PROPERTY C_STANDARD 99)

set_property(TARGET rc_ctrl_16 PROPERTY POSITION_INDEPENDENT_CODE TRUE)

target_link_libraries(rc_ctrl_16
    PRIVATE
    relay_controller::relay_16
)

install(
    TARGETS
    rc_ctrl_16
)

