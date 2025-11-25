/** @file
 *
 ******************************************************************************/
#pragma once

/* Includes
 ******************************************************************************/
// std
#include <type_traits>

// 3rd

// local

// namespace
namespace cxxqa {

/* Types
 ******************************************************************************/
template <typename E>
struct is_flags : std::false_type {};

template <typename E>
constexpr bool is_flags_v = is_flags<E>::value && std::is_enum_v<E>;

/* Functions
 ******************************************************************************/
template <typename E>
constexpr auto operator|(E lhs, E rhs) noexcept -> E
  requires(is_flags_v<E>)
{
  using underlying = std::underlying_type_t<E>;
  return static_cast<E>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

template <typename E>
constexpr auto operator&(E lhs, E rhs) noexcept -> E
  requires(is_flags_v<E>)
{
  using underlying = std::underlying_type_t<E>;
  return static_cast<E>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
}

template <typename E>
constexpr auto operator^(E lhs, E rhs) noexcept -> E
  requires(is_flags_v<E>)
{
  using underlying = std::underlying_type_t<E>;
  return static_cast<E>(static_cast<underlying>(lhs) ^ static_cast<underlying>(rhs));
}

template <typename E>
constexpr auto operator~(E value) noexcept -> E
  requires(is_flags_v<E>)
{
  using underlying = std::underlying_type_t<E>;
  return static_cast<E>(~static_cast<underlying>(value));
}

template <typename E>
constexpr auto operator|=(E& lhs, E rhs) noexcept -> E&
  requires(is_flags_v<E>)
{
  return lhs = lhs | rhs;
}

template <typename E>
constexpr auto operator&=(E& lhs, E rhs) noexcept -> E&
  requires(is_flags_v<E>)
{
  return lhs = lhs & rhs;
}

template <typename E>
constexpr auto operator^=(E& lhs, E rhs) noexcept -> E&
  requires(is_flags_v<E>)
{
  return lhs = lhs ^ rhs;
}

template <typename E>
constexpr auto operator!(E self) noexcept -> bool
  requires(is_flags_v<E>)
{
  return self == 0U;
}

}  // namespace cxxqa

/* Defines
 ******************************************************************************/
#define CXXQA_DECLARE_FLAGS(E) \
  template <>                  \
  struct cxxqa::is_flags<E> : std::true_type {}
