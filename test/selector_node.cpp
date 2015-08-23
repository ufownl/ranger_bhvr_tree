#include "test_util.hpp"
#include <gtest/gtest.h>

TEST(selector_node, double_true) {
	test_generator gen;
	auto root = gen.generate(
		"<bhvr_tree class = \"selector_node\">"
			"<bhvr_tree class = \"true_node\"/>"
			"<bhvr_tree class = \"true_node\"/>"
		"</bhvr_tree>"
	);
	ASSERT_NE(nullptr, root);

	test_agent agent;
	EXPECT_EQ(0, agent.get_true_counter());
	EXPECT_EQ(0, agent.get_false_counter());

	test_agent_proxy ap(agent);
	root->exec(ap, [] (bool result, test_agent* agent) {
		EXPECT_TRUE(result);
		EXPECT_EQ(1, agent->get_true_counter());
		EXPECT_EQ(0, agent->get_false_counter());
	});
}

TEST(selector_node, true_false) {
	test_generator gen;
	auto root = gen.generate(
		"<bhvr_tree class = \"selector_node\">"
			"<bhvr_tree class = \"true_node\"/>"
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
		EXPECT_EQ(1, agent->get_true_counter());
		EXPECT_EQ(0, agent->get_false_counter());
	});
}

TEST(selector_node, false_true) {
	test_generator gen;
	auto root = gen.generate(
		"<bhvr_tree class = \"selector_node\">"
			"<bhvr_tree class = \"false_node\"/>"
			"<bhvr_tree class = \"true_node\"/>"
		"</bhvr_tree>"
	);
	ASSERT_NE(nullptr, root);

	test_agent agent;
	EXPECT_EQ(0, agent.get_true_counter());
	EXPECT_EQ(0, agent.get_false_counter());

	test_agent_proxy ap(agent);
	root->exec(ap, [] (bool result, test_agent* agent) {
		EXPECT_TRUE(result);
		EXPECT_EQ(1, agent->get_true_counter());
		EXPECT_EQ(1, agent->get_false_counter());
	});
}

TEST(selector_node, double_false) {
	test_generator gen;
	auto root = gen.generate(
		"<bhvr_tree class = \"selector_node\">"
			"<bhvr_tree class = \"false_node\"/>"
			"<bhvr_tree class = \"false_node\"/>"
		"</bhvr_tree>"
	);
	ASSERT_NE(nullptr, root);

	test_agent agent;
	EXPECT_EQ(0, agent.get_true_counter());
	EXPECT_EQ(0, agent.get_false_counter());

	test_agent_proxy ap(agent);
	root->exec(ap, [] (bool result, test_agent* agent) {
		EXPECT_FALSE(result);
		EXPECT_EQ(0, agent->get_true_counter());
		EXPECT_EQ(2, agent->get_false_counter());
	});
}
