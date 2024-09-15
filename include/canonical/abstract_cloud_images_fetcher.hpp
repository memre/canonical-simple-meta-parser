#pragma once

#include <optional>
#include <string>
#include <vector>
#include <fmt/format.h>

namespace canonical
{

namespace details
{

struct release
{
    std::string release_title;
    std::string version;
    bool is_lts;
    bool is_supported;
    std::string sha_disk1;
};

} // namespace details

struct abstract_cloud_images_fetcher
{
    virtual ~abstract_cloud_images_fetcher() = default;
    [[nodiscard]] virtual std::vector<details::release> get_supported_amd64_releases() const noexcept = 0;
    [[nodiscard]] virtual std::optional<details::release> get_supported_current_lts_amd64_release() const noexcept = 0;
    [[nodiscard]] virtual std::string get_sha256(std::string_view release_title) const noexcept = 0;

    inline void print_supported_amd64_releases() const noexcept
    {
        auto releases = get_supported_amd64_releases();
        for (const auto& release : releases)
        {
            fmt::print("Release: {}, Version: {}, LTS: {}, sha256: {}\n",
                       release.release_title,
                       release.version,
                       release.is_lts,
                       release.sha_disk1);
        }
    }

    inline void print_supported_current_lts_amd64_release() const noexcept
    {
        auto lts_release = get_supported_current_lts_amd64_release();
        if (lts_release.has_value())
        {
            fmt::print("Current LTS Release: {}, Version: {}\n", lts_release->release_title, lts_release->version);
        }
        else
        {
            fmt::print("No LTS release found.\n");
        }
    }

    inline void print_sha256(std::string_view release_title) const noexcept
    {
        if (release_title.empty())
        {
            fmt::print(stderr, "Please provide a release title with --release_title\n");
            return;
        }

        auto sha256 = get_sha256(release_title);
        if (sha256.empty())
        {
            fmt::print("No image sha256 found for release: {}\n", release_title);
            return;
        }

        fmt::print("image sha256 for {}: {}\n", release_title, sha256);
    }
};

extern std::unique_ptr<abstract_cloud_images_fetcher> get_cloud_images_fetcher();

} // namespace canonical