#include <gnuplot.hpp>

#include <iostream>
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <cmath>
#include <vector>


gnuplot::gnuplot(std::string working_dir)
    : working_dir_(working_dir)
{

}

void gnuplot::common_config_helper(const std::string& file_name,
                                  const std::string& title,
                                  const std::string& data_suffix,
                                  const std::unordered_map<std::string,std::size_t>& event_stats,
                                  std::stringstream& cfg)
{
    for(auto&& stat : event_stats) {
        auto key = stat.first;
        auto iterations = stat.second;

        std::vector<std::string> iteration_strings;
        for(std::size_t i=0;i<iterations;i++) {
            std::stringstream ss;
            ss << "_" << std::setfill('0') << std::setw(3) << i;
            iteration_strings.emplace_back(ss.str());
        }

        std::string event_name;
        if (key.find("upper_led") != std::string::npos) {
            std::string title = "title 'Upper LED Event' ";
            bool first_round = true;
            for(auto iteration : iteration_strings) {
                if (!first_round) {
                    title = "notitle ";
                }
                cfg << "     '" << working_dir_ << "/event_upper_led_" << data_suffix << iteration << ".data' using 1:3 " << title << "with linespoints ls 6, \\" << std::endl;
                first_round = false;
            }
        } else if (key.find("lower_led") != std::string::npos) {
            std::string title = "title 'Lower LED Event' ";
            bool first_round = true;
            for(auto iteration : iteration_strings) {
                if (!first_round) {
                    title = "notitle ";
                }
                cfg << "     '" << working_dir_ << "/event_lower_led_" << data_suffix << iteration <<".data' using 1:3 " << title << "with linespoints ls 7, \\" << std::endl;
                first_round = false;
            }
        } else if (key.find("ventilation_fan") != std::string::npos) {
            std::string title = "title 'Ventilation Fan Event' ";
            bool first_round = true;
            for(auto iteration : iteration_strings) {
                if (!first_round) {
                    title = "notitle ";
                }
                cfg << "     '" << working_dir_ << "/event_ventilation_fan_" << data_suffix << iteration << ".data' using 1:3 " << title << "with linespoints ls 8, \\" << std::endl;
                first_round = false;
            }
        } else if (key.find("wind_sim_fan") != std::string::npos) {
            std::string title = "title 'Wind Sim Fan Event' ";
            bool first_round = true;
            for(auto iteration : iteration_strings) {
                if (!first_round) {
                    title = "notitle ";
                }
                cfg << "     '" << working_dir_ << "/event_wind_sim_fan_" << data_suffix << iteration << ".data' using 1:3 " << title << "with linespoints ls 9, \\" << std::endl;
                first_round = false;
            }
        } else if (key.find("cooling_rod") != std::string::npos) {
            std::string title = "title 'Cooling Rod Event' ";
            bool first_round = true;
            for(auto iteration : iteration_strings) {
                if (!first_round) {
                    title = "notitle ";
                }
                cfg << "     '" << working_dir_ << "/event_cooling_rod_" << data_suffix << iteration << ".data' using 1:3 " << title << "with linespoints ls 10, \\" << std::endl;
                first_round = false;
            }
        } else if (key.find("water_circulation") != std::string::npos) {
            std::string title = "title 'Water Circulation Event' ";
            bool first_round = true;
            for(auto iteration : iteration_strings) {
                if (!first_round) {
                    title = "notitle ";
                }
                cfg << "     '" << working_dir_ << "/event_water_circulation_" << data_suffix << iteration << ".data' using 1:3 " << title << "with linespoints ls 11, \\" << std::endl;
                first_round = false;
            }
        } else if (key.find("o2_electrolysis") != std::string::npos) {
            std::string title = "title 'O2 Electrolysis Event' ";
            bool first_round = true;
            for(auto iteration : iteration_strings) {
                if (!first_round) {
                    title = "notitle ";
                }
                cfg << "     '" << working_dir_ << "/event_o2_electrolysis_" << data_suffix << iteration << ".data' using 1:3 " << title << "with linespoints ls 12, \\" << std::endl;
                first_round = false;
            }
        }
    }

    // Terminate conditional lines above so that ', \\' can always be specified. This simplifies everything
    // The file does not exist and is ignored.
    cfg << "     '" << working_dir_ << "/end_marker" << ".data' using 1:3 title '' with linespoints ls 12" << std::endl;

    cfg << std::endl;

    std::stringstream path;
    path << working_dir_ << "/" << file_name << ".gnuplot";

    std::ofstream out(path.str());
    out << cfg.str();
    out.close();
}

void gnuplot::generate_temperature_config_file(const std::string& file_name,
    const std::string& title, const std::string& data_suffix,
    const std::unordered_map<std::string,std::size_t>& event_stats)
{
    std::stringstream cfg;
    cfg << "set terminal png size 1920,1080 font 'Helvetica,16'" << std::endl
        << "set output '" << working_dir_ << "/" << file_name << ".png'" << std::endl
        << "set tics font 'Helvetica,16'" << std::endl
        << "set lmargin screen 0.06" << std::endl
        << "set rmargin screen 0.83" << std::endl
        << "set tmargin screen 0.96" << std::endl
        << "set bmargin screen 0.10" << std::endl
        << "set xdata time" << std::endl
        << "set timefmt '%Y-%m-%d %H:%M:%S'" << std::endl
        << "set format x '%d / %H:%M'" << std::endl
        << "set grid" << std::endl
        << "set grid ytics xtics" << std::endl
        << "set autoscale x" << std::endl
        << "set autoscale y" << std::endl
        << "set xtics rotate by -270" << std::endl
        << "set style line 1 lc rgb '#95c85e' lt 1 lw 2 pt 7 pi -1 ps 1.1" << std::endl
        << "set style line 2 lc rgb '#97bada' lt 1 lw 2 pt 7 pi -1 ps 1.1" << std::endl
        << "set style line 3 lc rgb '#86591a' lt 1 lw 2 pt 7 pi -1 ps 1.1" << std::endl
        << "set style line 4 lc rgb '#000000' lt 1 lw 2 pt 7 pi -1 ps 1.1" << std::endl
        << "set style line 5 lc rgb '#000f00' lt 1 lw 2 pt 7 pi -1 ps 1.1" << std::endl
        << "set style line 6 lc rgb '#a25d07' lt 1 lw 1 pt 8 pi -1 ps 2" << std::endl
        << "set style line 7 lc rgb '#a25d07' lt 1 lw 1 pt 10 pi -1 ps 2" << std::endl
        << "set style line 8 lc rgb '#a25d07' lt 1 lw 1 pt 17 pi -1 ps 2" << std::endl
        << "set style line 9 lc rgb '#a25d07' lt 1 lw 1 pt 19 pi -1 ps 2" << std::endl
        << "set style line 10 lc rgb '#a25d07' lt 1 lw 1 pt 3 pi -1 ps 2" << std::endl
        << "set style line 11 lc rgb '#a25d07' lt 1 lw 1 pt 2 pi -1 ps 2" << std::endl
        << "set style line 12 lc rgb '#a25d07' lt 1 lw 1 pt 12 pi -1 ps 2" << std::endl
        << "set title '{/:Bold " << title << "}' font ',16'" << std::endl
        << "set ylabel 'Temperature (Celcius)' offset 0, 0 textcolor rgb '#000000'" << std::endl
        << "set key outside" << std::endl
        << std::endl;

     cfg << "plot '" << working_dir_ << "/sensor_ambient_temperature_" << data_suffix << ".data' using 1:3 title 'Ambient Temperature' with linespoints ls 1, \\" << std::endl
         << "     '" << working_dir_ << "/sensor_water_temperature_" << data_suffix << ".data' using 1:3 title 'Water Temperature' with linespoints ls 2, \\" << std::endl;

    common_config_helper(file_name,
                         title,
                         data_suffix,
                         event_stats,
                         cfg);
}

void gnuplot::generate_humidity_config_file(const std::string& file_name,
    const std::string& title, const std::string& data_suffix,
    const std::unordered_map<std::string,std::size_t>& event_stats)
{
    std::stringstream cfg;
    cfg << "set terminal png size 1920,1080 font 'Helvetica,16'" << std::endl
        << "set output '" << working_dir_ << "/" << file_name << ".png'" << std::endl
        << "set tics font 'Helvetica,16'" << std::endl
        << "set lmargin screen 0.06" << std::endl
        << "set rmargin screen 0.83" << std::endl
        << "set tmargin screen 0.96" << std::endl
        << "set bmargin screen 0.10" << std::endl
        << "set xdata time" << std::endl
        << "set timefmt '%Y-%m-%d %H:%M:%S'" << std::endl
        << "set format x '%d / %H:%M'" << std::endl
        << "set grid" << std::endl
        << "set grid ytics xtics" << std::endl
        << "set autoscale x" << std::endl
        << "set autoscale y" << std::endl
        << "set xtics rotate by -270" << std::endl
        << "set style line 1 lc rgb '#95c85e' lt 1 lw 2 pt 7 pi -1 ps 1.1" << std::endl
        << "set style line 2 lc rgb '#97bada' lt 1 lw 2 pt 7 pi -1 ps 1.1" << std::endl
        << "set style line 3 lc rgb '#86591a' lt 1 lw 2 pt 7 pi -1 ps 1.1" << std::endl
        << "set style line 4 lc rgb '#000000' lt 1 lw 2 pt 7 pi -1 ps 1.1" << std::endl
        << "set style line 5 lc rgb '#000f00' lt 1 lw 2 pt 7 pi -1 ps 1.1" << std::endl
        << "set style line 6 lc rgb '#a25d07' lt 1 lw 1 pt 8 pi -1 ps 2" << std::endl
        << "set style line 7 lc rgb '#a25d07' lt 1 lw 1 pt 10 pi -1 ps 2" << std::endl
        << "set style line 8 lc rgb '#a25d07' lt 1 lw 1 pt 17 pi -1 ps 2" << std::endl
        << "set style line 9 lc rgb '#a25d07' lt 1 lw 1 pt 19 pi -1 ps 2" << std::endl
        << "set style line 10 lc rgb '#a25d07' lt 1 lw 1 pt 3 pi -1 ps 2" << std::endl
        << "set style line 11 lc rgb '#a25d07' lt 1 lw 1 pt 2 pi -1 ps 2" << std::endl
        << "set style line 12 lc rgb '#a25d07' lt 1 lw 1 pt 12 pi -1 ps 2" << std::endl
        << "set title '{/:Bold " << title << "}' font ',16'" << std::endl
        << "set ylabel 'Humidity (percentage)' offset 0, 0 textcolor rgb '#000000'" << std::endl
        << "set key outside" << std::endl
        << std::endl;

     cfg << "plot '" << working_dir_ << "/sensor_ambient_humidity_" << data_suffix << ".data' using 1:3 title 'Ambient Humidity' with linespoints ls 1, \\" << std::endl;

    common_config_helper(file_name,
                         title,
                         data_suffix,
                         event_stats,
                         cfg);
}

void gnuplot::generate_water_ec_config_file(const std::string& file_name,
    const std::string& title, const std::string& data_suffix,
    const std::unordered_map<std::string,std::size_t>& event_stats)
{
    std::stringstream cfg;
    cfg << "set terminal png size 1920,1080 font 'Helvetica,16'" << std::endl
        << "set output '" << working_dir_ << "/" << file_name << ".png'" << std::endl
        << "set tics font 'Helvetica,16'" << std::endl
        << "set lmargin screen 0.06" << std::endl
        << "set rmargin screen 0.83" << std::endl
        << "set tmargin screen 0.96" << std::endl
        << "set bmargin screen 0.10" << std::endl
        << "set xdata time" << std::endl
        << "set timefmt '%Y-%m-%d %H:%M:%S'" << std::endl
        << "set format x '%d / %H:%M'" << std::endl
        << "set grid" << std::endl
        << "set grid ytics xtics" << std::endl
        << "set autoscale x" << std::endl
        << "set autoscale y" << std::endl
        << "set xtics rotate by -270" << std::endl
        << "set style line 1 lc rgb '#95c85e' lt 1 lw 2 pt 7 pi -1 ps 1.1" << std::endl
        << "set style line 2 lc rgb '#97bada' lt 1 lw 2 pt 7 pi -1 ps 1.1" << std::endl
        << "set style line 3 lc rgb '#86591a' lt 1 lw 2 pt 7 pi -1 ps 1.1" << std::endl
        << "set style line 4 lc rgb '#000000' lt 1 lw 2 pt 7 pi -1 ps 1.1" << std::endl
        << "set style line 5 lc rgb '#000f00' lt 1 lw 2 pt 7 pi -1 ps 1.1" << std::endl
        << "set style line 6 lc rgb '#a25d07' lt 1 lw 1 pt 8 pi -1 ps 2" << std::endl
        << "set style line 7 lc rgb '#a25d07' lt 1 lw 1 pt 10 pi -1 ps 2" << std::endl
        << "set style line 8 lc rgb '#a25d07' lt 1 lw 1 pt 17 pi -1 ps 2" << std::endl
        << "set style line 9 lc rgb '#a25d07' lt 1 lw 1 pt 19 pi -1 ps 2" << std::endl
        << "set style line 10 lc rgb '#a25d07' lt 1 lw 1 pt 3 pi -1 ps 2" << std::endl
        << "set style line 11 lc rgb '#a25d07' lt 1 lw 1 pt 2 pi -1 ps 2" << std::endl
        << "set style line 12 lc rgb '#a25d07' lt 1 lw 1 pt 12 pi -1 ps 2" << std::endl
        << "set title '{/:Bold " << title << "}' font ',16'" << std::endl
        << "set ylabel 'Water EC' offset 0, 0 textcolor rgb '#000000'" << std::endl
        << "set key outside" << std::endl
        << std::endl;

     cfg << "plot '" << working_dir_ << "/sensor_water_ec_" << data_suffix << ".data' using 1:3 title 'Water EC' with linespoints ls 1, \\" << std::endl;

        common_config_helper(file_name,
                         title,
                         data_suffix,
                         event_stats,
                         cfg);
}

void gnuplot::generate_data_file(const std::vector<measurement>& data, const std::string& file_name)
{
    std::stringstream path;
    path << working_dir_ << "/" << file_name << ".data";

    std::ofstream out(path.str());

    for(auto&& e : data) {
        auto epoch_ts = std::chrono::system_clock::to_time_t(e.ts);
        
        std::tm tm = *std::localtime(&epoch_ts);
        
        std::stringstream line;
        line << std::put_time(&tm, "%F %T") << "\t" << e.value << std::endl;

        out << line.str();
    }
    
    out.close();
}

void gnuplot::generate_event_file(const std::vector<measurement>& data,
        const std::string& file_name,
        double overall_sensor_min,
        double overall_sensor_max)
{
    if (data.empty())
    {
        return;
    }

    std::size_t idx = 0;
    for(auto&& e : data) {
        std::stringstream path;
        path << working_dir_ << "/" << file_name << "_" << std::setfill('0') << std::setw(3) << idx << ".data";

        std::ofstream out(path.str());

        auto epoch_ts = std::chrono::system_clock::to_time_t(e.ts);
        
        std::tm tm = *std::localtime(&epoch_ts);
        
         auto range = fabs(overall_sensor_max - overall_sensor_min);
         auto step = range / 12;

        auto v = overall_sensor_min;
        for(int i=0; i < 11; i++) {
            v += step;
        
            std::stringstream line;
            line << std::put_time(&tm, "%F %T") << "\t" << v << std::endl;
            out << line.str();
        }
        
        out.close();
        
        idx++;
    }
}

void gnuplot::render(const std::string& file_name)
{
    std::stringstream cmd;
    cmd << "/usr/bin/gnuplot " << working_dir_ << "/" << file_name << ".gnuplot";

    system(cmd.str().c_str());
}
