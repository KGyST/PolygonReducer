#ifndef SETTINGS_SINGLETON_HPP
#define SETTINGS_SINGLETON_HPP

#include "Settings/SettingsSingletonBase.template.hpp"


class PolygonReducerSettings
  : public SettingsSingletonBase<PolygonReducerSettings>
{
  friend class SettingsSingletonBase<PolygonReducerSettings>;
private:
  UINT m_iMinEdgeCount = 3;
  double m_rMinEdgeLength = 0.00;
public:
  PolygonReducerSettings();
  ~PolygonReducerSettings();

  // Getters / Setters
  UINT GetMinEdgeCount() { return m_iMinEdgeCount; }
  void SetMinEdgeCount(UINT i_iMinEdgeCount) { m_iMinEdgeCount = i_iMinEdgeCount; }

  double GetMinEdgeLength() { return m_rMinEdgeLength; }
  void SetMinEdgeLength(double i_rMinEdgeLength) { m_rMinEdgeLength = i_rMinEdgeLength; }
};

inline PolygonReducerSettings& SETTINGS = PolygonReducerSettings::GetInstance();
inline Logger& LOGGER = SETTINGS.GetLogger();


#endif // !SETTINGS_SINGLETON_HPP

