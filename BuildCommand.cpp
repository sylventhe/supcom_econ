#include "BuildCommand.h"
#include "BluePrint.h"
#include <sstream>
#include <iostream>
#include <algorithm>

// ----------------------------------------------------------------------
BuildCommand::BuildCommand(Army * army, Unit * owner, const UnitID & built_unit_id, const BluePrint * unit_template)
  : Command(army, owner), _built_unit_id(built_unit_id), _built_unit_bp(unit_template)
{
  _owner_id = _owner->GetUnitID();
  _time_remaining = _built_unit_bp->GetData()._build_cost_time;
  _mass_remaining = _built_unit_bp->GetData()._build_cost_mass;
  _energy_remaining = _built_unit_bp->GetData()._build_cost_energy;

  _build_cost_time = _time_remaining;
  _build_cost_mass = _mass_remaining;
  _build_cost_energy = _energy_remaining;
}

// ----------------------------------------------------------------------
BuildCommand::BuildCommand(Army * army, const UnitID & owner_id, const UnitID & built_unit_id, const BluePrint * unit_template)
  : Command(army, owner_id), _built_unit_id(built_unit_id), _built_unit_bp(unit_template){
  _time_remaining = _built_unit_bp->GetData()._build_cost_time;
  _mass_remaining = _built_unit_bp->GetData()._build_cost_mass;
  _energy_remaining = _built_unit_bp->GetData()._build_cost_energy;

  _build_cost_time = _time_remaining;
  _build_cost_mass = _mass_remaining;
  _build_cost_energy = _energy_remaining;
}

// ----------------------------------------------------------------------
Unit * BuildCommand::GetCompletedUnit(void) const
{
  if (IsComplete()) {
    return new Unit(_army, _built_unit_id, _built_unit_bp);
  }
  return nullptr;
}

// ----------------------------------------------------------------------
void BuildCommand::ComputeRequestedResources(double & req_mass, double & req_energy){
  double fval = ComputeTotalBuildPower() / _build_cost_time;
  req_mass = fval * _build_cost_mass;
  req_energy = fval * _build_cost_energy;

  req_mass = (std::min)(req_mass, _mass_remaining);
  req_energy = (std::min)(req_energy, _energy_remaining);
}

// ----------------------------------------------------------------------
bool BuildCommand::IsComplete(void) const{
  return (_mass_remaining <= 1e-10) && (_energy_remaining <= 1e-10);
}

// ----------------------------------------------------------------------
void BuildCommand::ProcessTick(double mass, double energy) {
  _mass_remaining -= mass;
  _energy_remaining -= energy;
  _time_remaining -= this->ComputeTotalBuildPower();
}

// ----------------------------------------------------------------------
std::string BuildCommand::ToString(void) const {
  std::ostringstream oss;

  oss << "CMD, Build, " << _built_unit_bp->GetData()._name << ", " << _owner_id << ", " 
    << _time_remaining << " ticks, "
    << _mass_remaining << " mass, "
    << _energy_remaining << " energy";
  return oss.str();
}
