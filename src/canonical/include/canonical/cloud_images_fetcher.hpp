#pragma once

#include <memory>
#include <string>
#include <vector>
#include <optional>

#include <canonical/abstract_cloud_images_fetcher.hpp>

namespace canonical
{

class cloud_images_fetcher : public abstract_cloud_images_fetcher
{
public:
    explicit cloud_images_fetcher(std::string host, std::string target);
    ~cloud_images_fetcher() override;

    [[nodiscard]] std::vector<details::release> get_supported_amd64_releases() const noexcept override;
    [[nodiscard]] std::optional<details::release> get_supported_current_lts_amd64_release() const noexcept override;
    [[nodiscard]] std::string get_sha256(std::string_view release_title) const noexcept override;

private:
    std::string host_;
    std::string target_;
};

} // namespace canonical