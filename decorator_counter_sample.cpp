#include "sample_util.hpp"
#include "ranger/bhvr_tree/decorator_counter_node.hpp"

int main() {
	using node_ptr = std::unique_ptr<ranger::bhvr_tree::abstract_node<sample_agent_proxy>>;

	node_ptr root(new ranger::bhvr_tree::decorator_counter_node<sample_agent_proxy>(5));
	root->attach_child(node_ptr(new true_node));

	sample_agent_proxy ap;
	for (auto i = 0; i < 10; ++i) {
		root->exec(ap, [] (bool result) {
			std::cout << result << std::endl;
		});
	}

	caf::await_all_actors_done();
	caf::shutdown();
	return 0;
}
