#include "sample_util.hpp"

int main() {
	sample_generator gen;
	auto root = gen.generate("parallel_selector_sample.xml");

	sample_agent_proxy ap;
	root->exec(ap, [] (bool result, void*) {
		std::cout << result << std::endl;
	});

	caf::await_all_actors_done();
	caf::shutdown();
	return 0;
}
