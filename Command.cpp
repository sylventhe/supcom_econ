#include "Command.h"
#include "Unit.h"
#include "Army.h"

#include <strstream>


// ----------------------------------------------------------------------
void Command::FindOwner(void) {
  if (_owner != nullptr) { return; }

  _owner = _army->FindUnit(_owner_id);
}

// ----------------------------------------------------------------------
Unit * Command::GetCompletedUnit(void) const
{
  return nullptr;
}

// ----------------------------------------------------------------------
void Command::ComputeRequestedResources(double & req_mass, double & req_energy) {
  req_mass = 0;
  req_energy = 0;
}


// ----------------------------------------------------------------------
void Command::ProcessTick(double mass, double energy) {
}

// ----------------------------------------------------------------------
void Command::AddAssistance(Unit * assistant){
  _assistants.insert(assistant);
}

// ----------------------------------------------------------------------
void Command::RemoveAssistance(Unit * assistant){
  _assistants.erase(assistant);
}

// ----------------------------------------------------------------------
double Command::ComputeTotalBuildPower() const {
  double retval = _owner->GetBuildPower();
  auto it = _assistants.cbegin();
  for (auto it = _assistants.cbegin(); it != _assistants.cend(); ++it) {
    const Unit* unit = (*it);

    retval += unit->GetBuildPower();
  }

  return retval;
}

// ----------------------------------------------------------------------
std::string Command::ToString(void) const
{
  std::ostrstream oss;

  oss << "CMD, VIRTUAL, " << _owner_id;
  return oss.str();
}


// ----------------------------------------------------------------------
Command::Command(Army *army, Unit * owner) : _army(army), _owner(owner) {
  if (nullptr != owner) {
    _owner_id = owner->GetUnitID();
  }
}


// ----------------------------------------------------------------------
Command::Command(Army * army, const UnitID & owner_id) : _army(army), _owner(nullptr), _owner_id(owner_id) {
}
