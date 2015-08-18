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

#ifndef BEHAVIOR_TREE_SELECTOR_NODE_HPP
#define BEHAVIOR_TREE_SELECTOR_NODE_HPP

#include "ranger/bhvr_tree/abstract_node.hpp"

namespace ranger { namespace bhvr_tree {

template <class Agent>
class selector_node : public abstract_node<Agent> {
public:
	void exec(agent_proxy<Agent>& ap, std::function<void(bool)> hdl) final {
		exec_impl(ap, this->get_first_child(), std::move(hdl));
	}

private:
	void exec_impl(agent_proxy<Agent>& ap, abstract_node<Agent>* node, std::function<void(bool)> hdl) {
		if (node) {
			node->exec(ap, [=, &ap] (bool result) {
				if (result) {
					hdl(true);
				} else {
					exec_impl(ap, node->get_next_sibling(), std::move(hdl));
				}
			});
		} else {
			hdl(false);
		}
	}
};

} }

#endif	// BEHAVIOR_TREE_SELECTOR_NODE_HPP
