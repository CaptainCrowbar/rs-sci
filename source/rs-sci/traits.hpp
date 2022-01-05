#pragma once

#include <iterator>
#include <type_traits>

namespace RS::Sci {

    template <typename Iterator>
    struct Irange {
        Iterator first, second;
        Iterator begin() const { return first; }
        Iterator end() const { return second; }
        bool empty() const noexcept { return first == second; }
    };

    template <typename Iterator>
    Irange<Iterator> irange(Iterator i, Iterator j) {
        return {i, j};
    }

    template <typename T>
    struct TotalOrder {
        friend bool operator!=(const T& a, const T& b) noexcept { return ! (a == b); }
        friend bool operator>(const T& a, const T& b) noexcept { return b < a; }
        friend bool operator<=(const T& a, const T& b) noexcept { return ! (b < a); }
        friend bool operator>=(const T& a, const T& b) noexcept { return ! (a < b); }
    };

    // Mixin                  Requires         Defines
    // =====                  ========         =======
    // InputIterator          *i ++i i==j      i-> i++ i!=j
    // OutputIterator         i=x              *i ++i i++
    // ForwardIterator        *i ++i i==j      i-> i++ i!=j
    // BidirectionalIterator  *i ++i --i i==j  i-> i++ i-- i!=j
    // RandomAccessIterator   *i i+=n i-j      i-> i[n] ++i i++ --i i-- i-=n i+n n+i i-n i==j i!=j i<j i>j i<=j i>=j

    template <typename T, typename CV>
    struct InputIterator {
        using difference_type = ptrdiff_t;
        using iterator_category = std::input_iterator_tag;
        using pointer = CV*;
        using reference = CV&;
        using value_type = std::remove_const_t<CV>;
        CV* operator->() const noexcept { return &*static_cast<const T&>(*this); }
        friend T operator++(T& t, int) { T rc = t; ++t; return rc; }
        friend bool operator!=(const T& a, const T& b) noexcept { return ! (a == b); }
    };

    template <typename T>
    struct OutputIterator {
        using difference_type = void;
        using iterator_category = std::output_iterator_tag;
        using pointer = void;
        using reference = void;
        using value_type = void;
        T& operator*() noexcept { return static_cast<T&>(*this); }
        friend T& operator++(T& t) noexcept { return t; }
        friend T operator++(T& t, int) noexcept { return t; }
    };

    template <typename T, typename CV>
    struct ForwardIterator:
    InputIterator<T, CV> {
        using iterator_category = std::forward_iterator_tag;
    };

    template <typename T, typename CV>
    struct BidirectionalIterator:
    ForwardIterator<T, CV> {
        using iterator_category = std::bidirectional_iterator_tag;
        friend T operator--(T& t, int) { T rc = t; --t; return rc; }
    };

    template <typename T, typename CV>
    struct RandomAccessIterator:
    BidirectionalIterator<T, CV>,
    TotalOrder<T> {
        using iterator_category = std::random_access_iterator_tag;
        CV& operator[](ptrdiff_t i) const noexcept { T t = static_cast<const T&>(*this); t += i; return *t; }
        friend T& operator++(T& t) { return t += 1; }
        friend T& operator--(T& t) { return t += -1; }
        friend T& operator-=(T& a, ptrdiff_t b) { return a += - b; }
        friend T operator+(const T& a, ptrdiff_t b) { T t = a; return t += b; }
        friend T operator+(ptrdiff_t a, const T& b) { T t = b; return t += a; }
        friend T operator-(const T& a, ptrdiff_t b) { T t = a; return t -= b; }
        friend bool operator==(const T& a, const T& b) noexcept { return a - b == 0; }
        friend bool operator<(const T& a, const T& b) noexcept { return a - b < 0; }
    };

}
