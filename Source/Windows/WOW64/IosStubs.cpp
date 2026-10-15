// MADEIRA (BO2 debugging): no-op stand-ins for the iOS-host runtime symbols the WOW64 module
// references unconditionally, so the fork also links as a plain PE for Wine/Hangover on Linux
// (used to debug FEX on the Mac in a Linux VM). Compiled only when FEX_IOS_HOST_BUILD is off.
#include <cstdint>
#include <windef.h>
#include <winternl.h>

extern "C" {
uintptr_t ios_fex_band_base = 0;
uintptr_t ios_fex_band_end = 0;
uint64_t IosCodeBufferSweepGate = 0;
void IosSweepRegisterThreadEx(void*, volatile uint8_t*, volatile uint8_t*) {}
void IosSweepUnregisterThread(void*) {}

__int64 _time64(__int64* t) {
  LARGE_INTEGER li;
  NtQuerySystemTime(&li);
  __int64 r = (li.QuadPart - 116444736000000000LL) / 10000000LL;
  if (t) {
    *t = r;
  }
  return r;
}
}
