#include <data_processor.hpp>

#include <gnuplot.hpp>

#include <iostream>
#include <vector>
#include <regex>
#include <iostream>
#include <cmath>
#include <limits>

#include <assert.h>

data_processor::data_processor()
{
    // Graph generation at next expiration
    graph_generation_ts_ = std::chrono::system_clock::now();
}

// Entry point: incoming raw data
void data_processor::data_ind(const char* buffer, const int buffer_len)
{
    std::vector<char> str;
    for(int i=0; i<buffer_len;i++) {
        // Filter: Exclude control characters
        if (buffer[i] >= 32) {
            str.emplace_back(buffer[i]);
        }
    }

    auto sensor_readings = std::string(str.data());

    data_ingestion(sensor_readings);
}

std::string extract_sensor_value(const std::string& sensor_readings, const std::string& regex)
{
    std::string value;

    try {
        const std::regex r(regex);
        std::smatch m;

        if (regex_search(sensor_readings, m, r)) {
            for (std::string &&str : m) {
                value = str;
            }
        }
    } catch (std::regex_error e) {
    
    }

    return value;
}

void data_processor::add_measurement(measurement_type type, const std::string& value)
{
    if (value.empty()) {
        return;
    }

    auto e = measurement();
    e.ts = std::chrono::system_clock::now();
    e.value = value;
    measurement_map_[type].emplace_back(e);
}

void data_processor::data_ingestion(const std::string& sensor_readings)
{
    if (sensor_readings.find("ambient_temperature") != std::string::npos) {
        auto raw = extract_sensor_value(sensor_readings, "ambient_temperature (.+),");

        double target = atof(raw.c_str());

        static bool initial_sample = true;
        static double dvalue = 0;

        if (initial_sample) {
            double v = target;
            v *= 0.99;
            dvalue = v;
            initial_sample = false;
        }

        dvalue = std::lerp(dvalue, target, 0.5f);
        std::stringstream svalue;
        svalue.precision(8);
        svalue << std::fixed << dvalue;

        add_measurement(measurement_type::sensor_ambient_temperature,svalue.str());
    }
    
    if (sensor_readings.find("ambient_humidity") != std::string::npos) {
        auto raw = extract_sensor_value(sensor_readings, "ambient_humidity (.+)$");

        double target = atof(raw.c_str());

        static bool initial_sample = true;
        static double dvalue = 0;

        if (initial_sample) {
            double v = target;
            v *= 0.99;
            dvalue = v;
            initial_sample = false;
        }

        dvalue = std::lerp(dvalue, target, 0.5f);
        std::stringstream svalue;
        svalue.precision(8);
        svalue << std::fixed << dvalue;

        add_measurement(measurement_type::sensor_ambient_humidity,svalue.str());
    }
    
    if (sensor_readings.find("water_temperature") != std::string::npos) {
        auto raw = extract_sensor_value(sensor_readings, "water_temperature (.+), ");

        double target = atof(raw.c_str());

        static bool initial_sample = true;
        static double dvalue = 0;

        if (initial_sample) {
            double v = target;
            v *= 0.99;
            dvalue = v;
            initial_sample = false;
        }

        dvalue = std::lerp(dvalue, target, 0.5f);
        std::stringstream svalue;
        svalue.precision(8);
        svalue << std::fixed << dvalue;

        add_measurement(measurement_type::sensor_water_temperature,svalue.str());
    }
    
    if (sensor_readings.find("water_tds_ec") != std::string::npos) {
        auto raw = extract_sensor_value(sensor_readings, "water_tds_ec '.*,(.+)'");

        double target = atof(raw.c_str());

        static bool initial_sample = true;
        static double dvalue = 0;

        if (initial_sample) {
            double v = target;
            v *= 0.99;
            dvalue = v;
            initial_sample = false;
        }

        dvalue = std::lerp(dvalue, target, 0.2f);
        std::stringstream svalue;
        svalue.precision(8);
        svalue << std::fixed << dvalue;

        add_measurement(measurement_type::sensor_water_ec,svalue.str());
    }

    if (sensor_readings.find("localhost_event") != std::string::npos) {
        auto ev = extract_sensor_value(sensor_readings, "localhost_event '(.+)'");

        if (ev.find("state_change_upper_led") != std::string::npos) {
            add_measurement(measurement_type::event_upper_led, "");
        } else if (ev.find("state_change_lower_led") != std::string::npos) {
            add_measurement(measurement_type::event_lower_led, "");
        } else if (ev.find("state_change_ventilation_fan") != std::string::npos) {
            add_measurement(measurement_type::event_ventilation_fan, "");
        } else if (ev.find("state_change_wind_sim_fan") != std::string::npos) {
            add_measurement(measurement_type::event_wind_sim_fan, "");
        } else if (ev.find("state_change_cooling_rod") != std::string::npos) {
            add_measurement(measurement_type::event_cooling_rod, "");
        } else if (ev.find("state_change_water_circulation") != std::string::npos) {
            add_measurement(measurement_type::event_water_circulation, "");
        } else if (ev.find("state_change_o2_electrolysis") != std::string::npos) {
            add_measurement(measurement_type::event_o2_electrolysis, "");
        }
    }

    purge_expired_data();

    generate_graphs(std::chrono::minutes(1));
}

void data_processor::generate_temperature_graphs(const std::vector<std::chrono::hours>& durations)
{
    for(auto&& duration : durations) {
        auto sensor_ambient_temperature_history = history_view(measurement_type::sensor_ambient_temperature, duration);
        auto sensor_water_temperature_history = history_view(measurement_type::sensor_water_temperature, duration);
        auto event_upper_led_history = history_view(measurement_type::event_upper_led, duration);
        auto event_lower_led_history = history_view(measurement_type::event_lower_led, duration);
        auto event_ventilation_fan_history = history_view(measurement_type::event_ventilation_fan, duration);
        auto event_wind_sim_fan_history = history_view(measurement_type::event_wind_sim_fan, duration);
        auto event_cooling_rod_history = history_view(measurement_type::event_cooling_rod, duration);
        auto event_water_circulation_history = history_view(measurement_type::event_water_circulation, duration);
        auto event_o2_electrolysis_history = history_view(measurement_type::event_o2_electrolysis, duration);

        // Overall min/max range identification: Used for drawing fake event data without
        // affecting the gnuplot autoscaling
        auto min1 = history_value_min(sensor_ambient_temperature_history);
        auto min2 = history_value_min(sensor_water_temperature_history);
        auto max1 = history_value_max(sensor_ambient_temperature_history);
        auto max2 = history_value_max(sensor_water_temperature_history);
        auto min = min1;
        if (min2 < min) {
            min = min2;
        }
        auto max = max1;
        if (max2 > max) {
            max = max2;
        }

        // Each event must have its own instantiation to avoid unwanted line plotted between
        // two event points in the graph due to them logically being connected. Split them up
        // with the help of stats
        std::unordered_map<std::string,std::size_t> event_stats;
        event_stats["upper_led"] = event_upper_led_history.size();
        event_stats["lower_led"] = event_lower_led_history.size();
        event_stats["ventilation_fan"] = event_ventilation_fan_history.size();
        event_stats["wind_sim_fan"] = event_wind_sim_fan_history.size();
        event_stats["cooling_rod"] = event_cooling_rod_history.size();
        event_stats["water_circulation"] = event_water_circulation_history.size();
        event_stats["o2_electrolysis"] = event_o2_electrolysis_history.size();

        auto gp = gnuplot("/tmp");

        std::stringstream file_name;
        std::stringstream title;
        std::stringstream data_suffix;

        file_name << "temperature_" << duration.count() << "h";
        title << "Temperature readings of the last " << duration.count() << "h";
        data_suffix << duration.count() << "h";

        gp.generate_temperature_config_file(file_name.str(), title.str(), data_suffix.str(), event_stats);

        gp.generate_data_file(sensor_ambient_temperature_history, "sensor_ambient_temperature_" + data_suffix.str());
        gp.generate_data_file(sensor_water_temperature_history, "sensor_water_temperature_" + data_suffix.str());
        gp.generate_event_file(event_upper_led_history, "event_upper_led_" + data_suffix.str(), min, max);
        gp.generate_event_file(event_lower_led_history, "event_lower_led_" + data_suffix.str(), min, max);
        gp.generate_event_file(event_ventilation_fan_history, "event_ventilation_fan_" + data_suffix.str(), min, max);
        gp.generate_event_file(event_wind_sim_fan_history, "event_wind_sim_fan_" + data_suffix.str(), min, max);
        gp.generate_event_file(event_cooling_rod_history, "event_cooling_rod_" + data_suffix.str(), min, max);
        gp.generate_event_file(event_water_circulation_history, "event_water_circulation_" + data_suffix.str(), min, max);
        gp.generate_event_file(event_o2_electrolysis_history, "event_o2_electrolysis_" + data_suffix.str(), min, max);

        gp.render("temperature_" + data_suffix.str());
    }
}

void data_processor::generate_humidity_graphs(const std::vector<std::chrono::hours>& durations)
{
    for(auto&& duration : durations) {
        auto sensor_ambient_humidity_history = history_view(measurement_type::sensor_ambient_humidity, duration);
        auto event_upper_led_history = history_view(measurement_type::event_upper_led, duration);
        auto event_lower_led_history = history_view(measurement_type::event_lower_led, duration);
        auto event_ventilation_fan_history = history_view(measurement_type::event_ventilation_fan, duration);
        auto event_wind_sim_fan_history = history_view(measurement_type::event_wind_sim_fan, duration);
        auto event_cooling_rod_history = history_view(measurement_type::event_cooling_rod, duration);
        auto event_water_circulation_history = history_view(measurement_type::event_water_circulation, duration);
        auto event_o2_electrolysis_history = history_view(measurement_type::event_o2_electrolysis, duration);

        // Overall min/max range identification: Used for drawing fake event data without
        // affecting the gnuplot autoscaling
        auto min = history_value_min(sensor_ambient_humidity_history);
        auto max = history_value_max(sensor_ambient_humidity_history);

        // Each event must have its own instantiation to avoid unwanted line plotted between
        // two event points in the graph due to them logically being connected. Split them up
        // with the help of stats
        std::unordered_map<std::string,std::size_t> event_stats;
        event_stats["upper_led"] = event_upper_led_history.size();
        event_stats["lower_led"] = event_lower_led_history.size();
        event_stats["ventilation_fan"] = event_ventilation_fan_history.size();
        event_stats["wind_sim_fan"] = event_wind_sim_fan_history.size();
        event_stats["cooling_rod"] = event_cooling_rod_history.size();
        event_stats["water_circulation"] = event_water_circulation_history.size();
        event_stats["o2_electrolysis"] = event_o2_electrolysis_history.size();

        auto gp = gnuplot("/tmp");

        std::stringstream file_name;
        std::stringstream title;
        std::stringstream data_suffix;

        file_name << "humidity_" << duration.count() << "h";
        title << "Humidity readings of the last " << duration.count() << "h";
        data_suffix << duration.count() << "h";

        gp.generate_humidity_config_file(file_name.str(), title.str(), data_suffix.str(), event_stats);

        gp.generate_data_file(sensor_ambient_humidity_history, "sensor_ambient_humidity_" + data_suffix.str());
        gp.generate_event_file(event_upper_led_history, "event_upper_led_" + data_suffix.str(), min, max);
        gp.generate_event_file(event_lower_led_history, "event_lower_led_" + data_suffix.str(), min, max);
        gp.generate_event_file(event_ventilation_fan_history, "event_ventilation_fan_" + data_suffix.str(), min, max);
        gp.generate_event_file(event_wind_sim_fan_history, "event_wind_sim_fan_" + data_suffix.str(), min, max);
        gp.generate_event_file(event_cooling_rod_history, "event_cooling_rod_" + data_suffix.str(), min, max);
        gp.generate_event_file(event_water_circulation_history, "event_water_circulation_" + data_suffix.str(), min, max);
        gp.generate_event_file(event_o2_electrolysis_history, "event_o2_electrolysis_" + data_suffix.str(), min, max);

        gp.render("humidity_" + data_suffix.str());
    }
}

void data_processor::generate_water_ec_graphs(const std::vector<std::chrono::hours>& durations)
{
    for(auto&& duration : durations) {
        auto sensor_water_ec_history = history_view(measurement_type::sensor_water_ec, duration);
        auto event_upper_led_history = history_view(measurement_type::event_upper_led, duration);
        auto event_lower_led_history = history_view(measurement_type::event_lower_led, duration);
        auto event_ventilation_fan_history = history_view(measurement_type::event_ventilation_fan, duration);
        auto event_wind_sim_fan_history = history_view(measurement_type::event_wind_sim_fan, duration);
        auto event_cooling_rod_history = history_view(measurement_type::event_cooling_rod, duration);
        auto event_water_circulation_history = history_view(measurement_type::event_water_circulation, duration);
        auto event_o2_electrolysis_history = history_view(measurement_type::event_o2_electrolysis, duration);

        // Overall min/max range identification: Used for drawing fake event data without
        // affecting the gnuplot autoscaling
        auto min = history_value_min(sensor_water_ec_history);
        auto max = history_value_max(sensor_water_ec_history);

        // Each event must have its own instantiation to avoid unwanted line plotted between
        // two event points in the graph due to them logically being connected. Split them up
        // with the help of stats
        std::unordered_map<std::string,std::size_t> event_stats;
        event_stats["upper_led"] = event_upper_led_history.size();
        event_stats["lower_led"] = event_lower_led_history.size();
        event_stats["ventilation_fan"] = event_ventilation_fan_history.size();
        event_stats["wind_sim_fan"] = event_wind_sim_fan_history.size();
        event_stats["cooling_rod"] = event_cooling_rod_history.size();
        event_stats["water_circulation"] = event_water_circulation_history.size();
        event_stats["o2_electrolysis"] = event_o2_electrolysis_history.size();

        auto gp = gnuplot("/tmp");

        std::stringstream file_name;
        std::stringstream title;
        std::stringstream data_suffix;

        file_name << "water_ec_" << duration.count() << "h";
        title << "Water EC readings of the last " << duration.count() << "h";
        data_suffix << duration.count() << "h";

        gp.generate_water_ec_config_file(file_name.str(), title.str(), data_suffix.str(), event_stats);

        gp.generate_data_file(sensor_water_ec_history, "sensor_water_ec_" + data_suffix.str());
        gp.generate_event_file(event_upper_led_history, "event_upper_led_" + data_suffix.str(), min, max);
        gp.generate_event_file(event_lower_led_history, "event_lower_led_" + data_suffix.str(), min, max);
        gp.generate_event_file(event_ventilation_fan_history, "event_ventilation_fan_" + data_suffix.str(), min, max);
        gp.generate_event_file(event_wind_sim_fan_history, "event_wind_sim_fan_" + data_suffix.str(), min, max);
        gp.generate_event_file(event_cooling_rod_history, "event_cooling_rod_" + data_suffix.str(), min, max);
        gp.generate_event_file(event_water_circulation_history, "event_water_circulation_" + data_suffix.str(), min, max);
        gp.generate_event_file(event_o2_electrolysis_history, "event_o2_electrolysis_" + data_suffix.str(), min, max);

        gp.render("water_ec_" + data_suffix.str());
    }
}

void data_processor::generate_graphs(std::chrono::minutes threshold)
{
    auto now = std::chrono::system_clock::now();
    auto elapsed = duration_cast<std::chrono::minutes>( now - graph_generation_ts_ );

    if (elapsed < threshold) {
        return;
    }

    std::cout << "Time since last graph generation " << elapsed.count() << " minutes. Refreshing..." << std::endl;

    std::vector<std::chrono::hours> durations;
    durations.emplace_back(1);
    durations.emplace_back(3);
    durations.emplace_back(6);
    durations.emplace_back(12);
    durations.emplace_back(24);
    durations.emplace_back(72);
    durations.emplace_back(168);
    durations.emplace_back(336);

    generate_temperature_graphs(durations);
    generate_humidity_graphs(durations);
    generate_water_ec_graphs(durations);

    graph_generation_ts_ = now;
}

std::vector<measurement>
data_processor::history_view(measurement_type type, std::chrono::hours duration)
{
    std::vector<measurement> h_view;

    if (measurement_map_.find(type) == measurement_map_.end()) {
        return h_view;
    }

    auto all_readings = measurement_map_[type];

    auto now = std::chrono::system_clock::now();

    // Switch to more precise unit to avoid including too much data
    auto duration_seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);

    // Find start position matching time window
    std::size_t idx;
    for(idx=0; idx < all_readings.size(); idx++) {
        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>( now - all_readings[idx].ts );
        if (elapsed_seconds <= duration_seconds) {
            break;
        }
    }
    
    if (idx < all_readings.size()) {
        for(; idx < all_readings.size(); idx++) {
            auto e = all_readings[idx];
            h_view.emplace_back(e);
        }
    }

    return h_view;
}

double
data_processor::history_value_min(std::vector<measurement> history)
{
    double min = std::numeric_limits<double>::max();

    for(auto&& e : history) {
        double v = atof(e.value.c_str());
        if (v < min) {
            min = v;
        }
    }

    return min;
}

double
data_processor::history_value_max(std::vector<measurement> history)
{
    double max = std::numeric_limits<double>::min();

    for(auto&& e : history) {
        double v = atof(e.value.c_str());
        if (v > max) {
            max = v;
        }
    }

    return max;
}

void data_processor::purge_expired_data()
{
    std::vector<measurement_type> types;
    types.emplace_back(measurement_type::sensor_ambient_temperature);
    types.emplace_back(measurement_type::sensor_ambient_humidity);
    types.emplace_back(measurement_type::sensor_water_temperature);
    types.emplace_back(measurement_type::sensor_water_ec);
    types.emplace_back(measurement_type::event_upper_led);
    types.emplace_back(measurement_type::event_lower_led);
    types.emplace_back(measurement_type::event_ventilation_fan);
    types.emplace_back(measurement_type::event_wind_sim_fan);
    types.emplace_back(measurement_type::event_cooling_rod);
    types.emplace_back(measurement_type::event_water_circulation);
    types.emplace_back(measurement_type::event_o2_electrolysis);

    for(auto&& type : types) {
        if (measurement_map_.find(type) == measurement_map_.end()) {
            continue;
        }

        auto all_readings = measurement_map_[type];

        auto now = std::chrono::system_clock::now();

        // max supported duration currently is 14 days
        auto duration = std::chrono::days(15);

        // Switch to more precise unit to avoid including too much data
        auto duration_seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);

        // Find start position matching time window
        std::size_t idx;
        for(idx=0; idx < all_readings.size(); idx++) {
            auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>( now - all_readings[idx].ts );
            if (elapsed_seconds <= duration_seconds) {
                break;
            }
        }

        if (idx != 0) {
            all_readings.erase(std::next(all_readings.begin(), 0), std::next(all_readings.begin(), idx));
        }

        measurement_map_[type] = all_readings;
    }

}


