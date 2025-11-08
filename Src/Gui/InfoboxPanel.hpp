#pragma once
#ifndef INFOBOX_PANEL_HPP
#define INFOBOX_PANEL_HPP
#define ACExtension

#include "DGModule.hpp"
#include "IAPIToolUIData.hpp"
#include "APIToolUIPanel.hpp"
#include "../PolygonReducer.hpp"
#include "../Geometry/Polygon.hpp"
#include <optional>
#include "Logger/Logger.hpp"

extern Logger logger;

namespace PolygonReducer {
  S::Polygon* GetFirstPolygonFromSelection(API_Guid* o_guid = NULL);

  // --- PolygonReducerPage ---------------------------------------------------------

  class PolygonReducerInfoboxPage :
    public DG::TabPage
  {
    friend class	PolygonReducerPageObserver;

  protected:
    enum Controls {
      iUIPointNumberId = 2,
      iUISliderId = 3,
      SettingsButtonId = 4,
      GDLButtonId = 5,
      iUISmallestLengthId, 
    };
    void SetControls();

    DG::PosIntEdit			iUIPointNumber;
    DG::Slider				  iSlider;
    DG::Button				  GDLButton;
    DG::Button				  SettingsButton;
    DG::RealEdit        iUISmallestLength;
    TBUI::IAPIToolUIData* uiData;
    API_Guid m_currentPolygonGUID;

  public:
    PolygonReducerInfoboxPage(const DG::TabControl& tabControl, TBUI::IAPIToolUIData* puiData);
    ~PolygonReducerInfoboxPage();
    void SetCurrentPolyGUID(API_Guid i_currentPolygonGUID);
    int GetPointNumber();
    GSErrCode SetPointNumber(int);
    double GetSmallestLength();
    GSErrCode SetSmallestLength(double);
  };


  // --- PolygonReducerPageObserver -------------------------------------------------

  class PolygonReducerPageObserver :
    public	DG::TextEditBaseObserver,
    public	DG::PosIntEditObserver,
    public  DG::CompoundItemObserver,
    public  TBUI::IAPIToolUIDataObserver,
    public	DG::ButtonItemObserver,
    public	DG::BarControlObserver
  {
  private:
    PolygonReducerInfoboxPage* m_tabPage;

    //void			GetUserDataFromDefault(WallUserData* data);
    //void			SetUserDataToDefault(WallUserData* sdata);
  protected:
    virtual	void		APIElementChanged(const TBUI::APIElemDefaultFieldMask& fieldMask) override;
    virtual void		PosIntEditChanged(const DG::PosIntEditChangeEvent& ev);
    virtual void		ButtonClicked(const DG::ButtonClickEvent& ev) override;
    virtual void		BarControlChanged(const DG::BarControlChangeEvent& ev) override;

  public:
    explicit PolygonReducerPageObserver(PolygonReducerInfoboxPage* testPage);
    ~PolygonReducerPageObserver(void);
  };

  // --- Wrapper class --------------------------------------------------------

  class PolygonReducerPanel : public TBUI::APIToolUIPanel
  {
  private:
    PolygonReducerPageObserver* observer;

  public:
    PolygonReducerPanel(Int32 refCon);
    virtual		~PolygonReducerPanel();

    virtual		bool	CreatePage(const DG::TabControl& tabControl, TBUI::IAPIToolUIData* data, DG::TabPage** tabPage) override;
    virtual		void	DestroyPage(void) override;
  };
}
#endif // !INFOBOX_PANEL_HPP
