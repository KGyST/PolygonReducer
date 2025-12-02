#ifndef INFOBOX_PANEL_HPP
#define INFOBOX_PANEL_HPP
//#define ACExtension

#include <optional>

#include "APIEnvir.h"
#include "ACAPinc.h"					// also includes APIdefs.h
#include "../AC27/APICommon.h"

#include "DGModule.hpp"
#include "IAPIToolUIData.hpp"
#include "APIToolUIPanel.hpp"

//#include "PolygonReducer.hpp"
#include "Geometry/Polygon.hpp"
#include "Utils/Resource.hpp"

extern Logger logger;

namespace PolygonReducer {
  class GUIDMixin
  {
  protected:
    API_Guid m_currentPolygonGUID;

  public:
    GUIDMixin();
    ~GUIDMixin() = default;

    void SetCurrentPolyGUID(API_Guid i_currentPolygonGUID);
  };


  template<typename InfoboxPage, typename PageObserver>
  class WrapperMixin:
    public TBUI::APIToolUIPanel
  {
  private:
    PageObserver* m_observer;
    InfoboxPage* m_typedPage;
    DG::TabPage* m_tabPage;

  public:
    WrapperMixin(Int32 refCon)
      : TBUI::APIToolUIPanel(refCon)
      , m_observer()
      , m_typedPage()
      , m_tabPage() {};
    virtual ~WrapperMixin() = default;

    bool	CreatePage(const DG::TabControl& tabControl, TBUI::IAPIToolUIData* data, DG::TabPage** tabPage) {
      m_typedPage = new InfoboxPage(tabControl, data);
      *tabPage = m_typedPage;

      m_observer = new PageObserver(m_typedPage);

      return true;
    }

    void	DestroyPage() {
      delete m_observer;		// this must be first
      m_observer = NULL;

      delete m_tabPage;			// this must be after destroying the observers
      m_tabPage = NULL;

      delete m_typedPage;
      m_typedPage = NULL;
    }
  };

  // --- PolygonReducerPage ---------------------------------------------------------

  class PointNrInfoboxPage :
    public DG::TabPage,
    public GUIDMixin
  {
    friend class	PointNrPageObserver;

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

  public:
    PointNrInfoboxPage(const DG::TabControl& tabControl, TBUI::IAPIToolUIData* puiData);
    ~PointNrInfoboxPage();

    // Getters / Setters
    int GetPointNumber();
    GSErrCode SetPointNumber(int);

    double GetSmallestLength();
    GSErrCode SetSmallestLength(double);

  };

  // --- PolygonReducerPageObserver -------------------------------------------------

  class PointNrPageObserver :
    public	DG::TextEditBaseObserver,
    public	DG::PosIntEditObserver,
    public	DG::RealEditObserver,
    public  DG::CompoundItemObserver,
    public	DG::ButtonItemObserver,
    public	DG::BarControlObserver,
    public  TBUI::IAPIToolUIDataObserver
  {
  private:
    PointNrInfoboxPage* m_tabPage;

  protected:
    virtual	void		APIElementChanged(const TBUI::APIElemDefaultFieldMask& fieldMask) override;
    virtual void		PosIntEditChanged(const DG::PosIntEditChangeEvent& ev);
    virtual void		RealEditChanged(const DG::RealEditChangeEvent& ev);
    virtual void		ButtonClicked(const DG::ButtonClickEvent& ev) override;
    virtual void		BarControlChanged(const DG::BarControlChangeEvent& ev) override;

  public:
    explicit PointNrPageObserver(PointNrInfoboxPage* testPage);
    ~PointNrPageObserver(void);
  };

  // --- Wrapper class --------------------------------------------------------

  class PointNrPanel 
    : public WrapperMixin<PointNrInfoboxPage, PointNrPageObserver > {
  public:
    PointNrPanel(Int32 refCon) : WrapperMixin(refCon) {}
  };

  // ------------------------------------------------------------
  // --- Length ---------------------------------------------------------
  // ------------------------------------------------------------

  class LengthInfoboxPage :
    public DG::TabPage,
    public GUIDMixin
  {
    friend class	LengthPageObserver;

  protected:
    enum Controls {
      iUIStoredLengthId = 1,
      StoredButtonId,
      ApplyButtonId,
      iUICurrentLengthId,
    };
    void SetControls();

    DG::RealEdit			iUIStoredLength;
    DG::Button				StoreButton;
    DG::Button				ApplyButton;
    DG::RealEdit			iUICurrentLength;
    TBUI::IAPIToolUIData* uiData;

  public:
    LengthInfoboxPage(const DG::TabControl& tabControl, TBUI::IAPIToolUIData* p_uiData);
    ~LengthInfoboxPage();

    //// Getters / Setters
    //int GetPointNumber();
    //GSErrCode SetPointNumber(int);

    //double GetSmallestLength();
    //GSErrCode SetSmallestLength(double);

    //void SetCurrentPolyGUID(API_Guid i_currentPolygonGUID);
  };

  // --- PolygonReducerPageObserver -------------------------------------------------

  class LengthPageObserver :
    public	DG::RealEditObserver,
    public	DG::ButtonItemObserver,
    public  DG::CompoundItemObserver
  {
  private:
    LengthInfoboxPage* m_tabPage;

  protected:
    virtual	void		APIElementChanged(const TBUI::APIElemDefaultFieldMask& fieldMask);
    virtual void		RealEditChanged(const DG::RealEditChangeEvent& ev);
    virtual void		ButtonClicked(const DG::ButtonClickEvent& ev) override;

  public:
    explicit LengthPageObserver(LengthInfoboxPage* testPage);
    ~LengthPageObserver(void);
  };

  // --- Wrapper class --------------------------------------------------------

  class LengthPanel
    : public WrapperMixin<LengthInfoboxPage, LengthPageObserver > {
  public:
    LengthPanel(Int32 refCon) : WrapperMixin(refCon) {}
  };
}


#endif // !INFOBOX_PANEL_HPP

