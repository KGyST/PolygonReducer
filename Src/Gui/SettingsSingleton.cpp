#include "SettingsSingleton.hpp"
#include "WinReg/WinReg.hpp"


PolygonReducerSettings::PolygonReducerSettings()
  : SettingsSingletonBase(COMPANY_NAME, APP_NAME) {
  m_iMinEdgeCount = GetRegIntOrDefault(GetRegPath(), "MinEdgeCount", 3);
}

PolygonReducerSettings::~PolygonReducerSettings() {
  SetRegInt(GetRegPath(), "MinEdgeCount", m_iMinEdgeCount);
}

