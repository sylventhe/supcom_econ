#include "BluePrint.h"

BluePrint::BluePrint(const BluePrintData & data) : _data(data) {
}

BluePrint::~BluePrint() {
}

const BluePrintData & BluePrint::GetData() const
{
  return _data;
}
