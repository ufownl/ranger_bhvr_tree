#include "sample_util.hpp"
#include <stdio.h>

using namespace ranger::bhvr_tree;

struct cond_node : abstract_node<sample_agent_proxy<size_t>> {
	static constexpr const char* name() {
		return "cond_node";
	}

	void exec(	sample_agent_proxy<size_t>& ap,
				sample_agent_proxy<size_t>::handler_type hdl) const final {
		ap(hdl, *ap > 0);
	}
};

struct tick_node : abstract_node<sample_agent_proxy<size_t>> {
	static constexpr const char* name() {
		return "tick_node";
	}

	void exec(	sample_agent_proxy<size_t>& ap,
				sample_agent_proxy<size_t>::handler_type hdl) const final {
		printf("%lu\n", (*ap)--);
		ap(hdl, true);
	}
};

struct true_node : abstract_node<sample_agent_proxy<size_t>> {
	static constexpr const char* name() {
		return "true_node";
	}

	void exec(	sample_agent_proxy<size_t>& ap,
				sample_agent_proxy<size_t>::handler_type hdl) const final {
		ap(hdl, true);
	}
};

class count_down_generator
	: public xml_generator<	sample_agent_proxy<size_t>,
							cond_node, tick_node, true_node> {
public:
	using node_pointer =
		std::unique_ptr<abstract_node<sample_agent_proxy<size_t>>>;

protected:
	using xml_generator<
		sample_agent_proxy<size_t>,
		cond_node, tick_node, true_node
	>::generate_node;

	node_pointer generate_node(	rapidxml::xml_node<>* data,
								generate_node_type<cond_node>) const final {
		return node_pointer(new cond_node);
	}

	node_pointer generate_node(	rapidxml::xml_node<>* data,
								generate_node_type<tick_node>) const final {
		return node_pointer(new tick_node);
	}

	node_pointer generate_node(	rapidxml::xml_node<>* data,
								generate_node_type<true_node>) const final {
		return node_pointer(new true_node);
	}
};

int main() {
	count_down_generator gen;
	auto root = gen.generate(
		"<bhvr_tree class = \"decorator_until_node\" expected = \"false\">"
			"<bhvr_tree class = \"sequence_node\">"
				"<bhvr_tree class = \"cond_node\"/>"
				"<bhvr_tree class = \"selector_node\">"
					"<bhvr_tree class = \"decorator_timer_node\" duration = \"1\">"
						"<bhvr_tree class = \"tick_node\"/>"
					"</bhvr_tree>"
					"<bhvr_tree class = \"true_node\"/>"
				"</bhvr_tree>"
			"</bhvr_tree>"
		"</bhvr_tree>"
	);

	size_t counter = 10;
	sample_agent_proxy<size_t> ap(counter);
	root->exec(ap, [] (bool result, size_t*) {
		puts("launch!");
	});

	caf::await_all_actors_done();
	caf::shutdown();
	return 0;
}
