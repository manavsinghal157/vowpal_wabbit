// Copyright (c) by respective owners including Yahoo!, Microsoft, and
// individual contributors. All rights reserved. Released under a BSD (revised)
// license as described in the file LICENSE.

#pragma once

#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

#include "test_common.h"
#include "rand_state.h"

#include <functional>
#include <map>
#include <vector>
#include <string>

namespace VW
{
struct workspace;
}

namespace simulator
{
class cb_sim;
// maps an int: # learned examples
// with a function to 'test' at that point in time in the simulator
using callback_map = typename std::map<size_t, std::function<bool(cb_sim&, VW::workspace&, multi_ex&)>>;

class cb_sim
{
  const float USER_LIKED_ARTICLE = -1.f;
  const float USER_DISLIKED_ARTICLE = 0.f;
  const std::vector<std::string> users;
  const std::vector<std::string> times_of_day;
  const std::vector<std::string> actions;
  rand_state random_state;
  float cost_sum = 0.f;
  std::vector<float> ctr;
  size_t callback_count;

public:
  std::string user_ns;
  std::string action_ns;

  cb_sim(uint64_t = 0);
  float get_cost(const std::map<std::string, std::string>&, const std::string&);
  std::vector<std::string> to_vw_example_format(
      const std::map<std::string, std::string>&, const std::string&, float = 0.f, float = 0.f);
  std::pair<int, float> sample_custom_pmf(std::vector<float>& pmf);
  std::pair<std::string, float> get_action(VW::workspace* vw, const std::map<std::string, std::string>&);
  const std::string& choose_user();
  const std::string& choose_time_of_day();
  std::vector<float> run_simulation(VW::workspace*, size_t, bool = true, size_t = 1);
  std::vector<float> run_simulation_hook(VW::workspace*, size_t, callback_map&, bool = true, size_t = 1);

private:
  void call_if_exists(VW::workspace&, multi_ex&, const callback_map&, const size_t);
};

std::vector<float> _test_helper(const std::string&, size_t = 3000, int = 10);
std::vector<float> _test_helper_save_load(const std::string&, size_t = 3000, int = 10);
std::vector<float> _test_helper_hook(const std::string&, callback_map&, size_t = 3000, int = 10);
}  // namespace simulator
