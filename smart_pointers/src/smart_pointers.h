#pragma once

namespace task {

    template <class T>
    class UniquePtr {

        T* pointer = nullptr;

    public:
        UniquePtr () = default;
        explicit UniquePtr (T*);
        UniquePtr (const UniquePtr&) = delete;
        UniquePtr (UniquePtr&&);

        UniquePtr& operator= (const UniquePtr&) = delete;
        UniquePtr& operator= (UniquePtr&&);
        T& operator* ();
        T* operator-> ();

        T* get ();
        T* release ();
        void reset (T* = 0);
        void swap (UniquePtr&);

        ~UniquePtr ();

    };

    template <class T>
    struct ControlBlock {
        T* pointer;
        std::size_t sharedCount;
        std::size_t weakCount;

        ControlBlock(T* ptr = nullptr) : pointer(ptr), sharedCount(1), weakCount(0) {}

        ~ControlBlock() {
            delete pointer;
        }
    };

    template <class T>
    class SharedPtr;

    template <class T>
    class WeakPtr ;

    template <class T>
    class SharedPtr {

        ControlBlock<T>* controlBlock = nullptr;

        void deleteSharedPointer ();

        friend class WeakPtr<T>;

    public:
        SharedPtr ();
        explicit SharedPtr (T*);
        SharedPtr (const SharedPtr&);
        SharedPtr (SharedPtr&&);
        SharedPtr (const WeakPtr<T>&);

        SharedPtr& operator= (const SharedPtr&);
        SharedPtr& operator= (SharedPtr&&);
        T& operator* ();
        T* operator-> ();

        std::size_t use_count() const;

        T* get ();
        void reset (T* = 0);
        void swap (SharedPtr&);

        ~SharedPtr ();
    };

    template <class T>
    class WeakPtr {

        ControlBlock<T>* controlBlock = nullptr;

        friend class SharedPtr<T>;

    public:
        WeakPtr ();
        explicit WeakPtr (T* ptr);
        WeakPtr (const WeakPtr& other);
        WeakPtr (WeakPtr&& other);
        WeakPtr (const SharedPtr<T>&);

        WeakPtr& operator= (const WeakPtr& other);
        WeakPtr& operator= (WeakPtr&& other);
        WeakPtr& operator= (const SharedPtr<T>& other);
        WeakPtr& operator= (SharedPtr<T>&& other);

        SharedPtr<T> lock () const;
        std::size_t use_count () const;
        bool expired () const;

        void reset ();
        void swap (WeakPtr&);

        ~WeakPtr ();
    };

}


#include "smart_pointers.tpp"
