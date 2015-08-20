#ifndef SAMPLE_UTIL_HPP
#define SAMPLE_UTIL_HPP

#include "ranger/bhvr_tree/agent_proxy.hpp"
#include "ranger/bhvr_tree/abstract_node.hpp"
#include <caf/all.hpp>
#include <chrono>
#include <stdio.h>

using true_atom = caf::atom_constant<caf::atom("true")>;
using false_atom = caf::atom_constant<caf::atom("false")>;
using result_actor = caf::typed_actor<	caf::replies_to<true_atom>::with<bool>,
										caf::replies_to<false_atom>::with<bool>	>;

result_actor::behavior_type result_actor_impl(result_actor::pointer self) {
	return {
		[=] (true_atom) {
			return true;
		},
		[=] (false_atom) {
			return false;
		},
		caf::after(std::chrono::seconds(3)) >> [=] {
			self->quit();
		}
	};
}

using sample_agent_proxy = ranger::bhvr_tree::agent_proxy<void>;

template <class Atom>
class behavior_node : public ranger::bhvr_tree::abstract_node<sample_agent_proxy> {
public:
	behavior_node() : m_target(caf::spawn(result_actor_impl)) {
		// nop
	}

	void exec(sample_agent_proxy& ap, std::function<void(bool)> hdl) const override {
		caf::spawn([this, hdl] (caf::event_based_actor* self) {
			self->sync_send(m_target, Atom::value).then(
				[=] (bool result) {
					hdl(result);
				}
			);
		});
	}

private:
	result_actor m_target;
};

struct true_node : public behavior_node<true_atom> {
	using super = behavior_node<true_atom>;

	void exec(sample_agent_proxy& ap, std::function<void(bool)> hdl) const final {
		puts("true_node::exec");
		super::exec(ap, std::move(hdl));
	}
};

struct false_node : public behavior_node<false_atom> {
	using super = behavior_node<false_atom>;

	void exec(sample_agent_proxy& ap, std::function<void(bool)> hdl) const final {
		puts("false_node::exec");
		super::exec(ap, std::move(hdl));
	}
};

#endif	// SAMPLE_UTIL_HPP
