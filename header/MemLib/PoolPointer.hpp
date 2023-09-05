#pragma once

template <typename T>
class PoolPointer
{
public:
    // Pointer pointer to the data
    T** m_pp; // = nullptr;
    // Pool size
    unsigned char m_ps; // = 0xff;
    // Pool index
    unsigned char m_pi; // = 0xff;

    PoolPointer(void* adress, unsigned char ps, unsigned char pi)
    {
        m_pp = (T**)adress;
        m_ps = ps;
        m_pi = pi;
    };

    PoolPointer()
    {
        m_pp = nullptr;
        m_ps = 0xff;
        m_pi = 0xff;
    };

    T* operator->()
    {
        return (*m_pp);
    };

    T& operator*()
    {
        return (**m_pp);
    };

    T& operator[](const size_t& idx)
    {
        return (*m_pp)[idx];
    };

    void operator=(const PoolPointer<T>& other)
    {
        m_pp = other.m_pp;
        m_pi = other.m_pi;
        m_ps = other.m_ps;
    };

    template<typename T2>
    operator PoolPointer<T2>()
    {
        PoolPointer<T2> retVal;
        retVal.m_pi = this->m_pi;
        retVal.m_pp = (T2**)this->m_pp;
        retVal.m_ps = this->m_ps;

        return retVal;
    };

    template<typename T2>
    bool operator==(const PoolPointer<T2>& other)
    {
        return (m_pp == other.m_pp) && (m_pi == other.m_pi) (m_ps == other.m_ps);
    }

    bool IsNullptr() const
    {
        return (*m_pp) == nullptr;
    }
};