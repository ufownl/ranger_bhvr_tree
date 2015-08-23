#include "sample_util.hpp"

int main() {
	sample_generator gen;
	for (auto str:	{
						"<bhvr_tree class = \"decorator_not_node\">"
							"<bhvr_tree class = \"true_node\"/>"
						"</bhvr_tree>",
						"<bhvr_tree class = \"decorator_not_node\">"
							"<bhvr_tree class = \"false_node\"/>"
						"</bhvr_tree>"
					}) {
		auto root = gen.generate(str);

		sample_agent_proxy ap;
		root->exec(ap, [] (bool result, void*) {
			std::cout << result << std::endl;
		});

		caf::await_all_actors_done();
	}

	caf::shutdown();
	return 0;
}
