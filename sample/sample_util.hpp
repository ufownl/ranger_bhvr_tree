#ifndef SAMPLE_UTIL_HPP
#define SAMPLE_UTIL_HPP

#include "ranger/bhvr_tree/agent_proxy.hpp"
#include "ranger/bhvr_tree/xml_generator.hpp"
#include <caf/all.hpp>
#include <stdio.h>

template <class Agent>
using handler_actor = caf::typed_actor<caf::reacts_to<bool, Agent*>>;

template <class Agent>
class sample_handler {
public:
  template <class T>
  sample_handler(const T& hdl)
    : m_actor(caf::spawn(
      [hdl] (typename handler_actor<Agent>::pointer self)
        -> typename handler_actor<Agent>::behavior_type {
        return {
          [=] (bool result, Agent* agent) {
            hdl(result, agent);
            self->quit();
          }
        };
      }
    )) {
    // nop
  }

  sample_handler(const sample_handler& rhs) : m_actor(rhs.m_actor) {
    // nop
  }

  sample_handler& operator = (const sample_handler& rhs) {
    if (this != &rhs) {
      m_actor = rhs.m_actor;
    }
    return *this;
  }

  sample_handler(sample_handler&& rhs) : m_actor(std::move(rhs.m_actor)) {
    // nop
  }

  sample_handler& operator = (sample_handler&& rhs) {
    if (this != &rhs) {
      m_actor = std::move(rhs.m_actor);
    }
    return *this;
  }

  void operator () (bool result, Agent* agent) const {
    caf::anon_send(m_actor, result, agent);
  }

private:
  handler_actor<Agent> m_actor;
};

template <class Agent>
using sample_agent_proxy = ranger::bhvr_tree::agent_proxy<Agent, sample_handler<Agent>>;

#endif  // SAMPLE_UTIL_HPP
