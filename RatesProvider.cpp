#include "RatesProvider.hpp"
#include "fixed_rates_23_05_2025.hpp"

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

void RatesProvider::_update() {
    _rates = fixed_rates;
}
std::map<std::string, double> _rates;
