#ifndef CHUCK_ALLOCATOR_SRC_CHUNK_ALLOCATOR_H_
#define CHUCK_ALLOCATOR_SRC_CHUNK_ALLOCATOR_H_

#include <iostream>
#include <memory>
#include <list>
#include "chunk.h"

template <typename T>
class chunk_allocator final {
  private:
    template<typename U>
    friend class chunk_allocator;

    std::shared_ptr<std::list<chunk>> _ptr;

  public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef void* void_pointer;
    typedef const void* void_const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef typename chunk::size_type size_type;
    typedef typename std::pointer_traits<pointer>::difference_type difference_type;
    typedef std::false_type is_always_equal;
    typedef std::true_type propagate_on_container_copy_assignment;
    typedef std::true_type propagate_on_container_move_assignment;
    typedef std::true_type propagate_on_container_swap;
    [[nodiscard]] chunk_allocator<T> select_on_container_copy_construction () const noexcept {
        return chunk_allocator<T>(*this);
    }

    chunk_allocator<T> () noexcept : _ptr(std::make_shared<std::list<chunk>>()) {}

    template <typename U>
    chunk_allocator<T> (const chunk_allocator<U>& other) noexcept : _ptr(other._ptr) {}

    template <typename U>
    chunk_allocator<T> (chunk_allocator<U>&& other) noexcept : _ptr(std::move(other._ptr)) {
        other._ptr = nullptr;
    }

    template <typename U>
    chunk_allocator<T>& operator= (const chunk_allocator<U>& other) noexcept {
        if (this != other) {
            ~chunk_allocator<T>();
            new (this) chunk_allocator<T>(other);
        }
        return *this;
    }

    template <typename U>
    bool operator== (const chunk_allocator<U>& other) const noexcept {
        return static_cast<void_pointer>(_ptr.get()) == static_cast<void_pointer>(other._ptr.get());
    }

    template <typename U>
    bool operator!= (const chunk_allocator<U>& other) const noexcept {
        return !(operator==(other));
    }

    [[nodiscard]] pointer allocate (const size_type& elements_count) {
        const size_type bytes_count = sizeof(value_type) * elements_count;
        if (max_size() < elements_count) {
            throw std::bad_alloc();
        }
        for (chunk& chunk : *_ptr) {
            if (chunk.can_reserve_block(bytes_count)) {
                return reinterpret_cast<pointer>(chunk.reserve_block(bytes_count));
            }
        }
        return reinterpret_cast<pointer>(_ptr->emplace_back().reserve_block(bytes_count));
    }

    void deallocate (pointer ptr, const size_type& elements_count) const {}

    template <typename... Args>
    void construct (pointer ptr, Args&&... args) const {
        new (ptr) T(std::forward<Args>(args)...);
    }

    void destroy (pointer ptr) const {
        ptr->~T();
    }

    [[nodiscard]] size_type max_size () const noexcept {
        return chunk::capacity / sizeof(value_type);
    }
};

#endif //CHUCK_ALLOCATOR_SRC_CHUNK_ALLOCATOR_H_
