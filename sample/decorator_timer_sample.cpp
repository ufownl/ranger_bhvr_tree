#include "sample_util.hpp"

int main() {
	sample_generator gen;
	auto root = gen.generate(
		"<bhvr_tree class = \"decorator_for_node\" count = \"5\">"
			"<bhvr_tree class = \"decorator_until_node\" expected = \"true\">"
				"<bhvr_tree class = \"decorator_timer_node\" duration = \"2.5\">"
					"<bhvr_tree class = \"true_node\"/>"
				"</bhvr_tree>"
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
