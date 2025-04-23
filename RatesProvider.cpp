#include "RatesProvider.hpp"
#include <fstream>
#include <sstream>

#include <curl/curl.h>
#include <json/json.h>

RatesProvider::RatesProvider(std::string base_rate_name) : base_rate_name(base_rate_name) {
    update();
}

RatesProvider::RatesProvider(RatesProvider&& other)
    : base_rate_name(other.base_rate_name),
    _rates(std::move(other._rates)) {}

void RatesProvider::update() {
    return _update();
}

const std::map<std::string, double>& RatesProvider::rates() const {
    return _rates;
}

size_t RatesProvider::_curl_callback(void *content, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)content, size * nmemb);
    return size * nmemb;
}

std::string RatesProvider::get_json_from_api() {
    std::ifstream in("./er-api.json");

    std::stringstream result;

    result << in.rdbuf();

    return result.str();

    CURL* curl;
    CURLcode res;
    std::string buf;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(
            curl,
            CURLOPT_URL,
            // (std::string("https://open.er-api.com/v6/latest/") + base_rate_name).c_str()
            "https://jsonplaceholder.typicode.com/todos/"
        );
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _curl_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return buf;
}

void RatesProvider::_update() {
    Json::Value root;
    Json::Reader reader;

    reader.parse(get_json_from_api(), root);

    for (auto name : root["rates"].getMemberNames()) {
        _rates[name] = root["rates"][name].asDouble();
    }
}
std::map<std::string, double> _rates;
