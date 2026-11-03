/**
 * Mango Vector
 **/

#pragma once

#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include <memory>
#include <iterator>
#include <compare>
#include <utility>
#include <initializer_list>
#include <type_traits>
#include <concepts>

#if __cplusplus >= 202002L
#define MANGO_VECTOR_CONSTEXPR constexpr
#else
#define MANGO_VECTOR_CONSTEXPR
#endif

template <typename T>
class MangoVector
{
public:
    // 类型定义
    using value_type = T;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static constexpr size_type npos = static_cast<size_type>(-1);

private:
    // 存储结构
    struct Storage {
        T* data;
        size_type size;
        size_type capacity;

        MANGO_VECTOR_CONSTEXPR Storage() noexcept : data(nullptr), size(0), capacity(0) {}
    } storage_;

    // 内存分配和释放
    void allocate(size_type new_capacity) {
        if (new_capacity == 0) {
            if (storage_.data) {
                delete[] storage_.data;
                storage_.data = nullptr;
            }
            storage_.capacity = 0;
            return;
        }

        T* new_data = new T[new_capacity];
        storage_.data = new_data;
        storage_.capacity = new_capacity;
    }

    void deallocate() {
        if (storage_.data) {
            destroy_elements(storage_.data, storage_.data + storage_.size);
            delete[] storage_.data;
            storage_.data = nullptr;
        }
        storage_.size = 0;
        storage_.capacity = 0;
    }

    // 复制元素
    template<typename InputIt>
    void copy_elements(InputIt first, InputIt last, T* dest) {
        for (; first != last; ++first, ++dest) {
            *dest = *first;
        }
    }

    // 移动元素
    template<typename InputIt>
    void move_elements(InputIt first, InputIt last, T* dest) {
        for (; first != last; ++first, ++dest) {
            *dest = std::move(*first);
        }
    }

    // 构造元素
    template<typename... Args>
    void emplace_construct(T* pos, Args&&... args) {
        new (pos) T(std::forward<Args>(args)...);
    }

    // 销毁元素
    void destroy_elements(T* first, T* last) {
        for (; first != last; ++first) {
            first->~T();
        }
    }

    // 统一构造实现
    MANGO_VECTOR_CONSTEXPR void construct_from_initializer_list(std::initializer_list<T> ilist) {
        if (ilist.size() > 0) {
            reserve(ilist.size());
            for (const auto& elem : ilist) {
                push_back(elem);
            }
        }
    }

    MANGO_VECTOR_CONSTEXPR void construct_from_vector(const MangoVector& other) {
        if (!other.empty()) {
            reserve(other.size());
            for (const auto& elem : other) {
                push_back(elem);
            }
        }
    }

    template<typename InputIt>
    void construct_from_range(InputIt first, InputIt last) {
        for (; first != last; ++first) {
            push_back(*first);
        }
    }

    MANGO_VECTOR_CONSTEXPR void construct_from_fill(size_type count, const T& value) {
        if (count > 0) {
            reserve(count);
            for (size_type i = 0; i < count; ++i) {
                push_back(value);
            }
        }
    }

public:
    // 构造函数
    MANGO_VECTOR_CONSTEXPR MangoVector() noexcept = default;

    // 拷贝构造函数
    MANGO_VECTOR_CONSTEXPR MangoVector(const MangoVector& other) {
        construct_from_vector(other);
    }

    // 移动构造函数
    MANGO_VECTOR_CONSTEXPR MangoVector(MangoVector&& other) noexcept
        : storage_(other.storage_) {
        other.storage_.data = nullptr;
        other.storage_.size = 0;
        other.storage_.capacity = 0;
    }

    // 初始化列表构造函数
    MANGO_VECTOR_CONSTEXPR MangoVector(std::initializer_list<T> ilist) {
        construct_from_initializer_list(ilist);
    }

    // 填充构造函数
    MANGO_VECTOR_CONSTEXPR MangoVector(size_type count, const T& value = T()) {
        construct_from_fill(count, value);
    }

    // 迭代器范围构造函数
    template<typename InputIt>
        requires std::input_iterator<InputIt>
    MangoVector(InputIt first, InputIt last) {
        construct_from_range(first, last);
    }

    // 析构函数
    ~MangoVector() {
        deallocate();
    }

    // 拷贝赋值操作符
    MANGO_VECTOR_CONSTEXPR MangoVector& operator=(const MangoVector& other) {
        if (this != &other) {
            clear();
            reserve(other.size());
            for (const auto& elem : other) {
                push_back(elem);
            }
        }
        return *this;
    }

    // 移动赋值操作符
    MANGO_VECTOR_CONSTEXPR MangoVector& operator=(MangoVector&& other) noexcept {
        if (this != &other) {
            deallocate();
            storage_ = other.storage_;
            other.storage_.data = nullptr;
            other.storage_.size = 0;
            other.storage_.capacity = 0;
        }
        return *this;
    }

    // 初始化列表赋值操作符
    MANGO_VECTOR_CONSTEXPR MangoVector& operator=(std::initializer_list<T> ilist) {
        clear();
        construct_from_initializer_list(ilist);
        return *this;
    }

    // 容量相关
    MANGO_VECTOR_CONSTEXPR size_type size() const noexcept { return storage_.size; }
    MANGO_VECTOR_CONSTEXPR size_type capacity() const noexcept { return storage_.capacity; }
    MANGO_VECTOR_CONSTEXPR bool empty() const noexcept { return storage_.size == 0; }
    MANGO_VECTOR_CONSTEXPR size_type max_size() const noexcept { return std::numeric_limits<size_type>::max() / sizeof(T); }

    void reserve(size_type new_capacity) {
        if (new_capacity > capacity()) {
            T* new_data = new T[new_capacity];
            for (size_type i = 0; i < storage_.size; ++i) {
                new_data[i] = std::move(storage_.data[i]);
            }
            delete[] storage_.data;
            storage_.data = new_data;
            storage_.capacity = new_capacity;
        }
    }

    void shrink_to_fit() {
        if (storage_.size < storage_.capacity) {
            if (storage_.size == 0) {
                deallocate();
            } else {
                T* new_data = new T[storage_.size];
                for (size_type i = 0; i < storage_.size; ++i) {
                    new_data[i] = std::move(storage_.data[i]);
                }
                delete[] storage_.data;
                storage_.data = new_data;
                storage_.capacity = storage_.size;
            }
        }
    }

    // 元素访问
    MANGO_VECTOR_CONSTEXPR reference operator[](size_type pos) noexcept {
        // 添加边界检查，防止访问越界
        if (pos < storage_.size) {
            return storage_.data[pos];
        }
        // 如果越界，返回第一个元素（安全的默认行为）
        static T dummy;
        return dummy;
    }

    MANGO_VECTOR_CONSTEXPR const_reference operator[](size_type pos) const noexcept {
        // 添加边界检查，防止访问越界
        if (pos < storage_.size) {
            return storage_.data[pos];
        }
        // 如果越界，返回第一个元素（安全的默认行为）
        static T dummy;
        return dummy;
    }

    MANGO_VECTOR_CONSTEXPR reference at(size_type pos) {
        if (pos >= storage_.size) {
            throw std::out_of_range("MangoVector::at: position out of range");
        }
        return storage_.data[pos];
    }

    MANGO_VECTOR_CONSTEXPR const_reference at(size_type pos) const {
        if (pos >= storage_.size) {
            throw std::out_of_range("MangoVector::at: position out of range");
        }
        return storage_.data[pos];
    }

    MANGO_VECTOR_CONSTEXPR reference front() {
        if (empty()) {
            throw std::out_of_range("MangoVector::front: empty vector");
        }
        return storage_.data[0];
    }

    MANGO_VECTOR_CONSTEXPR const_reference front() const {
        if (empty()) {
            throw std::out_of_range("MangoVector::front: empty vector");
        }
        return storage_.data[0];
    }

    MANGO_VECTOR_CONSTEXPR reference back() {
        if (empty()) {
            throw std::out_of_range("MangoVector::back: empty vector");
        }
        return storage_.data[storage_.size - 1];
    }

    MANGO_VECTOR_CONSTEXPR const_reference back() const {
        if (empty()) {
            throw std::out_of_range("MangoVector::back: empty vector");
        }
        return storage_.data[storage_.size - 1];
    }

    MANGO_VECTOR_CONSTEXPR pointer data() noexcept { return storage_.data; }
    MANGO_VECTOR_CONSTEXPR const_pointer data() const noexcept { return storage_.data; }

    // 迭代器
    MANGO_VECTOR_CONSTEXPR iterator begin() noexcept { return data(); }
    MANGO_VECTOR_CONSTEXPR const_iterator begin() const noexcept { return data(); }
    MANGO_VECTOR_CONSTEXPR const_iterator cbegin() const noexcept { return data(); }

    MANGO_VECTOR_CONSTEXPR iterator end() noexcept { return data() + size(); }
    MANGO_VECTOR_CONSTEXPR const_iterator end() const noexcept { return data() + size(); }
    MANGO_VECTOR_CONSTEXPR const_iterator cend() const noexcept { return data() + size(); }

    MANGO_VECTOR_CONSTEXPR reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    MANGO_VECTOR_CONSTEXPR const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    MANGO_VECTOR_CONSTEXPR const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    MANGO_VECTOR_CONSTEXPR reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    MANGO_VECTOR_CONSTEXPR const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    MANGO_VECTOR_CONSTEXPR const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

    // 修改器
    void clear() noexcept {
        destroy_elements(storage_.data, storage_.data + storage_.size);
        storage_.size = 0;
    }

    void push_back(const T& value) {
        if (storage_.size == storage_.capacity) {
            reserve(std::max(size_type(1), storage_.capacity * 2));
        }
        storage_.data[storage_.size++] = value;
    }

    void push_back(T&& value) {
        if (storage_.size == storage_.capacity) {
            reserve(std::max(size_type(1), storage_.capacity * 2));
        }
        storage_.data[storage_.size++] = std::move(value);
    }

    template<typename... Args>
    reference emplace_back(Args&&... args) {
        if (storage_.size == storage_.capacity) {
            reserve(std::max(size_type(1), storage_.capacity * 2));
        }
        emplace_construct(storage_.data + storage_.size, std::forward<Args>(args)...);
        return storage_.data[storage_.size++];
    }

    void pop_back() {
        if (empty()) {
            throw std::out_of_range("MangoVector::pop_back: empty vector");
        }
        destroy_elements(storage_.data + storage_.size - 1, storage_.data + storage_.size);
        --storage_.size;
    }

    iterator insert(const_iterator pos, const T& value) {
        difference_type diff = pos - cbegin();
        if (diff < 0 || diff > static_cast<difference_type>(size())) {
            throw std::out_of_range("MangoVector::insert: position out of range");
        }

        if (storage_.size == storage_.capacity) {
            reserve(std::max(size_type(1), storage_.capacity * 2));
        }

        iterator insert_pos = begin() + diff;
        for (iterator it = end(); it != insert_pos; --it) {
            *it = std::move(*(it - 1));
        }
        *insert_pos = value;
        ++storage_.size;
        return insert_pos;
    }

    iterator insert(const_iterator pos, T&& value) {
        difference_type diff = pos - cbegin();
        if (diff < 0 || diff > static_cast<difference_type>(size())) {
            throw std::out_of_range("MangoVector::insert: position out of range");
        }

        if (storage_.size == storage_.capacity) {
            reserve(std::max(size_type(1), storage_.capacity * 2));
        }

        iterator insert_pos = begin() + diff;
        for (iterator it = end(); it != insert_pos; --it) {
            *it = std::move(*(it - 1));
        }
        *insert_pos = std::move(value);
        ++storage_.size;
        return insert_pos;
    }

    iterator insert(const_iterator pos, size_type count, const T& value) {
        difference_type diff = pos - cbegin();
        if (diff < 0 || diff > static_cast<difference_type>(size())) {
            throw std::out_of_range("MangoVector::insert: position out of range");
        }

        if (count == 0) {
            return begin() + diff;
        }

        const size_type new_size = size() + count;
        if (new_size > capacity()) {
            reserve(new_size);
        }

        iterator insert_pos = begin() + diff;
        iterator new_end = begin() + new_size;
        iterator old_end = begin() + size();

        // 移动元素腾出空间
        for (iterator it = old_end; it != insert_pos; --it) {
            *(it + count - 1) = std::move(*(it - 1));
        }

        // 插入新元素
        for (size_type i = 0; i < count; ++i) {
            *(insert_pos + i) = value;
        }

        storage_.size = new_size;
        return insert_pos;
    }

    template<typename InputIt>
        requires std::input_iterator<InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last) {
        difference_type diff = pos - cbegin();
        if (diff < 0 || diff > static_cast<difference_type>(size())) {
            throw std::out_of_range("MangoVector::insert: position out of range");
        }

        size_type count = std::distance(first, last);
        if (count == 0) {
            return begin() + diff;
        }

        const size_type new_size = size() + count;
        if (new_size > capacity()) {
            reserve(new_size);
        }

        iterator insert_pos = begin() + diff;
        iterator new_end = begin() + new_size;
        iterator old_end = begin() + size();

        // 移动元素腾出空间
        for (iterator it = old_end; it != insert_pos; --it) {
            *(it + count - 1) = std::move(*(it - 1));
        }

        // 插入新元素
        for (InputIt it = first; it != last; ++it, ++insert_pos) {
            *insert_pos = *it;
        }

        storage_.size = new_size;
        return begin() + diff;
    }

    iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
        return insert(pos, ilist.begin(), ilist.end());
    }

    template<typename... Args>
    iterator emplace(const_iterator pos, Args&&... args) {
        difference_type diff = pos - cbegin();
        if (diff < 0 || diff > static_cast<difference_type>(size())) {
            throw std::out_of_range("MangoVector::emplace: position out of range");
        }

        if (storage_.size == storage_.capacity) {
            reserve(std::max(size_type(1), storage_.capacity * 2));
        }

        iterator insert_pos = begin() + diff;
        for (iterator it = end(); it != insert_pos; --it) {
            *it = std::move(*(it - 1));
        }
        emplace_construct(insert_pos, std::forward<Args>(args)...);
        ++storage_.size;
        return insert_pos;
    }

    iterator erase(const_iterator pos) {
        return erase(pos, pos + 1);
    }

    iterator erase(const_iterator first, const_iterator last) {
        if (first == last) {
            return begin() + (first - cbegin());
        }

        difference_type diff = first - cbegin();
        difference_type count = last - first;
        if (diff < 0 || diff + count > static_cast<difference_type>(size())) {
            throw std::out_of_range("MangoVector::erase: range out of range");
        }

        iterator erase_first = begin() + diff;
        iterator erase_last = erase_first + count;
        iterator new_end = std::move(erase_last, end(), erase_first);
        
        // 销毁多余的元素
        destroy_elements(new_end, end());
        storage_.size -= count;
        return erase_first;
    }

    void resize(size_type count, const T& value = T()) {
        if (count < storage_.size) {
            // 销毁多余的元素
            destroy_elements(storage_.data + count, storage_.data + storage_.size);
            storage_.size = count;
        } else if (count > storage_.size) {
            if (count > storage_.capacity) {
                reserve(count);
            }
            for (size_type i = storage_.size; i < count; ++i) {
                storage_.data[i] = value;
            }
            storage_.size = count;
        }
    }

    // 交换
    MANGO_VECTOR_CONSTEXPR void swap(MangoVector& other) noexcept {
        std::swap(storage_.data, other.storage_.data);
        std::swap(storage_.size, other.storage_.size);
        std::swap(storage_.capacity, other.storage_.capacity);
    }

    // 比较操作符
    MANGO_VECTOR_CONSTEXPR auto operator<=>(const MangoVector& other) const noexcept {
        return std::lexicographical_compare_three_way(
            begin(), end(), other.begin(), other.end());
    }

    MANGO_VECTOR_CONSTEXPR bool operator==(const MangoVector& other) const noexcept {
        if (size() != other.size()) return false;
        return std::equal(begin(), end(), other.begin());
    }
};

// 非成员函数

template<typename T>
void swap(MangoVector<T>& lhs, MangoVector<T>& rhs) noexcept {
    lhs.swap(rhs);
}

// 哈希支持
namespace std {
    template<typename T>
    struct hash<MangoVector<T>> {
        size_t operator()(const MangoVector<T>& vec) const noexcept {
            size_t seed = vec.size();
            for (const auto& elem : vec) {
                seed ^= hash<T>{}(elem) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };
}
