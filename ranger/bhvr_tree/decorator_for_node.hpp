// Copyright (c) 2015, RangerUFO
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// 
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// 
// * Neither the name of ranger_bhvr_tree nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef RANGER_BHVR_TREE_DECORATOR_FOR_NODE_HPP
#define RANGER_BHVR_TREE_DECORATOR_FOR_NODE_HPP

#include "ranger/bhvr_tree/abstract_node.hpp"

namespace ranger { namespace bhvr_tree {

template <class AgentProxy>
class decorator_for_node : public abstract_node<AgentProxy> {
public:
  static constexpr const char* name() {
    return "decorator_for_node";
  }

  decorator_for_node(size_t count) : m_count(count) {
    // nop
  }

  void exec(AgentProxy& ap, typename AgentProxy::handler_type hdl) const final {
    auto node = this->get_first_child();
    if (node && m_count > 0) {
      exec_impl(ap, node, m_count, false, std::move(hdl));
    } else {
      ap(hdl, false);
    }
  }

private:
  void exec_impl(AgentProxy& ap,
                 abstract_node<AgentProxy>* node,
                 size_t count,
                 bool current,
                 typename AgentProxy::handler_type hdl) const {
    if (count > 0) {
      node->exec(ap, [=, &ap] (bool result, typename AgentProxy::agent_type*) {
        exec_impl(ap, node, count - 1, current || result, std::move(hdl));
      });
    } else {
      ap(hdl, current);
    }
  }

  size_t m_count;
};

} }

#endif  // RANGER_BHVR_TREE_DECORATOR_FOR_NODE_HPP
