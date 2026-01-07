#ifndef SETTINGS_SINGLETON_HPP
#define SETTINGS_SINGLETON_HPP

#include "Settings/SettingsSingletonBase.template.hpp"
#include "Gui/Observable.template.hpp"


class PolygonReducerSettings
  : public SettingsSingletonBase<PolygonReducerSettings>
{
  friend class SettingsSingletonBase<PolygonReducerSettings>;
private:
  UINT m_iMinEdgeCount = 3;
  double m_rMinEdgeLength = 0.00;
  Observable <> m_settingsChangedObservable;

public:
  PolygonReducerSettings();
  ~PolygonReducerSettings();

  // Getters / Setters
  UINT GetMinEdgeCount() { return m_iMinEdgeCount; }
  void SetMinEdgeCount(UINT i_iMinEdgeCount) { m_iMinEdgeCount = i_iMinEdgeCount; }

  double GetMinEdgeLength() { return m_rMinEdgeLength; }
  void SetMinEdgeLength(double i_rMinEdgeLength) { m_rMinEdgeLength = i_rMinEdgeLength; }

  void Register(const void* owner, const Observable<>::Callback cb) {
    m_settingsChangedObservable.Register(owner, cb);
  }

  void Unregister(const void* owner) {
    m_settingsChangedObservable.Unregister(owner);
  }

  void Notify() {
    m_settingsChangedObservable.Notify();
  }
};

inline PolygonReducerSettings& SETTINGS = PolygonReducerSettings::GetInstance();
inline Logger& LOGGER = SETTINGS.GetLogger();


#endif // !SETTINGS_SINGLETON_HPP

