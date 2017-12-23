#include "Army.h"
#include "Command.h"
#include "BluePrint.h"

#include <algorithm>
#include <sstream>
#include <cstdio>
#include <iostream>

// ----------------------------------------------------------------------
Army::Army(const std::string & name) : _name(name){
}

// ----------------------------------------------------------------------
void Army::Execute(void) {
  // Assume our initial units and commands have already been added
  
  // 1. Compute Storage Capacity
  ComputeStorageCapacity(_mass._max_storage, _energy._max_storage);

  // 2. Fill any storages
  _mass._stored = _mass._max_storage;
  _energy._stored = _energy._max_storage;

  double gametick = 0.0;
  size_t ii = 0;
  // 3. While we have active commands,
  while (ii < 200) {
    // 3a. Let all units know we are starting a game tick.
    BeginGameTick(gametick);

    // 3A. Consume Upkeep (alters production)
    ConsumeUpkeep();

    // 3B. Produce Resources
    ProduceResources();

    // 3C. Execute Commands
    ExecuteCommands();

    // 3D. Complete the Game Tick
    CompleteGameTick(gametick);

    std::cerr << gametick << ", " << this->ToString() << std::endl;
    gametick += 1.0;
    ++ii;
  }
}

// ----------------------------------------------------------------------
void Army::AddUnit(Unit * unit) {
  _units.insert(std::make_pair(unit->GetUnitID(),unit));

  std::ostringstream oss;

  const BluePrint *bp = unit->GetBluePrint();
  oss << "ADDUNIT, " << unit->GetUnitID() << ", " << bp->GetData()._name;

  std::cout << oss.str() << std::endl;

  double mstore = 0.0;
  double estore = 0.0;
  unit->GetStorageCapacities(mstore, estore);
  _delta_mass._max_storage += mstore;
  _delta_energy._max_storage += estore;
  _mass._max_storage += mstore;
  _energy._max_storage += estore;

  // Look for unqueued commands
  auto it = _unqueued_commands.find(unit->GetUnitID());
  if (it != _unqueued_commands.end()) {
    std::vector<Command*> &cmd_vec = it->second;

    for (size_t ii = 0; ii < cmd_vec.size(); ++ii) {
      cmd_vec[ii]->FindOwner();
      unit->AddCommand(cmd_vec[ii]);
    }

    _unqueued_commands.erase(it);
  }
}

// ----------------------------------------------------------------------
void Army::DestroyUnit(Unit * unit) {
  _units.erase(unit->GetUnitID());
  _idle_units.erase(unit->GetUnitID());
}

// ----------------------------------------------------------------------
void Army::BeginGameTick(const double tick){
  _delta_mass.Clear();
  _delta_energy.Clear();

  for (auto it = _units.begin(); it != _units.end(); ++it) {
    Unit *unit = it->second;
    unit->GameTickBegin(tick);
  }
}

// ----------------------------------------------------------------------
void Army::CompleteGameTick(const double tick) {
  for (auto it = _units.begin(); it != _units.end(); ++it) {
    Unit *unit = it->second;
    unit->GameTickComplete(tick);
  }

  _mass.CapResource(_mass._max_storage);
  _energy.CapResource(_energy._max_storage);
}

// ----------------------------------------------------------------------
void Army::AddCommand(const UnitID & id, Command * cmd) {
  auto it = _units.find(id);
  if (it != _units.end()) {
    it->second->AddCommand(cmd);
  }
  else {
    auto it1 = _unqueued_commands.find(id);
    if (it1 == _unqueued_commands.end()) {
      std::vector<Command*> cmd_vec(0);
      _unqueued_commands.insert(std::make_pair(id, cmd_vec));
      it1 = _unqueued_commands.find(id);
    }

    std::vector<Command*>& cmd_vec = it1->second;
    cmd_vec.push_back(cmd);
  }
}

// ----------------------------------------------------------------------
Unit * Army::FindUnit(const UnitID & id) const
{
  auto it = _units.find(id);
  Unit *retval = nullptr;

  if (it != _units.end()) {
    retval = it->second;
  }

  return retval;
}

// ----------------------------------------------------------------------
double Army::ComputeTotalBuildPowerInUse(void) const {
  double retval = 0.0;
  for (auto it = _units.begin(); it != _units.end(); ++it) {
    Unit *unit = it->second;
    Command *cmd = unit->GetCurrentCommand();
    retval += cmd->ComputeTotalBuildPower();
  }
  return retval;
}

// ----------------------------------------------------------------------
void Army::ComputeTotalUpkeepRequested(double & req_mass, double & req_energy) const {
  req_mass = 0;
  req_energy = 0;

  for (auto it = _units.begin(); it != _units.end(); ++it) {
    Unit *unit = it->second;

    double rm = 0;
    double re = 0;
    unit->GetUpkeepAmounts(rm, re);
    req_mass += rm;
    req_mass += re;
  }
}

// ----------------------------------------------------------------------
void Army::ComputeTotalResourcesRequested(double & req_mass, double & req_energy) const {
  req_mass = 0;
  req_energy = 0;

  for (auto it = _units.begin(); it != _units.end(); ++it) {
    Unit *unit = it->second;
    Command *cmd = unit->GetCurrentCommand();

    double rm = 0;
    double re = 0;
    cmd->ComputeRequestedResources(rm, re);
    req_mass += rm;
    req_mass += re;
  }
}

// ----------------------------------------------------------------------
double Army::ComputeBuildRateMultplier(double & req_mass, double & req_energy) const {
  double m_val = 1.0;
  double e_val = 1.0;

  if (req_mass > _mass.GetStored()) {
    m_val = _mass.GetStored() / req_mass;
  }

  if (req_energy > _energy.GetStored()) {
    e_val = _energy.GetStored() / req_energy;
  }

  return (std::min)(m_val, e_val);
}

// ----------------------------------------------------------------------
void Army::ConsumeUpkeep(void) {
  double mreq = 0.0;
  double ereq = 0.0;

  ComputeTotalUpkeepRequested(mreq, ereq);

  double production_multiplier = ComputeBuildRateMultplier(mreq, ereq);

  for (auto it = _units.begin(); it != _units.end(); ++it) {
    Unit *unit = it->second;

    double rm = 0;
    double re = 0;
    unit->GetUpkeepAmounts(rm, re);
    rm *= production_multiplier;
    re *= production_multiplier;
    unit->Upkeep(rm, re);

    _delta_mass.Upkeep(rm);
    _delta_energy.Upkeep(re);

    _mass.Upkeep(rm);
    _energy.Upkeep(re);
  }
}

// ----------------------------------------------------------------------
void Army::ProduceResources(void) {
  double mprod = 0.0;
  double eprod = 0.0;

  for (auto it = _units.begin(); it != _units.end(); ++it) {
    Unit *unit = it->second;

    double rm = 0;
    double re = 0;
    unit->GetProductionAmounts(mprod, eprod);
    _mass.AddProduction(mprod);
    _energy.AddProduction(eprod);

    _delta_mass.AddProduction(mprod);
    _delta_energy.AddProduction(eprod);
  }
}

// ----------------------------------------------------------------------
void Army::ExecuteCommands(void) {
  double req_mass = 0.0;
  double req_energy = 0.0;
  double build_power = 0.0;

  for (auto it = _units.begin(); it != _units.end(); ++it) {
    Unit *unit = it->second;
    Command *cmd = unit->GetCurrentCommand();
    // This means the unit is idle
    if (nullptr == cmd) { continue; }

    double rm = 0;
    double re = 0;
    cmd->ComputeRequestedResources(rm, re);
    req_mass += rm;
    req_energy += re;
    build_power += cmd->ComputeTotalBuildPower();
  }

  double build_multiplier = ComputeBuildRateMultplier(req_mass, req_energy);

  for (auto it = _units.begin(); it != _units.end(); ++it) {
    Unit *unit = it->second;
    Command *cmd = unit->GetCurrentCommand();
    // This means the unit is idle
    if (nullptr == cmd) { continue; }

    double rm = 0;
    double re = 0;
    cmd->ComputeRequestedResources(rm, re);
    rm *= build_multiplier;
    re *= build_multiplier;
    cmd->ProcessTick(rm, re);

    _mass.Build(rm);
    _energy.Build(re);

    _delta_mass.Build(rm);
    _delta_energy.Build(re);
  }
}

// ----------------------------------------------------------------------
std::string Army::ToString(void) const
{
  std::ostringstream oss;

  oss << "ARMY, "
    << _mass.GetStored() << " (" << _delta_mass.GetStored() << ")," << _mass._max_storage << " (" << _delta_mass._max_storage << "), "
    << _energy.GetStored() << " (" << _delta_energy.GetStored() << ")," << _energy._max_storage << " (" << _delta_energy._max_storage << ")";
  std::string retval = oss.str();
  return retval;
}

// ----------------------------------------------------------------------
void Army::ComputeStorageCapacity(double & mstorage, double & estorage) const {
  mstorage = 0;
  estorage = 0;

  double ms = 0;
  double es = 0;

  for (auto it = _units.begin(); it != _units.end(); ++it) {
    Unit *unit = it->second;

    unit->GetStorageCapacities(ms, es);
    mstorage += ms;
    estorage += es;
  }
}
