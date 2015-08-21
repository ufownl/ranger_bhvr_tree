#ifndef SAMPLE_UTIL_HPP
#define SAMPLE_UTIL_HPP

#include "ranger/bhvr_tree/agent_proxy.hpp"
#include "ranger/bhvr_tree/abstract_node.hpp"
#include <caf/all.hpp>
#include <chrono>
#include <stdio.h>

using handler_actor = caf::typed_actor<caf::reacts_to<bool, void*>>;

class sample_handler {
public:
	template <class T>
	sample_handler(const T& hdl)
		: m_actor(caf::spawn([hdl] (handler_actor::pointer self) -> handler_actor::behavior_type {
			return {
				[=] (bool result, void* agent) {
					hdl(result, agent);
					self->quit();
				}
			};
		})) {
		// nop
	}

	sample_handler(const sample_handler& rhs) : m_actor(rhs.m_actor) {
		// nop
	}

	sample_handler& operator = (const sample_handler& rhs) {
		if (this != &rhs) {
			m_actor = rhs.m_actor;
		}
		return *this;
	}

	sample_handler(sample_handler&& rhs) : m_actor(std::move(rhs.m_actor)) {
		// nop
	}

	sample_handler& operator = (sample_handler&& rhs) {
		if (this != &rhs) {
			m_actor = std::move(rhs.m_actor);
		}
		return *this;
	}

	void operator () (bool result, void* agent) const {
		caf::anon_send(m_actor, result, agent);
	}

private:
	handler_actor m_actor;
};

using sample_agent_proxy = ranger::bhvr_tree::agent_proxy<void, sample_handler>;

template <bool Result>
class behavior_node : public ranger::bhvr_tree::abstract_node<sample_agent_proxy> {
public:
	void exec(sample_agent_proxy& ap, sample_handler hdl) const override {
		ap(hdl, Result);
	}
};

struct true_node : public behavior_node<true> {
	using super = behavior_node<true>;

	void exec(sample_agent_proxy& ap, sample_handler hdl) const final {
		puts("true_node::exec");
		super::exec(ap, std::move(hdl));
	}
};

struct false_node : public behavior_node<false> {
	using super = behavior_node<false>;

	void exec(sample_agent_proxy& ap, sample_handler hdl) const final {
		puts("false_node::exec");
		super::exec(ap, std::move(hdl));
	}
};

#endif	// SAMPLE_UTIL_HPP
