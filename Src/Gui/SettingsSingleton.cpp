#include "SettingsSingleton.hpp"
#include "WinReg/WinReg.hpp"


PolygonReducerSettings::PolygonReducerSettings()
  : SettingsSingletonBase(COMPANY_NAME, APP_NAME) {
  m_iMinEdgeCount = GetRegIntOrDefault(GetRegPath(), "MinEdgeCount", 3);
  m_rMinEdgeLength = GetRegDoubleOrDefault(GetRegPath(), "MinEdgeLength", 0.00);
}

PolygonReducerSettings::~PolygonReducerSettings() {
  SetRegInt(GetRegPath(), "MinEdgeCount", m_iMinEdgeCount);
  SetRegDouble(GetRegPath(), "MinEdgeLength", m_rMinEdgeLength);
}

