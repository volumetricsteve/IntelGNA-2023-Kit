/**
 @copyright Copyright (C) 2017-2022 Intel Corporation
 SPDX-License-Identifier: LGPL-2.1-or-later
*/

#pragma once

#include <cstdint>

#include "functional"

namespace GNA
{

// Generic not defined Address class
template<typename T> class Address;

// Address class for operating on const pointers
template<typename T> class Address<T*const>
{
public:
    Address() = default;
    Address(void * const value) :
        buffer(const_cast<void*>(value))
    {}
    Address(const void * const value) :
        buffer(const_cast<void*>(value))
    {}
    Address(const T * value) :
        buffer(const_cast<T*>(value))
    {}
    template<class C> Address(const Address<C*>& address) :
        buffer(const_cast<C*>(address.Get()))
    {}
    template<class C> Address(const Address<C*const>& address) :
        buffer(address.Get())
    {}
    ~Address() = default;

    Address operator+(const uint32_t& right) const
    {
        Address plus(*this);
        plus.buffer = plus.Get<T>() + right;
        return plus;
    }
    Address operator-(const uint32_t& right) const
    {
        Address left(*this);
        left.buffer = left.Get<T>() - right;
        return left;
    }

    bool operator ==(const std::nullptr_t &right) const
    {
        return buffer == right;
    }

    explicit operator bool() const
    {
        return (nullptr != buffer);
    }

    bool operator!() const
    {
        return (nullptr == buffer);
    }

    template<class X> operator X* () const
    {
        return static_cast<X *>(buffer);
    }

    template<class X> X * Get() const
    {
        return static_cast<X *>(buffer);
    }

    T * Get() const
    {
        return static_cast<T *>(buffer);
    }

    T& operator*() const
    {
        return *(static_cast<T*>(buffer));
    }

    template<class X> uint32_t GetOffset(const Address<X*const>& base) const
    {
        if (this->operator! ()) return 0;
        return static_cast<uint32_t>(
            reinterpret_cast<uintptr_t>(
              reinterpret_cast<uint8_t*>(this->Get<uint8_t>() - base.template Get<uint8_t>())));
    }

    bool InRange(void *memory, uint32_t memorySize) const
    {
        if (buffer < memory)
        {
            return false;
        }

        auto memoryEnd = reinterpret_cast<uint8_t*>(memory) + memorySize;
        if (buffer >= memoryEnd)
        {
            return false;
        }

        return true;
    }

protected:
    void * buffer = nullptr;
};

// Address class for operating on non-const pointers
template<typename T> class Address<T *> : public Address<T*const>
{
public:
    Address() = default;
    Address(void * value) :
        Address<T*const>(value)
    {}
    Address(const void * value) :
        Address<T*const>(value)
    {}
    template<class C> Address(const Address<C*>& address) :
        Address<T*const>(address)
    {}
    template<class C> Address(const Address<C*const>& address) :
        Address<T*const>(address.Get())
    {}

    template<class X> X * Get() const
    {
        return static_cast<X *>(this->buffer);
    }

    T * Get() const
    {
        return static_cast<T*>(this->buffer);
    }

    T * operator->() const
    {
        return static_cast<T * const>(this->buffer);
    }

    const Address operator++(int)
    {
        Address tmp{ *this };
        this->buffer = this->Get() + 1;
        return tmp;
    }
    const Address& operator-=(const uint32_t& right)
    {
        this->buffer = this->Get() - right;
        return *this;
    }
    const Address& operator+=(const uint32_t& right)
    {
        this->buffer = this->Get() + right;
        return *this;
    }
    ~Address() = default;
    Address(const Address&) = default;
    const Address& operator =(const T& right)
    {
        *this->buffer = right;
        return *this;
    }
};

// Address Aliases
using BaseAddress = Address<uint8_t * const>;

}
