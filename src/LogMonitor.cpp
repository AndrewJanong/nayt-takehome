#include "LogMonitor.h"
#include <iostream>

LogMonitor::LogMonitor(const Config& config) : config_(config) {}

LogMonitor::~LogMonitor() {
    if (input_stream_.is_open()) input_stream_.close();
    if (output_stream_.is_open()) output_stream_.close();
}

void LogMonitor::run() {
    std::cout << config_.input_file << " -> " << config_.output_file << "\n";
}

void LogMonitor::stop() {
    std::cout << "stopped monitoring\n";
}