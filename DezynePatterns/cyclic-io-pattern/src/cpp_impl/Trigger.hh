/**
 * @author Moba B.V. The Netherlands www.moba.nl
 * @copyright Copyright (c) 2019 - All rights reserved
 * @file Trigger.hh
 * @brief ...
 */

#pragma once

#include "ITrigger.hh"
class Trigger : public skel::Trigger
{
public:
  explicit Trigger(const dzn::locator& dzn_locator) :
    skel::Trigger(dzn_locator)
  {
  }

  void run()
  {
    api.out.trigger();
  }
};
