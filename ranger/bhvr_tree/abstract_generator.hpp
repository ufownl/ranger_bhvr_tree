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

#ifndef RANGER_BHVR_TREE_ABSTRACT_GENERATOR_HPP
#define RANGER_BHVR_TREE_ABSTRACT_GENERATOR_HPP

#include "ranger/bhvr_tree/abstract_node.hpp"
#include <iostream>
#include <string>
#include <map>

namespace ranger { namespace bhvr_tree {

template <class T>
struct generate_node_type {
  // nop
};

template <class AgentProxy, class Data, class T>
class generator_unit {
protected:
  virtual std::unique_ptr<abstract_node<AgentProxy>>
  generate_node(Data, generate_node_type<T>) const = 0;
};

template <class AgentProxy, class Data, class... Ts>
class generator_interface;

template <class AgentProxy, class Data, class T, class... Ts>
class generator_interface<AgentProxy, Data, T, Ts...>
  : public generator_unit<AgentProxy, Data, T>
  , public generator_interface<AgentProxy, Data, Ts...> {
protected:
  using generator_unit<AgentProxy, Data, T>::generate_node;
  using generator_interface<AgentProxy, Data, Ts...>::generate_node;
};

template <class AgentProxy, class Data, class T>
class generator_interface<AgentProxy, Data, T> : generator_unit<AgentProxy, Data, T> {
protected:
  using generator_unit<AgentProxy, Data, T>::generate_node;
};

template <class AgentProxy, class Data, class... Ts>
class abstract_generator : public generator_interface<AgentProxy, Data, Ts...> {
public:
  template <class... Us>
  using extend = abstract_generator<AgentProxy, Data, Ts..., Us...>;

  using node_type = abstract_node<AgentProxy>;
  using node_pointer = std::unique_ptr<node_type>;

  abstract_generator() {
    handler_register<Ts...>::regist(m_generate_handlers);
  }

  virtual ~abstract_generator() {
    // nop
  }

  node_pointer generate_node_by_name(const char* name, Data data) const {
    auto it = m_generate_handlers.find(name);
    if (it == m_generate_handlers.end()) {
      std::cerr << "bhvr_tree_error: cannot find node[" << name << "]'s generate handler" << std::endl;
      return node_pointer();
    }

    return (this->*it->second)(data);
  }

private:
  template <class... Us>
  struct handler_register;

  template <class U, class... Us>
  struct handler_register<U, Us...> {
    template <class T>
    static void regist(T& handlers) {
      handlers.emplace(U::name(), &abstract_generator<AgentProxy, Data, Ts...>::generate_node_helper<U>);
      handler_register<Us...>::regist(handlers);
    }
  };

  template <class U>
  struct handler_register<U> {
    template <class T>
    static void regist(T& handlers) {
      handlers.emplace(U::name(), &abstract_generator<AgentProxy, Data, Ts...>::generate_node_helper<U>);
    }
  };

  template <class T>
  node_pointer generate_node_helper(Data data) const {
    return this->generate_node(data, generate_node_type<T>());
  }

  std::map<
    std::string,
    node_pointer (abstract_generator<AgentProxy, Data, Ts...>::*)(Data) const
  > m_generate_handlers;
};

} }

#endif  // RANGER_BHVR_TREE_ABSTRACT_GENERATOR_HPP
