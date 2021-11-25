#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <iostream>
#include <string>
#include "httplib.h"
#include "rapidjson/document.h"

using std::string;

string getCookie(const httplib::Headers &h)
{
    string res;
    auto it1 = h.lower_bound("set-cookie");
    auto it2 = h.upper_bound("set-cookie");
    while (it1 != it2)
    {
        auto s = it1->second;
        auto pos = s.find("path=/");
        res += s.substr(0, pos);
        it1++;
    }
    return res;
}

string getMsg(const string &s)
{
    rapidjson::Document doc;
    doc.Parse(s.c_str());

    return doc["msg"].GetString();
}

int main(int, char *args[])
{
    if (args[1] == nullptr || args[2] == nullptr)
    {
        std::cout << "参数不对" << std::endl;
        return 0;
    }

    httplib::Client cli("https://jinkela.cyou");
    cli.enable_server_certificate_verification(false);

    //登录获取cookie
    httplib::Headers h = {
        {"accept","charset=UTF-8"}};
    httplib::MultipartFormDataItems items = {
        {"email", args[1], "", ""},
        {"passwd", args[2], "", ""}};
    auto login = cli.Post("/auth/login", h, items);
    auto cookie = getCookie(login->headers);
    std::cout << getMsg(login->body) << std::endl;

    //签到
    httplib::Headers headers = {
        {"cookie", cookie},{"accept","charset=UTF-8"}};
    auto checkin = cli.Post("/user/checkin", headers, "", "");
    std::cout << getMsg(checkin->body) << std::endl;
    return 0;
}
