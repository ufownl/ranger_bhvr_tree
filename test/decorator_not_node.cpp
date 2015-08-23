#include "test_util.hpp"
#include <gtest/gtest.h>

TEST(decorator_not_node, not_true) {
	test_generator gen;
	auto root = gen.generate(
		"<bhvr_tree class = \"decorator_not_node\">"
			"<bhvr_tree class = \"true_node\"/>"
		"</bhvr_tree>"
	);
	ASSERT_NE(nullptr, root);

	test_agent agent;
	EXPECT_EQ(0, agent.get_true_counter());
	EXPECT_EQ(0, agent.get_false_counter());

	test_agent_proxy ap(agent);
	root->exec(ap, [] (bool result, test_agent* agent) {
		EXPECT_FALSE(result);
		EXPECT_EQ(1, agent->get_true_counter());
		EXPECT_EQ(0, agent->get_false_counter());
	});
}

TEST(decorator_not_node, not_false) {
	test_generator gen;
	auto root = gen.generate(
		"<bhvr_tree class = \"decorator_not_node\">"
			"<bhvr_tree class = \"false_node\"/>"
		"</bhvr_tree>"
	);
	ASSERT_NE(nullptr, root);

	test_agent agent;
	EXPECT_EQ(0, agent.get_true_counter());
	EXPECT_EQ(0, agent.get_false_counter());

	test_agent_proxy ap(agent);
	root->exec(ap, [] (bool result, test_agent* agent) {
		EXPECT_TRUE(result);
		EXPECT_EQ(0, agent->get_true_counter());
		EXPECT_EQ(1, agent->get_false_counter());
	});
}
