//          Copyright Boston University SESA Group 2013 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#ifndef COMMON_SRC_INCLUDE_EBBRT_GLOBALSTATICIDS_H_
#define COMMON_SRC_INCLUDE_EBBRT_GLOBALSTATICIDS_H_

#include "Hash.h"

namespace ebbrt {
enum : EbbId { kGlobalIdMapId, kFirstLocalId };
const constexpr EbbId kFirstStaticUserId = 0x8000;
const constexpr EbbId GenerateStaticEbbId(hash::conststr a) {
  return kFirstStaticUserId | (static_string_hash(a) % 0x1000);
}
}  // namespace ebbrt

#endif  // COMMON_SRC_INCLUDE_EBBRT_GLOBALSTATICIDS_H_
