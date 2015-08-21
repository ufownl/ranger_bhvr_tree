#include "sample_util.hpp"
#include "ranger/bhvr_tree/parallel_selector_node.hpp"

int main() {
	using node_ptr = std::unique_ptr<ranger::bhvr_tree::abstract_node<sample_agent_proxy>>;

	node_ptr root(new ranger::bhvr_tree::parallel_selector_node<sample_agent_proxy>);
	root->attach_child(node_ptr(new true_node));
	root->attach_child(node_ptr(new false_node));
	root->attach_child(node_ptr(new false_node));

	sample_agent_proxy ap;
	root->exec(ap, [] (bool result, void*) {
		std::cout << result << std::endl;
	});

	caf::await_all_actors_done();
	caf::shutdown();
	return 0;
}
