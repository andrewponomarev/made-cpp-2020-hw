#pragma once
#include <iterator>


namespace task {

    template<class T, class Alloc = std::allocator<T>>
    class list {
      private:

        class Node {
        public:
            T data;
            Node *next;
            Node *prev;

            Node(Node* n, Node* p) : next(n), prev(p) {};
            Node(const T& d, Node* n, Node* p) : data(d), next(n), prev(p) {};
            Node(T&& d, Node* n, Node* p) : data(std::move(d)), next(n), prev(p) {};
        };

        using allocator_type = typename std::allocator_traits<Alloc>::template rebind_alloc<Node>;
        allocator_type allocator;
        Node *head;
        Node *tail;
        size_t size_;

      public:
        class iterator {
        public:
            using difference_type = ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::bidirectional_iterator_tag;
            iterator();
            iterator(const iterator&);
            iterator& operator=(const iterator&);
            iterator& operator++();
            iterator operator++(int);
            reference operator*() const;
            pointer operator->() const;
            iterator& operator--();
            iterator operator--(int);
            bool operator==(iterator other) const;
            bool operator!=(iterator other) const;
            friend class list;
        private:
            Node *cur;
        };


        class const_iterator {
          public:
            using difference_type = ptrdiff_t;
            using value_type = T;
            using pointer = const T*;
            using reference = const T&;
            using iterator_category = std::bidirectional_iterator_tag;
            const_iterator();
            const_iterator(const const_iterator&);
            const_iterator(const iterator&);
            const_iterator& operator=(const const_iterator&);
            const_iterator& operator++();
            const_iterator operator++(int);
            reference operator*() const;
            pointer operator->() const;
            const_iterator& operator--();
            const_iterator operator--(int);
            bool operator==(const_iterator other) const;
            bool operator!=(const_iterator other) const;
            friend class list;
        private:
            const Node *cur;
        };

        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        list();
        explicit list(const Alloc& alloc);
        list(size_t count, const T& value, const Alloc& alloc = Alloc());
        explicit list(size_t count, const Alloc& alloc = Alloc());

        ~list();

        list(const list& other);
        list(list&& other);

        list& operator=(const list& other);
        list& operator=(list&& other);

        Alloc get_allocator() const;

        T& front();
        const T& front() const;

        T& back();
        const T& back() const;

        iterator begin();
        iterator end();

        const_iterator cbegin() const;
        const_iterator cend() const;

        reverse_iterator rbegin();
        reverse_iterator rend();

        const_reverse_iterator crbegin() const;
        const_reverse_iterator crend() const;

        bool empty() const;
        size_t size() const;
        size_t max_size() const;
        void clear();

        iterator insert(const_iterator pos, const T& value);
        iterator insert(const_iterator pos, T&& value);
        iterator insert(const_iterator pos, size_t count, const T& value);

        iterator erase(const_iterator pos);
        iterator erase(const_iterator first, const_iterator last);

        void push_back(const T& value);
        void push_back(T&& value);
        void pop_back();
        void push_front(const T& value);
        void push_front(T&& value);
        void pop_front();

        template <class... Args>
        iterator emplace(const_iterator pos, Args&&... args);
        template <class... Args>
        void emplace_back(Args&&... args);
        template <class... Args>
        void emplace_front(Args&&... args);
        void resize(size_t count);
        void swap(list& other);
        void merge(list& other);
        void splice(const_iterator pos, list& other);
        void remove(const T& value);
        void reverse();
        void unique();
        void sort();
    };

    template <class T, class Alloc>
    list<T, Alloc>::iterator::iterator() {
        cur = nullptr;
    }

    template <class T, class Alloc>
    list<T, Alloc>::iterator::iterator(const iterator& other) {
        cur = other.cur;
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::iterator& list<T, Alloc>::iterator::operator=(const task::list<T, Alloc>::iterator& other) {
        cur = other.cur;
        return *this;
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::iterator& list<T, Alloc>::iterator::operator++() {
        cur = cur->next;
        return *this;
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::iterator::operator++(int) {
        iterator it = *this;
        cur = cur->next;
        return it;
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::iterator::reference list<T, Alloc>::iterator::operator*() const {
        return cur->data;
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::iterator::pointer list<T, Alloc>::iterator::operator->() const {
        return &(cur->data);
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::iterator& list<T, Alloc>::iterator::operator--() {
        cur = cur->prev;
        return *this;
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::iterator::operator--(int) {
        iterator it = *this;
        cur = cur->prev;
        return it;
    }

    template <class T, class Alloc>
    bool list<T, Alloc>::iterator::operator==(iterator other) const {
        return cur == other.cur;
    }

    template <class T, class Alloc>
    bool list<T, Alloc>::iterator::operator!=(iterator other) const {
        return cur != other.cur;
    }

    template <class T, class Alloc>
    list<T, Alloc>::const_iterator::const_iterator() {
        cur = nullptr;
    }

    template <class T, class Alloc>
    list<T, Alloc>::const_iterator::const_iterator(const const_iterator& other) {
        cur = other.cur;
    }

    template <class T, class Alloc>
    list<T, Alloc>::const_iterator::const_iterator(const iterator& other) {
        cur = other.cur;
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::const_iterator& list<T, Alloc>::const_iterator::operator=(const task::list<T, Alloc>::const_iterator& other) {
        cur = other.cur;
        return *this;
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::const_iterator& list<T, Alloc>::const_iterator::operator++() {
        cur = cur->next;
        return *this;
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::const_iterator list<T, Alloc>::const_iterator::operator++(int) {
        const_iterator it = *this;
        cur = cur->next;
        return it;
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::const_iterator::reference list<T, Alloc>::const_iterator::operator*() const {
        return cur->data;
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::const_iterator::pointer list<T, Alloc>::const_iterator::operator->() const {
        return &(cur->data);
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::const_iterator& list<T, Alloc>::const_iterator::operator--() {
        cur = cur->prev;
        return *this;
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::const_iterator list<T, Alloc>::const_iterator::operator--(int) {
        iterator it = *this;
        cur = cur->prev;
        return it;
    }

    template <class T, class Alloc>
    bool list<T, Alloc>::const_iterator::operator==(const_iterator other) const {
        return cur == other.cur;
    }

    template <class T, class Alloc>
    bool list<T, Alloc>::const_iterator::operator!=(const_iterator other) const {
        return cur != other.cur;
    }

    template <class T, class Alloc>
    list<T, Alloc>::list() : size_(0) {
        head = allocator.allocate(1);
        tail = allocator.allocate(1);
        head->prev = nullptr;
        head->next = tail;
        tail->prev = head;
        tail->next = nullptr;
    }

    template <class T, class Alloc>
    list<T, Alloc>::list(const Alloc& alloc) : list<T, Alloc>() {
        allocator = alloc;
    }

    template <class T, class Alloc>
    list<T, Alloc>::list(size_t count, const T& value, const Alloc& alloc) : list<T, Alloc>(alloc) {
        while (size_ < count) {
            push_back(value);
        }
    }

    template <class T, class Alloc>
    list<T, Alloc>::list(size_t count, const Alloc& alloc) : list<T, Alloc>(alloc) {
        resize(count);
    }

    template <class T, class Alloc>
    list<T, Alloc>::~list() {
        clear();
        allocator.deallocate(head, 1);
        allocator.deallocate(tail, 1);
    }

    template <class T, class Alloc>
    list<T, Alloc>::list(const list& other) : list<T, Alloc>() {
        for (const_iterator it = other.cbegin(); it != other.cend(); it++) {
            push_back(*it);
        }
    }

    template <class T, class Alloc>
    list<T, Alloc>::list(list&& other) : list<T, Alloc>() {
        head->next = other.head->next;
        head->next->prev = head;
        tail->prev = other.tail->prev;
        tail->prev->next = tail;
        size_ = other.size_;
        other.head->next = other.tail;
        other.tail->prev = other.head;
        other.size_ = 0;
    }

    template <class T, class Alloc>
    list<T, Alloc>& list<T, Alloc>::operator=(const list& other) {
        clear();
        for (const_iterator it = other.cbegin(); it != other.cend(); it++) {
            push_back(*it);
        }
        return *this;
    }

    template <class T, class Alloc>
    list<T, Alloc>& list<T, Alloc>::operator=(list&& other) {
        clear();
        head->next = other.head->next;
        head->next->prev = head;
        tail->prev = other.tail->prev;
        tail->prev->next = tail;
        size_ = other.size_;
        other.head->next = other.tail;
        other.tail->prev = other.head;
        other.size_ = 0;
        return *this;
    }

    template <class T, class Alloc>
    Alloc list<T, Alloc>::get_allocator() const {
        return allocator;
    }

    template <class T, class Alloc>
    T& list<T, Alloc>::front() {
        return head->next->data;
    }

    template <class T, class Alloc>
    const T& list<T, Alloc>::front() const {
        return head->next->data;
    }

    template <class T, class Alloc>
    T& list<T, Alloc>::back() {
        return tail->prev->data;
    }

    template <class T, class Alloc>
    const T& list<T, Alloc>::back() const {
        return tail->prev->data;
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::begin() {
        iterator it;
        it.cur = head->next;
        return it;
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::end() {
        iterator it;
        it.cur = tail;
        return it;
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::const_iterator list<T, Alloc>::cbegin() const {
        const_iterator it;
        it.cur = head->next;
        return it;
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::const_iterator list<T, Alloc>::cend() const {
        const_iterator it;
        it.cur = tail;
        return it;
    }

    template <class T, class Alloc>
    typename task::list<T, Alloc>::reverse_iterator list<T, Alloc>::rbegin() {
        reverse_iterator it;
        it.ptr = tail->prev;
        return it;
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::reverse_iterator list<T, Alloc>::rend() {
        reverse_iterator it;
        it.ptr = head;
        return it;
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::const_reverse_iterator list<T, Alloc>::crbegin() const {
        const_reverse_iterator it(cend());
        return it;
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::const_reverse_iterator list<T, Alloc>::crend() const {
        const_reverse_iterator it(cbegin());
        return it;
    }

    template <class T, class Alloc>
    bool list<T, Alloc>::empty() const {
        return size_ == 0;
    }

    template <class T, class Alloc>
    size_t list<T, Alloc>::size() const {
        return size_;
    }

    template <class T, class Alloc>
    size_t list<T, Alloc>::max_size() const {
        return allocator.max_size();
    }

    template <class T, class Alloc>
    void list<T, Alloc>::clear() {
        while (!empty()) {
            pop_front();
        }
    }


    template <class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::insert(const_iterator pos, const T& value) {
        Node *prev = pos.cur->prev;
        Node *node = allocator.allocate(1);
        allocator.construct(node, value, prev->next, prev);
        prev->next->prev = node;
        prev->next = node;
        iterator it;
        it.cur = node;
        ++size_;
        return it;
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::insert(const_iterator pos, T&& value) {
        Node *prev = pos.cur->prev;
        Node *node = allocator.allocate(1);
        allocator.construct(node, std::move(value), prev->next, prev);
        prev->next->prev = node;
        prev->next = node;
        iterator it;
        it.cur = node;
        ++size_;
        return it;
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::insert(const_iterator pos, size_t count, const T& value) {
        iterator it;
        for (int i = 0; i < count; i++) {
            it = insert(pos, value);
        }
        return it;
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::erase(const_iterator pos) {
        Node *to_del = const_cast<Node*>(pos.cur);
        iterator it;
        it.cur = to_del->next;
        it.cur->prev = to_del->prev;
        to_del->prev->next = to_del->next;
        --size_;
        allocator.destroy(to_del);
        allocator.deallocate(to_del, 1);
        return it;
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::erase(const_iterator first, const_iterator last) {
        const_iterator c_it = first;
        while (c_it != last) {
            c_it = erase(c_it);
        }
        iterator it;
        it.cur = const_cast<Node*>(c_it.cur);
        return it;
    }

    template <class T, class Alloc>
    void list<T, Alloc>::push_back(const T& value) {
        Node *node = allocator.allocate(1);
        allocator.construct(node, value, tail, tail->prev);
        tail->prev = tail->prev->next = node;
        ++size_;
    }

    template <class T, class Alloc>
    void list<T, Alloc>::push_back(T&& value) {
        Node *node = allocator.allocate(1);
        allocator.construct(node, std::move(value), tail, tail->prev);
        tail->prev = tail->prev->next = node;
        ++size_;
    }

    template <class T, class Alloc>
    void list<T, Alloc>::pop_back() {
        if (!empty()) {
            Node *node = tail->prev;
            node->prev->next = node->next;
            node->next->prev = node->prev;
            allocator.destroy(node);
            allocator.deallocate(node, 1);
            --size_;
        }
    }

    template <class T, class Alloc>
    void list<T, Alloc>::push_front(const T& value) {
        Node *node = allocator.allocate(1);
        allocator.construct(node, value, head->next, head);
        head->next = head->next->prev = node;
        ++size_;
    }

    template <class T, class Alloc>
    void list<T, Alloc>::push_front(T&& value) {
        Node *node = allocator.allocate(1);
        allocator.construct(node, std::move(value), head->next, head);
        head->next = head->next->prev = node;
        ++size_;
    }

    template <class T, class Alloc>
    void list<T, Alloc>::pop_front() {
        if (!empty()) {
            Node *node = head->next;
            node->prev->next = node->next;
            node->next->prev = node->prev;
            allocator.destroy(node);
            allocator.deallocate(node, 1);
            --size_;
        }
    }

    template <class T, class Alloc>
    template <class... Args>
    typename list<T, Alloc>::iterator list<T, Alloc>::emplace(typename list<T, Alloc>::const_iterator pos, Args&&... args) {
        Node *prev = pos.cur->prev;
        Alloc data_allocator;
        Node *node = allocator.allocate(1);
        T *data = data_allocator.allocate(1);
        data_allocator.construct(data, std::forward<Args>(args)...);
        allocator.construct(node, std::move(*data), prev->next, prev);
        data_allocator.destroy(data);
        data_allocator.deallocate(data, 1);
        prev->next->prev = node;
        prev->next = node;
        iterator it;
        it.cur = pos.cur->prev;
        ++size_;
        return it;
    }

    template <class T, class Alloc>
    template <class... Args>
    void list<T, Alloc>::emplace_back(Args&&... args) {
        emplace(end(), std::forward<Args>(args)...);
    }

    template <class T, class Alloc>
    template <class... Args>
    void list<T, Alloc>::emplace_front(Args&&... args) {
        emplace(begin(), std::forward<Args>(args)...);
    }

    template <class T, class Alloc>
    void list<T, Alloc>::resize(size_t count) {
        while (size_ < count) {
            Node *node = allocator.allocate(1);
            allocator.construct(node, tail, tail->prev);
            tail->prev = tail->prev->next = node;
            ++size_;
        }
        while (size_ > count) {
            pop_back();
        }
    }

    template <class T, class Alloc>
    void list<T, Alloc>::swap(list& other) {
        Node *node = head;
        head = other.head;
        other.head = node;
        node = other.tail;
        other.tail = tail;
        tail = node;
        size_t s = size_;
        size_ = other.size_;
        other.size_ = s;
    }

    template <class T, class Alloc>
    void list<T, Alloc>::merge(list& other) {
        if (!other.empty()) {
            Node *node = other.head->next;
            Node *current = head->next;
            while (node != other.tail) {
                while (current != tail && node->data > current->data) {
                    current = current->next;
                }
                Node *next = node->next;
                node->prev = current->prev;
                node->next = current;
                current->prev->next = node;
                current->prev = node;
                node = next;
            }
            other.head->next = other.tail;
            other.tail->prev = other.head;
            size_ += other.size_;
            other.size_ = 0;
        }
    }

    template <class T, class Alloc>
    void list<T, Alloc>::splice(const_iterator pos, list& other) {
        if (!other.empty()) {
            Node *node = const_cast<Node*>(pos.cur);
            node->prev->next = other.head->next;
            other.head->next->prev = node->prev;
            node->prev = other.tail->prev;
            other.tail->prev->next = node;
            other.head->next = other.tail;
            other.tail->prev = other.head;
            size_ += other.size_;
            other.size_ = 0;
        }
    }

    template <class T, class Alloc>
    void task::list<T, Alloc>::remove(const T& value) {
        list<iterator> to_del;
        iterator it = begin();

        while (it != end()) {
            if (*it == value) {
                to_del.push_back(it);
            }
            ++it;
        }
        for (auto it = to_del.begin(); it != to_del.end(); ++it) {
            erase(*it);
        }
    }

    template <class T, class Alloc>
    void task::list<T, Alloc>::reverse() {
        Node *node = head->next;
        while (node != tail) {
            Node *next = node->next;
            node->next = node->prev;
            node->prev = next;
            node = next;
        }
        head->prev = head->next;
        head->next = nullptr;
        tail->next = tail->prev;
        tail->prev = nullptr;
        node = head;
        head = tail;
        tail = node;
    }

    template <class T, class Alloc>
    void task::list<T, Alloc>::unique() {
        iterator it = begin();
        while (it != --end()) {
            if (*it == *(++it)) {
                it = erase(it);
                --it;
            }
        }
    }

    template <class T, class Alloc>
    void task::list<T, Alloc>::sort() {
        if (size_ > 1) {
            list left, right;
            Node *l_node = left.head, *r_node = right.head;
            Node *node = head->next;
            int count = 0;
            while (count < size_) {
                if (count < size_/2) {
                    l_node->next = node;
                    node->prev = l_node;
                    l_node = l_node->next;
                    left.size_++;
                }
                else {
                    r_node->next = node;
                    node->prev = r_node;
                    r_node = r_node->next;
                    right.size_++;
                }
                node = node->next;
                count++;
            }
            left.tail->prev = l_node;
            l_node->next = left.tail;
            right.tail->prev = r_node;
            r_node->next = right.tail;
            head->next = tail;
            tail->prev = head;
            size_ = 0;
            left.sort();
            right.sort();
            left.merge(right);
            (*this) = left;
        }
    }

}  // namespace task
