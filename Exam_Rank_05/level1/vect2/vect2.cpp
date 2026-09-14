#include "vect2.hpp"

vect2::vect2(int x, int y) { v[0] = x, v[1] = y; }
int& vect2::operator[](int i) { return v[i]; }
const int& vect2::operator[](int i) const { return v[i]; }
vect2& vect2::operator+=(const vect2& o) { v[0]+=o[0], v[1]+=o[1]; return *this; }
vect2& vect2::operator-=(const vect2& o) { v[0]-=o[0], v[1]-=o[1]; return *this; }
vect2 vect2::operator+(const vect2& o) const { return vect2(*this) += o; }
vect2 vect2::operator-(const vect2& o) const { return vect2(*this) -= o; }
vect2& vect2::operator*=(int s) { v[0]*=s, v[1]*=s; return *this; }
vect2 vect2::operator*(int s) const { return vect2(*this) *= s; }
vect2& vect2::operator++() { ++v[0], ++v[1]; return *this; }
vect2 vect2::operator++(int) { vect2 t(*this); ++(*this); return t; }
vect2& vect2::operator--() { --v[0], --v[1]; return *this; }
vect2 vect2::operator--(int) { vect2 t(*this); --(*this); return t; }
vect2 vect2::operator-() const { return vect2(-v[0], -v[1]); }
bool vect2::operator==(const vect2& o) const { return v[0]==o[0] && v[1]==o[1]; }
bool vect2::operator!=(const vect2& o) const { return !(*this == o); }
vect2 operator*(int s, const vect2& o) { return o * s; }
std::ostream& operator<<(std::ostream& os, const vect2& o) { return os << "{" << o[0] << ", " << o[1] << "}"; }