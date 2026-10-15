// MADEIRA (local patch, not for upstream): x86 debug-register execute breakpoints for the
// Windows frontends.  The frontend keeps each thread's enabled DR0-3 execute addresses in a
// uint32_t[4] table and publishes its pointer in the thread's CPUState DeferredSignalRefCount
// slot (unused on Windows).  This process-wide registry lists every address any thread has a
// breakpoint on, so the compiler only emits the (per-thread, run-time) check at those addresses.
#pragma once
#include <atomic>
#include <cstddef>
#include <cstdint>

namespace FEXCore::HwBp {
constexpr size_t MaxEntries = 128;
struct Registry {
  std::atomic<uint32_t> Count {0};
  std::atomic<uint32_t> Addr[MaxEntries] {};
  std::atomic<uint32_t> Refs[MaxEntries] {};
};
extern Registry Reg;
void Add(uint32_t Addr);
void Remove(uint32_t Addr);
inline bool Contains(uint64_t Rip) {
  if (!Reg.Count.load(std::memory_order_relaxed)) {
    return false;
  }
  for (size_t i = 0; i < MaxEntries; ++i) {
    if (Reg.Refs[i].load(std::memory_order_relaxed) && Reg.Addr[i].load(std::memory_order_relaxed) == Rip) {
      return true;
    }
  }
  return false;
}
} // namespace FEXCore::HwBp
