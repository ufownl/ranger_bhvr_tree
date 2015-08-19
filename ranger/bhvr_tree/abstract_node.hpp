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

#ifndef RANGER_BHVR_TREE_ABSTRACT_NODE_HPP
#define	RANGER_BHVR_TREE_ABSTRACT_NODE_HPP

#include <functional>
#include <memory>

namespace ranger { namespace bhvr_tree {

template <class AgentProxy>
class abstract_node {
public:
	abstract_node() : m_last_child(nullptr) {
		// nop
	}

	virtual ~abstract_node() {
		// nop
	}

	abstract_node(const abstract_node<AgentProxy>&) = delete;
	abstract_node<AgentProxy>& operator = (const abstract_node<AgentProxy>&) = delete;

	virtual void exec(AgentProxy&, std::function<void(bool)>) = 0;

	void attach_child(std::unique_ptr<abstract_node<AgentProxy>> node) {
		if (!node) {
			return;
		}

		if (m_last_child) {
			m_last_child->m_next_sibling = std::move(node);
			m_last_child = m_last_child->m_next_sibling.get();
		} else {
			m_first_child = std::move(node);
			m_last_child = m_first_child.get();
		}
	}

	abstract_node<AgentProxy>* get_next_sibling() const {
		return m_next_sibling.get();
	}

	abstract_node<AgentProxy>* get_first_child() const {
		return m_first_child.get();
	}

private:
	std::unique_ptr<abstract_node<AgentProxy>> m_next_sibling;
	std::unique_ptr<abstract_node<AgentProxy>> m_first_child;
	abstract_node<AgentProxy>* m_last_child;
};

} }

#endif	// RANGER_BHVR_TREE_ABSTRACT_NODE_HPP
