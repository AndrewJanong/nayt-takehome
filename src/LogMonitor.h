#ifndef LOG_MONITOR_H
#define LOG_MONITOR_H

#include <string>
#include <fstream>

class LogMonitor {
public:
    struct Config {
        std::string input_file;
        std::string output_file;
    };

    explicit LogMonitor(const Config& config);
    ~LogMonitor();

    // starts monitoring
    void run();

    // stop monitoring
    void stop();
private:
    Config config_;

    std::ifstream input_stream_;
    std::ofstream output_stream_;
};

#endif // LOG_MONITOR_H