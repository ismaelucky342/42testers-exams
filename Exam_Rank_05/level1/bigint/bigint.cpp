#include "bigint.hpp"
#include <sstream>
#include <algorithm>
#include <cstdlib>

void bigint::_trim() {
    size_t i = 0;
    while (i + 1 < _val.length() && _val[i] == '0') i++;
    _val = _val.substr(i);
}

bigint::bigint() : _val("0") {}
bigint::bigint(unsigned int n) { std::stringstream ss; ss << n; _val = ss.str(); }
bigint::bigint(std::string str) : _val(str) { _trim(); }

bigint bigint::operator+(const bigint& other) const {
    std::string res;
    int carry = 0, i = _val.length() - 1, j = other._val.length() - 1;
    while (i >= 0 || j >= 0 || carry) {
        int sum = carry + (i >= 0 ? _val[i--] - '0' : 0) + (j >= 0 ? other._val[j--] - '0' : 0);
        res += sum % 10 + '0'; carry = sum / 10;
    }
    std::reverse(res.begin(), res.end());
    return bigint(res);
}
bigint& bigint::operator+=(const bigint& other) { return *this = *this + other; }

bigint bigint::operator<<(unsigned int n) const {
    return _val == "0" ? *this : bigint(_val + std::string(n, '0'));
}
bigint bigint::operator>>(unsigned int n) const {
    return n >= _val.length() ? bigint("0") : bigint(_val.substr(0, _val.length() - n));
}
bigint& bigint::operator<<=(unsigned int n) { return *this = *this << n; }
bigint& bigint::operator>>=(unsigned int n) { return *this = *this >> n; }
int bigint::toInt() const { return std::atoi(_val.c_str()); }
bigint bigint::operator<<(const bigint& other) const { return *this << other.toInt(); }
bigint bigint::operator>>(const bigint& other) const { return *this >> other.toInt(); }
bigint& bigint::operator<<=(const bigint& other) { return *this = *this << other.toInt(); }
bigint& bigint::operator>>=(const bigint& other) { return *this = *this >> other.toInt(); }
bool bigint::operator==(const bigint& other) const { return _val == other._val; }
bool bigint::operator<(const bigint& other) const {
    return _val.length() != other._val.length() ? _val.length() < other._val.length() : _val < other._val;
}
bool bigint::operator!=(const bigint& other) const { return !(*this == other); }
bool bigint::operator>(const bigint& other) const { return other < *this; }
bool bigint::operator<=(const bigint& other) const { return !(other < *this); }
bool bigint::operator>=(const bigint& other) const { return !(*this < other); }
bigint& bigint::operator++() { return *this += bigint(1); }
bigint bigint::operator++(int) { bigint tmp(*this); ++*this; return tmp; }
std::string bigint::getVal() const { return _val; }
std::ostream& operator<<(std::ostream& os, const bigint& obj) { return os << obj.getVal(); }