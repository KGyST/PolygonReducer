#ifndef SETTINGS_SINGLETON_HPP
#define SETTINGS_SINGLETON_HPP

#include "Settings/SettingsSingletonBase.template.hpp"


class PolygonReducerSettings
  : public SettingsSingletonBase<PolygonReducerSettings>
{
  friend class SettingsSingletonBase<PolygonReducerSettings>;
private:
  UINT m_iMinEdgeCount = 3;
public:
  PolygonReducerSettings();
  ~PolygonReducerSettings();

  // Getters / Setters
  UINT GetMinEdgeCount() { return m_iMinEdgeCount; }
  void SetMinEdgeCount(UINT i_iMinEdgeCount) { m_iMinEdgeCount = i_iMinEdgeCount; }
};

inline PolygonReducerSettings& SETTINGS = PolygonReducerSettings::GetInstance();
inline Logger& LOGGER = SETTINGS.GetLogger();


#endif // !SETTINGS_SINGLETON_HPP

