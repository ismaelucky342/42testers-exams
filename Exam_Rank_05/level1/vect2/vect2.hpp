#ifndef VECT2_HPP
# define VECT2_HPP


# include <iostream>



class vect2{

    private:
        int v[2];

    public:
        vect2(int x = 0, int y = 0);

        int& operator[](int index);
        const int& operator[](int index) const;

        vect2& operator+=(const vect2 &other);
        vect2& operator-=(const vect2 &other);

        vect2 operator+(const vect2 &other) const;
        vect2 operator-(const vect2 &other) const;

        vect2& operator*=(int scalar);
        vect2 operator*(int scalar) const;

        vect2& operator++();
        vect2 operator++(int);
        vect2& operator--();
        vect2 operator--(int);

        vect2 operator-() const;

        bool operator==(const vect2& other) const;
        bool operator!=(const vect2& other) const;

};

vect2 operator*(int scalar, const vect2& other);

std::ostream& operator<<(std::ostream& os, const vect2& other);


#endif