#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <iostream>
#include <locale>
#include <codecvt>
#include <string>
#include <httplib.h>

using std::string;

std::string unicodeToString(const std::string &str)
{
    std::string u8str;
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    for (size_t i = 0; i < str.length();)
    {
        char32_t uhan = strtol(str.substr(i, 4).c_str(), nullptr, 16);
        u8str += converter.to_bytes(uhan);

        i += 4;
    }
    return u8str;
}

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
    string u_res, res;
    string msg = "\"msg\":\"";
    auto i = s.find(msg, 0);
    i += msg.size();
    auto j = s.find("\"", i);
    u_res = s.substr(i, j - i);
    i = 0;
    while ((i = u_res.find("\\u", i)) != string::npos)
    {
        i += 2;
        auto temp = u_res.substr(i, 4);
        res += temp;
    }
    return unicodeToString(res);
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
    httplib::MultipartFormDataItems items = {
        {"email", args[1], "", ""},
        {"passwd", args[2], "", ""}};
    auto login = cli.Post("/auth/login", items);
    auto cookie = getCookie(login->headers);
    std::cout << getMsg(login->body) << std::endl;

    //签到
    httplib::Headers headers = {
        {"cookie", cookie}};
    auto checkin = cli.Post("/user/checkin", headers,"","");
    std::cout << getMsg(checkin->body) << std::endl;
    return 0;
}
