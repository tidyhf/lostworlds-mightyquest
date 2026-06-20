/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 09-01-2018
    License: MIT
    Notes:
        Provides an implementation of HTTP over TCP parsing.
*/

#include "../../Stdinclude.hpp"
#include "../../Utility/Thirdparty/http_parser.h"

// Request information.
struct HTTPHeader
{
    std::string Field;
    std::string Value;
};
struct HTTPRequest
{
    bool Parsed;
    std::string URL;
    std::string Method;
    std::vector<HTTPHeader> Headers;
    std::string Body;
};

namespace
{
    // Parsing callbacks.
    inline int Parse_Messagebegin(http_parser *Parser, HTTPRequest *Request)
    {
        Request->Headers.clear();
        return 0;
    }
    inline int Parse_URL(http_parser *Parser, HTTPRequest *Request, const char *Data, size_t Length)
    {
        Request->URL = std::string(Data, Length);
        return 0;
    }
    inline int Parse_Headerfield(http_parser *Parser, HTTPRequest *Request, const char *Data, size_t Length)
    {
        HTTPHeader Header;
        Header.Field = std::string(Data, Length);

        Request->Headers.push_back(Header);
        return 0;
    }
    inline int Parse_Headervalue(http_parser *Parser, HTTPRequest *Request, const char *Data, size_t Length)
    {
        HTTPHeader *Header = &Request->Headers.back();
        Header->Value = std::string(Data, Length);
        return 0;
    }
    inline int Parse_Headerscomplete(http_parser *Parser, HTTPRequest *Request)
    {
        Request->Method = http_method_str((http_method)Parser->method);
        return 0;
    }
    inline int Parse_Body(http_parser *Parser, HTTPRequest *Request, const char *Data, size_t Length)
    {
        Request->Body += std::string(Data, Length);
        return 0;
    }
    inline int Parse_Messagecomplete(http_parser *Parser, HTTPRequest *Request)
    {
        Request->Parsed = true;
        return 0;
    }
}

// The HTTP server just decodes the data and pass it to a callback.
struct IHTTPServer : IStreamserver
{
    // HTTP parser information.
    std::unordered_map<size_t, http_parser> Parser;
    std::unordered_map<size_t, HTTPRequest> Parsedrequest;
    std::unordered_map<size_t, http_parser_settings> Parsersettings;

    // Callbacks on parsed data.
    virtual void onGET(const size_t Socket, HTTPRequest &Request) = 0;
    virtual void onPUT(const size_t Socket, HTTPRequest &Request) = 0;
    virtual void onPOST(const size_t Socket, HTTPRequest &Request) = 0;
    virtual void onCOPY(const size_t Socket, HTTPRequest &Request) = 0;
    virtual void onDELETE(const size_t Socket, HTTPRequest &Request) = 0;

    // Callback on incoming data.
    virtual void onData(const size_t Socket, std::vector<uint8_t> &Stream)
    {
        // Initialize a parser for the socket if needed.
        if (Parser.find(Socket) == Parser.end())
        {
            http_parser_init(&Parser[Socket], HTTP_BOTH);
            http_parser_settings_init(&Parsersettings[Socket]);
            Parsedrequest[Socket].Parsed = false;
            Parser[Socket].data = &Parsedrequest[Socket];

            Parsersettings[Socket].on_message_begin = [](http_parser *parser) -> int
            {
                return Parse_Messagebegin(parser, (HTTPRequest *)parser->data);
            };
            Parsersettings[Socket].on_url = [](http_parser* parser, const char* at, size_t len) -> int
            {
                return Parse_URL(parser, (HTTPRequest *)parser->data, at, len);
            };
            Parsersettings[Socket].on_header_field = [](http_parser* parser, const char* at, size_t len) -> int
            {
                return Parse_Headerfield(parser, (HTTPRequest *)parser->data, at, len);
            };
            Parsersettings[Socket].on_header_value = [](http_parser* parser, const char* at, size_t len) -> int
            {
                return Parse_Headervalue(parser, (HTTPRequest *)parser->data, at, len);
            };
            Parsersettings[Socket].on_headers_complete = [](http_parser* parser) -> int
            {
                return Parse_Headerscomplete(parser, (HTTPRequest *)parser->data);
            };
            Parsersettings[Socket].on_body = [](http_parser* parser, const char* at, size_t len) -> int
            {
                return Parse_Body(parser, (HTTPRequest *)parser->data, at, len);
            };
            Parsersettings[Socket].on_message_complete = [](http_parser* parser) -> int
            {
                return Parse_Messagecomplete(parser, (HTTPRequest *)parser->data);
            };
        }

        // Clear any old data.
        if (Parsedrequest[Socket].Parsed)
        {
            Parsedrequest[Socket].Body.clear();
            Parsedrequest[Socket].Headers.clear();
            Parsedrequest[Socket].Method.clear();
            Parsedrequest[Socket].URL.clear();
            Parsedrequest[Socket].Parsed = false;
        }

        // Parse the incoming data.
        size_t Read = http_parser_execute(&Parser[Socket], &Parsersettings[Socket], (const char *)Stream.data(), Stream.size());
        Stream.erase(Stream.begin(), Stream.begin() + Read);

        // Forward to the callbacks if parsed.
        if (Parsedrequest[Socket].Parsed)
        {
            Threadguard.unlock();
            switch (Hash::FNV1a_32(Parsedrequest[Socket].Method.c_str()))
            {
                case Hash::FNV1a_32("GET"): onGET(Socket, Parsedrequest[Socket]); break;
                case Hash::FNV1a_32("PUT"): onPUT(Socket, Parsedrequest[Socket]); break;
                case Hash::FNV1a_32("POST"): onPOST(Socket, Parsedrequest[Socket]); break;
                case Hash::FNV1a_32("COPY"): onCOPY(Socket, Parsedrequest[Socket]); break;
                case Hash::FNV1a_32("DELETE"): onDELETE(Socket, Parsedrequest[Socket]); break;
            }
            Threadguard.lock();
        }
    }
};

#if __has_include(<openssl\ssl.h>)

struct IHTTPSServer : ISSLServer
{
    // HTTP parser information.
    std::unordered_map<size_t, http_parser> Parser;
    std::unordered_map<size_t, HTTPRequest> Parsedrequest;
    std::unordered_map<size_t, http_parser_settings> Parsersettings;

    // Callbacks on parsed data.
    virtual void onGET(const size_t Socket, HTTPRequest &Request) = 0;
    virtual void onPUT(const size_t Socket, HTTPRequest &Request) = 0;
    virtual void onPOST(const size_t Socket, HTTPRequest &Request) = 0;
    virtual void onCOPY(const size_t Socket, HTTPRequest &Request) = 0;
    virtual void onDELETE(const size_t Socket, HTTPRequest &Request) = 0;

    // Callback on incoming data.
    virtual void onStreamdecrypted(const size_t Socket, std::vector<uint8_t> &Stream)
    {
        // Initialize a parser for the socket if needed.
        if (Parser.find(Socket) == Parser.end())
        {
            http_parser_init(&Parser[Socket], HTTP_BOTH);
            http_parser_settings_init(&Parsersettings[Socket]);
            Parsedrequest[Socket].Parsed = false;
            Parser[Socket].data = &Parsedrequest[Socket];

            Parsersettings[Socket].on_message_begin = [](http_parser *parser) -> int
            {
                return Parse_Messagebegin(parser, (HTTPRequest *)parser->data);
            };
            Parsersettings[Socket].on_url = [](http_parser* parser, const char* at, size_t len) -> int
            {
                return Parse_URL(parser, (HTTPRequest *)parser->data, at, len);
            };
            Parsersettings[Socket].on_header_field = [](http_parser* parser, const char* at, size_t len) -> int
            {
                return Parse_Headerfield(parser, (HTTPRequest *)parser->data, at, len);
            };
            Parsersettings[Socket].on_header_value = [](http_parser* parser, const char* at, size_t len) -> int
            {
                return Parse_Headervalue(parser, (HTTPRequest *)parser->data, at, len);
            };
            Parsersettings[Socket].on_headers_complete = [](http_parser* parser) -> int
            {
                return Parse_Headerscomplete(parser, (HTTPRequest *)parser->data);
            };
            Parsersettings[Socket].on_body = [](http_parser* parser, const char* at, size_t len) -> int
            {
                return Parse_Body(parser, (HTTPRequest *)parser->data, at, len);
            };
            Parsersettings[Socket].on_message_complete = [](http_parser* parser) -> int
            {
                return Parse_Messagecomplete(parser, (HTTPRequest *)parser->data);
            };
        }

        // Clear any old data.
        if (Parsedrequest[Socket].Parsed)
        {
            Parsedrequest[Socket].Body.clear();
            Parsedrequest[Socket].Headers.clear();
            Parsedrequest[Socket].Method.clear();
            Parsedrequest[Socket].URL.clear();
            Parsedrequest[Socket].Parsed = false;
        }

        // Parse the incoming data.
        size_t Read = http_parser_execute(&Parser[Socket], &Parsersettings[Socket], (const char *)Stream.data(), Stream.size());
        Stream.erase(Stream.begin(), Stream.begin() + Read);

        // Forward to the callbacks if parsed.
        if (Parsedrequest[Socket].Parsed)
        {
            Threadguard.unlock();
            switch (Hash::FNV1a_32(Parsedrequest[Socket].Method.c_str()))
            {
                case Hash::FNV1a_32("GET"): onGET(Socket, Parsedrequest[Socket]); break;
                case Hash::FNV1a_32("PUT"): onPUT(Socket, Parsedrequest[Socket]); break;
                case Hash::FNV1a_32("POST"): onPOST(Socket, Parsedrequest[Socket]); break;
                case Hash::FNV1a_32("COPY"): onCOPY(Socket, Parsedrequest[Socket]); break;
                case Hash::FNV1a_32("DELETE"): onDELETE(Socket, Parsedrequest[Socket]); break;
            }
            Threadguard.lock();
        }
    }
};

#endif
