#include "UnitTemplate.h"

// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
void UnitTemplate::SetAttributeValue(const AttributeIndex & attr, const AttributeValue & value) {
  _attributes[attr] = value;
}

// ----------------------------------------------------------------------
const AttributeValue & UnitTemplate::GetAttributeValue(const AttributeIndex & attr) const {
  return _attributes[attr];
}

// ----------------------------------------------------------------------
double UnitTemplate::GetAttributeValueAsDouble(const AttributeIndex & attr) const {
  const AttributeValue &value = GetAttributeValue(attr);

  return std::stod(value);
}
