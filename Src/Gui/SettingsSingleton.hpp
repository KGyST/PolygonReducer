#ifndef SETTINGS_SINGLETON_HPP
#define SETTINGS_SINGLETON_HPP

#include "Settings/SettingsSingletonBase.template.hpp"

namespace PolygonReducer {
  class PointNrPanel;
  //class PolygonReducer::LengthPanel;
}


class PolygonReducerSettings
  : public SettingsSingletonBase<PolygonReducerSettings>
{
  friend class SettingsSingletonBase<PolygonReducerSettings>;
private:
  UINT m_iMinEdgeCount = 3;
  //LengthPanel* m_lengthInfoBox = NULL;
public:
  PolygonReducer::PointNrPanel* m_pointNrInfoBox = NULL;
  PolygonReducerSettings();
  ~PolygonReducerSettings();

  // Getters / Setters
  UINT GetMinEdgeCount() { return m_iMinEdgeCount; }
  void SetMinEdgeCount(UINT i_iMinEdgeCount) { m_iMinEdgeCount = i_iMinEdgeCount; }

  //PointNrPanel* GetPointNrPanel() const { return m_pointNrInfoBox; }
  //LengthPanel* GetLengthPanel() const { return m_lengthInfoBox; }
  //void SetPointNrPanel(PointNrPanel* panel) { m_pointNrInfoBox = panel; }
  //void SetLengthPanel(LengthPanel* panel) { m_lengthInfoBox = panel; }
};

inline PolygonReducerSettings& SETTINGS = PolygonReducerSettings::GetInstance();
inline Logger& LOGGER = SETTINGS.GetLogger();


#endif // !SETTINGS_SINGLETON_HPP

