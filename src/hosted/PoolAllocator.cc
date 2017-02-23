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

void ebbrt::PoolAllocator::AllocateNode() {
  std::lock_guard<std::mutex> guard(m_);
  node_descriptors_.push_back(
      ebbrt::node_allocator->AllocateNode(binary_path_)
  );
  std::cerr << "Allocated Node: " << num_nodes_ << std::endl;
  pool_futures[num_nodes_].SetValue(1);
  num_nodes_++;
}

void ebbrt::PoolAllocator::AllocatePool(std::string binary_path, 
    int num_nodes) {
  num_nodes_ = 0;
  binary_path_ = binary_path;
  std::cerr << "Pool Allocation Details: " << std::endl;
  std::cerr << "|   img: " << binary_path_ << std::endl;
  std::cerr << "| nodes: " << num_nodes_ << std::endl;
  for (int i = 0; i < num_nodes; i++) {
    ebbrt::event_manager->Spawn([this]() {
        AllocateNode();
    }, true);
  }
}
