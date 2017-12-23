#pragma once
#include <vector>
#include <string>

// All of the attributes a unit can have that we actually care about
enum AttributeIndex {
  ATTR_TEMPLATE_ID = 0,
  ATTR_TEMPLATE_NAME,
  ATTR_BUILD_RATE,
  ATTR_BUILD_COST_TIME,
  ATTR_BUILD_COST_MASS,
  ATTR_BUILD_COST_ENERGY,
  ATTR_PRODUCTION_MASS,
  ATTR_PRODUCTION_ENERGY,
  ATTR_STORAGE_MASS,
  ATTR_STORAGE_ENERGY,
  ATTR_UPKEEP_COST_MASS,
  ATTR_UPKEEP_COST_ENERGY
};

typedef std::string TemplateID;
typedef std::string AttributeName;
typedef std::string AttributeValue;
typedef std::vector<AttributeValue> AttributeValueVector;


// This is the blueprint of the unit, describing the attributes of the unit
class UnitTemplate {
public:
  UnitTemplate(const TemplateID &id, const AttributeValueVector &attrs) : _id(id), _attributes(attrs) { }

  const TemplateID& GetTemplateID() const { return _id; }
  const AttributeValue& GetTemplateName() const { return _attributes[ATTR_TEMPLATE_NAME]; }
  const AttributeValueVector& GetAttributes() const { return _attributes; }
  virtual void SetAttributeValue(const AttributeIndex& attr, const AttributeValue &value);
  const AttributeValue& GetAttributeValue(const AttributeIndex& attr) const;
  double GetAttributeValueAsDouble(const AttributeIndex &attr) const;

private:
  TemplateID _id;
  AttributeValueVector _attributes;
};
