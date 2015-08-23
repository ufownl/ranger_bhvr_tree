#include "sample_util.hpp"

int main() {
	sample_generator gen;
	auto root = gen.generate(
		"<bhvr_tree class = \"sequence_node\">"
			"<bhvr_tree class = \"true_node\"/>"
			"<bhvr_tree class = \"false_node\"/>"
			"<bhvr_tree class = \"true_node\"/>"
		"</bhvr_tree>"
	);

	sample_agent_proxy ap;
	root->exec(ap, [] (bool result, void*) {
		std::cout << result << std::endl;
	});

	caf::await_all_actors_done();
	caf::shutdown();
	return 0;
}
