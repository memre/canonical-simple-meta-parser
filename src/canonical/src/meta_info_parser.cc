#include <iostream>
#include <string_view>

#include "meta_info_parser.hpp"

#include <fmt/format.h>
#include <nlohmann/json.hpp>
#include <range/v3/all.hpp>

namespace canonical
{

namespace
{

constexpr std::string_view valid_arch = "amd64";

constexpr std::string_view products_tag = "products";
constexpr std::string_view supported_tag = "supported";
constexpr std::string_view version_tag = "version";
constexpr std::string_view arch_tag = "arch";
constexpr std::string_view release_title_tag = "release_title";

inline bool _is_lts(std::string_view release_title) noexcept
{
    return release_title.find("LTS") != std::string::npos;
}

inline std::string _get_sha(nlohmann::json versions)
{
    try
    {
        auto first_version = versions.items().begin().value();
        return first_version["items"]["disk1.img"]["sha256"].get<std::string>();
    }
    catch (...)
    {
        // No-op: sha256 does not exist
    }

    return "<no sha256sum for disk1.img>";
}

} // namespace

meta_info_parser::meta_info_parser(std::string json_content)
{
    _parse(std::move(json_content));
}

meta_info_parser::~meta_info_parser() = default;

void meta_info_parser::_parse(std::string raw_json)
{
    try
    {
        std::vector<details::release> releases;
        json_ = json::parse(std::move(raw_json));

        if (!json_.contains(products_tag))
        {
            fmt::println(stderr, "json doesn't have `{}' key", products_tag);
            return;
        }
        if (!json_[products_tag].is_object())
        {
            fmt::println(stderr, "json doesn't `{}' object", products_tag);
            return;
        }

        for (auto &[key, val]: json_[products_tag].items()) {
            if (!val.contains(supported_tag) || !val[supported_tag].is_boolean() || !val.contains(arch_tag) ||
                !val[arch_tag].is_string() || val[arch_tag].get<std::string>() != valid_arch)
            {
                continue;
            }

              auto release_title = val[release_title_tag].get<std::string>();
              auto version = val[version_tag].get<std::string>();
              auto is_lts = _is_lts(release_title);
              auto is_supported = val[supported_tag].get<bool>();
              std::string sha256 = _get_sha(val["versions"]);
              releases_.push_back({release_title, version, is_lts, is_supported, sha256});
        }
//        auto products = json_[products_tag].items();
//
//        auto valid_releases = products | ranges::views::filter([](const auto&& item) {
//                                  const auto& val = item.value();
//                                  return val.contains(supported_tag) && val[supported_tag].is_boolean() &&
//                                         val.contains(arch_tag) && val[arch_tag].is_string() &&
//                                         val[arch_tag].get<std::string>() == valid_arch;
//                              }) |
//                              ranges::views::transform([](const auto&& item) -> details::release {
//                                  const auto& val = item.value();
//                                  auto release_title = val[release_title_tag].get<std::string>();
//                                  auto version = val[version_tag].get<std::string>();
//                                  auto is_lts = _is_lts(release_title);
//                                  auto is_supported = val[supported_tag].get<bool>();
//                                  std::string sha256 = _get_sha(val["versions"]);
//                                  return details::release{release_title, version, is_lts, is_supported, sha256};
//                              });
//        releases_ = valid_releases | ranges::to<std::vector>();
    }
    catch (std::exception& e)
    {
        fmt::println(stderr, "json could not be parsed {}", e.what());
    }
}

[[nodiscard]] std::vector<details::release> meta_info_parser::get_amd64_releases() const noexcept
{
    return releases_;
}

[[nodiscard]] std::vector<details::release> meta_info_parser::get_supported_amd64_releases() const noexcept
{
    auto releases = get_amd64_releases();
    return releases | ranges::views::filter([](const auto& rel) { return rel.is_supported; }) |
           ranges::to<std::vector>();
}

[[nodiscard]] std::optional<details::release> meta_info_parser::get_supported_current_lts_amd64_release() const noexcept
{
    auto releases = get_amd64_releases();
    auto supported_lts_releases =
        releases | ranges::views::filter([](const auto& rel) { return rel.is_supported && rel.is_lts; }) |
        ranges::to<std::vector>();

    ranges::actions::sort(supported_lts_releases, [](const details::release& r0, const details::release& r1) {
        return r0.version > r1.version; // just string compare, desc
    });

    for (auto& rel : supported_lts_releases)
    {
        return rel;
    }

    return {};
}

[[nodiscard]] std::string meta_info_parser::get_sha256(std::string_view release_title) const noexcept
{
    auto releases = get_amd64_releases();

    for (auto &release: releases) {
        if (release.release_title != release_title)
            continue;

        return release.sha_disk1;
    }
    return "";
}

} // namespace canonical
