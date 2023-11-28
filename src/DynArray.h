/**
 * @file dyn_array.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-09-18
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __DYN_ARRAY_H
#define __DYN_ARRAY_H

#include <cstddef>

template <typename TElem>
class DynArray
{
public:
  DynArray() :
      m_data(new TElem[defaultCapacity]), m_capacity(defaultCapacity), m_size(0)
  {
  }

  DynArray(const DynArray& other) = delete;
  DynArray& operator=(const DynArray& other) = delete;

  DynArray(size_t size) : m_size(size), m_capacity(defaultCapacity)
  {
    while (m_capacity <= m_size)
    {
      m_capacity <<= 1;
    }
    m_data = new TElem[m_capacity];
  }

  DynArray(size_t size, const TElem& val) :
      m_size(size), m_capacity(defaultCapacity)
  {
    while (m_capacity <= m_size)
    {
      m_capacity <<= 1;
    }
    m_data = new TElem[m_capacity];

    for (size_t i = 0; i < m_size; ++i)
    {
      m_data = val;
    }
  }

  ~DynArray() { delete[] m_data; }

  void pushBack(const TElem& elem)
  {
    tryGrow();
    m_data[m_size] = elem;

    ++m_size;
  }

  void popBack()
  {
    --m_size;
    m_data[m_size] = TElem();

    tryShrink();
  }

  const TElem& front() const { return m_data[0]; }
  const TElem& back() const { return m_data[m_size - 1]; }

  void clear()
  {
    if (m_capacity > defaultCapacity)
    {
      TElem* new_data = new TElem[defaultCapacity];

      m_data     = new_data;
      m_capacity = defaultCapacity;
    }
    else
    {
      for (size_t i = 0; i < m_size; ++i)
      {
        m_data[i] = TElem();
      }
    }

    m_size = 0;
  }

  size_t getSize() const { return m_size; }
  bool   isEmpty() const { return m_size == 0; }

  const TElem& operator[](size_t index) const { return m_data[index]; }
  TElem&       operator[](size_t index) { return m_data[index]; }

private:
  static constexpr size_t defaultSize     = 64;
  static constexpr size_t defaultCapacity = defaultSize / sizeof(TElem);
  TElem*                  m_data;
  size_t                  m_capacity;
  size_t                  m_size;

  void tryGrow()
  {
    if (m_size < m_capacity)
      return;

    const size_t new_capacity = 2 * m_capacity;
    TElem*       new_data     = new TElem[new_capacity];
    for (size_t i = 0; i < m_size; ++i)
      new_data[i] = m_data[i];
    delete[] m_data;

    m_capacity = new_capacity;
    m_data     = new_data;
  }

  void tryShrink()
  {
    if (2 * m_size >= m_capacity / 2)
      return;

    const size_t new_capacity = m_capacity / 2;
    TElem*       new_data     = new TElem[new_capacity];
    for (size_t i = 0; i < m_size; ++i)
      new_data[i] = m_data[i];
    delete[] m_data;

    m_capacity = new_capacity;
    m_data     = new_data;
  }
};

#endif /* DynArray.h */
