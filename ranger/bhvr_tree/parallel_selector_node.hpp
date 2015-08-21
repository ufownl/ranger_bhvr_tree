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

#ifndef RANGER_BHVR_TREE_PARALLEL_SELECTOR_NODE_HPP
#define RANGER_BHVR_TREE_PARALLEL_SELECTOR_NODE_HPP

#include "ranger/bhvr_tree/abstract_node.hpp"
#include <mutex>

namespace ranger { namespace bhvr_tree {

template <class AgentProxy>
class parallel_selector_node : public abstract_node<AgentProxy> {
public:
	using mutex_type = typename AgentProxy::mutex_type;

	void exec(AgentProxy& ap, typename AgentProxy::handler_type hdl) const final {
		auto data = std::make_shared<internal_data>();
		for (auto node = this->get_first_child(); node; node = node->get_next_sibling()) {
			++data->count;
		}

		if (data->count > 0) {
			for (auto node = this->get_first_child(); node; node = node->get_next_sibling()) {
				node->exec(ap, [=, &ap] (bool result, void*) {
					std::lock_guard<mutex_type> lock(data->mtx);
					data->result = data->result || result;
					if (--data->count == 0) {
						ap(hdl, data->result);
					}
				});
			}
		} else {
			ap(hdl, data->result);
		}
	}

private:
	struct internal_data {
		size_t count {0};
		bool result {false};
		mutex_type mtx;
	};
};

} }

#endif	// RANGER_BHVR_TREE_PARALLEL_SELECTOR_NODE_HPP
