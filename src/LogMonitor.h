#ifndef LOG_MONITOR_H
#define LOG_MONITOR_H

#include <string>
#include <fstream>

class LogMonitor {
public:
    struct Config {
        std::string input_file;
        std::string output_file;
        size_t buffer_size = 8192; // 8KB
    };

    explicit LogMonitor(const Config& config);
    ~LogMonitor();

    // starts monitoring
    void run();

    // stop monitoring
    void stop();
private:
    Config config_;
    bool running_;

    std::ifstream input_stream_;
    std::ofstream output_stream_;

    std::string current_line_;

    bool openFiles();
    void processBuffer(const char* buffer, size_t bytes_read);
    void processLine(const std::string& line);
    void waitForData();

};

#endif // LOG_MONITOR_H