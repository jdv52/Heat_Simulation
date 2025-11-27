#pragma once

#include <atomic>
#include <cstddef>
#include <memory>

template <typename T, typename Alloc = std::allocator<T>>
class SPSCQueue : private Alloc {
  std::size_t capacity_;
  T *ring_;
  std::atomic<std::size_t> pushCursor_{};
  std::atomic<std::size_t> popCursor_{};

  static_assert(std::atomic<std::size_t>::is_always_lock_free);

public:
  explicit SPSCQueue(std::size_t capacity, Alloc const &alloc = Alloc{})
      : Alloc{alloc}, capacity_{capacity},
        ring_{std::allocator_traits<Alloc>::allocate(*this, capacity)} {}

  ~SPSCQueue() {
    while (not empty()) {
      ring_[popCursor_ % capacity_].~T();
      ++popCursor_;
    }
    std::allocator_traits<Alloc>::deallocate(*this, ring_, capacity_);
  }

  auto capacity() const { return capacity_; }
  auto size() const { return pushCursor_ - popCursor_; }
  auto empty() const { return size() == 0; }
  auto full() const { return size() == capacity(); }

  auto push(T const &value) {
    if (full()) {
      return false;
    }
    new (&ring_[pushCursor_ % capacity_]) T(value);
    ++pushCursor_;
    return true;
  }

  auto pop(T &value) {
    if (empty()) {
      return false;
    }
    value = ring_[popCursor_ % capacity_];
    ring_[popCursor_ % capacity_].~T();
    ++popCursor_;
    return true;
  }
};
