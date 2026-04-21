#include <iostream>
#include <ifaddrs.h>
#include <linux/if_link.h>
#include <string>
#include <iomanip>

const std::string VERSION = "1.0.0";

void print_help() {
    std::cout << "Usage: nwusage [options]\n"
              << "Options:\n"
              << "  -hr       Print in human-readable format (KiB, MiB, GiB)\n"
              << "  -v        Show version\n"
              << "  -h        Show this help message\n";
}

std::string to_human(unsigned long long bytes) {
    const char* units[] = {"B", "KiB", "MiB", "GiB", "TiB"};
    int i = 0;
    double d_bytes = bytes;
    while (d_bytes >= 1024 && i < 4) {
        d_bytes /= 1024;
        i++;
    }
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << d_bytes << " " << units[i];
    return ss.str();
}

int main(int argc, char* argv[]) {
    bool human_readable = false;

    // Simple Argument Parser
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h") { print_help(); return 0; }
        if (arg == "-v") { std::cout << "nwusage version " << VERSION << std::endl; return 0; }
        if (arg == "-hr") { human_readable = true; }
    }

    struct ifaddrs *ifaddr, *ifa;
    unsigned long long max_traffic = 0;
    unsigned long long best_rx = 0, best_tx = 0;

    if (getifaddrs(&ifaddr) == -1) return 1;

    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr || ifa->ifa_addr->sa_family != AF_PACKET || !ifa->ifa_data)
            continue;

        struct rtnl_link_stats *stats = (struct rtnl_link_stats *)ifa->ifa_data;
        unsigned long long total = stats->rx_bytes + stats->tx_bytes;

        if (std::string(ifa->ifa_name) != "lo" && total > max_traffic) {
            max_traffic = total;
            best_rx = stats->rx_bytes;
            best_tx = stats->tx_bytes;
        }
    }

    if (max_traffic > 0) {
        if (human_readable) {
            std::cout << "DL " << to_human(best_rx) << " UL " << to_human(best_tx) << std::endl;
        } else {
            std::cout << "DL " << best_rx << " UL " << best_tx << std::endl;
        }
    }

    freeifaddrs(ifaddr);
    return 0;
}
