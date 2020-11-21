#ifndef CHUCK_ALLOCATOR_SRC_CHUNK_H_
#define CHUCK_ALLOCATOR_SRC_CHUNK_H_

#include <iostream>
#include <memory>

class chunk final {
  public:
    typedef std::size_t size_type;
    typedef char value;
    typedef value* pointer;
    typedef std::pointer_traits<pointer>::difference_type difference_type;

  private:
    pointer _start;
    pointer _size_end;
    pointer _cap_end;

    [[nodiscard]] size_type _free_space_size () const noexcept {
        return static_cast<size_type>(_cap_end - _size_end);
    }

  public:

    const static size_type capacity = 1024 * 1024;

    chunk () noexcept {
        _cap_end =
            (_start = _size_end = new value[capacity]) +
            static_cast<difference_type>(capacity);
    }

    chunk (const chunk& other) = delete;
    chunk (chunk&&) = delete;
    chunk& operator= (const chunk& other) = delete;
    chunk& operator= (chunk&&) = delete;

    ~chunk () noexcept {
        delete[] _start;
    }

    [[nodiscard]] bool can_reserve_block (const size_type& bytes_count) const noexcept {
        return bytes_count <= _free_space_size();
    }

    pointer reserve_block (const size_type& bytes_count) {
        pointer ptr = _size_end;
        _size_end += difference_type(bytes_count);
        return ptr;
    }
};

#endif //CHUCK_ALLOCATOR_SRC_CHUNK_H_