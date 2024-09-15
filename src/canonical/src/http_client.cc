#include "http_client.hpp"

#include <utility>
#include <iostream>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

namespace canonical
{

http_client::http_client(std::string host, std::string target)
    : host_{std::move(host)}, target_{std::move(target)}
{
}

std::string http_client::fetch() noexcept
{
    tcp::resolver resolver_(ioc_);
    beast::tcp_stream stream_(ioc_);
    try {
        auto const results = resolver_.resolve(host_, "80");

        stream_.connect(results); // NOLINT(*-unused-return-value)

        http::request<http::string_body> req{http::verb::get, target_, 11};
        req.set(http::field::host, host_);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        http::write(stream_, req);
        beast::flat_buffer buffer;
        http::response<http::dynamic_body> res;

        http::response_parser<http::dynamic_body> parser;
        // Increase the body limit to 32 MB since I got error from beast due to huge payload
        parser.body_limit(32 * 1024 * 1024);

        http::read(stream_, buffer, parser);
        res = parser.get();
        return beast::buffers_to_string(res.body().data());
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return "";
    }
}

} // namespace canonical