#include "ResourceStatistics.h"

#include <algorithm>

// ----------------------------------------------------------------------
ResourceStatistics::ResourceStatistics() : _stored(0), _total_reclaimed(0), _total_produced(0), _total_building(0), _total_upkeep(0)  {
}

// ----------------------------------------------------------------------
void ResourceStatistics::Clear(void)
{
  _stored = 0.0;
  _max_storage = 0.0;
  _total_building = 0.0;
  _total_upkeep = 0.0;
  _total_produced = 0.0;
  _total_reclaimed = 0.0;
}

// ----------------------------------------------------------------------
void ResourceStatistics::Add(const ResourceStatistics &peer) {
  _stored += peer._stored;
  _max_storage += peer._max_storage;
  _total_building += peer._total_building;
  _total_upkeep += peer._total_upkeep;
  _total_reclaimed += peer._total_reclaimed;
}

// ----------------------------------------------------------------------
void ResourceStatistics::CapResource(const double storage_max) {
  if (_stored > storage_max) {
    _stored = storage_max;
  }
}

// ----------------------------------------------------------------------
double ResourceStatistics::GetTotalCollected(void) const {
  return _total_produced + _total_reclaimed;
}

// ----------------------------------------------------------------------
double ResourceStatistics::GetTotalWasted(void) const {
  return _total_produced - GetTotalConsumed();
}

// ----------------------------------------------------------------------
void ResourceStatistics::AddReclaim(const double amount){
  _stored += amount;
  _total_reclaimed += amount;
}

// ----------------------------------------------------------------------
void ResourceStatistics::AddProduction(const double amount){
  _stored += amount;
  _total_produced += amount;
  
}

// ----------------------------------------------------------------------
double ResourceStatistics::Build(const double amount) {
  _stored -= amount;
  _total_building += amount;

  return amount;
}

// ----------------------------------------------------------------------
double ResourceStatistics::Upkeep(const double amount) {
  _stored -= amount;
  _total_upkeep += amount;

  return amount;
}

