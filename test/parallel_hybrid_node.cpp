// Copyright (c) 2015, RangerUFO
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// 
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// 
// * Neither the name of ranger_bhvr_tree nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "test_util.hpp"
#include <gtest/gtest.h>

TEST(parallel_hybrid_node, none_true) {
  test_generator gen;
  auto root = gen.generate(
    "<bhvr_tree class = \"parallel_hybrid_node\" count = \"0\" expected = \"true\">"
      "<bhvr_tree class = \"true_node\"/>"
      "<bhvr_tree class = \"false_node\"/>"
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
    EXPECT_FALSE(result);
    EXPECT_EQ(2, agent->get_true_counter());
    EXPECT_EQ(2, agent->get_false_counter());
  });
}

TEST(parallel_hybrid_node, single_true) {
  test_generator gen;
  auto root = gen.generate(
    "<bhvr_tree class = \"parallel_hybrid_node\" count = \"1\" expected = \"true\">"
      "<bhvr_tree class = \"true_node\"/>"
      "<bhvr_tree class = \"false_node\"/>"
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
    EXPECT_FALSE(result);
    EXPECT_EQ(2, agent->get_true_counter());
    EXPECT_EQ(2, agent->get_false_counter());
  });
}

TEST(parallel_hybrid_node, double_true) {
  test_generator gen;
  auto root = gen.generate(
    "<bhvr_tree class = \"parallel_hybrid_node\" count = \"2\" expected = \"true\">"
      "<bhvr_tree class = \"true_node\"/>"
      "<bhvr_tree class = \"false_node\"/>"
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
    EXPECT_EQ(2, agent->get_true_counter());
    EXPECT_EQ(2, agent->get_false_counter());
  });
}

TEST(parallel_hybrid_node, none_false) {
  test_generator gen;
  auto root = gen.generate(
    "<bhvr_tree class = \"parallel_hybrid_node\" count = \"0\" expected = \"false\">"
      "<bhvr_tree class = \"true_node\"/>"
      "<bhvr_tree class = \"false_node\"/>"
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
    EXPECT_FALSE(result);
    EXPECT_EQ(2, agent->get_true_counter());
    EXPECT_EQ(2, agent->get_false_counter());
  });
}

TEST(parallel_hybrid_node, single_false) {
  test_generator gen;
  auto root = gen.generate(
    "<bhvr_tree class = \"parallel_hybrid_node\" count = \"1\" expected = \"false\">"
      "<bhvr_tree class = \"true_node\"/>"
      "<bhvr_tree class = \"false_node\"/>"
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
    EXPECT_FALSE(result);
    EXPECT_EQ(2, agent->get_true_counter());
    EXPECT_EQ(2, agent->get_false_counter());
  });
}

TEST(parallel_hybrid_node, double_false) {
  test_generator gen;
  auto root = gen.generate(
    "<bhvr_tree class = \"parallel_hybrid_node\" count = \"2\" expected = \"false\">"
      "<bhvr_tree class = \"true_node\"/>"
      "<bhvr_tree class = \"false_node\"/>"
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
    EXPECT_EQ(2, agent->get_true_counter());
    EXPECT_EQ(2, agent->get_false_counter());
  });
}
