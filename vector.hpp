#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include <cstddef>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <algorithm>

namespace sjtu {

template<typename T>
class vector {
private:
    T* data_;
    size_t size_;
    size_t capacity_;

    void reallocate(size_t new_capacity) {
        T* new_data = static_cast<T*>(::operator new(new_capacity * sizeof(T)));
        
        // Move or copy elements
        for (size_t i = 0; i < size_; ++i) {
            new (new_data + i) T(std::move(data_[i]));
            data_[i].~T();
        }
        
        ::operator delete(data_);
        data_ = new_data;
        capacity_ = new_capacity;
    }

    void grow() {
        size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
        reallocate(new_capacity);
    }

public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    class iterator {
    private:
        T* ptr_;
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator() : ptr_(nullptr) {}
        explicit iterator(T* ptr) : ptr_(ptr) {}
        
        reference operator*() const { return *ptr_; }
        pointer operator->() const { return ptr_; }
        
        iterator& operator++() { ++ptr_; return *this; }
        iterator operator++(int) { iterator tmp = *this; ++ptr_; return tmp; }
        iterator& operator--() { --ptr_; return *this; }
        iterator operator--(int) { iterator tmp = *this; --ptr_; return tmp; }
        
        iterator& operator+=(difference_type n) { ptr_ += n; return *this; }
        iterator& operator-=(difference_type n) { ptr_ -= n; return *this; }
        
        iterator operator+(difference_type n) const { return iterator(ptr_ + n); }
        iterator operator-(difference_type n) const { return iterator(ptr_ - n); }
        difference_type operator-(const iterator& other) const { return ptr_ - other.ptr_; }
        
        reference operator[](difference_type n) const { return ptr_[n]; }
        
        bool operator==(const iterator& other) const { return ptr_ == other.ptr_; }
        bool operator!=(const iterator& other) const { return ptr_ != other.ptr_; }
        bool operator<(const iterator& other) const { return ptr_ < other.ptr_; }
        bool operator>(const iterator& other) const { return ptr_ > other.ptr_; }
        bool operator<=(const iterator& other) const { return ptr_ <= other.ptr_; }
        bool operator>=(const iterator& other) const { return ptr_ >= other.ptr_; }
        
        T* base() const { return ptr_; }
    };

    class const_iterator {
    private:
        const T* ptr_;
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        const_iterator() : ptr_(nullptr) {}
        explicit const_iterator(const T* ptr) : ptr_(ptr) {}
        const_iterator(const iterator& it) : ptr_(it.base()) {}
        
        reference operator*() const { return *ptr_; }
        pointer operator->() const { return ptr_; }
        
        const_iterator& operator++() { ++ptr_; return *this; }
        const_iterator operator++(int) { const_iterator tmp = *this; ++ptr_; return tmp; }
        const_iterator& operator--() { --ptr_; return *this; }
        const_iterator operator--(int) { const_iterator tmp = *this; --ptr_; return tmp; }
        
        const_iterator& operator+=(difference_type n) { ptr_ += n; return *this; }
        const_iterator& operator-=(difference_type n) { ptr_ -= n; return *this; }
        
        const_iterator operator+(difference_type n) const { return const_iterator(ptr_ + n); }
        const_iterator operator-(difference_type n) const { return const_iterator(ptr_ - n); }
        difference_type operator-(const const_iterator& other) const { return ptr_ - other.ptr_; }
        
        reference operator[](difference_type n) const { return ptr_[n]; }
        
        bool operator==(const const_iterator& other) const { return ptr_ == other.ptr_; }
        bool operator!=(const const_iterator& other) const { return ptr_ != other.ptr_; }
        bool operator<(const const_iterator& other) const { return ptr_ < other.ptr_; }
        bool operator>(const const_iterator& other) const { return ptr_ > other.ptr_; }
        bool operator<=(const const_iterator& other) const { return ptr_ <= other.ptr_; }
        bool operator>=(const const_iterator& other) const { return ptr_ >= other.ptr_; }
        
        const T* base() const { return ptr_; }
    };

    // Constructors
    vector() : data_(nullptr), size_(0), capacity_(0) {}
    
    explicit vector(size_t n) : data_(nullptr), size_(0), capacity_(0) {
        if (n > 0) {
            data_ = static_cast<T*>(::operator new(n * sizeof(T)));
            capacity_ = n;
            for (size_t i = 0; i < n; ++i) {
                new (data_ + i) T();
                ++size_;
            }
        }
    }
    
    vector(size_t n, const T& value) : data_(nullptr), size_(0), capacity_(0) {
        if (n > 0) {
            data_ = static_cast<T*>(::operator new(n * sizeof(T)));
            capacity_ = n;
            for (size_t i = 0; i < n; ++i) {
                new (data_ + i) T(value);
                ++size_;
            }
        }
    }
    
    template<typename InputIterator>
    vector(InputIterator first, InputIterator last) : data_(nullptr), size_(0), capacity_(0) {
        for (auto it = first; it != last; ++it) {
            push_back(*it);
        }
    }
    
    vector(const vector& other) : data_(nullptr), size_(0), capacity_(0) {
        if (other.size_ > 0) {
            data_ = static_cast<T*>(::operator new(other.size_ * sizeof(T)));
            capacity_ = other.size_;
            for (size_t i = 0; i < other.size_; ++i) {
                new (data_ + i) T(other.data_[i]);
                ++size_;
            }
        }
    }
    
    vector(vector&& other) noexcept 
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    
    ~vector() {
        clear();
        if (data_) {
            ::operator delete(data_);
        }
    }
    
    vector& operator=(const vector& other) {
        if (this != &other) {
            vector tmp(other);
            swap(tmp);
        }
        return *this;
    }
    
    vector& operator=(vector&& other) noexcept {
        if (this != &other) {
            clear();
            if (data_) {
                ::operator delete(data_);
            }
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }
    
    // Element access
    reference at(size_t pos) {
        if (pos >= size_) throw std::out_of_range("vector::at");
        return data_[pos];
    }
    
    const_reference at(size_t pos) const {
        if (pos >= size_) throw std::out_of_range("vector::at");
        return data_[pos];
    }
    
    reference operator[](size_t pos) { return data_[pos]; }
    const_reference operator[](size_t pos) const { return data_[pos]; }
    
    reference front() { return data_[0]; }
    const_reference front() const { return data_[0]; }
    
    reference back() { return data_[size_ - 1]; }
    const_reference back() const { return data_[size_ - 1]; }
    
    T* data() noexcept { return data_; }
    const T* data() const noexcept { return data_; }
    
    // Iterators
    iterator begin() noexcept { return iterator(data_); }
    const_iterator begin() const noexcept { return const_iterator(data_); }
    const_iterator cbegin() const noexcept { return const_iterator(data_); }
    
    iterator end() noexcept { return iterator(data_ + size_); }
    const_iterator end() const noexcept { return const_iterator(data_ + size_); }
    const_iterator cend() const noexcept { return const_iterator(data_ + size_); }
    
    // Capacity
    bool empty() const noexcept { return size_ == 0; }
    size_t size() const noexcept { return size_; }
    size_t capacity() const noexcept { return capacity_; }
    
    void reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            reallocate(new_capacity);
        }
    }
    
    void shrink_to_fit() {
        if (size_ < capacity_) {
            if (size_ == 0) {
                ::operator delete(data_);
                data_ = nullptr;
                capacity_ = 0;
            } else {
                reallocate(size_);
            }
        }
    }
    
    // Modifiers
    void clear() noexcept {
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        size_ = 0;
    }
    
    iterator insert(iterator pos, const T& value) {
        size_t index = pos - begin();
        if (size_ == capacity_) {
            grow();
        }
        for (size_t i = size_; i > index; --i) {
            new (data_ + i) T(std::move(data_[i - 1]));
            data_[i - 1].~T();
        }
        new (data_ + index) T(value);
        ++size_;
        return iterator(data_ + index);
    }
    
    iterator insert(const_iterator pos, const T& value) {
        size_t index = pos - cbegin();
        if (size_ == capacity_) {
            grow();
        }
        for (size_t i = size_; i > index; --i) {
            new (data_ + i) T(std::move(data_[i - 1]));
            data_[i - 1].~T();
        }
        new (data_ + index) T(value);
        ++size_;
        return iterator(data_ + index);
    }
    
    iterator erase(iterator pos) {
        size_t index = pos - begin();
        data_[index].~T();
        for (size_t i = index; i < size_ - 1; ++i) {
            new (data_ + i) T(std::move(data_[i + 1]));
            data_[i + 1].~T();
        }
        --size_;
        return iterator(data_ + index);
    }
    
    iterator erase(const_iterator pos) {
        size_t index = pos - cbegin();
        data_[index].~T();
        for (size_t i = index; i < size_ - 1; ++i) {
            new (data_ + i) T(std::move(data_[i + 1]));
            data_[i + 1].~T();
        }
        --size_;
        return iterator(data_ + index);
    }
    
    iterator erase(iterator first, iterator last) {
        size_t start = first - begin();
        size_t count = last - first;
        for (size_t i = start; i < start + count; ++i) {
            data_[i].~T();
        }
        for (size_t i = start; i < size_ - count; ++i) {
            new (data_ + i) T(std::move(data_[i + count]));
            data_[i + count].~T();
        }
        size_ -= count;
        return iterator(data_ + start);
    }
    
    void push_back(const T& value) {
        if (size_ == capacity_) {
            grow();
        }
        new (data_ + size_) T(value);
        ++size_;
    }
    
    void push_back(T&& value) {
        if (size_ == capacity_) {
            grow();
        }
        new (data_ + size_) T(std::move(value));
        ++size_;
    }
    
    void pop_back() {
        --size_;
        data_[size_].~T();
    }
    
    void resize(size_t count) {
        if (count > size_) {
            if (count > capacity_) {
                reserve(count);
            }
            for (size_t i = size_; i < count; ++i) {
                new (data_ + i) T();
            }
        } else {
            for (size_t i = count; i < size_; ++i) {
                data_[i].~T();
            }
        }
        size_ = count;
    }
    
    void resize(size_t count, const T& value) {
        if (count > size_) {
            if (count > capacity_) {
                reserve(count);
            }
            for (size_t i = size_; i < count; ++i) {
                new (data_ + i) T(value);
            }
        } else {
            for (size_t i = count; i < size_; ++i) {
                data_[i].~T();
            }
        }
        size_ = count;
    }
    
    void swap(vector& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }
};

} // namespace sjtu

#endif // SJTU_VECTOR_HPP
