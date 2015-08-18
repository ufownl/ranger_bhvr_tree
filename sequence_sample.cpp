#include "sample_util.hpp"
#include "ranger/bhvr_tree/sequence_node.hpp"

int main() {
	using node_ptr = std::unique_ptr<ranger::bhvr_tree::abstract_node<void>>;

	node_ptr root(new ranger::bhvr_tree::sequence_node<void>);
	root->attach_child(node_ptr(new true_node));
	root->attach_child(node_ptr(new false_node));
	root->attach_child(node_ptr(new true_node));

	ranger::bhvr_tree::agent_proxy<void> ap;
	root->exec(ap, [] (bool result) {
		std::cout << result << std::endl;
	});

	caf::await_all_actors_done();
	caf::shutdown();
	return 0;
}
