#include <canonical/cloud_images_fetcher.hpp>
#include <utility>

#include "http_client.hpp"
#include "meta_info_parser.hpp"

namespace canonical
{

namespace {

constexpr std::string_view cloud_images_url = "cloud-images.ubuntu.com";
constexpr std::string_view cloud_images_target = "/releases/streams/v1/com.ubuntu.cloud:released:download.json";

inline std::optional<canonical::meta_info_parser> _get_meta_info(const std::string &host, const std::string &target)
{
    canonical::http_client client(host, target);
    auto content = client.fetch();

    if (content.empty())
    {
        return {};
    }

    return std::optional<canonical::meta_info_parser>(content);
}

}

std::unique_ptr<abstract_cloud_images_fetcher> get_cloud_images_fetcher()
{
    return std::make_unique<canonical::cloud_images_fetcher>(std::string(cloud_images_url), std::string(cloud_images_target));
}

cloud_images_fetcher::cloud_images_fetcher(std::string host, std::string target)
    : host_{std::move(host)}, target_{std::move(target)}
{
}

cloud_images_fetcher::~cloud_images_fetcher() = default;

std::vector<details::release> cloud_images_fetcher::get_supported_amd64_releases() const noexcept
{
    auto maybe_meta_info = _get_meta_info(host_, target_);
    if (!maybe_meta_info) {
        return {};
    }

    return maybe_meta_info->get_supported_amd64_releases();
}

std::optional<details::release> cloud_images_fetcher::get_supported_current_lts_amd64_release() const noexcept
{
    auto maybe_meta_info = _get_meta_info(host_, target_);
    if (!maybe_meta_info) {
        return {};
    }

    return maybe_meta_info->get_supported_current_lts_amd64_release();
}

std::string cloud_images_fetcher::get_sha256(std::string_view release_title) const noexcept
{
    auto maybe_meta_info = _get_meta_info(host_, target_);
    if (!maybe_meta_info) {
        return {};
    }

    return maybe_meta_info->get_sha256(release_title);
}

} // namespace canonical
