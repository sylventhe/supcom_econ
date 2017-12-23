#include "Unit.h"
#include "Army.h"
#include "BluePrint.h"
#include "Command.h"

#include <algorithm>
#include <sstream>
#include <iostream>

// ----------------------------------------------------------------------
Unit::Unit(Army * parent, const UnitID & id, const BluePrint * bp) :
_parent(parent),
_id(id),
_bp(bp),
_active_command(nullptr)
{
}

// ----------------------------------------------------------------------
double Unit::GetBuildPower(void) const {
  return _bp->GetData()._build_rate;
}

// ----------------------------------------------------------------------
bool Unit::IsIdle() const {
  return false;
}

// ----------------------------------------------------------------------
void Unit::GetStorageCapacities(double & mstorage, double & estorage) const {
  mstorage = _bp->GetData()._storage_mass;
  estorage = _bp->GetData()._storage_energy;
}

// ----------------------------------------------------------------------
void Unit::GetProductionAmounts(double & mproduced, double & eproduced) const {
  mproduced = _bp->GetData()._production_mass * _production_multiplier;
  eproduced = _bp->GetData()._production_energy * _production_multiplier;
}

// ----------------------------------------------------------------------
void Unit::GetUpkeepAmounts(double & mreq, double & ereq) const {
  mreq = _bp->GetData()._upkeep_mass;
  ereq = _bp->GetData()._upkeep_energy;
}

// ----------------------------------------------------------------------
void Unit::Upkeep(const double mval, const double eval)
{
  double mm = 1.0;
  double me = 1.0;

  double mreq = 0.0;
  double ereq = 0.0;

  GetUpkeepAmounts(mreq, ereq);

  if (mval < mreq) {
    mm = mval / mreq;
  }

  if (eval < ereq) {
    me = eval / ereq;
  }

  _production_multiplier = (std::min)(mm, me);
}

// ----------------------------------------------------------------------
void Unit::AddCommand(Command * com) {
  _commands.push_back(com);
}

// ----------------------------------------------------------------------
Command* Unit::GetCurrentCommand(void) {
  if (nullptr == _active_command) {
    if (_commands.size() > 0) {
      _active_command = _commands.front();
      std::cout << "UNIT, BEGIN, CMD, " << _active_command->ToString() << std::endl;
    }
  }
  return _active_command;
}

// ----------------------------------------------------------------------
void Unit::SetCurrentCommand(Command * com) {
  if (_active_command != nullptr) {
    delete _active_command;
  }

  _active_command = com;
}

// ----------------------------------------------------------------------
void Unit::GameTickBegin(const double time){
}

// ----------------------------------------------------------------------
void Unit::GameTickComplete(const double time){
  Command *cmd = GetCurrentCommand();
  if (nullptr == cmd) { return; }
  std::cout << "UNIT, STATUS, CMD, " << cmd->ToString() << std::endl;

  if (cmd->IsComplete()) {
    Unit *built_unit = cmd->GetCompletedUnit();

    std::ostringstream oss;

    oss << "UNIT, FINISH, CMD, " << cmd->ToString();
    std::cout << oss.str() << std::endl;
    if (nullptr != built_unit) {
      _parent->AddUnit(built_unit);
    }

    _commands.erase(_commands.begin());
    SetCurrentCommand(nullptr);
  }
}

// ----------------------------------------------------------------------
void Unit::SignalIdleUnit(){
}
