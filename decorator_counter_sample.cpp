#include "sample_util.hpp"
#include "ranger/bhvr_tree/decorator_counter_node.hpp"

void exec(	sample_agent_proxy& ap,
			ranger::bhvr_tree::abstract_node<sample_agent_proxy>& root,
			size_t count) {
	if (count > 0) {
		root.exec(ap, [&ap, &root, count] (bool result) {
			std::cout << result << std::endl;
			exec(ap, root, count - 1);
		});
	}
}

int main() {
	using node_ptr = std::unique_ptr<ranger::bhvr_tree::abstract_node<sample_agent_proxy>>;

	node_ptr root(new ranger::bhvr_tree::decorator_counter_node<sample_agent_proxy>(6));
	root->attach_child(node_ptr(new true_node));

	sample_agent_proxy ap;
	exec(ap, *root, 10);

	caf::await_all_actors_done();
	caf::shutdown();
	return 0;
}
