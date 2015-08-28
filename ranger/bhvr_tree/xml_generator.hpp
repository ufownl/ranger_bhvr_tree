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

#ifndef RANGER_BHVR_TREE_XML_GENERATOR_HPP
#define RANGER_BHVR_TREE_XML_GENERATOR_HPP

#include "ranger/bhvr_tree/basic_generator.hpp"
#include <rapidxml/rapidxml.hpp>
#include <vector>
#include <stdlib.h>
#include <string.h>

namespace ranger { namespace bhvr_tree {

template <class AgentProxy, class... Ts>
class xml_generator
	: public basic_generator<AgentProxy, rapidxml::xml_node<>*>::template extend<Ts...> {
public:
	using super =
		typename basic_generator<AgentProxy, rapidxml::xml_node<>*>::template extend<Ts...>;

	using node_type = typename super::node_type;
	using node_pointer = typename super::node_pointer;

	node_pointer generate(const char* str) const {
		return generate(str, strlen(str));
	}

	node_pointer generate(const std::string& str) const {
		return generate(str.c_str(), str.size());
	}

protected:
	using super::generate_node;

	node_pointer generate_node(	rapidxml::xml_node<>* data,
								generate_node_type<selector_node<AgentProxy>>) const final {
		return node_pointer(new selector_node<AgentProxy>);
	}

	node_pointer generate_node(	rapidxml::xml_node<>* data,
								generate_node_type<sequence_node<AgentProxy>>) const final {
		return node_pointer(new sequence_node<AgentProxy>);
	}

	node_pointer generate_node(	rapidxml::xml_node<>* data,
								generate_node_type<parallel_selector_node<AgentProxy>>) const final {
		return node_pointer(new parallel_selector_node<AgentProxy>);
	}

	node_pointer generate_node(	rapidxml::xml_node<>* data,
								generate_node_type<parallel_sequence_node<AgentProxy>>) const final {
		return node_pointer(new parallel_sequence_node<AgentProxy>);
	}

	node_pointer generate_node(	rapidxml::xml_node<>* data,
								generate_node_type<parallel_hybrid_node<AgentProxy>>) const final {
		size_t count = 0;
		bool expected = false;

		auto attr = data->first_attribute("count");
		if (attr) {
			char* end;
			count = strtoul(attr->value(), &end, 10);
		}

		attr = data->first_attribute("expected");
		if (attr && strcmp(attr->value(), "true") == 0) {
			expected = true;
		}

		return node_pointer(new parallel_hybrid_node<AgentProxy>(count, expected));
	}

	node_pointer generate_node(	rapidxml::xml_node<>* data,
								generate_node_type<decorator_not_node<AgentProxy>>) const final {
		return node_pointer(new decorator_not_node<AgentProxy>);
	}

	node_pointer generate_node(	rapidxml::xml_node<>* data,
								generate_node_type<decorator_for_node<AgentProxy>>) const final {
		size_t count = 0;

		auto attr = data->first_attribute("count");
		if (attr) {
			char* end;
			count = strtoul(attr->value(), &end, 10);
		}

		return node_pointer(new decorator_for_node<AgentProxy>(count));
	}

	node_pointer generate_node(	rapidxml::xml_node<>* data,
								generate_node_type<decorator_until_node<AgentProxy>>) const final {
		bool expected = false;

		auto attr = data->first_attribute("expected");
		if (attr && strcmp(attr->value(), "true") == 0) {
			expected = true;
		}

		return node_pointer(new decorator_until_node<AgentProxy>(expected));
	}

	node_pointer generate_node(	rapidxml::xml_node<>* data,
								generate_node_type<decorator_counter_node<AgentProxy>>) const final {
		size_t count = 0;

		auto attr = data->first_attribute("count");
		if (attr) {
			char* end;
			count = strtoul(attr->value(), &end, 10);
		}

		return node_pointer(new decorator_counter_node<AgentProxy>(count));
	}

	node_pointer generate_node(	rapidxml::xml_node<>* data,
								generate_node_type<decorator_timer_node<AgentProxy>>) const final {
		double dur = 0.0;

		auto attr = data->first_attribute("duration");
		if (attr) {
			char* end;
			dur = strtod(attr->value(), &end);
		}

		return node_pointer(new decorator_timer_node<AgentProxy>(std::chrono::duration<double>(dur)));
	}

private:
	node_pointer generate(const char* str, size_t len) const {
		try {
			std::vector<char> buf(str, str + len + 1);
			rapidxml::xml_document<> doc;
			doc.parse<0>(buf.data());

			auto data = doc.first_node("bhvr_tree");
			if (!data) {
				std::cerr << "bhvr_tree_error: cannot find root node" << std::endl;
				return node_pointer();
			}

			auto node = generate_node_by_data(data);
			if (node) {
				generate_children(data, *node);
			}

			return node;
		} catch (const rapidxml::parse_error& e) {
			std::cerr << "parse_error: " << e.what() << " [" << e.where<const char>() << "]" << std::endl;
		}

		return node_pointer();
	}

	node_pointer generate_node_by_data(rapidxml::xml_node<>* data) const {
		auto name = data->first_attribute("class");
		if (!name) {
			std::cerr << "bhvr_tree_error: cannot find class name attribute" << std::endl;
			return node_pointer();
		}

		return this->generate_node_by_name(name->value(), data);
	}

	void generate_children(rapidxml::xml_node<>* data, node_type& node) const {
		for (auto i = data->first_node("bhvr_tree"); i; i = i->next_sibling("bhvr_tree")) {
			auto child = generate_node_by_data(i);
			if (!child) {
				continue;
			}

			generate_children(i, *node.attach_child(std::move(child)));
		}
	}
};

} }

#endif	// RANGER_BHVR_TREE_XML_GENERATOR_HPP
