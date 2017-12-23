#pragma once

#include <vector>
#include <string>
#include <map>
#include "ResourceStatistics.h"
#include "Unit.h"

class UnitTemplate;
class Builder;
class Command;

class Army {
public:
  Army(const std::string &name);

  void Execute(void);
  void AddUnit(Unit *unit);
  void DestroyUnit(Unit *unit);
  void BeginGameTick(const double tick);
  void CompleteGameTick(const double tick);
  void AddCommand(const UnitID& id, Command* cmd);
  Unit *FindUnit(const UnitID& id) const;

  double ComputeTotalBuildPowerInUse(void) const;
  void ComputeTotalUpkeepRequested(double &req_mass, double &req_energy) const;
  void ComputeTotalResourcesRequested(double &req_mass, double &req_energy) const;
  double ComputeBuildRateMultplier(double &req_mass, double &req_energy) const;
  void ConsumeUpkeep(void);
  void ProduceResources(void);
  void ExecuteCommands(void);

  std::string ToString(void) const;
  
private:
  void ComputeStorageCapacity(double &mstorage, double &estorage) const;

  std::string _name;
  // Amount of each resource affected for this game tick
  ResourceStatistics _delta_mass;
  ResourceStatistics _delta_energy;

  // Amount of each resource affected for the entire simulation
  ResourceStatistics _mass;
  ResourceStatistics _energy;

  std::map<UnitID, Unit*> _units;
  std::map<UnitID, Unit*> _idle_units;
  std::map<UnitID, std::vector<Command*>> _unqueued_commands;
};

