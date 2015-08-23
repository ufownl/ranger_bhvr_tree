#ifndef SAMPLE_UTIL_HPP
#define SAMPLE_UTIL_HPP

#include "ranger/bhvr_tree/agent_proxy.hpp"
#include "ranger/bhvr_tree/xml_generator.hpp"
#include <caf/all.hpp>
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
	static constexpr const char* name() {
		return "true_node";
	}

	void exec(sample_agent_proxy& ap, sample_handler hdl) const final {
		puts("true_node::exec");
		behavior_node<true>::exec(ap, std::move(hdl));
	}
};

struct false_node : public behavior_node<false> {
	static constexpr const char* name() {
		return "false_node";
	}

	void exec(sample_agent_proxy& ap, sample_handler hdl) const final {
		puts("false_node::exec");
		behavior_node<false>::exec(ap, std::move(hdl));
	}
};

class sample_generator
	: public ranger::bhvr_tree::xml_generator<sample_agent_proxy, true_node, false_node> {
public:
	using node_pointer =
		std::unique_ptr<ranger::bhvr_tree::abstract_node<sample_agent_proxy>>;

protected:
	using ranger::bhvr_tree::xml_generator<sample_agent_proxy, true_node, false_node>::generate_node;

	node_pointer generate_node(	rapidxml::xml_node<>* data,
								ranger::bhvr_tree::generate_node_type<true_node>) const final {
		return node_pointer(new true_node);
	}

	node_pointer generate_node(	rapidxml::xml_node<>* data,
								ranger::bhvr_tree::generate_node_type<false_node>) const final {
		return node_pointer(new false_node);
	}
};

#endif	// SAMPLE_UTIL_HPP
