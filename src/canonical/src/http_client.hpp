#pragma once

#include <memory>
#include <string>

#include <utility>
#include <iostream>

#include "boost/asio.hpp"
#include "boost/beast/core.hpp"
#include "boost/beast/http.hpp"
#include "boost/beast/version.hpp"

namespace canonical
{

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

class http_client
{
public:
    http_client(std::string host, std::string target);
    ~http_client() = default;

    [[nodiscard]] std::string fetch() noexcept;

private:
    std::string host_;
    std::string target_;
    net::io_context ioc_;
};

} // namespace canonical