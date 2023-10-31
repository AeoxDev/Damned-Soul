#pragma once
#ifdef _DEBUG
#include <assert.h>
#endif // _DEBUG

template <typename T>
class PoolPointer
{
public:
#ifdef _DEBUG
    // The allocated count
    size_t m_ac = 0x0;
#endif // _DEBUG
    // Pointer pointer to the data
    T** m_pp = nullptr;
    // Pool size
    unsigned char m_ps = 0xff;
    // Pool index
    unsigned char m_pi = 0xff;
#ifdef _DEBUG
    PoolPointer(void* adress, unsigned char ps, unsigned char pi, const size_t& as)
    {
        m_ac = as / sizeof(T);
#else
    PoolPointer(void* adress, unsigned char ps, unsigned char pi)
    {
#endif // _DEBUG
        m_pp = (T**)adress;
        m_ps = ps;
        m_pi = pi;
    };

    PoolPointer()
    {
#ifdef _DEBUG
        m_ac = 0x0;
#endif // _DEBUG
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
#ifdef _DEBUG
        assert(idx < m_ac);
#endif // _DEBUG
        return (*m_pp)[idx];
    };

    void operator=(const PoolPointer<T>& other)
    {
#ifdef _DEBUG
        m_ac = other.m_ac;
#endif // _DEBUG
        m_pp = other.m_pp;
        m_pi = other.m_pi;
        m_ps = other.m_ps;
    };

    template<typename T2>
    operator PoolPointer<T2>()
    {
        PoolPointer<T2> retVal;
#ifdef _DEBUG
        retVal.m_ac = (this->m_ac * sizeof(T)) / sizeof(T2);
#endif // _DEBUG
        retVal.m_pi = this->m_pi;
        retVal.m_pp = (T2**)this->m_pp;
        retVal.m_ps = this->m_ps;

        return retVal;
    };

    template<typename T2>
    bool operator==(const PoolPointer<T2>& other)
    {
        return (m_pp == other.m_pp) && (m_pi == other.m_pi) (m_ps == other.m_ps);
    };

    bool IsNullptr() const
    {
        return (m_pp == nullptr) || (*m_pp == nullptr);
    };

    //T* operator++()
    //{
    //    T*& temp = (T*)(*m_pp);
    //    return temp++;
    //};

    operator T*() const
    {
        return (T*)(*m_pp);
    };
};