#include "LogMonitor.h"
#include <iostream>
#include <csignal>

static LogMonitor* g_monitor = nullptr;

static void handle_sigint(int) {
    if (g_monitor) g_monitor->stop();
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Missing arguments!\n";
        return 1;
    }

    LogMonitor::Config cfg;
    cfg.input_file = argv[1];
    cfg.output_file = argv[2];

    for (int i = 3; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--bench-stamp") {
            cfg.bench_stamp = true;
            continue;
        }
        if (arg.rfind("--pin-reader=", 0) == 0) {
            cfg.reader_cpu = std::atoi(arg.c_str() + 13);
            continue;
        }
        if (arg.rfind("--pin-consumer=", 0) == 0) {
            cfg.consumer_cpu = std::atoi(arg.c_str() + 15);
            continue;
        }

        cfg.keywords.push_back(std::move(arg));
    }

    LogMonitor monitor(cfg);
    g_monitor = &monitor;

    // Ctrl-C support
    std::signal(SIGINT, handle_sigint);
#ifdef SIGTERM
    std::signal(SIGTERM, handle_sigint);
#endif

    monitor.run();

    std::cout << "Done\n";
    return 0;
}
