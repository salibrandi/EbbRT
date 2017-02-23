//          Copyright Boston University SESA Group 2013 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


// TODO: find out what this means
#ifndef HOSTED_SRC_INCLUDE_EBBRT_POOLALLOCATOR_H_
#define HOSTED_SRC_INCLUDE_EBBRT_POOLALLOCATOR_H_

#include <string>
//#include <unordered_map>
//#include <vector>

//#include <boost/asio.hpp>

#include "../StaticSharedEbb.h"
#include "EbbRef.h"
#include "Messenger.h"
#include "StaticIds.h"
#include "NodeAllocator.h"

namespace ebbrt {
  class PoolAllocator : public StaticSharedEbb<PoolAllocator> {
    private:
      ebbrt::Messenger::NetworkId network_id_;
      int num_nodes_;
      std::mutex m_;
      std::string binary_path_;

    public:
      std::vector<ebbrt::Promise<int>> pool_futures;
      std::vector<ebbrt::NodeAllocator::NodeDescriptor> node_descriptors_;

      void AllocatePool(std::string binary_path, int numNodes);
      void AllocateNode();
      ebbrt::NodeAllocator::NodeDescriptor GetNodeDescriptor(int i);
  };
  const constexpr auto pool_allocator = EbbRef<PoolAllocator>(kPoolAllocatorId);
}
#endif  // HOSTED_SRC_INCLUDE_EBBRT_POOLALLOCATOR_H_
