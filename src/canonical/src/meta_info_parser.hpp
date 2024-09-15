#pragma once

#include <memory>
#include <string>
#include <vector>
#include <optional>

#include <nlohmann/json.hpp>

#include "canonical/abstract_cloud_images_fetcher.hpp"

namespace canonical
{

class meta_info_parser
{
public:
    using json = nlohmann::json;

    explicit meta_info_parser(std::string json_content);
    ~meta_info_parser();

    [[nodiscard]] std::vector<details::release> get_amd64_releases() const noexcept;
    [[nodiscard]] std::vector<details::release> get_supported_amd64_releases() const noexcept;
    [[nodiscard]] std::optional<details::release> get_supported_current_lts_amd64_release() const noexcept;
    [[nodiscard]] std::string get_sha256(std::string_view release_title) const noexcept;

private:
    void _parse(std::string raw_json);

private:
    json json_;
    std::vector<details::release> releases_;
};

} // namespace canonical