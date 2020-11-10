#include "smart_pointers.h"

namespace task {

    template<class T>
    UniquePtr<T>::UniquePtr (T* ptr): pointer(ptr) {}

    template<class T>
    UniquePtr<T>::UniquePtr (UniquePtr&& other) {
        this->pointer = other.pointer;
        other.pointer = nullptr;
    }

    template<class T>
    UniquePtr<T>& UniquePtr<T>::operator= (UniquePtr&& other) {
        reset(other.pointer);
        other.pointer = nullptr;
        return *this;
    }


    template<class T>
    T& UniquePtr<T>::operator* () {
        return *(this->pointer);
    }

    template<class T>
    T* UniquePtr<T>::operator-> () {
        return get();
    }

    template<class T>
    T* UniquePtr<T>::get () {
        return this->pointer;
    }

    template<class T>
    T* UniquePtr<T>::release () {
        T* result = get();
        this->pointer = nullptr;
        return result;
    }

    template<class T>
    void UniquePtr<T>::reset (T* ptr) {
        delete pointer;
        this->pointer = ptr;
    }

    template<class T>
    void UniquePtr<T>::swap (UniquePtr& other) {
        T* temp = this->data;
        this->data = other.data;
        other.data = temp;
    }

    template<class T>
    UniquePtr<T>::~UniquePtr () {
        delete pointer;
    }

    template <class T>
    SharedPtr<T>::SharedPtr () {
        this->controlBlock = new ControlBlock<T>();
    }

    template <class T>
    SharedPtr<T>::SharedPtr (T* ptr) {
        this->controlBlock = new ControlBlock<T>(ptr);
    }

    template <class T>
    SharedPtr<T>::SharedPtr (const SharedPtr& other) {
        this->controlBlock = other.controlBlock;
        this->controlBlock->sharedCount++;
    }

    template <class T>
    SharedPtr<T>::SharedPtr (SharedPtr&& other) {
        this->controlBlock = other.controlBlock;
        other.controlBlock = nullptr;
    }

    template <class T>
    void SharedPtr<T>::deleteSharedPointer () {
        if (!this->controlBlock) {
            return;
        }
        this->controlBlock->sharedCount--;
        if (this->controlBlock->sharedCount == 0) {
            delete this->controlBlock->pointer;
            this->controlBlock->pointer = nullptr;
        }
        if (this->controlBlock->sharedCount == 0 && this->controlBlock->weakCount == 0) {
            delete this->controlBlock;
        }
    }

    template <class T>
    SharedPtr<T>& SharedPtr<T>::operator= (const SharedPtr& other) {
        if (&other == this) {
            return *this;
        }
        deleteSharedPointer();
        this->controlBlock = other.controlBlock;
        this->controlBlock->sharedCount++;
        return *this;
    }

    template <class T>
    SharedPtr<T>& SharedPtr<T>::operator= (SharedPtr&& other) {
        if (&other == this) {
            return *this;
        }
        deleteSharedPointer();
        this->controlBlock = other.controlBlock;
        other.controlBlock = nullptr;
        return *this;
    }

    template <class T>
    SharedPtr<T>::SharedPtr (const WeakPtr<T>& other) {
        if (other.controlBlock) {
            this->controlBlock = other.controlBlock;
            this->controlBlock->sharedCount++;
        }
//        } else {
//            throw std::invalid_argument("Weak pointer should not be empty");
//        }
    }

    template <class T>
    T* SharedPtr<T>::get () {
        return this->controlBlock->pointer;
    }

    template <class T>
    T& SharedPtr<T>::operator* () {
        return *get();
    }

    template <class T>
    T* SharedPtr<T>::operator-> () {
        return get();
    }

    template <class T>
    size_t SharedPtr<T>::use_count () const {
        if (this->controlBlock->pointer) {
            return this->controlBlock->sharedCount;
        } else {
            return 0;
        }
    }

    template <class T>
    void SharedPtr<T>::reset (T* ptr) {
        deleteSharedPointer();
        this->controlBlock = new ControlBlock<T>(ptr);
    }

    template <class T>
    void SharedPtr<T>::swap (SharedPtr& other) {
        ControlBlock<T>* temp = this->controlBlock;
        this->controlBlock = other.controlBlock;
        other.controlBlock = temp;
    }

    template <class T>
    SharedPtr<T>::~SharedPtr () {
        deleteSharedPointer();
    }

    template <class T>
    WeakPtr<T>::WeakPtr () : WeakPtr(SharedPtr<T>()) {}

    template <class T>
    WeakPtr<T>::WeakPtr (const SharedPtr<T>& other) {
        this->controlBlock = other.controlBlock;
        this->controlBlock->weakCount++;
    }

    template <class T>
    WeakPtr<T>::WeakPtr (const WeakPtr& other) {
        this->controlBlock = other.controlBlock;
        this->controlBlock->weakCount++;
    }

    template <typename T>
    WeakPtr<T>::WeakPtr (WeakPtr&& other) {
        this->controlBlock = other.controlBlock;
        other.controlBlock = nullptr;
    }

    template <class T>
    WeakPtr<T>& WeakPtr<T>::operator= (const WeakPtr& other) {
        if (this == &other) {
            return *this;
        }
        reset();
        this->controlBlock = other.controlBlock;
        this->controlBlock->weakCount++;
        return *this;
    }

    template <class T>
    WeakPtr<T>& WeakPtr<T>::operator= (WeakPtr&& other) {
        if (this == &other) {
            return *this;
        }
        this->controlBlock = other.controlBlock;
        other.controlBlock = nullptr;
        return *this;
    }

    template <class T>
    WeakPtr<T>& WeakPtr<T>::operator= (const SharedPtr<T>& other) {
        this->controlBlock = other.controlBlock;
        this->controlBlock->weakCount++;
        return *this;
    }

    template <class T>
    SharedPtr<T> WeakPtr<T>::lock () const {
        if (expired()) {
            return SharedPtr<T>();
        } else {
            return SharedPtr<T>(*this);
        }
    }

    template <class T>
    size_t WeakPtr<T>::use_count () const {
        if (this->controlBlock) {
            return this->controlBlock->sharedCount;
        } else {
            return 0;
        }
    }

    template <class T>
    bool WeakPtr<T>::expired () const {
        return use_count() == 0;
    }

    template <class T>
    void WeakPtr<T>::reset () {
        this->controlBlock->weakCount--;
        if (this->controlBlock->sharedCount == 0 && this->controlBlock->weakCount == 0) {
            delete this->controlBlock;
        }
    }

    template <typename T>
    void WeakPtr<T>::swap(WeakPtr& other) {
        ControlBlock<T> temp = this->controlBlock;
        this->controlBlock = other.controlBlock;
        other.controlBlock = temp;
    }

    template <typename T>
    WeakPtr<T>::~WeakPtr() {
        if (!this->controlBlock) {
            return;
        }
        reset();
    }
}