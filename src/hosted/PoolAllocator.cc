//          Copyright Boston University SESA Group 2013 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#ifndef NDEBUG
#include <sys/socket.h>
#endif
//#include <ctime>
//#include <fstream>
#include <iostream>
//#include <sstream>

//#include "../CapnpMessage.h"
#include "Debug.h"
#include "Messenger.h"
#include "PoolAllocator.h"
#include "NodeAllocator.h"
//#include <RuntimeInfo.capnp.h>
//#include <boost/filesystem.hpp>
//#include <capnp/message.h>

#include "../GlobalIdMap.h"

ebbrt::Future<void> ebbrt::PoolAllocator::AllocateNetwork() {
  auto network_id = ebbrt::messenger->LocalNetworkId();
  network_id_ = network_id;
  return ebbrt::global_id_map->Set(kPoolAllocatorId, network_id.ToBytes());
}

void ebbrt::PoolAllocator::AllocateNode() {
  node_descriptors_.push_back(
      ebbrt::node_allocator->AllocateNode(binary_path_));
}

void ebbrt::PoolAllocator::AllocatePool(std::string binary_path, 
    int num_nodes) {
  num_nodes_ = num_nodes;
  binary_path_ = binary_path;
  std::cerr << "Pool Allocation Details: " << std::endl;
  std::cerr << "|   img: " << binary_path_ << std::endl;
  std::cerr << "| nodes: " << num_nodes_ << std::endl;
  for (int i = 0; i < num_nodes_; i++) {
    AllocateNode();
  }
}
