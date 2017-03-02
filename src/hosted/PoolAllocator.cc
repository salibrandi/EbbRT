//          Copyright Boston University SESA Group 2013 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#ifndef NDEBUG
#include <sys/socket.h>
#endif
#include <iostream>

#include "Debug.h"
#include "Cpu.h"
#include "Messenger.h"
#include "PoolAllocator.h"
#include "NodeAllocator.h"

#include "../GlobalIdMap.h"

void ebbrt::PoolAllocator::AllocateNode(int i) {
  auto nd = ebbrt::node_allocator->AllocateNode(binary_path_);
  nd.NetworkId().Then([this,i](ebbrt::Future<ebbrt::Messenger::NetworkId> inner) {
    auto nid =inner.Get();
#ifndef NDEBUG
    std::cerr << "# allocated Node: " << i << std::endl;
#endif
    num_nodes_alloc_.fetch_add(1);
    nids_[i] = nid;
    
    if (num_nodes_alloc_ == num_nodes_) {
      // Node ready to work, set future to true
      pool_promise_.SetValue(); 
    }
  });
}

void ebbrt::PoolAllocator::AllocatePool(std::string binary_path, 
    int num_nodes) {

  num_nodes_ = num_nodes;
  num_nodes_alloc_.store(0);
  binary_path_ = binary_path;
  nids_ = new ebbrt::Messenger::NetworkId [num_nodes_];

  int cpu_num = ebbrt::Cpu::GetPhysCpus();

  std::cerr << "Pool Allocation Details: " << std::endl;
  std::cerr << "|   img: " << binary_path_ << std::endl;
  std::cerr << "| nodes: " << num_nodes << std::endl;
  std::cerr << "|  cpus: " << cpu_num << std::endl;

  // Round robin through the available cpus
  for (int i = 0; i < num_nodes; i++) {
    auto cpu_i = ebbrt::Cpu::GetByIndex(i % cpu_num);
    auto ctxt = cpu_i->get_context();
    ebbrt::event_manager->Spawn([this, i]() { AllocateNode(i); }, ctxt, true);
  }
}
