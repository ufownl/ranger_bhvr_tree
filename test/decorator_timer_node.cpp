#include "test_util.hpp"
#include <gtest/gtest.h>
#include <thread>
#include <chrono>

TEST(decorator_timer_node, timer_true) {
	test_generator gen;
	auto root = gen.generate(
		"<bhvr_tree class = \"decorator_timer_node\" duration = \"1.5\">"
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
		EXPECT_EQ(0, agent->get_true_counter());
		EXPECT_EQ(0, agent->get_false_counter());
	});

	std::this_thread::sleep_for(std::chrono::seconds(1));

	root->exec(ap, [] (bool result, test_agent* agent) {
		EXPECT_FALSE(result);
		EXPECT_EQ(0, agent->get_true_counter());
		EXPECT_EQ(0, agent->get_false_counter());
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	root->exec(ap, [] (bool result, test_agent* agent) {
		EXPECT_TRUE(result);
		EXPECT_EQ(1, agent->get_true_counter());
		EXPECT_EQ(0, agent->get_false_counter());
	});

	root->exec(ap, [] (bool result, test_agent* agent) {
		EXPECT_FALSE(result);
		EXPECT_EQ(1, agent->get_true_counter());
		EXPECT_EQ(0, agent->get_false_counter());
	});
}

TEST(decorator_timer_node, timer_false) {
	test_generator gen;
	auto root = gen.generate(
		"<bhvr_tree class = \"decorator_timer_node\" duration = \"1.5\">"
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
		EXPECT_EQ(0, agent->get_false_counter());
	});

	std::this_thread::sleep_for(std::chrono::seconds(1));

	root->exec(ap, [] (bool result, test_agent* agent) {
		EXPECT_FALSE(result);
		EXPECT_EQ(0, agent->get_true_counter());
		EXPECT_EQ(0, agent->get_false_counter());
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	root->exec(ap, [] (bool result, test_agent* agent) {
		EXPECT_FALSE(result);
		EXPECT_EQ(0, agent->get_true_counter());
		EXPECT_EQ(1, agent->get_false_counter());
	});

	root->exec(ap, [] (bool result, test_agent* agent) {
		EXPECT_FALSE(result);
		EXPECT_EQ(0, agent->get_true_counter());
		EXPECT_EQ(1, agent->get_false_counter());
	});
}
