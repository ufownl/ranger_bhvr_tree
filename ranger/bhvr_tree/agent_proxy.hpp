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

#ifndef RANGER_BHVR_TREE_AGENT_PROXY_HPP
#define	RANGER_BHVR_TREE_AGENT_PROXY_HPP

#include <mutex>
#include <map>

namespace ranger { namespace bhvr_tree {

template <class AgentProxy>
class decorator_counter_node;

namespace detail {

template <class Mutex, class AgentProxy>
class agent_proxy_base {
public:
	agent_proxy_base() = default;

	agent_proxy_base(const agent_proxy_base<Mutex, AgentProxy>&) = delete;
	agent_proxy_base<Mutex, AgentProxy>& operator = (const agent_proxy_base<Mutex, AgentProxy>&) = delete;

	bool less_then_increase(decorator_counter_node<AgentProxy>* key, size_t value) {
		std::lock_guard<Mutex> lock(m_counters_mtx);

		auto it = m_counters.emplace(key, 0).first;
		if (it->second >= value) {
			return false;
		}

		++it->second;
		return true;
	}

private:
	std::map<decorator_counter_node<AgentProxy>*, size_t> m_counters;
	Mutex m_counters_mtx;
};

}

template <class Agent, class Mutex = std::mutex>
class agent_proxy : public detail::agent_proxy_base<Mutex, agent_proxy<Agent, Mutex>> {
public:
	using agent_type = Agent;
	using mutex_type = Mutex;

	agent_proxy(Agent& agent) : m_agent(agent) {
		// nop
	}

	Agent& get_agent() const {
		return m_agent;
	}

	Agent* operator -> () const {
		return &m_agent;
	}

private:
	Agent& m_agent;
};

template <class Mutex>
class agent_proxy<void, Mutex> : public detail::agent_proxy_base<Mutex, agent_proxy<void, Mutex>> {
public:
	using agent_type = void;
	using mutex_type = Mutex;
};

// An optional policy for agent_proxy.
struct dummy_mutex {
	void lock() {
		// nop
	}

	bool try_lock() {
		return true;
	}

	void unlock() {
		// nop
	}
};

} }

#endif	// RANGER_BHVR_TREE_AGENT_PROXY_HPP
