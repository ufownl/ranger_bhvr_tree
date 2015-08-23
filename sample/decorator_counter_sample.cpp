#include "sample_util.hpp"

void exec(	sample_agent_proxy& ap,
			const ranger::bhvr_tree::abstract_node<sample_agent_proxy>& root,
			size_t count) {
	if (count > 0) {
		root.exec(ap, [&ap, &root, count] (bool result, void*) {
			std::cout << result << std::endl;
			exec(ap, root, count - 1);
		});
	}
}

int main() {
	sample_generator gen;
	auto root = gen.generate(
		"<bhvr_tree class = \"decorator_counter_node\" count = \"6\">"
			"<bhvr_tree class = \"true_node\"/>"
		"</bhvr_tree>"
	);

	sample_agent_proxy ap;
	exec(ap, *root, 10);

	caf::await_all_actors_done();
	caf::shutdown();
	return 0;
}
