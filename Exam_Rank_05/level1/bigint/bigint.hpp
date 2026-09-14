#ifndef BIGINT_HPP
# define BIGINT_HPP

# include <iostream>
# include <string>


class bigint
{
    private:
        std::string _val;
        void _trim();

    public:
        bigint();
        bigint(unsigned int n);
        bigint(std::string str);

        bigint operator+(const bigint &other) const;
        bigint& operator+=(const bigint &other);

        bigint operator<<(unsigned int n) const;
        bigint operator>>(unsigned int n) const;
        bigint& operator<<=(unsigned int n);
        bigint& operator>>=(unsigned int n);

        // reuse
        int toInt() const;
        bigint operator<<(const bigint &other) const;
        bigint operator>>(const bigint &other) const;
        bigint& operator<<=(const bigint &other);
        bigint& operator>>=(const bigint &other);

        bool operator==(const bigint &other) const;
        bool operator<(const bigint &other) const;
        // the rest derives from those two
        bool operator!=(const bigint &other) const;
        bool operator>(const bigint &other) const;
        bool operator<=(const bigint &other) const;
        bool operator>=(const bigint &other) const;

        bigint& operator++();
        bigint operator++(int);

        std::string getVal() const;

};

std::ostream& operator<<(std::ostream& os, const bigint& obj);

#endif
