//
// Created by sergi on 9/13/2023.
//

#include "http_request_task.hpp"

#include <spdlog/spdlog.h>

#include <boost/asio/connect.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>

#include <iostream>
#include <tracy/Tracy.hpp>

namespace rapic {

namespace {

void load_root_certificates(boost::asio::ssl::context& ctx, boost::system::error_code& ec) {
    std::string const cert =
      /*  This is the DigiCert root certificate.

          CN = DigiCert High Assurance EV Root CA
          OU = www.digicert.com
          O = DigiCert Inc
          C = US

      Valid to: Sunday, ?November ?9, ?2031 5:00:00 PM

                          Thumbprint(sha1):
                      5f b7 ee 06 33 e2 59 db ad 0c 4c 9a e6 d3 8f 1a 61 c7 dc 25
                        */
      "-----BEGIN CERTIFICATE-----\n"
      "MIIDxTCCAq2gAwIBAgIQAqxcJmoLQJuPC3nyrkYldzANBgkqhkiG9w0BAQUFADBs\n"
      "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
      "d3cuZGlnaWNlcnQuY29tMSswKQYDVQQDEyJEaWdpQ2VydCBIaWdoIEFzc3VyYW5j\n"
      "ZSBFViBSb290IENBMB4XDTA2MTExMDAwMDAwMFoXDTMxMTExMDAwMDAwMFowbDEL\n"
      "MAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQd3d3\n"
      "LmRpZ2ljZXJ0LmNvbTErMCkGA1UEAxMiRGlnaUNlcnQgSGlnaCBBc3N1cmFuY2Ug\n"
      "RVYgUm9vdCBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMbM5XPm\n"
      "+9S75S0tMqbf5YE/yc0lSbZxKsPVlDRnogocsF9ppkCxxLeyj9CYpKlBWTrT3JTW\n"
      "PNt0OKRKzE0lgvdKpVMSOO7zSW1xkX5jtqumX8OkhPhPYlG++MXs2ziS4wblCJEM\n"
      "xChBVfvLWokVfnHoNb9Ncgk9vjo4UFt3MRuNs8ckRZqnrG0AFFoEt7oT61EKmEFB\n"
      "Ik5lYYeBQVCmeVyJ3hlKV9Uu5l0cUyx+mM0aBhakaHPQNAQTXKFx01p8VdteZOE3\n"
      "hzBWBOURtCmAEvF5OYiiAhF8J2a3iLd48soKqDirCmTCv2ZdlYTBoSUeh10aUAsg\n"
      "EsxBu24LUTi4S8sCAwEAAaNjMGEwDgYDVR0PAQH/BAQDAgGGMA8GA1UdEwEB/wQF\n"
      "MAMBAf8wHQYDVR0OBBYEFLE+w2kD+L9HAdSYJhoIAu9jZCvDMB8GA1UdIwQYMBaA\n"
      "FLE+w2kD+L9HAdSYJhoIAu9jZCvDMA0GCSqGSIb3DQEBBQUAA4IBAQAcGgaX3Nec\n"
      "nzyIZgYIVyHbIUf4KmeqvxgydkAQV8GK83rZEWWONfqe/EW1ntlMMUu4kehDLI6z\n"
      "eM7b41N5cdblIZQB2lWHmiRk9opmzN6cN82oNLFpmyPInngiK3BD41VHMWEZ71jF\n"
      "hS9OMPagMRYjyOfiZRYzy78aG6A9+MpeizGLYAiJLQwGXFK3xPkKmNEVX58Svnw2\n"
      "Yzi9RKR/5CYrCsSXaQ3pjOLAEFe4yHYSkVXySGnYvCoCWw9E1CAx2/S6cCZdkGCe\n"
      "vEsXCS+0yx5DaMkHJ8HSXPfqIbloEpw8nL+e/IBcm2PN7EeqJSdnoDfzAIJ9VNep\n"
      "+OkuE6N36B9K\n"
      "-----END CERTIFICATE-----\n"
      /*  This is the GeoTrust root certificate.

          CN = GeoTrust Global CA
          O = GeoTrust Inc.
          C = US
          Valid to: Friday, ‎May ‎20, ‎2022 9:00:00 PM

          Thumbprint(sha1):
          ‎de 28 f4 a4 ff e5 b9 2f a3 c5 03 d1 a3 49 a7 f9 96 2a 82 12
      */
      "-----BEGIN CERTIFICATE-----\n"
      "MIIDVDCCAjygAwIBAgIDAjRWMA0GCSqGSIb3DQEBBQUAMEIxCzAJBgNVBAYTAlVT\n"
      "MRYwFAYDVQQKEw1HZW9UcnVzdCBJbmMuMRswGQYDVQQDExJHZW9UcnVzdCBHbG9i\n"
      "YWwgQ0EwHhcNMDIwNTIxMDQwMDAwWhcNMjIwNTIxMDQwMDAwWjBCMQswCQYDVQQG\n"
      "EwJVUzEWMBQGA1UEChMNR2VvVHJ1c3QgSW5jLjEbMBkGA1UEAxMSR2VvVHJ1c3Qg\n"
      "R2xvYmFsIENBMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA2swYYzD9\n"
      "9BcjGlZ+W988bDjkcbd4kdS8odhM+KhDtgPpTSEHCIjaWC9mOSm9BXiLnTjoBbdq\n"
      "fnGk5sRgprDvgOSJKA+eJdbtg/OtppHHmMlCGDUUna2YRpIuT8rxh0PBFpVXLVDv\n"
      "iS2Aelet8u5fa9IAjbkU+BQVNdnARqN7csiRv8lVK83Qlz6cJmTM386DGXHKTubU\n"
      "1XupGc1V3sjs0l44U+VcT4wt/lAjNvxm5suOpDkZALeVAjmRCw7+OC7RHQWa9k0+\n"
      "bw8HHa8sHo9gOeL6NlMTOdReJivbPagUvTLrGAMoUgRx5aszPeE4uwc2hGKceeoW\n"
      "MPRfwCvocWvk+QIDAQABo1MwUTAPBgNVHRMBAf8EBTADAQH/MB0GA1UdDgQWBBTA\n"
      "ephojYn7qwVkDBF9qn1luMrMTjAfBgNVHSMEGDAWgBTAephojYn7qwVkDBF9qn1l\n"
      "uMrMTjANBgkqhkiG9w0BAQUFAAOCAQEANeMpauUvXVSOKVCUn5kaFOSPeCpilKIn\n"
      "Z57QzxpeR+nBsqTP3UEaBU6bS+5Kb1VSsyShNwrrZHYqLizz/Tt1kL/6cdjHPTfS\n"
      "tQWVYrmm3ok9Nns4d0iXrKYgjy6myQzCsplFAMfOEVEiIuCl6rYVSAlk6l5PdPcF\n"
      "PseKUgzbFbS9bZvlxrFUaKnjaZC2mqUPuLk/IH2uSrW4nOQdtqvmlKXBx4Ot2/Un\n"
      "hw4EbNX/3aBd7YdStysVAq45pmp06drE57xNNB6pXE0zX5IJL4hmXXeXxx12E6nV\n"
      "5fEWCRE11azbJHFwLJhWC9kXtNHjUStedejV0NxPNO3CBWaAocvmMw==\n"
      "-----END CERTIFICATE-----\n";

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

    ZoneScopedN("HttpsRequest");

    std::stringstream ss;
    ss << std::this_thread::get_id();
    spdlog::error("task: {}", ss.str());

    spdlog::error("1");

    // We need an io_context
    boost::asio::io_context ioc;
    ioc.run();

    // The SSL context is required, and holds certificates
    ssl::context ctx(ssl::context::tlsv12_client);

    // This holds the root certificate used for verification
    load_root_certificates(ctx);

    // Verify the remote server's certificate
    ctx.set_verify_mode(ssl::verify_peer);

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
    //stream.connect(results);

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
        ZoneScopedN("Callback execution");
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
