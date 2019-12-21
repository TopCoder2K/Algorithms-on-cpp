#ifndef BIGINTEGER_BIGINTEGER_H
#define BIGINTEGER_BIGINTEGER_H

#include <iostream>
#include <string>
#include <vector>



constexpr int32_t BASE = 10;

uint32_t max(uint32_t x, uint32_t y) {
    return x < y ? y : x;
}

class BigInteger {
public:
    BigInteger();
    BigInteger(int64_t x);
    BigInteger(const BigInteger& other) noexcept : digits_(other.digits_), negative_(other.negative_) {}
    BigInteger(BigInteger&& other) noexcept : digits_(std::move(other.digits_)), negative_(other.negative_) {}
    BigInteger& operator=(const BigInteger& other) & noexcept {
        digits_ = other.digits_;
        negative_ = other.negative_;
        return *this;
    }
    BigInteger& operator=(BigInteger&& other) & noexcept {
        if (this == &other) {
            return *this;
        }

        digits_ = std::move(other.digits_);
        negative_ = other.negative_;
        return *this;
    }
    ~BigInteger() = default;

    BigInteger& operator+=(const BigInteger& other) &;
    friend BigInteger operator+(const BigInteger& lhs, const BigInteger& rhs);
    BigInteger& operator-=(const BigInteger& other) &;
    friend BigInteger operator-(const BigInteger& lhs, const BigInteger& rhs);
    BigInteger& operator*=(const BigInteger& other) &;
    friend BigInteger operator*(const BigInteger& lhs, const BigInteger& rhs);
    BigInteger& operator/=(const BigInteger& other) &;
    friend BigInteger operator/(const BigInteger& lhs, const BigInteger& rhs);
    BigInteger& operator%=(const BigInteger& other) & noexcept;
    friend BigInteger operator%(const BigInteger& lhs, const BigInteger& rhs) noexcept;

    BigInteger operator-();
    BigInteger& operator++() &;
    BigInteger operator++(int32_t);
    BigInteger& operator--() &;
    BigInteger operator--(int32_t);

    friend bool operator<(const BigInteger& lhs, const BigInteger& rhs) noexcept;
    friend bool operator>(const BigInteger& lhs, const BigInteger& rhs) noexcept;
    friend bool operator<=(const BigInteger& lhs, const BigInteger& rhs) noexcept;
    friend bool operator>=(const BigInteger& lhs, const BigInteger& rhs) noexcept;
    friend bool operator==(const BigInteger& lhs, const BigInteger& rhs) noexcept;
    friend bool operator!=(const BigInteger& lhs, const BigInteger& rhs) noexcept;

    std::string toString() const;

    friend std::ostream& operator<<(std::ostream& out, const BigInteger& x);
    friend std::istream& operator>>(std::istream& in, BigInteger& x);

    explicit operator bool() const noexcept;

private:
    static BigInteger Karatsuba_mul_(const BigInteger& lhs, const BigInteger& rhs);
    static int32_t divide_on_parts_(BigInteger& part1, BigInteger& part2, const BigInteger& origin);
    static BigInteger make_shift_(const BigInteger& to_shift, int32_t shift);
    static void delete_leading_zeros(BigInteger& big_int);
    std::vector<char> digits_;   // Reversed order.
    bool negative_;
};

int32_t BigInteger::divide_on_parts_(BigInteger& part1, BigInteger& part2, const BigInteger& origin) {
    int32_t size = origin.digits_.size();
    bool odd = size % 2;
    int32_t part1_size = size / 2 + odd;
    part1.digits_.resize(part1_size);
    part2.digits_.resize(size / 2);
    int32_t i = 0;
    for (; i < part1_size; ++i) {
        part1.digits_[i] = origin.digits_[i];
    }
    for (; i < size; ++i) {
        part2.digits_[i - part1_size] = origin.digits_[i];
    }

    return part1_size;
}

BigInteger BigInteger::make_shift_(const BigInteger& to_shift, int32_t shift) {
    BigInteger shifted;
    uint32_t size = to_shift.digits_.size();
    shifted.digits_.resize(size + shift, 0);

    for (uint32_t i = 0; i < size; ++i) {
        shifted.digits_[shift + i] = to_shift.digits_[i];
    }

    return shifted;
}

void BigInteger::delete_leading_zeros(BigInteger& big_int) {
    while (big_int.digits_.size() > 1 && big_int.digits_.back() == 0) {
        big_int.digits_.pop_back();
    }
}

BigInteger BigInteger::Karatsuba_mul_(const BigInteger& lhs, const BigInteger& rhs) {
    if (lhs.digits_.size() < 2 && rhs.digits_.size() < 2) {
        int32_t lhs_ = lhs.digits_[0];
        int32_t rhs_ = rhs.digits_[0];
        BigInteger tmp(lhs_ * rhs_);
        return tmp;
    }

    // Make the sizes equal.
    BigInteger lhs_tmp = lhs;
    BigInteger rhs_tmp = rhs;
    while (lhs_tmp.digits_.size() < rhs_tmp.digits_.size()) {
        lhs_tmp.digits_.push_back(0);
    }
    while (lhs_tmp.digits_.size() > rhs_tmp.digits_.size()) {
        rhs_tmp.digits_.push_back(0);
    }

    // Divide on two parts.
    BigInteger lhs1;
    BigInteger lhs2;
    int32_t part1_size = divide_on_parts_(lhs1, lhs2, lhs_tmp);
    BigInteger rhs1;
    BigInteger rhs2;
    divide_on_parts_(rhs1, rhs2, rhs_tmp);

    // Delete leading zeros.
    delete_leading_zeros(lhs1);
    delete_leading_zeros(lhs2);
    delete_leading_zeros(rhs1);
    delete_leading_zeros(rhs2);

    // Get the necessary parts of the sum.
    BigInteger lhs_parts_sum = lhs1 + lhs2;
    BigInteger rhs_parts_sum = rhs1 + rhs2;

    BigInteger cross_parts_product = Karatsuba_mul_(lhs_parts_sum, rhs_parts_sum);
    BigInteger first_part_product = Karatsuba_mul_(lhs1, rhs1);
    BigInteger second_part_product = Karatsuba_mul_(lhs2, rhs2);

    cross_parts_product -= first_part_product;
    cross_parts_product -= second_part_product;

    // int32_t size = lhs_tmp.digits_.size();
    cross_parts_product = make_shift_(cross_parts_product, part1_size);
    second_part_product = make_shift_(second_part_product, 2 * part1_size);
    cross_parts_product += first_part_product;
    cross_parts_product += second_part_product;
    delete_leading_zeros(cross_parts_product);

    return cross_parts_product;
}


BigInteger::BigInteger() : BigInteger(0) {}
BigInteger::BigInteger(int64_t x) {
    negative_ = false;
    if (x < 0) {
        negative_ = true;
        x *= -1;
    }

    if (x == 0) {
        digits_.push_back(0);
    }

    while(x > 0) {
        digits_.push_back(x % 10);
        x /= 10;
    }
}

BigInteger& BigInteger::operator+=(const BigInteger& other) & {
    // Only not negative numbers will be summarised.
    if (negative_) {
        negative_ = false;
        if (other.negative_) {
            BigInteger tmp = other;
            tmp.negative_ = false;
            *this += tmp;
            negative_ = true;
            return *this;
        }

        BigInteger tmp = other;
        tmp -= *this;
        return *this = tmp;
    }
    if (other.negative_) {
        BigInteger tmp = other;
        tmp.negative_ = false;
        return *this -= tmp;
    }

    int32_t carryout = 0;
    for (uint32_t i = 0; i < max(digits_.size(), other.digits_.size()) || carryout; ++i) {
        if (i == digits_.size()) {
            digits_.push_back(0);
        }
        digits_[i] += carryout + (i < other.digits_.size() ? other.digits_[i] : 0);
        carryout = (digits_[i] >= BASE);
        if (carryout) {
            digits_[i] -= BASE;
        }
    }
    return *this;
}

BigInteger operator+(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger result = lhs;
    return result += rhs;
}

BigInteger& BigInteger::operator-=(const BigInteger& other) & {
    if (negative_) {
        negative_ = false;
        if (other.negative_) {
            BigInteger tmp = other;
            tmp.negative_ = false;
            tmp -= *this;
            return *this = tmp;
        }

        *this += other;
        negative_ = true;
        return *this;
    }
    if (other.negative_) {
        BigInteger tmp = other;
        tmp.negative_ = false;
        return *this += tmp;
    }

    if (*this < other) {
        BigInteger tmp = other;
        tmp -= *this;
        *this = tmp;
        negative_ = true;
        return *this;
    }

    int32_t carryout = 0;
    uint32_t size = other.digits_.size();
    for (uint32_t i = 0; i < size || carryout; ++i) {
        digits_[i] -= carryout + (i < size ? other.digits_[i] : 0);
        carryout = (digits_[i] < 0);
        if (carryout) {
            digits_[i] += BASE;
        }
    }

    // Delete all leading zeros.
    while (digits_.size() > 1 && digits_.back() == 0) {
        digits_.pop_back();
    }

    return *this;
}

BigInteger operator-(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger result = lhs;
    return result -= rhs;
}

BigInteger& BigInteger::operator*=(const BigInteger& other) & {
    bool negative;
    if (negative_ && other.negative_) {
        negative = false;
    } else {
        negative = negative_ || other.negative_;
    }

    BigInteger tmp = other;
    tmp.negative_= false;
    negative_ = false;

    *this = Karatsuba_mul_(*this, tmp);
    negative_ = negative;

    delete_leading_zeros(*this);

    return *this;
}

BigInteger operator*(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger tmp = lhs;
    return tmp *= rhs;
}

BigInteger& BigInteger::operator/=(const BigInteger& other) & {
    if (other == 0) {
        throw std::runtime_error("Division by zero.");
    }

    bool negative;
    if (negative_ && other.negative_) {
        negative = true;
    } else {
        negative = negative_ || other.negative_;
    }

    BigInteger tmp = other;
    tmp.negative_ = false;
    negative_ = false;

    if (*this < tmp) {
        *this = 0;
        return *this;
    }

    BigInteger shifted = make_shift_(tmp, digits_.size() - tmp.digits_.size());
    std::vector<char> result_reversed;
    while (shifted >= tmp) {
        int32_t cur_digit = 0;
        while (*this >= shifted) {
            *this -= shifted;
            ++cur_digit;
        }
        if (!(cur_digit == 0 && result_reversed.empty())) {
            result_reversed.push_back(cur_digit);
        }

        shifted.digits_.erase(shifted.digits_.begin());
    }

    BigInteger result;
    uint32_t size = result_reversed.size();
    result.digits_.resize(result_reversed.size());
    for (uint32_t i = 0; i < size; ++i) {
        result.digits_[i] = result_reversed[size - 1 - i];
    }
    result.negative_ = negative;

    *this = result;
    return *this;
}

BigInteger operator/(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger tmp = lhs;
    tmp /= rhs;
    return tmp;
}

BigInteger& BigInteger::operator%=(const BigInteger& other) & noexcept {
    BigInteger tmp = other;
    bool negative = negative_;
    if (tmp.negative_) {
        tmp.negative_ = false;
    }
    if (negative_) {
        negative_ = false;
    }

    *this = *this - (*this / tmp * tmp);
    negative_ = negative;

    return *this;
}

BigInteger operator%(const BigInteger& lhs, const BigInteger& rhs) noexcept {
    BigInteger tmp = lhs;
    tmp %= rhs;
    return tmp;
}


BigInteger BigInteger::operator-() {
    BigInteger tmp = *this;
    tmp.negative_ = !negative_;
    return tmp;
}

BigInteger& BigInteger::operator++()& {
    (*this) += 1;
    return *this;
}

BigInteger BigInteger::operator++(int32_t) {
    BigInteger tmp = *this;
    ++(*this);
    return tmp;
}

BigInteger& BigInteger::operator--() &{
    (*this) -= 1;
    return *this;
}

BigInteger BigInteger::operator--(int32_t) {
    BigInteger tmp = *this;
    --(*this);
    return tmp;
}


bool operator<(const BigInteger& lhs, const BigInteger& rhs) noexcept {
    // negative_ == 0 <==> *this >= 0.
    if (!lhs && !rhs) {
        return false;
    }
    // Sign comparison.
    if (lhs.negative_ && !rhs.negative_) {
        return true;
    }
    if (!lhs.negative_ && rhs.negative_) {
        return false;
    }

    // Size comparison.
    if (lhs.digits_.size() < rhs.digits_.size()) {
        return !lhs.negative_;
    }
    if (lhs.digits_.size() > rhs.digits_.size()) {
        return lhs.negative_;
    }

    // Digits comparison.
    uint32_t size = lhs.digits_.size();
    for (int32_t i = size - 1; i >= 0; --i) {
        if (lhs.digits_[i] < rhs.digits_[i]) {
            return !lhs.negative_;
        }
        if (lhs.digits_[i] > rhs.digits_[i]) {
            return lhs.negative_;
        }
    }

    return false;
}

bool operator>(const BigInteger& lhs, const BigInteger& rhs) noexcept {
    return rhs < lhs;
}

bool operator<=(const BigInteger& lhs, const BigInteger& rhs) noexcept {
    return !(rhs < lhs);
}

bool operator>=(const BigInteger& lhs, const BigInteger& rhs) noexcept {
    return !(lhs < rhs);
}

bool operator==(const BigInteger& lhs, const BigInteger& rhs) noexcept {
    return !(lhs < rhs) && !(rhs < lhs);
}

bool operator!=(const BigInteger& lhs, const BigInteger& rhs) noexcept {
    return !(lhs == rhs);
}



std::string BigInteger::toString() const {
    // -0 check:
    if (*this == 0) {
        return std::string("0");
    }

    int32_t size = digits_.size();
    std::string string(size + negative_, 0);
    string[0] = negative_ ? '-' : '0';
    for (int32_t i = size - 1; i >= 0; --i) {
        string[size - 1 + negative_ - i] = digits_[i] + 48;
    }

    return string;
}


std::ostream& operator<<(std::ostream& out, const BigInteger& x) {
    std::string string = x.toString();
    out << string;
    return out;
}
std::istream& operator>>(std::istream& in, BigInteger& x) {
    std::string string;
    in >> string;
    if (string.empty()) {
        x = BigInteger();
        return in;
    }

    x.digits_.clear();
    x.negative_ = (string[0] == '-');

    uint32_t size = string.size() - x.negative_;
    x.digits_.resize(size);
    for (uint32_t i = 0; i < size; ++i) {
        x.digits_[size - 1 - i] = string[i + x.negative_] - 48;
    }

    return in;
}


BigInteger::operator bool() const noexcept {
    return !(digits_.size() == 1 && digits_[0] == 0);
}

#endif //BIGINTEGER_BIGINTEGER_H
