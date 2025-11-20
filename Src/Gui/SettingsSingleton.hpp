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
  PolygonReducerSettings()
  : SettingsSingletonBase(COMPANY_NAME, APP_NAME) {}
  ~PolygonReducerSettings() = default;
};

inline PolygonReducerSettings& SETTINGS = PolygonReducerSettings::GetInstance();
inline Logger& LOGGER = SETTINGS.GetLogger();


#endif // !SETTINGS_SINGLETON_HPP

