#pragma once
#include <vector>
#include <iostream>


namespace task {

    std::vector<double> operator+ (const std::vector<double> &a, const std::vector<double> &b) {
        std::vector<double> c;
        for (auto i = 0; i < a.size(); ++i){
            c.push_back(a[i] + b[i]);
        }
        return c;
    }

    std::vector<double> operator- (const std::vector<double> &a, const std::vector<double> &b) {
        std::vector<double> c;
        for (auto i = 0; i < a.size(); ++i){
            c.push_back(a[i] - b[i]);
        }
        return c;
    }

    std::vector<double> operator+ (const std::vector<double> &a) {
        return a;
    }

    std::vector<double> operator- (const std::vector<double> &a) {
        std::vector<double> b;
        for (auto i = 0; i < a.size(); ++i){
            b.push_back(-a[i]);
        }
        return b;
    }

    double operator* (const std::vector<double> &a, const std::vector<double> &b) {
        double sum = 0;
        for (auto i = 0; i < a.size(); ++i){
            sum += a[i] * b[i];
        }
        return sum;
    }

    std::vector<double> operator% (const std::vector<double> &a, const std::vector<double> &b) {
        std::vector<double> c;
        c.push_back(a[1] * b[2] - a[2] * b[1]);
        c.push_back(a[2] * b[0] - a[0] * b[2]);
        c.push_back(a[0] * b[1] - a[1] * b[0]);
        return c;
    }

    std::ostream &operator<< (std::ostream &os, const std::vector<double> &a) {
        for (auto i = 0; i < a.size(); ++i) {
            os << a[i] << " ";
        }
        os << '\n';
        return os;
    }

    std::istream &operator>> (std::istream &is, std::vector<double> &a) {
        size_t n = 0;
        is >> n;
        a.resize(n);
        double x;
        for (auto i = 0; i < n; ++i){
            is >> x;
            a[i] = x;
        }
        return is;
    }

    std::vector<double> operator* (const std::vector<double> &a, const double k) {
        std::vector<double> c;
        for (auto i = 0; i < a.size(); ++i){
            c.push_back(a[i] * k);
        }
        return c;
    }

    bool operator== (const std::vector<double> &a, const std::vector<double> &b) {
        const double EPS = 1e-10;
        for (auto i = 0; i < a.size(); ++i) {
            if ((a[i] - b[i] > EPS) || (a[i] - b[i] < -EPS)) {
                return false;
            }
        }
        return true;
    }

    bool operator|| (const std::vector<double> &a, const std::vector<double> &b) {
        double n = a[0] / b[0];
        return a == (b * n);
    }

    bool operator&& (const std::vector<double> &a, const std::vector<double> &b) {
        return (a * b > 0) && (a || b);
    }

    void reverse (std::vector<double> &a) {
        for (auto i = 0; i < a.size() / 2; ++i) {
            std::swap(a[i], a[a.size() - 1 - i]);
        }
    }


    std::vector<int> operator| (const std::vector<int> &a, const std::vector<int> &b) {
        std::vector<int> c;
        for (auto i = 0; i < a.size(); ++i){
            c.push_back(a[i] | b[i]);
        }
        return c;
    }

    std::vector<int> operator& (const std::vector<int> &a, const std::vector<int> &b) {
        std::vector<int> c;
        for (auto i = 0; i < a.size(); ++i){
            c.push_back(a[i] & b[i]);
        }
        return c;
    }
}
