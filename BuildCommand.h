#pragma once

#include "Army.h"
#include "Unit.h"
#include "Command.h"

class BuildCommand : public Command {
public:
  BuildCommand(Army *army, Unit *owner, const UnitID& built_unit_id, const BluePrint *unit_template);
  BuildCommand(Army *army, const UnitID& owner_id, const UnitID& built_unit_id, const BluePrint *unit_template);

  virtual Unit *GetCompletedUnit(void) const;
  virtual void ComputeRequestedResources(double &req_mass, double &req_energy);
  virtual bool IsComplete(void) const;
  virtual void ProcessTick(double mass, double energy);

  virtual std::string ToString(void) const;

private:
  UnitID _built_unit_id;
  const BluePrint *_built_unit_bp;

  double _time_remaining;
  double _mass_remaining;
  double _energy_remaining;
  double _build_cost_time;
  double _build_cost_mass;
  double _build_cost_energy;
};

