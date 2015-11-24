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

#ifndef RANGER_BHVR_TREE_DETAIL_AGENT_PROXY_BASE
#define RANGER_BHVR_TREE_DETAIL_AGENT_PROXY_BASE

#include <mutex>
#include <chrono>
#include <map>

namespace ranger { namespace bhvr_tree {

template <class AgentProxy>
class decorator_counter_node;

template <class AgentProxy>
class decorator_timer_node;

namespace detail {

template <class Mutex, class Clock, class AgentProxy>
class agent_proxy_base {
public:
  agent_proxy_base() = default;

  agent_proxy_base(const agent_proxy_base<Mutex, Clock, AgentProxy>&) = delete;
  agent_proxy_base<Mutex, Clock, AgentProxy>& operator = (const agent_proxy_base<Mutex, Clock, AgentProxy>&) = delete;

  bool less_then_increase(const decorator_counter_node<AgentProxy>* key, size_t value) {
    std::lock_guard<Mutex> lock(m_counters_mtx);

    auto it = m_counters.emplace(key, 0).first;
    if (it->second >= value) {
      return false;
    }

    ++it->second;
    return true;
  }

  bool expired_then_update(const decorator_timer_node<AgentProxy>* key,
                           const typename Clock::duration& dur) {
    auto now = Clock::now();
    {
      std::lock_guard<Mutex> lock(m_time_points_mtx);

      auto it = m_time_points.emplace(key, now).first;
      if (now - it->second < dur) {
        return false;
      }

      it->second = now;
      return true;
    }
  }

  void clear_all_state() {
    {
      std::lock_guard<Mutex> lock(m_counters_mtx);
      m_counters.clear();
    }

    {
      std::lock_guard<Mutex> lock(m_time_points_mtx);
      m_time_points.clear();
    }
  }

private:
  std::map<const decorator_counter_node<AgentProxy>*, size_t> m_counters;
  Mutex m_counters_mtx;

  std::map<
    const decorator_timer_node<AgentProxy>*,
    std::chrono::time_point<Clock>
  > m_time_points;
  Mutex m_time_points_mtx;
};

}

} }

#endif  // RANGER_BHVR_TREE_DETAIL_AGENT_PROXY_BASE
