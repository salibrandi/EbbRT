//          Copyright Boston University SESA Group 2013 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <chrono>
#include <signal.h>

#include <boost/filesystem.hpp>

#include <ebbrt/Cpu.h>
#include <ebbrt/hosted/PoolAllocator.h>

#include "Printer.h"

static char* ExecName = 0;
static unsigned int NumNodes = 2;

void AppMain() {
  auto bindir = boost::filesystem::system_complete(ExecName).parent_path() /
    "/bm/helloworld.elf32";
  unsigned int num_nodes = NumNodes;

  Printer::Init().Then([bindir, num_nodes](ebbrt::Future<void> f) {
    f.Get();
    try {
      ebbrt::pool_allocator->AllocatePool(bindir.string(), num_nodes);
    } catch (std::runtime_error& e) {
      std::cout << e.what() << std::endl;
      exit(1);
    }
  });

  // Initialize Pool futures
  for (unsigned int i=0 ; i < num_nodes; i++) {
    ebbrt::pool_allocator->pool_futures.push_back(ebbrt::Promise<int>());
  }

  for (unsigned int i=0 ; i < num_nodes; i++) {
    // Waiting for Future
    auto nf = ebbrt::pool_allocator->pool_futures[i].GetFuture();
    nf.Block();
    ebbrt::pool_allocator->node_descriptors_[i].NetworkId().Then([](ebbrt::Future<ebbrt::Messenger::NetworkId> inner) {
      auto nid = inner.Get();
      printer->Execute(nid);
    });
  }

}

int main(int argc, char** argv) {
  void* status;

  ExecName = argv[0];
  if (argc > 1) {
    NumNodes = std::atoi(argv[1]);
  }

  pthread_t tid = ebbrt::Cpu::EarlyInit(5);

  pthread_join(tid, &status);

  // This is redundant I think but I think it is also likely safe
  ebbrt::Cpu::Exit(0);
  return 0;
}
