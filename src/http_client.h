#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <vector>
#include <sstream>
#include <string.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <map>
using std::cout;
using std::endl;
using std::map;
using std::string;
using std::stringstream;
using std::vector;

constexpr int BUF_SIZE = 2048;
struct Result
{
    // http state code
    int code;
    // http header
    string header;
    // http response body
    string body;
};

class Client
{

private:
    bool use_proxy = false;
    bool enable_ssl = false;

    string ip;
    string host_name;
    int port;

    string proxy_ip;
    int proxy_port;

public:
    Client(const char *host)
    {
        host_name = host;
        ip = getIpByHostName(host)[0];
        cout << "ip is: " << ip << endl;
        port = 80;
    };

    Client(string server_ip, int server_port) : ip(server_ip), port(server_port){};

    vector<string> getIpByHostName(const char *name);

    void set_proxy(const string &ip, const int &port);

    // enable https
    void enable_https()
    {
        enable_ssl = true;
        port = 443;
    };

    Result do_get(string path, string cookie = "");

    Result do_post(string path, map<string, string> form, string cookie = "");

    Result parser(const string &msg);
};
// create socket
int socket_connect(const string &ip, const int &port)
{
    int sock;
    struct sockaddr_in serv_addr = {0};
    // ipv4 tcp类型socket
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1)
        cout << "socket fail" << endl;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        cout << "connect error" << endl;
    return sock;
}

// create proxy_sock
int proxy_connect(const string &proxy_ip, const int &proxy_port)
{
    auto sock = socket_connect(proxy_ip, proxy_port);
    if (sock == -1)
        cout << "set proxy failed" << endl;

    // // construct req
    // stringstream stream;
    // stream << "CONNECT "
    //        << "jinkela.cyou"
    //        << ":"
    //        << std::to_string(443);
    // stream << " HTTP/1.1\r\n";
    // stream << "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3\r\n";
    // stream << "Connection:Keep-Alive\r\n\r\n";
    // string req = stream.str();

    // // send req
    // write(sock, req.c_str(), req.size());
    cout << "connected proxy" << endl;
    // send req
    write(sock, req.c_str(), req.size());

    return sock;
}

// use ssl req
string ssl_request(const int &sock, const string &req)
{
    SSLeay_add_ssl_algorithms();
    SSL_load_error_strings();
    const SSL_METHOD *meth = TLS_client_method();
    SSL_CTX *ctx = SSL_CTX_new(meth);

    // create ssl
    int ret;
    SSL *ssl = SSL_new(ctx);
    if (ssl == nullptr)
    {
        cout << "SSL NEW error";
    }
    // socket ssl bind
    SSL_set_fd(ssl, sock);
    // ssl connect
    ret = SSL_connect(ssl);
    if (ret == -1)
    {
        cout << "SSL ACCEPT error ";
    }
    const char *sendData = req.c_str();
    ret = SSL_write(ssl, sendData, req.size());
    if (ret == -1)
    {
        cout << "SSL write error !";
    }
    char buf[1024 * 1024];
    int start = 0;
    while ((ret = SSL_read(ssl, buf + start, 1024)) > 0)
    {
        start += ret;
    }
    buf[start] = 0;
    return buf;

    // colse ssl
    SSL_shutdown(ssl);
    // free ssl
    SSL_free(ssl);
    // free ctx
    SSL_CTX_free(ctx);
    // close socket
    close(sock);
}
// use norml req
string request(const int &sock, const string &req)
{
    // send req
    write(sock, req.c_str(), req.size());

    // get response
    string msg;
    char buf[BUF_SIZE] = {0};
    auto offs = 0;
    while ((offs = read(sock, buf, BUF_SIZE - 1)) > 0)
    {
        msg += buf;
        memset(buf, 0, BUF_SIZE);
        offs = 0;
    }

    write(sock, req.c_str(), req.size());
    sleep(1);
    offs = read(sock, buf, BUF_SIZE - 1);

    close(sock);
    return msg;
}
// set proxy
void Client::set_proxy(const string &proxy_ip, const int &proxy_port)
{
    use_proxy = true;
    this->proxy_ip = proxy_ip;
    this->proxy_port = proxy_port;
}
// get ip by hostname
vector<string> Client::getIpByHostName(const char *name)
{
    vector<string> ips;
    struct hostent *host;
    host = gethostbyname(name);
    for (int i = 0; host->h_addr_list[i]; i++)
    {
        ips.push_back(inet_ntoa(*(struct in_addr *)host->h_addr_list[i]));
    }
    return ips;
}
// parse http response
Result Client::parser(const string &msg)
{
    Result res;

    //get stats code
    auto a = msg.find(" ", 0);
    auto b = msg.find(" ", ++a);
    res.code = atoi(msg.substr(a, (b - a)).c_str());

    //get header
    auto header = msg.find("\r\n", 0);
    header += 2;
    auto header_end = msg.find("\r\n\r\n", header);
    res.header = msg.substr(header, header_end - header);

    //get body
    auto body = msg.find("\r\n\r\n", 0);
    body += 4;
    body = msg.find("\r\n", body);
    body += 2;
    res.body = msg.substr(body, msg.size() - body);
    return res;
}
// http get
Result Client::do_get(string path, string cookie)
{

    // construct req
    stringstream stream;
    if (enable_ssl || use_proxy)
    {
        stream << "GET "
               << "https://" // if use https,should append host name
               << host_name
               << "/"
               << path.data();
        //    << "?";
    }
    else
    {
        stream << "GET "
               << "/"
               << path.data()
               << "?";
    }
    stream << " HTTP/1.1\r\n";
    stream << "Accept: */*\r\n";
    stream << "Host: " << ip << ":" << "443" << "\r\n";
    stream << "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.45 Safari/537.36 Edg/96.0.1054.29\r\n";
    if (cookie.size() != 0)
        stream << "Cookie：" << cookie << "\r\n";
    stream << "Connection: keep-alive\r\n\r\n";
    stream << "User-Agent: PostmanRuntime/7.28.4\r\n";
    stream << "Connection: close\r\n\r\n";

    cout << stream.str();

    // socket
    int sock = -1;

    // if use proxy
    if (use_proxy)
        sock = proxy_connect(proxy_ip,proxy_port);
    else
        sock = socket_connect(ip, port);

    // response
    string res;
    if (enable_ssl && !use_proxy)
        res = ssl_request(sock, stream.str());
    else
        res = request(sock, stream.str());
    cout << res << endl;
    return parser(res);
}

// http post
Result Client::do_post(string path, map<string, string> form, string cookie)
{
    // parse form
    string formData;
    for (auto iter = form.begin(); iter != form.end(); iter++)
    {
        if (iter != form.begin())
            formData += "&";
        formData += iter->first + "=" + iter->second;
    }

    // construct req
    stringstream stream;
    if (enable_ssl || use_proxy)
    {
        stream << "POST "
               << "https://" // if use https,should append host name
               << host_name
               << "/"
               << path.data()
               << "?";
    }
    else
    {
        stream << "POST "
               << "/"
               << path.data()
               << "?";
    }
    stream << " HTTP/1.1\r\n";
    stream << "Accept: */*\r\n";
    stream << "Host: " << host_name << "\r\n";
    stream << "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3\r\n";
    if (cookie.size() != 0)
        stream << "cookie：" << cookie << "\r\n";
    stream << "Connection: Close\r\n";
    stream << "Content-Type: application/x-www-form-urlencoded\r\n";
    stream << "Content-Length: ";
    stream << formData.size() << "\n\n";
    stream << formData;

    // socket
    int sock = -1;

    // if use proxy
    if (use_proxy)
        sock = proxy_connect(proxy_ip,proxy_port);
    else
        sock = socket_connect(ip, port);

    // response
    string res;
    if (enable_ssl && !use_proxy)
        res = ssl_request(sock, stream.str());
    else
        res = request(sock, stream.str());
    return parser(res);
}
