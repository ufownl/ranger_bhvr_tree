#include "sample_util.hpp"

int main() {
	sample_generator gen;
	for (auto path: {"decorator_not_sample_0.xml", "decorator_not_sample_1.xml"}) {
		auto root = gen.generate(path);

		sample_agent_proxy ap;
		root->exec(ap, [] (bool result, void*) {
			std::cout << result << std::endl;
		});

		caf::await_all_actors_done();
	}

	caf::shutdown();
	return 0;
}
