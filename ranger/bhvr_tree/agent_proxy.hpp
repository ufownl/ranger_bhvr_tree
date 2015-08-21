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

#include "ranger/bhvr_tree/detail/agent_proxy_base.hpp"
#include <functional>

namespace ranger { namespace bhvr_tree {

template <	class Agent,
			class Handler = std::function<void(bool, Agent*)>,
			class Mutex = std::mutex,
			class Clock = std::chrono::high_resolution_clock>
class agent_proxy
	: public detail::agent_proxy_base<Mutex, Clock, agent_proxy<Agent, Handler, Mutex, Clock>> {
public:
	using agent_type = Agent;
	using handler_type = Handler;
	using mutex_type = Mutex;
	using clock_type = Clock;

	agent_proxy(agent_type& agent) : m_agent(agent) {
		// nop
	}

	agent_type& operator * () const {
		return m_agent;
	}

	agent_type* operator -> () const {
		return &m_agent;
	}

	void operator () (const handler_type& hdl, bool result) const {
		hdl(result, &m_agent);
	}

private:
	agent_type& m_agent;
};

template <class Handler, class Mutex, class Clock>
class agent_proxy<void, Handler, Mutex, Clock>
	: public detail::agent_proxy_base<Mutex, Clock, agent_proxy<void, Handler, Mutex, Clock>> {
public:
	using agent_type = void;
	using handler_type = Handler;
	using mutex_type = Mutex;
	using clock_type = Clock;

	void operator () (const handler_type& hdl, bool result) const {
		hdl(result, nullptr);
	}
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
