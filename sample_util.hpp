#ifndef SAMPLE_UTIL_HPP
#define SAMPLE_UTIL_HPP

#include "ranger/bhvr_tree/agent_proxy.hpp"
#include "ranger/bhvr_tree/abstract_node.hpp"
#include <caf/all.hpp>
#include <sstream>
#include <thread>
#include <chrono>

namespace std {

inline std::string to_string(const std::thread::id& tid) {
	std::stringstream ss;
	ss << tid;
	return ss.str();
}

}

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

template <class Atom>
class behavior_node : public ranger::bhvr_tree::abstract_node<void> {
public:
	behavior_node() : m_target(caf::spawn(result_actor_impl)) {
		// nop
	}

	void exec(ranger::bhvr_tree::agent_proxy<void>& ap, std::function<void(bool)> hdl) final {
		std::cout << "behavior_node::exec" << std::endl;
		caf::spawn([this, hdl] (caf::event_based_actor* self) {
			self->sync_send(m_target, Atom::value).then(
				[=] (bool result) {
					caf::aout(self) << std::this_thread::get_id() << std::endl;
					hdl(result);
				}
			);
		});
	}

private:
	result_actor m_target;
};

using true_node = behavior_node<true_atom>;
using false_node = behavior_node<false_atom>;

#endif	// SAMPLE_UTIL_HPP
