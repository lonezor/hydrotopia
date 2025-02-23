#pragma once

#include <chrono>
#include <string>

enum class measurement_type {
    sensor_ambient_temperature,
    sensor_ambient_humidity,
    sensor_water_temperature,
    sensor_water_ec,
    event_upper_led,
    event_lower_led,
    event_ventilation_fan,
    event_wind_sim_fan,
    event_cooling_rod,
    event_water_circulation,
    event_o2_electrolysis,
};
constexpr size_t measurement_event_index_start = 4;

struct measurement {
    /** Timestamp */
    std::chrono::time_point<std::chrono::system_clock> ts;

    /** Sensor reading (string since text file is output) */
    std::string value;
};