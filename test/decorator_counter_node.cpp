#include "test_util.hpp"
#include <gtest/gtest.h>

TEST(decorator_counter_node, counter_true) {
	test_generator gen;
	auto root = gen.generate(
		"<bhvr_tree class = \"decorator_counter_node\" count = \"5\">"
			"<bhvr_tree class = \"true_node\"/>"
		"</bhvr_tree>"
	);
	ASSERT_NE(nullptr, root);

	test_agent agent;
	EXPECT_EQ(0, agent.get_true_counter());
	EXPECT_EQ(0, agent.get_false_counter());

	test_agent_proxy ap(agent);
	for (size_t i = 0; i < 10; ++i) {
		root->exec(ap, [i] (bool result, test_agent* agent) {
			if (i < 5) {
				EXPECT_TRUE(result);
				EXPECT_EQ(i + 1, agent->get_true_counter());
				EXPECT_EQ(0, agent->get_false_counter());
			} else {
				EXPECT_FALSE(result);
				EXPECT_EQ(5, agent->get_true_counter());
				EXPECT_EQ(0, agent->get_false_counter());
			}
		});
	}
}

TEST(decorator_counter_node, counter_false) {
	test_generator gen;
	auto root = gen.generate(
		"<bhvr_tree class = \"decorator_counter_node\" count = \"5\">"
			"<bhvr_tree class = \"false_node\"/>"
		"</bhvr_tree>"
	);
	ASSERT_NE(nullptr, root);

	test_agent agent;
	EXPECT_EQ(0, agent.get_true_counter());
	EXPECT_EQ(0, agent.get_false_counter());

	test_agent_proxy ap(agent);
	for (size_t i = 0; i < 10; ++i) {
		root->exec(ap, [i] (bool result, test_agent* agent) {
			EXPECT_FALSE(result);
			if (i < 5) {
				EXPECT_EQ(0, agent->get_true_counter());
				EXPECT_EQ(i + 1, agent->get_false_counter());
			} else {
				EXPECT_EQ(0, agent->get_true_counter());
				EXPECT_EQ(5, agent->get_false_counter());
			}
		});
	}
}
