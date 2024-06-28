#pragma once
#include "stdafx.h"

template<typename TypeToClone>
concept cloneable = requires(TypeToClone obj)
{
    obj.clone();
};

//Non-movable array that stores pointers
template <cloneable T>
class PtrArray
{
public:
    class Iterator;
    class Wrapper;

    //Aliases for std library algorithms
    using value_type = Wrapper;
    using pointer = value_type*;
    using reference = value_type&;

private:
    //Fields
    size_t _length = 0;
    size_t _capacity = _init_capacity;
    Wrapper* _array = nullptr;

    //Private methods
    void _change_Array(Wrapper* array, size_t length, size_t capacity)
    {
        this->_array = array;
        this->_capacity = capacity;
        this->_length = length;
    }

    //Delete all the pointers and set _array to nullptr
    void _deallocate()
    {
        //Clear all the data pointers point at
        for (size_t i = 0; i < this->_length; ++i)
            delete this->_array[i];
        delete[] this->_array;

        //Set pointer to nullptr to avoid unexpected behaviour
        this->_array = nullptr;
    }

    //Check whether _array is full of elements
    bool _isFull() const
    {
        return this->_length >= this->_capacity;
    }

    //Allocate array of nullptrs if _new_Capacity > 0 otherwise _array = nullptr, set new capacity
    void _allocate(size_t _new_Capacity)
    {
        this->_array = _new_Capacity > 0 ?
            new Wrapper[_new_Capacity]
            : nullptr;

        this->_capacity = _new_Capacity;
    }

    //Double capacity -> move to new array -> assign
    void _expandAndMove()
    {
        size_t newCapacity = (this->_capacity + 1) << 1;

        auto newArray = new Wrapper[newCapacity];
        std::move(this->begin(), this->end(), newArray);

        delete[] _array;
        this->_array = newArray;
        this->_capacity = newCapacity;
    }

    template<typename U>
    void _emplaceChecked(U&& obj, Iterator iter)
    {
        *iter = obj;

        ++this->_length;
    }

    const size_t _init_capacity = 10;
public:
    //Define wrapper for T pointer to handle the assignment operator for dereferenced iterators properly
    //It's done for the purpose of array wotking with STL library algorithms
    struct Wrapper
    {
    private:
        using value_type = T*;
        using reference = value_type&;
        using pointer = value_type*;

        //Pointer at data
        pointer _wrapped;

    public:
        explicit Wrapper() noexcept
            : _wrapped(new value_type{ nullptr }) { }

        explicit Wrapper(pointer& ptr) noexcept
            : _wrapped(&ptr) { }

        //Wrapper cannot exist outside of the array so restrict moving
        explicit Wrapper(pointer&& ptr) noexcept = delete;

        Wrapper(Wrapper const& other) noexcept = default;

        Wrapper(Wrapper&& other) noexcept = default;

        ~Wrapper()
        {
            delete this->_wrapped;
        }

        //Allow implicit convrsion to value type
        explicit(false) operator value_type () const { return *this->_wrapped; }

        //Copy assignment operators for both Wrapper and pointer
        Wrapper& operator=(value_type const& ptr) noexcept
        {
            if (*this->_wrapped != ptr)
            {
                //If this has data
                if (*this->_wrapped)
                    //Then delete it
                    delete* this->_wrapped;

                //if other has data then clone, otherwise nullptr
                *this->_wrapped = ptr ? ptr->clone() : nullptr;
            }

            return *this;
        }

        Wrapper& operator=(Wrapper const& other) noexcept
        {
            this->operator=(*other._wrapped);

            return *this;
        }

        //The move assignable operator behaves as copy operator
        Wrapper& operator=(Wrapper&& other) noexcept
        {
            this->operator=(std::forward<value_type>(*other._wrapped));

            return *this;
        }

        //Move assignment operators for pointer to avoid memory leak when assigning rvalue
        Wrapper& operator=(value_type&& ptr) noexcept
        {
            if (*this->_wrapped != ptr)
            {
                delete* this->_wrapped;
                *this->_wrapped = std::move(ptr);
                ptr = nullptr;
            }

            return *this;
        }

        //Overload '->' to get access to data without converting
        value_type operator->() const
        {
            return *this->_wrapped;
        }

        ////Ensure ValueSwappable
        //static friend void swap(Wrapper& lhs, Wrapper& rhs)
        //{
        //    lhs->
        //}
    };

    class Iterator 
    {
    private:
        Wrapper* m_ptr;

    public:
        //Aliases for std library algorithms
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = Wrapper;
        using pointer = T*;
        using reference = T&;

        explicit Iterator(Wrapper* m_ptr) : m_ptr(m_ptr) { };

        Iterator(Iterator const& other) : m_ptr(other.m_ptr) { }

        Wrapper& operator*() { return *m_ptr; }

        Wrapper& operator*() const { return *m_ptr; }

        pointer operator->() { return *m_ptr; }

        Iterator& operator++() { ++m_ptr;  return *this; }

        Iterator operator++(int) { auto temp = *this; ++m_ptr;  return temp; }

        Iterator& operator--() { --m_ptr;  return *this; }

        Iterator operator--(int) { auto temp = *this; --m_ptr;  return temp; }

        Iterator operator+(difference_type n) const { return Iterator(m_ptr + n); }

        Iterator operator-(difference_type n) const { return Iterator(m_ptr - n); }

        Iterator& operator=(Iterator const& other) { this->m_ptr = other.m_ptr; return *this; }

        difference_type operator-(Iterator const& rhs) const { return m_ptr - rhs.m_ptr; }

        std::strong_ordering operator<=>(Iterator const& rhs) const = default;
    };

    //Constructors
    PtrArray()
    {
        this->_allocate(this->_capacity);
    }

    PtrArray(PtrArray<T> const& other)
    {
        this->operator=(other);
    }

    PtrArray(PtrArray<T>&& other) noexcept
    {
        this->operator=(std::move(other));
    }

    ~PtrArray()
    {
        this->_deallocate();
    }

    //Modifiers
    template <typename U>
    void emplace(Iterator position, U&& obj)
    {
        size_t index = position - this->begin();
        if (this->_isFull())
        {
            this->_expandAndMove();
            position = this->begin() + index;
        }

        // Move elements to make space for the new element
        std::move_backward(position, this->end(), position + 2);

        this->_emplaceChecked(std::forward<U>(obj), position);
    }

    template<typename U>
    void emplace_back(U&& obj)
    {
        this->emplace(this->end(), std::forward<U>(obj));
    }

    template<typename U>
    void push_back(U&& obj)
    {
        this->emplace(this->end(), std::forward<U>(obj));
    }

    //Erase elements at [first, last)
    void erase(Iterator _First, Iterator _Last)
    {
        if (this->empty() || _First < this->begin() || _Last > this->end() || _First >= _Last)
            return;

        auto _dist = std::distance(_First, _Last);

        std::move(_Last, this->end(), _First);
        for (decltype(_dist) i = 0; i < _dist; ++i)
            *(this->end() - i - 1) = nullptr;

        this->_length -= _dist;
    }

    void erase(Iterator position)
    {
        this->erase(position, position + 1);
    }

    //Capacity
    size_t size() const noexcept
    {
        return this->_length;
    }

    void clear()
    {
        this->_deallocate();
        this->_length = 0;
        this->_capacity = _init_capacity;
    }

    bool empty() const noexcept
    {
        return !this->_length;
    }

    T const& at(const size_t index) const noexcept(false)
    {
        if (index >= this->_length)
            throw std::out_of_range("Index of the array is out of the range");

        return *this->_array[index];
    }

    T* operator[](const size_t index) const noexcept
    {
        if (index >= this->_length)
            return this->_array[0];

        return this->_array[index];
    }

    //Copy and assignment operators
    PtrArray<T>& operator=(PtrArray<T> const& other)
    {
        this->_deallocate();
        if (!other.empty())
        {
            this->_allocate(other._capacity);
            std::copy(other.begin(), other.begin() + other._capacity, this->begin());
        }
        else this->_array = nullptr;

        this->_length = other._length;
        this->_capacity = other._capacity;
        return *this;
    }

    PtrArray<T>& operator=(PtrArray<T>&& other) noexcept
    {
        //Move data from other to current object
        this->_deallocate();
        this->_change_Array(other._array, other._length, other._capacity);

        //Clear the other
        other._change_Array(nullptr, 0, 0);
        return *this;
    }

    Iterator begin() const
    {
        return Iterator(this->_array);
    }

    Iterator end() const
    {
        return Iterator(this->_array + this->_length);
    }
};

