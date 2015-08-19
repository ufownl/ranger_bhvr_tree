#include "sample_util.hpp"
#include "ranger/bhvr_tree/decorator_until_node.hpp"
#include "ranger/bhvr_tree/decorator_counter_node.hpp"

int main() {
	using node_ptr = std::unique_ptr<ranger::bhvr_tree::abstract_node<sample_agent_proxy>>;

	node_ptr root(new ranger::bhvr_tree::decorator_until_node<sample_agent_proxy>(false));
	node_ptr trunk(new ranger::bhvr_tree::decorator_counter_node<sample_agent_proxy>(5));
	trunk->attach_child(node_ptr(new true_node));
	root->attach_child(std::move(trunk));

	sample_agent_proxy ap;
	root->exec(ap, [] (bool result) {
		std::cout << result << std::endl;
	});

	caf::await_all_actors_done();
	caf::shutdown();
	return 0;
}
