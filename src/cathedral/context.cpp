#include "cathedral.hpp"
using namespace ct;
/** status_code_message returns the appropriate status code message. 
 * @param code a http status code.
 * @return A std::string of the http status text.
 * Ex: 200 -> OK
 * @todo add remaining status codes https://developer.mozilla.org/en-US/docs/Web/HTTP/Status
**/
std::string Context::status_code_message(const int &code)
{
    switch (code)
    {
    case 100:
        return "Continue";
        break;
    case 101:
        return "Switching Protocols";
        break;
    case 102:
        return "102 Processing";
        break;
    case 103:
        return "103 Early Hints";

    case 200:
        return "OK";
        break;
    default:
        return "";
        break;
    }
}

void Context::send_string(int code, std::string body)
{
    this->rsp.status = std::string{(char)code};
    this->rsp.status_msg = Context::status_code_message(code);
    this->rsp.body.append(body);
}

void Context::header(std::string key, std::string value)
{
    this->headers[key] = value;
}

void Context::from(std::string request)
{
    bool reached_body = false;
    auto lines = split(request, '\n');
    for (size_t i = 0; i < lines.size(); i++)
    {
        auto words = split(lines[i], ' ');
        for (size_t j = 0; j < words.size(); j++)
        {
            if (!lines[i].empty())
            {
                if (!reached_body && lines[i] == "\r")
                {
                    reached_body = true;
                }

                if (!reached_body)
                {
                    if (i == 0)
                    {
                        this->method = words[0];
                        this->route = words[1];
                        this->protocol = words[2];
                    }
                    else
                    {
                        if (words.size() == 2)
                        {
                            this->header(words[0], words[1]);
                        }
                    }
                    continue;
                }
                this->body.push_back(lines);
            }
        }
    }
}

std::string Context::response()
{
    this->rsp.header.append("HTTP/1.1").append(" ").append(this->rsp.status).append(this->rsp.status_msg).append(this->rsp.nl);
    for (const auto &[key, val] : this->headers)
    {
        std::string hdr;
        hdr.append(key).append(": ").append(val).append("\r\n");
        this->rsp.header.append(hdr);
    }
    std::string response;
    response.append(this->rsp.header).append("\r\n\r\n");
    response.append(this->rsp.body);
    std::cout << response << std::endl;
    return response;
}