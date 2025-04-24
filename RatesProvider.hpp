#include <string>
#include <map>

class RatesProvider {
public:
    std::string base_rate_name;

    RatesProvider(std::string base_rate_name);

    RatesProvider(RatesProvider&& other);

    void update();

    const std::map<std::string, double>& rates() const;

private:
    static size_t _curl_callback(void *content, size_t size, size_t nmemb, void* userp);

    void _update();
    std::map<std::string, double> _rates;
};
