#pragma once

#include "Unit.h"
#include <set>

class Army;

class Command {
public:
  virtual void FindOwner(void);
  virtual Unit *GetCompletedUnit(void) const;
  virtual void ComputeRequestedResources(double &req_mass, double &req_energy);
  virtual bool IsComplete(void) const = 0;
  virtual void ProcessTick(double mass, double energy);

  virtual void AddAssistance(Unit *assistant);
  virtual void RemoveAssistance(Unit *assistant);

  virtual double ComputeTotalBuildPower() const;

  virtual std::string ToString(void) const;
  
protected:
  Command(Army* army, Unit *owner);
  Command(Army* army, const UnitID& owner_id);

  Army *_army;
  Unit *_owner;
  UnitID _owner_id;
  std::set<Unit*> _assistants;
};


