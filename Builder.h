#pragma once

#include "Unit.h"

class Builder : public Unit {
public:
  Builder(Army *parent, const UnitID &id, const UnitTemplate *tmpl);

  virtual bool IsBuilder() const { return true; }
  virtual bool CanBuildTemplate(const UnitTemplate *tmpl);

};

