#include <config.hpp>

#include <string>

#include <cxxopts.hpp>
#include <fmt/format.h>

#include <canonical/abstract_cloud_images_fetcher.hpp>

int main(int argc, char* argv[])
{
    cxxopts::Options options("meta_fetcher", "A tool to fetch Ubuntu cloud release info");
    decltype(options.parse(argc, argv)) op_results;

    try {
        // Add options for different functionality
        options.add_options()
            ("l,list", "Return a list of all currently supported Ubuntu releases")
            ("L,lts", "Return the current Ubuntu LTS version")
            ("r,release_title", "Return the sha256 of disk1.img for a given Ubuntu release",
                     cxxopts::value<std::string>()->default_value(""))
            ("h,help", "Print usage");

        op_results = options.parse(argc, argv);
    }
    catch (const cxxopts::exceptions::exception& e)
    {
        fmt::print(stderr, "Error parsing options: {}\n", e.what());
        return 1;
    }
    catch (...)
    {
        fmt::print(stderr, "Unknown error");
        return 1;
    }

    if (argc == 1 || op_results.count("help")) {
        fmt::println(options.help());
        return 0;
    }

    {
        auto fetcher = canonical::get_cloud_images_fetcher();

        // Option 1: Return a list of all supported Ubuntu releases
        if (op_results.count("list"))
        {
            fetcher->print_supported_amd64_releases();
            return 0;
        }

        // Option 2: Return the current Ubuntu LTS version
        if (op_results.count("lts"))
        {
            fetcher->print_supported_current_lts_amd64_release();
            return 0;
        }

        // Option 3: Return the sha256 of disk1.img for a given release
        if (op_results.count("release_title"))
        {
            fetcher->print_sha256(op_results["release_title"].as<std::string>());
            return 0;
        }
    }

    fmt::println(stderr, "BUG: should never reach here!");
    return 1;
}
