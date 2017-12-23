#pragma once
#include <string>

typedef std::string BluePrintID;


struct BluePrintData {
  BluePrintID _id;
  std::string _name;
  double _build_rate;
  double _build_cost_time;
  double _build_cost_mass;
  double _build_cost_energy;
  double _production_mass;
  double _production_energy;
  double _storage_mass;
  double _storage_energy;
  double _upkeep_mass;
  double _upkeep_energy;
};

class BluePrint {
public:
  BluePrint(const BluePrintData& data);
  ~BluePrint();

  const BluePrintData& GetData() const;

private:
  BluePrintData _data;
};
