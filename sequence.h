#ifndef SEQUENCE_SEQUENCE_H
#define SEQUENCE_SEQUENCE_H

#include <cstddef>
#include <type_traits>
#include <iterator>

struct seq_traits {
    using type = char;
    static constexpr bool eq(type a, type b) noexcept {
        return a == b;
    }
    static constexpr bool lt(type a, type b) noexcept {
        return a < b;
    }
    static constexpr void assign(type &r, const type &a) {
        r = a;
    }
    static constexpr std::size_t length(const type *s) {
        size_t result = 0;
        for (const type* it = s; it; it++){
            result++;
        }
        return result;
    }
};

template<class T, class Traits>
struct sequence {
    using size_type = std::size_t;
    using const_iterator = const T*;
    using const_reverse_iterator = const std::reverse_iterator<const_iterator>;
    using const_reference = const T&;
    static constexpr size_type npos = size_type(-1);

private:
    const_iterator seq_begin, seq_end;
    size_type length;

public:
    constexpr sequence() noexcept {
        seq_begin = seq_end = nullptr;
        length = 0;
    }
    constexpr sequence(const sequence& original) noexcept {
        seq_begin = original.begin();
        length = original.length;
        seq_end = original.end();
    }
    constexpr sequence(const T* s, size_type count) {
        seq_begin = s;
        length = count;
        seq_end = s + length;
    }
    constexpr sequence(const T* s) {
        seq_begin = s;
        length = Traits::length(s);
        seq_end = s + length;
    }

    constexpr sequence& operator=(const sequence& original) noexcept {
        seq_begin = original.begin();
        seq_end = original.end();
        seq_begin = original.size();
        return *this;
    }

    constexpr const_iterator begin() const noexcept {
        return seq_begin;
    }
    constexpr const_iterator end() const noexcept {
        return seq_end;
    }
    constexpr const_reverse_iterator rbegin() const noexcept {
        return std::make_reverse_iterator(end());
    }
    constexpr const_reverse_iterator rend() const noexcept {
        return std::make_reverse_iterator(begin());
    }

    constexpr const_reference operator[](size_type pos) const {
        return *(seq_begin + pos);
    }
    constexpr const_reference at(size_type pos) const {
        if (pos >= length) {
            throw std::out_of_range("you cannot take the elements out of sequence");
        }
        return *(seq_begin + pos);
    }
    constexpr const_reference front() const {
        return *seq_begin;
    }
    constexpr const_reference back() const {
        return *(seq_end - 1);
    }

    constexpr size_type size() const noexcept {
        return length;
    }
    constexpr bool empty() const noexcept {
        return length == 0;
    }

    constexpr void remove_prefix(size_type n) {
        seq_begin += n;
        length -= n;
    }
    constexpr void remove_suffix(size_type n) {
        seq_end -= n;
        length -= n;
    }
    constexpr void swap(sequence& v) noexcept {
        std::swap(seq_begin, v.seq_begin);
        std::swap(seq_end, v.seq_end);
        std::swap(length, v.length);
    }

    //copies no more than count characters, return number of copyed elements
    size_type copy(T* dest, size_type count, size_type pos = 0) const {
        if (pos >= length) {
            throw std::out_of_range("you cannot copy elements out of sequence");
        }
        size_type i = 0;
        for (i = 0; i < count && pos + i < length; i++) {
            Traits::assign(dest[i], *(seq_begin + pos + i));
//            dest[i] = *(seq_begin + pos + i);
        }
        return i;
    }
    constexpr int compare(sequence v) const noexcept {
        auto it1 = begin(), it2 = v.begin();
        for (size_type i = 0; i < length && i < v.length; i++) {
            if (Traits::lt(*(it1 + i), *(it2 + i))) {
                return -1;
            } else if (Traits::lt(*(it2 + i), *(it1 + i))) {
                return 1;
            }
        }
        if (length > v.length) {
            return 1;
        } else if (length < v.length) {
            return -1;
        } else {
            return 0;
        }
    }
    constexpr sequence subseq(size_type pos = 0, size_type count = npos) const {
        if (pos >= length || pos + count - 1 > length) {
            throw std::out_of_range("you cannot create subseq out of sequence");
        }
        return sequence(seq_begin + pos, count);
    }

    constexpr size_type find(sequence v, size_type pos = 0) const noexcept {
        const_iterator it1 = begin(), it2 = v.begin();
        size_type len1 = size(), len2 = v.size();
        for (size_type i = pos; i < len1 - len2 + 1; i++) {
            bool f = true;
            for (size_type j = 0; j < len2; j++) {
                if (!Traits::eq(*(it1 + i + j), *(it2 + j))) {
                    f = false;
                }
                if (!f) break;
            }
            if (f) return i;
        }
        return npos;
    }
    constexpr size_type rfind(sequence v, size_type pos = npos) const noexcept {
        size_type k = npos;
        pos++;
        const_iterator it1 = begin(), it2 = v.begin();
        size_type len1 = size(), len2 = v.size();
        for (size_type i = pos; i < len1 - len2 + 1; i++) {
            bool f = true;
            for (size_type j = 0; j < len2; j++) {
                if (!Traits::eq(*(it1 + i + j), *(it2 + j))) {
                    f = false;
                }
                if (!f) break;
            }
            if (f) k = i;
        }
        return k;
    }

    template<class T2, class Traits2>
    friend constexpr bool operator==(sequence<T2, Traits2> lhs, sequence<T2,Traits2> rhs) noexcept;
    template<class T2, class Traits2>
    friend constexpr bool operator!=(sequence<T2, Traits2> lhs, sequence<T2,Traits2> rhs) noexcept;
    template<class T2, class Traits2>
    friend constexpr bool operator>(sequence<T2, Traits2> lhs, sequence<T2,Traits2> rhs) noexcept;
    template<class T2, class Traits2>
    friend constexpr bool operator<(sequence<T2, Traits2> lhs, sequence<T2,Traits2> rhs) noexcept;
    template<class T2, class Traits2>
    friend constexpr bool operator<=(sequence<T2, Traits2> lhs, sequence<T2,Traits2> rhs) noexcept;
    template<class T2, class Traits2>
    friend constexpr bool operator>=(sequence<T2, Traits2> lhs, sequence<T2,Traits2> rhs) noexcept;
};

template<class T2, class Traits2>
constexpr bool operator==(sequence<T2, Traits2> lhs, sequence<T2, Traits2> rhs) noexcept {
    auto it1 = lhs.begin(), it2 = rhs.begin();
    size_t i = 0;
    for (i = 0; i < lhs.length, i < rhs.length; i++) {
        if (!Traits2::eq(*(it1 + i), *(it2 + i))) {
            return false;
        }
    }
    return i == lhs.length && i == rhs.length;
}

template<class T2, class Traits2>
constexpr bool operator!=(sequence<T2, Traits2> lhs, sequence<T2, Traits2> rhs) noexcept {
    return !(lhs == rhs);
}

template<class T2, class Traits2>
constexpr bool operator>(sequence<T2, Traits2> lhs, sequence<T2, Traits2> rhs) noexcept {
    auto it1 = lhs.begin(), it2 = rhs.begin();
    size_t i = 0;
    for (i = 0; i < lhs.length, i < rhs.length; i++) {
        if (Traits2::lt(*(it1 + i), *(it2 + i))) {
            return false;
        }
    }
    return i < lhs.length && i == rhs.length;
}

template<class T2, class Traits2>
constexpr bool operator<(sequence<T2, Traits2> lhs, sequence<T2, Traits2> rhs) noexcept {
    auto it1 = lhs.begin(), it2 = rhs.begin();
    size_t i = 0;
    for (i = 0; i < lhs.length, i < rhs.length; i++) {
        if (!Traits2::lt(*(it1 + i), *(it2 + i))) {
            return false;
        }
    }
    return i == lhs.length && i < rhs.length;
}

template<class T2, class Traits2>
constexpr bool operator<=(sequence<T2, Traits2> lhs, sequence<T2, Traits2> rhs) noexcept {
    return !(lhs > rhs);
}

template<class T2, class Traits2>
constexpr bool operator>=(sequence<T2, Traits2> lhs, sequence<T2, Traits2> rhs) noexcept {
    return !(lhs < rhs);
}

namespace std {
    template<class T, class Traits>
    struct hash<sequence<T, Traits>> {
        std::size_t operator()(const sequence<T, Traits>& s) const noexcept {
            size_t h = 0;
            for (auto it = s.begin(); it != s.end(); it++) {
                h ^= hash(*it) * 32898 + 10000714;
            }
            return h;
        }
    };
}
#endif //SEQUENCE_SEQUENCE_H
