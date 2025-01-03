#pragma once

#include <string>

#include <measurement.hpp>
#include <vector>
#include <unordered_map>

class data_processor
{
    public:
        data_processor();

        /** Data indication from network layer */
        void data_ind(const char* buffer, const int buffer_len);

    private:
        /** Top level ingestion function */
        void data_ingestion(const std::string& sensor_readings);
        
        /** Extend measurement map with latest reading */
        void add_measurement(measurement_type type, const std::string& value);
        
        /** Clear all data beyond largest supported duration */
        void purge_expired_data();
        
        void generate_graphs(std::chrono::minutes threshold);

        void generate_temperature_graphs(const std::vector<std::chrono::hours>& durations);
        void generate_humidity_graphs(const std::vector<std::chrono::hours>& durations);
        void generate_water_ec_graphs(const std::vector<std::chrono::hours>& durations);


        std::vector<measurement>
        history_view(measurement_type type, std::chrono::hours duration);
        
        double history_value_min(std::vector<measurement> history);
        double history_value_max(std::vector<measurement> history);
        
        std::unordered_map<measurement_type,std::vector<measurement>> measurement_map_;

        std::chrono::time_point<std::chrono::system_clock> graph_generation_ts_;
};