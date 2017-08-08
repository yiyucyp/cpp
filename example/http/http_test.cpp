#include <iostream>
#include "simple-web-server/client_http.hpp"
#include "url_parse/EdUrlParser.h"
#include "http/Uri.hpp"

#include <boost/asio.hpp>
class asio_http_client
{
public:
    asio_http_client(): content(content_buffer)
    {}
    std::string get(const std::string& server, const std::string path)
    {
        try
        {
            boost::asio::ip::tcp::resolver resolver(io_service_);
            boost::asio::ip::tcp::resolver::query query(server, "http");
            boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

            // Try each endpoint until we successfully establish a connection.
            boost::asio::ip::tcp::socket socket(io_service_);
            boost::asio::connect(socket, endpoint_iterator);

            // Form the request. We specify the "Connection: close" header so that the
            // server will close the socket after transmitting the response. This will
            // allow us to treat all data up until the EOF as the content.
            boost::asio::streambuf request;
            std::ostream request_stream(&request);
            request_stream << "GET " << path << " HTTP/1.0\r\n";
            request_stream << "Host: " << server << "\r\n";
            request_stream << "Accept: */*\r\n";
            request_stream << "Connection: close\r\n\r\n";

            boost::asio::write(socket, request);

            // Read the response status line. The response streambuf will automatically
            // grow to accommodate the entire line. The growth may be limited by passing
            // a maximum size to the streambuf constructor.
            boost::asio::streambuf response;
            boost::asio::read_until(socket, response, "\r\n");

            // Check that response is OK.
            std::istream response_stream(&response);
            //std::string http_version;
            response_stream >> http_version;
            unsigned int status_code;
            response_stream >> status_code;
            std::string status_message;
            std::getline(response_stream, status_message);
            if (!response_stream || http_version.substr(0, 5) != "HTTP/")
            {
                return "Invalid response\n";
                //return 1;
            }
            if (status_code != 200)
            {
                std::cout << "Response returned with status code " << status_code << "\n";
                return "Response returned with status code";
            }

            // Read the response headers, which are terminated by a blank line.
            boost::asio::read_until(socket, response, "\r\n\r\n");

            // Process the response headers.
            std::string header;
            while (std::getline(response_stream, header) && header != "\r")
                std::cout << header << "\n";
            std::cout << "\n";

            // Write whatever content we already have to output.
            if (response.size() > 0)
                std::cout << &response;

            // Read until EOF, writing data to output as we go.
            boost::system::error_code error;
            while (boost::asio::read(socket, response,
                boost::asio::transfer_at_least(1), error))
                std::cout << &response;
            if (error != boost::asio::error::eof)
                throw boost::system::system_error(error);

            return "";
        }
        catch (std::exception& e) {

        }

    }
    class Content : public std::istream 
    {
        friend class asio_http_client;
    public:
        size_t size() noexcept {
            return streambuf.size();
        }
        /// Convenience function to return std::string. The stream buffer is consumed.
        std::string string() noexcept {
            try {
                std::stringstream ss;
                ss << rdbuf();
                return ss.str();
            }
            catch (...) {
                return std::string();
            }
        }

    private:
        boost::asio::streambuf &streambuf;
        Content(boost::asio::streambuf &streambuf) noexcept : std::istream(&streambuf), streambuf(streambuf) {}
    };

    class Response {
        friend class asio_http_client;

    public:
        std::string http_version, status_code;

        Content content;
        //typedef std::unordered_multimap<std::string, std::string, CaseInsensitiveHash, CaseInsensitiveEqual> CaseInsensitiveMultimap;
        //CaseInsensitiveMultimap header;

    private:
        boost::asio::streambuf content_buffer;

        Response() noexcept : content(content_buffer) {}
    };
private:
    boost::asio::io_service io_service_;
    std::string http_version, status_code;
    boost::asio::streambuf content_buffer;
    Content content;

    std::string header;


};

typedef SimpleWeb::Client<SimpleWeb::HTTP> HttpClient;

void test_http()
{
    EdUrlParser* purl = EdUrlParser::parseUrl("http://ww.mydogo.com/api/kaokeacc/get_taokeacc.php");
    http::Uri uri("http://ww.mydogo.com/api/kaokeacc/get_taokeacc.php");

    asio_http_client asioclient;
    asioclient.get(purl->hostName, purl->path);
    HttpClient client(purl->hostName);
    auto r1 = client.request("GET", purl->path, "", { {"Accept", "*/*"},{ "Connection", "close\r\n"} });
    r1 = client.request("GET", purl->path);
    std::cout << r1->content.string();
}

