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

#ifndef TEST_UTIL_HPP
#define TEST_UTIL_HPP

#include "ranger/bhvr_tree/agent_proxy.hpp"
#include "ranger/bhvr_tree/xml_generator.hpp"

class test_agent {
public:
	void increase_true_counter() {
		++m_true_counter;
	}

	void increase_false_counter() {
		++m_false_counter;
	}

	size_t get_true_counter() const {
		return m_true_counter;
	}

	size_t get_false_counter() const {
		return m_false_counter;
	}

private:
	size_t m_true_counter {0};
	size_t m_false_counter {0};
};

using test_handler = std::function<void(bool, test_agent*)>;

using test_agent_proxy =
	ranger::bhvr_tree::agent_proxy<test_agent, test_handler, ranger::bhvr_tree::dummy_mutex>;

template <bool Result>
class behavior_node : public ranger::bhvr_tree::abstract_node<test_agent_proxy> {
public:
	void exec(test_agent_proxy& ap, test_handler hdl) const override {
		ap(hdl, Result);
	}
};

struct true_node : public behavior_node<true> {
	static constexpr const char* name() {
		return "true_node";
	}

	void exec(test_agent_proxy& ap, test_handler hdl) const final {
		ap->increase_true_counter();
		behavior_node<true>::exec(ap, std::move(hdl));
	}
};

struct false_node : public behavior_node<false> {
	static constexpr const char* name() {
		return "false_node";
	}

	void exec(test_agent_proxy& ap, test_handler hdl) const final {
		ap->increase_false_counter();
		behavior_node<false>::exec(ap, std::move(hdl));
	}
};

class test_generator
	: public ranger::bhvr_tree::xml_generator<test_agent_proxy, true_node, false_node> {
public:
	using super = ranger::bhvr_tree::xml_generator<test_agent_proxy, true_node, false_node>;

	using node_type = super::node_type;
	using node_pointer = super::node_pointer;

protected:
	using super::generate_node;

	node_pointer generate_node(	rapidxml::xml_node<>* data,
								ranger::bhvr_tree::generate_node_type<true_node>) const final {
		return node_pointer(new true_node);
	}

	node_pointer generate_node(	rapidxml::xml_node<>* data,
								ranger::bhvr_tree::generate_node_type<false_node>) const final {
		return node_pointer(new false_node);
	}
};

#endif	// TEST_UTIL_HPP
