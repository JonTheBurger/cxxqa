/** @file
 ******************************************************************************/
#pragma once

/* Includes
 ******************************************************************************/
// std
#include <optional>
#include <utility>

// 3rd

// local

// namespace
namespace cxxqa {

/* Constants
 ******************************************************************************/

/* Types
 ******************************************************************************/
/** Heap-allocated optional.
 * Use this type instead of `std::optional` when `T` is very large an often not active.
 */
template <typename T>
class heap_optional {
private:
  T* _ptr;

public:
  constexpr heap_optional() noexcept
    : _ptr{ nullptr }
  {
  }

  constexpr heap_optional(std::nullptr_t) noexcept
    : _ptr{ nullptr }
  {
  }

  heap_optional(const T& value)
    : _ptr{ new T(value) }
  {
  }

  heap_optional(T&& value)
    : _ptr{ new T(std::move(value)) }
  {
  }

  heap_optional(const heap_optional& other)
    : _ptr{ other._ptr ? new T(*other._ptr) : nullptr }
  {
  }

  heap_optional(heap_optional&& other) noexcept
    : _ptr{ other._ptr }
  {
    other._ptr = nullptr;
  }

  ~heap_optional()
  {
    delete _ptr;
  }

  auto operator=(const heap_optional& other) -> heap_optional&
  {
    if (this != &other)
    {
      delete _ptr;
      _ptr = other._ptr ? new T(*other._ptr) : nullptr;
    }
    return *this;
  }

  auto operator=(heap_optional&& other) noexcept -> heap_optional&
  {
    if (this != &other)
    {
      delete _ptr;
      _ptr       = other._ptr;
      other._ptr = nullptr;
    }
    return *this;
  }

  auto operator=(const T& value) -> heap_optional&
  {
    if (_ptr)
    {
      *_ptr = value;
    }
    else
    {
      _ptr = new T(value);
    }
    return *this;
  }

  auto operator=(T&& value) -> heap_optional&
  {
    if (_ptr)
    {
      *_ptr = std::move(value);
    }
    else
    {
      _ptr = new T(std::move(value));
    }
    return *this;
  }

  auto operator=(std::nullptr_t) noexcept -> heap_optional&
  {
    reset();
    return *this;
  }

  template <typename... Args>
  auto emplace(Args&&... args) -> T&
  {
    // TODO can throw
    delete _ptr;
    _ptr = new T(std::forward<Args>(args)...);
    return *_ptr;
  }

  void reset() noexcept
  {
    delete _ptr;
    _ptr = nullptr;
  }

  constexpr bool has_value() const noexcept
  {
    return _ptr != nullptr;
  }

  constexpr explicit operator bool() const noexcept
  {
    return has_value();
  }

  auto value() & -> T&
  {
    if (_ptr == nullptr)
    {
      throw std::bad_optional_access();
    }
    return *_ptr;
  }

  auto value() const& -> const T&
  {
    if (_ptr == nullptr)
    {
      throw std::bad_optional_access();
    }
    return *_ptr;
  }

  auto value() && -> T&&
  {
    if (_ptr == nullptr)
    {
      throw std::bad_optional_access();
    }
    return std::move(*_ptr);
  }

  auto value() const&& -> const T&&
  {
    if (_ptr == nullptr)
    {
      throw std::bad_optional_access();
    }
    return std::move(*_ptr);
  }

  auto operator*() & -> T&
  {
    return *_ptr;
  }

  auto operator*() const& -> const T&
  {
    return *_ptr;
  }

  auto operator*() && -> T&&
  {
    return std::move(*_ptr);
  }

  auto operator*() const&& -> const T&&
  {
    return std::move(*_ptr);
  }

  auto operator->() noexcept -> T*
  {
    return _ptr;
  }

  auto operator->() const noexcept -> const T*
  {
    return _ptr;
  }

  template <typename U>
  constexpr auto value_or(U&& default_value) const& -> T
  {
    return _ptr ? *_ptr : static_cast<T>(std::forward<U>(default_value));
  }

  template <typename U>
  constexpr auto value_or(U&& default_value) && -> T
  {
    return _ptr ? std::move(*_ptr) : static_cast<T>(std::forward<U>(default_value));
  }

  void swap(heap_optional& other) noexcept
  {
    std::swap(_ptr, other._ptr);
  }
};

/* Functions
 ******************************************************************************/
template <typename T>
void swap(heap_optional<T>& lhs, heap_optional<T>& rhs) noexcept
{
  lhs.swap(rhs);
}

template <typename T, typename... Args>
auto make_heap_optional(Args&&... args) -> heap_optional<T>
{
  heap_optional<T> ptr;
  ptr.emplace(std::forward<Args>(args)...);
  return ptr;
}

template <typename T, typename U>
auto operator==(const heap_optional<T>& lhs, const heap_optional<U>& rhs) -> bool
{
  if (lhs.has_value() != rhs.has_value())
  {
    return false;
  }
  if (!lhs.has_value())
  {
    return true;
  }
  return *lhs == *rhs;
}

template <typename T, typename U>
auto operator!=(const heap_optional<T>& lhs, const heap_optional<U>& rhs) -> bool
{
  return !(lhs == rhs);
}

template <typename T>
auto operator==(const heap_optional<T>& ptr, std::nullptr_t) noexcept -> bool
{
  return !ptr.has_value();
}

template <typename T>
auto operator==(std::nullptr_t, const heap_optional<T>& ptr) noexcept -> bool
{
  return !ptr.has_value();
}

template <typename T>
auto operator!=(const heap_optional<T>& ptr, std::nullptr_t) noexcept -> bool
{
  return ptr.has_value();
}

template <typename T>
auto operator!=(std::nullptr_t, const heap_optional<T>& ptr) noexcept -> bool
{
  return ptr.has_value();
}

template <typename T, typename U>
auto operator==(const heap_optional<T>& opt, const U& value) -> bool
{
  return (opt.has_value()) && (*opt == value);
}

template <typename T, typename U>
auto operator==(const T& value, const heap_optional<U>& opt) -> bool
{
  return (opt.has_value()) && (value == *opt);
}

template <typename T, typename U>
auto operator!=(const heap_optional<T>& opt, const U& value) -> bool
{
  return (!opt.has_value()) || (*opt != value);
}

template <typename T, typename U>
auto operator!=(const T& value, const heap_optional<U>& opt) -> bool
{
  return (!opt.has_value()) || (value != *opt);
}

}  // namespace cxxqa
