#ifndef SETTINGS_SINGLETON_HPP
#define SETTINGS_SINGLETON_HPP

#include "Settings/SettingsSingletonBase.template.hpp"
#include "../PolygonReducer_Resource.h"
#include "Utils/Resource.hpp"


class PolygonReducerSettings
  : public SettingsSingletonBase<PolygonReducerSettings>
{
  friend class SettingsSingletonBase<PolygonReducerSettings>;
private:
  UINT m_iMinEdgeCount = 3;
public:
  PolygonReducerSettings()
  : SettingsSingletonBase(GetStringFromResource(IDS_APP_NAME, 1), GetStringFromResource(IDS_APP_NAME, 2)){};
  ~PolygonReducerSettings() = default;
};

inline PolygonReducerSettings& SETTINGS = PolygonReducerSettings::GetInstance();
inline Logger& LOGGER = SETTINGS.GetLogger();


#endif // !SETTINGS_SINGLETON_HPP

