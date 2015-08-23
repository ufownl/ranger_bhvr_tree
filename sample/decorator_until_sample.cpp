#include "sample_util.hpp"

int main() {
	sample_generator gen;
	auto root = gen.generate(
		"<bhvr_tree class = \"decorator_until_node\" expected = \"false\">"
			"<bhvr_tree class = \"decorator_counter_node\" count = \"5\">"
				"<bhvr_tree class = \"true_node\"/>"
			"</bhvr_tree>"
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
