//
// Created by sergi on 9/13/2023.
//

#include "http_request_task.hpp"

#include <iostream>

#include <spdlog/spdlog.h>

#include <boost/asio/connect.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>

namespace rapic {

namespace {

void load_root_certificates(boost::asio::ssl::context& ctx, boost::system::error_code& ec) {
    std::string const cert = R"cert(
    -----BEGIN CERTIFICATE-----
    MIIFMzCCBNmgAwIBAgIQBR7vAT2cAAYGAFsa8gAvfjAKBggqhkjOPQQDAjBKMQsw
    CQYDVQQGEwJVUzEZMBcGA1UEChMQQ2xvdWRmbGFyZSwgSW5jLjEgMB4GA1UEAxMX
    Q2xvdWRmbGFyZSBJbmMgRUNDIENBLTMwHhcNMjMwNTE0MDAwMDAwWhcNMjQwNTEz
    MjM1OTU5WjB1MQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQG
    A1UEBxMNU2FuIEZyYW5jaXNjbzEZMBcGA1UEChMQQ2xvdWRmbGFyZSwgSW5jLjEe
    MBwGA1UEAxMVc25pLmNsb3VkZmxhcmVzc2wuY29tMFkwEwYHKoZIzj0CAQYIKoZI
    zj0DAQcDQgAEFE92679mwN1mP5i39w460mTZrltgGwvD2ceXnZpDq0K28F2KoXKD
    ci25iN5H8f93B730+YA9tMHgYh8UgIutoqOCA3QwggNwMB8GA1UdIwQYMBaAFKXO
    N+rrsHUOlGeItEX62SQQh5YfMB0GA1UdDgQWBBQdKROjxay3VnccvOrYNkT7bH4a
    wDA4BgNVHREEMTAvgglyZXFyZXMuaW6CCyoucmVxcmVzLmlughVzbmkuY2xvdWRm
    bGFyZXNzbC5jb20wDgYDVR0PAQH/BAQDAgeAMB0GA1UdJQQWMBQGCCsGAQUFBwMB
    BggrBgEFBQcDAjB7BgNVHR8EdDByMDegNaAzhjFodHRwOi8vY3JsMy5kaWdpY2Vy
    dC5jb20vQ2xvdWRmbGFyZUluY0VDQ0NBLTMuY3JsMDegNaAzhjFodHRwOi8vY3Js
    NC5kaWdpY2VydC5jb20vQ2xvdWRmbGFyZUluY0VDQ0NBLTMuY3JsMD4GA1UdIAQ3
    MDUwMwYGZ4EMAQICMCkwJwYIKwYBBQUHAgEWG2h0dHA6Ly93d3cuZGlnaWNlcnQu
    Y29tL0NQUzB2BggrBgEFBQcBAQRqMGgwJAYIKwYBBQUHMAGGGGh0dHA6Ly9vY3Nw
    LmRpZ2ljZXJ0LmNvbTBABggrBgEFBQcwAoY0aHR0cDovL2NhY2VydHMuZGlnaWNl
    cnQuY29tL0Nsb3VkZmxhcmVJbmNFQ0NDQS0zLmNydDAMBgNVHRMBAf8EAjAAMIIB
    gAYKKwYBBAHWeQIEAgSCAXAEggFsAWoAdwDuzdBk1dsazsVct520zROiModGfLzs
    3sNRSFlGcR+1mwAAAYgYGRB2AAAEAwBIMEYCIQCHiM2cYex1tIOwmPUaNF3E4Zkr
    w0O4Co0MNdV5Fw0XdgIhAOPE5I1qpSQH/geH5Cz5CYVDHcqEjl8Ni/DLrvZ/e9p3
    AHcASLDja9qmRzQP5WoC+p0w6xxSActW3SyB2bu/qznYhHMAAAGIGBkQWgAABAMA
    SDBGAiEAzlTkhbX5wfLFKounirhwt1/qLf3ovTKqHF9SXRtyl7gCIQCsSjMjg/0v
    l59elFTI8+155PZZ+s3Rp9qPfnk27ECpUAB2ANq2v2s/tbYin5vCu1xr6HCRcWy7
    UYSFNL2kPTBI1/urAAABiBgZECkAAAQDAEcwRQIhAN451TG67pchNqvE51douo2p
    QmaXhGP7oanlShw7ogAvAiAK27Kv/FAh/Kel5dhBODx8ScK2A7ZWVM2eHS0ZQrHm
    tjAKBggqhkjOPQQDAgNIADBFAiEAlRoLWeZkJWCXEaR463mb2eATaVuFQ7QbLqwl
    /J5YuywCIF8LPJI+VREUxuNebmOuBVVeq1k+Ilp+FhwdFhfhACov
    -----END CERTIFICATE-----
    )cert";

    ctx.add_certificate_authority(boost::asio::buffer(cert.data(), cert.size()), ec);
    if (ec) return;
}

inline void load_root_certificates(boost::asio::ssl::context& ctx) {
    boost::system::error_code ec;
    load_root_certificates(ctx, ec);
    if (ec) throw boost::system::system_error{ec};
}

void HttpsRequest(const std::string& base_url, const Request& request, const RestClient::Callback& callback, std::chrono::milliseconds timeout) {
    using tcp = boost::asio::ip::tcp;
    namespace http = boost::beast::http;
    namespace beast = boost::beast;
    namespace ssl = boost::asio::ssl;

    std::stringstream ss;
    ss << std::this_thread::get_id();
    spdlog::error("task: {}", ss.str());

    spdlog::error("1");

    // We need an io_context
    boost::asio::io_context ioc;

    // The SSL context is required, and holds certificates
    ssl::context ctx(ssl::context::sslv23_client);
    ctx.set_default_verify_paths();

    spdlog::error("1.2");

    // This holds the root certificate used for verification
    // load_root_certificates(ctx);

    // Verify the remote server's certificate
    ctx.set_verify_mode(ssl::verify_none);
    // ctx.set_verify_callback(ssl::rfc2818_verification(base_url));

    spdlog::error("1.5");

    // We will resolve the destination and communicate using this socket
    tcp::resolver resolver(ioc);
    // tcp::socket socket(ioc);
    beast::ssl_stream<beast::tcp_stream> stream(ioc, ctx);

    // Set SNI Hostname (many hosts need this to handshake successfully)
    if (!SSL_set_tlsext_host_name(stream.native_handle(), base_url.c_str())) {
        beast::error_code ec{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
        throw beast::system_error{ec};
    }

    std::string full_url = base_url + request.url;

    // Look up the domain name
    spdlog::info("base url: {}; path: {}; full url: {};", base_url, request.url, full_url);
    auto const results = resolver.resolve(base_url, "https");

    spdlog::error("2");

    // Connect to the server
    // boost::asio::connect(socket, results.begin(), results.end());
    // stream.connect(results);

    // Make the connection on the IP address we get from a lookup
    beast::get_lowest_layer(stream).connect(results);

    // Perform the SSL handshake
    stream.handshake(ssl::stream_base::client);

    spdlog::error("3");

    // Define timeout
    //    boost::asio::steady_timer timer(ioc);
    //    timer.expires_after(timeout);

    // Create request string from the Request instance
    // Since Beast does not understand our Request structure, we must manually
    // construct the HTTP message.

    // Assumed: request.method is HttpMethod::GET
    // Time and version are hard coded for simplicity
    http::request<http::empty_body> req{http::verb::get, request.url, 11};
    req.set(http::field::host, base_url);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    // Adding headers to HTTP request
    for (const auto& header : request.headers) {
        req.set(header.first, header.second);
    }

    // Send HTTP request
    http::write(stream, req);

    // This buffer will be used to hold the response
    boost::beast::flat_buffer buffer;

    // Response container
    http::response<http::dynamic_body> res;

    // Receive HTTP response
    http::read(stream, buffer, res);

    // Prepare our custom Response
    Response response;
    response.status_code = res.result_int();
    response.status_message = res.reason();

    for (const auto& header : res.base()) {
        response.headers.insert(std::make_pair(std::to_string(static_cast<std::uint32_t>(header.name())), header.value()));
    }

    response.body = boost::beast::buffers_to_string(res.body().data());

    spdlog::error("before callback");

    // shutdown
    beast::error_code ec;
    stream.shutdown(ec);
    {
        callback(response);
    }

    spdlog::error("final");
}

}  // namespace

std::packaged_task<void(void)>
CreateHttpsRequestTask(const std::string& base_url, const Request& request, const RestClient::Callback& callback, std::chrono::milliseconds timeout) {
    return std::packaged_task<void(void)>{[&request, &callback, &timeout, &base_url] { return HttpsRequest(base_url, request, callback, timeout); }};
}
}  // namespace rapic
