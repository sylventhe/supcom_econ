#pragma once
#include <string>
#include <vector>

typedef std::string UnitID;
class Army;
class Command;
class BluePrint;

class Unit {
public:
  Unit(Army *parent, const UnitID &id, const BluePrint *bp);

  virtual const UnitID& GetUnitID(void) const { return _id; }
  virtual const BluePrint* GetBluePrint(void) const { return _bp; }

  virtual bool IsBuilder() const { return false; }
  virtual double GetBuildPower(void) const;
  virtual bool IsIdle() const;

  virtual void GetStorageCapacities(double &mstorage, double &estorage) const;
  virtual void GetProductionAmounts(double &mproduced, double &eproduced) const;
  virtual void GetUpkeepAmounts(double &mreq, double &ereq) const;
  virtual void Upkeep(const double mval, const double eval);
  virtual void AddCommand(Command *com);
  virtual Command *GetCurrentCommand(void);
  virtual void SetCurrentCommand(Command *com);

  void GameTickBegin(const double time);
  void GameTickComplete(const double time);
  
protected:
  void SignalIdleUnit();
  Army *_parent;
  UnitID _id;
  const BluePrint *_bp;

  Command* _active_command;
  std::vector<Command*> _commands;

  double _production_multiplier;
};

