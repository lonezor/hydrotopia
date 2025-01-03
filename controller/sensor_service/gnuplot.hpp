#pragma once

#include <measurement.hpp>

#include <string>
#include <vector>
#include <unordered_map>

class gnuplot
{
    public:
        explicit gnuplot(std::string working_dir);

        void generate_temperature_config_file(const std::string& file_name,
            const std::string& title,
            const std::string& data_suffix,
            const std::unordered_map<std::string,std::size_t>& event_stats);

        void generate_humidity_config_file(const std::string& file_name,
            const std::string& title,
            const std::string& data_suffix,
            const std::unordered_map<std::string,std::size_t>& event_stats);

        void generate_water_ec_config_file(const std::string& file_name,
            const std::string& title,
            const std::string& data_suffix,
            const std::unordered_map<std::string,std::size_t>& event_stats);

        void common_config_helper(const std::string& file_name,
                                  const std::string& title,
                                  const std::string& data_suffix,
                                  const std::unordered_map<std::string,std::size_t>& event_stats,
                                  std::stringstream& cfg);

        void generate_data_file(const std::vector<measurement>& data, const std::string& file_name);

        void generate_event_file(const std::vector<measurement>& data,
                const std::string& file_name,
                double overall_sensor_min,
                double overall_sensor_max);
        
        void render(const std::string& file_name);

    private:
        std::string working_dir_;
};
