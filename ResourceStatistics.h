#pragma once

// ----------------------------------------------------------------------
// Resource Statistics
// + Statistics used for the two economy resources we want to track.
//
class ResourceStatistics {
public:
  ResourceStatistics();

  void Clear(void);
  void Add(const ResourceStatistics &peer);
  void CapResource(const double storage_max);
  double GetTotalCollected(void) const;
  double GetStored(void) const { return _stored; }
  double GetTotalProduced(void) const { return _total_produced; }
  double GetTotalReclaimed(void) const { return _total_reclaimed; }
  double GetTotalConsumed(void) const { return _total_building + _total_upkeep; }
  double GetTotalBuilding(void) const { return _total_building; }
  double GetTotalUpkeep(void) const { return _total_upkeep; }
  double GetTotalWasted(void) const;
  void AddReclaim(const double amount);
  void AddProduction(const double amount);
  double Build(const double amount);
  double Upkeep(const double amount);

  double _stored;            /* Amount currently in storage */
  double _max_storage;       /* Amount we can currently store */
  double _total_building;    /* Amount actually spent building units */
  double _total_upkeep;      /* Amount consumed to keep buildings producing (e.g.-mass fabricators) */
  double _total_produced;    /* Amount produced by economy structures (e.g.- power generators) */
  double _total_reclaimed;   /* Amount of Mass reclaimed from the battlefield. */
};

