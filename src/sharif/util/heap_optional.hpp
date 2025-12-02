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
namespace sharif {

/* Constants
 ******************************************************************************/

/* Types
 ******************************************************************************/
/** Heap-allocated optional.
 * Use this type instead of `std::optional` when `T` is very large and often not active.
 */
template <typename T>
class heap_optional {
public:
  constexpr heap_optional() noexcept
    : _ptr{ nullptr }
  {
  }

  explicit(false) constexpr heap_optional(std::nullopt_t /* none */) noexcept
    : _ptr{ nullptr }
  {
  }

  explicit(false) heap_optional(const T& value)
    : _ptr{ new T(value) }
  {
  }

  explicit(false) heap_optional(T&& value)
    : _ptr{ new T(std::move(value)) }
  {
  }

  heap_optional(const heap_optional& other)
    : _ptr{ (other._ptr) ? (new T(*other._ptr)) : (nullptr) }
  {
  }

  heap_optional(heap_optional&& other) noexcept
    : _ptr{ std::exchange(other._ptr, nullptr) }
  {
  }

  ~heap_optional()
  {
    delete _ptr;
  }

  auto operator=(const heap_optional& other) -> heap_optional&
  {
    if (this != &other)
    {
      heap_optional tmp{other};
      std::swap(*this, tmp);
    }
    return *this;
  }

  auto operator=(heap_optional&& other) noexcept -> heap_optional&
  {
    if (this != &other)
    {
      heap_optional tmp{std::move(other)};
      std::swap(*this, tmp);
    }
    return *this;
  }

  auto operator=(const T& value) -> heap_optional&
  {
    if (_ptr == nullptr)
    {
      _ptr = new T(value);
    }
    else
    {
      *_ptr = value;
    }
    return *this;
  }

  auto operator=(T&& value) -> heap_optional&
  {
    if (_ptr == nullptr)
    {
      _ptr = new T(std::move(value));
    }
    else
    {
      *_ptr = std::move(value);
    }
    return *this;
  }

  auto operator=(std::nullopt_t /* none */) noexcept -> heap_optional&
  {
    reset();
    return *this;
  }

  template <typename... Args>
  auto emplace(Args&&... args) -> T&
  {
    heap_optional tmp{std::forward<Args>(args)...};
    std::swap(*this, tmp);
    return *_ptr;
  }

  void reset() noexcept
  {
    delete _ptr;
    _ptr = nullptr;
  }

  constexpr auto has_value() const noexcept -> bool
  {
    return _ptr != nullptr;
  }

  constexpr explicit operator bool() const noexcept
  {
    return has_value();
  }

  auto value(this auto&& self) -> auto&&
  {
    if (self._ptr == nullptr)
    {
      throw std::bad_optional_access();
    }
    return *std::forward_like<decltype(self)>(self)._ptr;
  }

  auto operator*(this auto&& self) -> auto&&
  {
    return std::forward_like<decltype(self)>(*self._ptr);
  }

  auto operator->() noexcept -> T*
  {
    return _ptr;
  }

  auto operator->() const noexcept -> const T*
  {
    return _ptr;
  }

  template <typename Self, typename U>
  constexpr auto value_or(this Self&& self, U&& default_value) -> T
  {
    return (self._ptr) ? (*std::forward<Self>(self)) : (T{std::forward<U>(default_value)});
  }

  void swap(heap_optional& other) noexcept
  {
    std::swap(_ptr, other._ptr);
  }

private:
  T* _ptr;
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
auto operator==(const heap_optional<T>& ptr, std::nullopt_t /* none */) noexcept -> bool
{
  return !ptr.has_value();
}

template <typename T>
auto operator==(std::nullopt_t /* none */, const heap_optional<T>& ptr) noexcept -> bool
{
  return !ptr.has_value();
}

template <typename T>
auto operator!=(const heap_optional<T>& ptr, std::nullopt_t /* none */) noexcept -> bool
{
  return ptr.has_value();
}

template <typename T>
auto operator!=(std::nullopt_t /* none */, const heap_optional<T>& ptr) noexcept -> bool
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

}  // namespace sharif
