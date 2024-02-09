#pragma once

#include <algorithm>
#include <array>
#include <concepts>
#include <cstdio>
#include <iterator>

namespace loveka::components::utils {

template <typename T, size_t Size>
class circular_buffer;

template <typename T, typename ValueType = typename T::value_type>
struct circular_buffer_iterator {
public:
    using buffer_type = T;

    using value_type      = ValueType;
    using pointer         = value_type*;
    using const_pointer   = value_type const*;
    using reference       = value_type&;
    using const_reference = value_type const&;
    using size_type       = typename T::size_type;
    using difference_type = typename T::difference_type;

    circular_buffer_iterator(buffer_type* buf, size_type pos) : buf_{buf}, pos_{pos} {}

    inline reference
    operator*()
    {
        return (*buf_)[pos_];
    }

    inline const_reference
    operator*() const
    {
        return (*buf_)[pos_];
    }

    inline pointer
    operator->()
    {
        return &(operator*());
    }

    inline const_pointer
    operator->() const
    {
        return &(operator*());
    }

    inline bool
    operator==(circular_buffer_iterator const& rhs) const
    {
        return pos_ == rhs.pos_;
    }

    inline bool
    operator!=(circular_buffer_iterator const& rhs) const
    {
        return !(*this == rhs); // NO LINT
    }

    inline bool
    operator<(circular_buffer_iterator const& rhs) const
    {
        return pos_ < rhs.pos_;
    }

    inline bool
    operator<=(circular_buffer_iterator const& rhs) const
    {
        return pos_ <= rhs.pos_;
    }

    inline bool
    operator>(circular_buffer_iterator const& rhs) const
    {
        return pos_ > rhs.pos_;
    }

    inline bool
    operator>=(circular_buffer_iterator const& rhs) const
    {
        return pos_ >= rhs.pos_;
    }

    inline circular_buffer_iterator&
    operator++()
    {
        ++pos_;
        return *this;
    }

    inline const circular_buffer_iterator
    operator++(int)
    {
        circular_buffer_iterator tmp{*this};
        ++(*this);
        return tmp;
    }

    inline circular_buffer_iterator&
    operator--()
    {
        --pos_;
        return *this;
    }

    inline const circular_buffer_iterator
    operator--(int)
    {
        circular_buffer_iterator tmp{*this};
        --(*this);
        return tmp;
    }

    inline circular_buffer_iterator&
    operator+=(std::size_t n)
    {
        pos_ += n;
        return *this;
    }

    inline circular_buffer_iterator&
    operator+=(difference_type n)
    {
        pos_ += n;
        return *this;
    }

    inline circular_buffer_iterator
    operator+(difference_type n) const
    {
        circular_buffer_iterator tmp{*this};
        tmp += n;
        return tmp;
    }

    inline circular_buffer_iterator&
    operator-=(difference_type n)
    {
        return *this += -n;
    }

    inline circular_buffer_iterator
    operator-(difference_type n) const
    {
        circular_buffer_iterator tmp{*this};
        tmp -= n;
        return tmp;
    }

    inline difference_type
    operator-(circular_buffer_iterator const& rhs) const
    {
        return pos_ - rhs.pos_;
    }

private:
    buffer_type* buf_;
    size_type    pos_;
};

template <typename T, size_t Size>
struct circular_buffer {
public:
    using value_type      = T;
    using pointer         = value_type*;
    using const_pointer   = value_type const*;
    using reference       = value_type&;
    using const_reference = value_type const&;
    using r_value         = value_type&&;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using storage_type    = std::aligned_storage_t<sizeof(value_type), alignof(value_type)>;

    using iterator       = circular_buffer_iterator<circular_buffer>;
    using const_iterator = circular_buffer_iterator<circular_buffer const, value_type const>;

    using reverse_iterator = std::reverse_iterator<circular_buffer_iterator<circular_buffer>>;
    using const_reverse_iterator
        = std::reverse_iterator<circular_buffer_iterator<circular_buffer const, value_type const>>;

    circular_buffer() = default;

    inline size_type
    tail() noexcept
    {
        return tail_;
    }

    [[nodiscard]] inline constexpr size_type
    tail() const noexcept
    {
        return tail_;
    }

    inline size_type
    head() noexcept
    {
        return tail_ - size_;
    }

    [[nodiscard]] inline constexpr size_type
    head() const noexcept
    {
        return tail_ - size_;
    }

    [[nodiscard]] inline constexpr size_type
    size() const noexcept
    {
        return size_;
    }

    [[nodiscard]] inline constexpr size_type
    capacity() const noexcept
    {
        return Size;
    }

    [[nodiscard]] inline constexpr bool
    empty() const noexcept
    {
        return size_ == 0;
    }

    [[nodiscard]] inline constexpr bool
    full() const noexcept
    {
        return size_ >= Size;
    }

    inline void
    clear() noexcept
    {
        size_ = tail_ = 0;
    }

    inline reference
    front() noexcept
    {
        return data(head());
    }

    inline constexpr const_reference
    front() const noexcept
    {
        return data(head());
    }

    inline reference
    back() noexcept
    {
        return data(tail_ - 1);
    }

    inline constexpr const_reference
    back() const noexcept
    {
        return data(tail_ - 1);
    }

    inline reference
    operator[](size_t index) noexcept
    {
        return data(head() + index);
    }

    inline constexpr const_reference
    operator[](size_t index) const noexcept
    {
        return data(head() + index);
    }

    inline void
    push(const_reference item) noexcept
    {
        data(tail_++) = item;
        ++size_;
        size_ = std::min(size_, Size);
    }

    inline void
    push(r_value item) noexcept
    {
        data(tail_++) = item;
        ++size_;
        size_ = std::min(size_, Size);
    }

    inline void
    pop() noexcept
    {
        size_ -= 1 && !empty();
    }

    inline iterator
    begin() noexcept
    {
        return iterator{this, 0};
    }

    inline constexpr const_iterator
    begin() const noexcept
    {
        return const_iterator{this, 0};
    }

    inline iterator
    end() noexcept
    {
        return iterator{this, size_};
    }

    inline constexpr const_iterator
    end() const noexcept
    {
        return const_iterator{this, size_};
    }

    inline iterator
    rbegin() noexcept
    {
        return reverse_iterator{this, size_};
    }

    inline constexpr const_iterator
    rbegin() const noexcept
    {
        return const_reverse_iterator{this, size_};
    }

    inline iterator
    rend() noexcept
    {
        return reverse_iterator{this, 0};
    }

    inline constexpr const_iterator
    rend() const noexcept
    {
        return const_reverse_iterator{this, 0};
    }

    inline iterator
    mend() noexcept
    {
        return iterator{this, size_circle_end()};
    }

    inline constexpr const_iterator
    mend() const noexcept
    {
        return const_iterator{this, size_circle_end()};
    }

    inline value_type*
    storage() noexcept
    {
        return &(data(0));
    }
    inline constexpr size_type
    storage_size_in_bytes() const noexcept
    {
        return sizeof(data_);
    }

    void
    update_head(size_type head_next) noexcept
    {
        const size_type tail  = tail_ % Size;
        head_next             = head_next % Size;
        const std::size_t inc = (tail > head_next) ? (Size - tail + head_next) : (head_next - tail);
        size_ += inc;
        tail_ += inc;
        size_ = std::min(size_, Size);
    }

private:
    std::array<storage_type, Size> data_;
    size_type                      tail_{0};
    size_type                      size_{0};

    inline reference
    data(size_t index) noexcept
    {
        return *reinterpret_cast<pointer>(&data_[index % Size]);
    }

    inline constexpr const_reference
    data(size_t index) const noexcept
    {
        return *reinterpret_cast<const_pointer>(&data_[index % Size]);
    }

    [[nodiscard]] inline constexpr size_type
    size_circle_end() const noexcept
    {
        return std::min(size_, ((head() / Size) + 1) * Size - head());
    }

    [[nodiscard]] inline constexpr size_type
    circle_end() const noexcept
    {
        return (empty()) ? (tail_) : (head() + size_circle_end());
    }
};

template <typename T, size_t Size, std::ranges::common_range Array>
circular_buffer<T, Size>&
operator<<(circular_buffer<T, Size>& buffer, const Array& in_data)
{
    for (auto& item : in_data) {
        buffer.push(item);
    }
    return buffer;
}

template <typename T, size_t Size>
circular_buffer<T, Size>&
operator>>(circular_buffer<T, Size>& buffer, std::size_t size)
{
    const auto end{buffer.begin() + size};
    for (auto iter{buffer.begin()}; iter < end; ++iter) {
        buffer.pop();
    }
    return buffer;
}

template <typename T, size_t Size, std::ranges::common_range Array>
circular_buffer<T, Size>&
operator>>(circular_buffer<T, Size>& buffer, Array& out_data)
{
    if (buffer.size() < out_data.size())
        return buffer;
    for (auto& item : out_data) {
        item = buffer.front();
        buffer.pop();
    }
    return buffer;
}

template <typename T, size_t Size, std::ranges::common_range Array>
bool
operator==(const circular_buffer<T, Size>& buffer, const Array& in_data)
{
    bool res;
    auto iter{buffer.begin()};
    for (auto& item : in_data) {
        res = res & (item == (*(iter++)));
    }
    return res;
}

}    // namespace loveka::components::utils
