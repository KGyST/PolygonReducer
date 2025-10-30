#include "InfoboxPanel.hpp"
#include "../PolygonReducer_Resource.h"
#include "../APIEnvir.h"
#include "ACAPinc.h"
#include "GDLDialog.hpp"
#include "SettingsDialog.hpp"
#include "../PolygonReducer.template.hpp"
#include "../Utils/Utils.hpp"
#include "Array.hpp"
#include <APIdefs_Elements.h>

namespace PolygonReducer {

  //-----------------------   Utility functions //-----------------------

  void SetOriginalPolyGUID(const API_Guid i_elemGuid, const API_Guid i_originalGuid, GSErrCode* o_err = nullptr)
  {
    GSErrCode err;
    API_Elem_Head elemHead{};

    elemHead.guid = i_elemGuid;

    API_ElementUserData userdata;

    userdata.dataVersion = 1;
    userdata.platformSign = GS::Act_Platform_Sign;
    userdata.flags = APIUserDataFlag_FillWith | APIUserDataFlag_Pickup;
    userdata.dataHdl = BMAllocateHandle(sizeof(i_originalGuid), ALLOCATE_CLEAR, 0);

    *reinterpret_cast<API_Guid*> (*userdata.dataHdl) = i_originalGuid;

    err = ACAPI_Element_SetUserData(&elemHead, &userdata);

    if (o_err)
      *o_err = err;
  }

  std::optional<API_Guid> GetOriginalPolyGUID(const API_Guid& i_guid, GSErrCode* o_err = nullptr)
  {
    GSErrCode err;
    API_Elem_Head elemHead{};

    elemHead.guid = i_guid;

    API_ElementUserData userData = {};
    err = ACAPI_Element_GetUserData(&elemHead, &userData);

    if (o_err)
      *o_err = err;

    if (err != NoError || userData.dataHdl == nullptr)
      return std::nullopt;

    return *reinterpret_cast<API_Guid*>(*userData.dataHdl);
  }

  //-----------------------

  S::Polygon* GetPolygonByGUID(const API_Guid i_guid, GSErrCode* o_err)
  {
    GSErrCode err;
    API_Element element{};
    API_ElementMemo _memo;
    element.header.guid = i_guid;

    err = ACAPI_Element_Get(&element);

    if (err != NoError) {
      return nullptr;
    }

    if (IsItPolygon(&element))
    {
      GSErrCode err = ACAPI_Element_GetMemo(element.header.guid, &_memo);

      return new S::Polygon{ &_memo };
    }

    if (o_err)
      *o_err = err;

    return nullptr;
  }


  std::optional<API_Guid> GetFirstPolygonGUIDFromSelection(GSErrCode* o_err = nullptr)
  {
    GSErrCode           err;
    API_SelectionInfo   selectionInfo;
    GS::Array<API_Neig> neigs{};
    API_ElementMemo _memo{};

    err = ACAPI_Selection_Get(&selectionInfo, &neigs, false);
    if (o_err)
      *o_err = err;

    if (err != NoError)
      return std::nullopt;

    for (auto& neig : neigs)
    {
      if (IsItPolygon(neig))
      {
        return neig.guid;
      }
    }

    if (o_err)
      *o_err = APIERR_NOSEL;
    return std::nullopt;
  }

  //----------------------- / Utility functions //-----------------------

  PolygonReducerInfoboxPage::PolygonReducerInfoboxPage(const DG::TabControl& tabControl, TBUI::IAPIToolUIData* p_uiData)
    :DG::TabPage(tabControl, 1, ACAPI_GetOwnResModule(), InfoBoxPageId, InvalidResModule)
    , iUIPointNumber(GetReference(), iUIPointNumberId)
    , iSlider(GetReference(), iUISliderId)
    , GDLButton(GetReference(), GDLButtonId)
    , SettingsButton(GetReference(), SettingsButtonId)
    , uiData(p_uiData)
  {
    int _iPoint = GetPointNumber();

    iUIPointNumber.SetValue(_iPoint);
    DGSetItemMaxLong(InfoBoxPageId, iUISliderId, _iPoint);
    DGSetItemValLong(InfoBoxPageId, iUISliderId, _iPoint);
  }

  PolygonReducerInfoboxPage::~PolygonReducerInfoboxPage() {
    uiData = NULL;
  }

  void PolygonReducerInfoboxPage::SetCurrentPolygon(S::Polygon* currentPolygon)
  {
    m_currentPolygon = currentPolygon;
  }

  GSErrCode PolygonReducerInfoboxPage::SetPointNumber(const int i_iPoint/*, int i_iMax*/)
  {
    GSErrCode   err;
    API_Guid guid{};
    std::optional<API_Guid> original_guid;

    if (auto guidOpt = GetFirstPolygonGUIDFromSelection())
      guid = *guidOpt;
    else
      return APIERR_NOSEL;

    original_guid = GetOriginalPolyGUID(guid);

    err = ACAPI_CallUndoableCommand("Optimize polygons",
      [&]() -> GSErrCode {
        S::Polygon pgon;
        API_ElementMemo _memo{};
        API_Element element{}, mask;
        API_ElementMemo memo{};

        if (original_guid) {
          //pgon = new S::Polygon(&*original_guid);
          pgon = S::Polygon(&*original_guid);
        }
        else {
          //pgon = new S::Polygon(&guid);
          pgon = S::Polygon(&guid);
        }

        logger.Log(GS::UniString("Original polygon: ") + pgon.ToString() );

        pgon.SetPointCount(i_iPoint);

        logger.Log(GS::UniString("Result polygon: ") + pgon.ToString());

        GSErrCode _err = NoError;

        API_Polygon apiPoly = pgon.ToPoly();

        element.header.guid = guid;

        _err = ACAPI_Element_Get(&element);
        ACAPI_ELEMENT_MASK_CLEAR(mask);

        if (!pgon.m_isPolyline)
        {
          element.hatch.poly = apiPoly;
          ACAPI_ELEMENT_MASK_SET(mask, API_HatchType, poly);
        }
        else
        {
          element.polyLine.poly = apiPoly;
          ACAPI_ELEMENT_MASK_SET(mask, API_PolyLineType, poly);
        }

        pgon.GetMemo(memo);

        if (original_guid) {
          _err = ACAPI_Element_Change(&element, &mask, &memo, APIMemoMask_Polygon, true);
        }
        else
        {
          _err = ACAPI_Element_Create(&element, &memo);

          SetOriginalPolyGUID(element.header.guid, guid);
        }

        //delete pgon;

        return _err;
      });

    return err;
  }

  int PolygonReducerInfoboxPage::GetPointNumber()
  {
    S::Polygon* pgon = GetFirstPolygonFromSelection();

    if (pgon != NULL) {
      UINT16 iPoints = (UINT16)pgon->GetPointCount();
      delete pgon;

      return iPoints;
    }
    else {
      return 0; // no polygon found in selection
    }
  }

  S::Polygon* GetFirstPolygonFromSelection(API_Guid* o_guid)
  {
    GSErrCode           err;
    API_SelectionInfo   selectionInfo;
    GS::Array<API_Neig> neigs{};
    API_ElementMemo _memo{};

    err = ACAPI_Selection_Get(&selectionInfo, &neigs, false);

    if (err == APIERR_NOSEL)
      return nullptr;

    if (err != NoError) {
      return nullptr;
    }

    for (auto& neig : neigs)
    {
      if (IsItPolygon(neig))
      {
        GSErrCode err = ACAPI_Element_GetMemo(neig.guid, &_memo);

        if (o_guid) *o_guid = neig.guid;

     return new S::Polygon{ &_memo };
      }
    }

    return nullptr;
  }

  // --- PolygonReducerPageObserver -------------------------------------------------

  void PolygonReducerPageObserver::APIElementChanged(const TBUI::APIElemDefaultFieldMask& fieldMask) {
    if (fieldMask.GetRegDataChanged()) {
      //	// the regdata has changed
      //	int _n = GetPointNumber();
      //	tabPage->iUIPointNumber.SetValue(_n);
    }
  }

  void PolygonReducerPageObserver::PosIntEditChanged(const DG::PosIntEditChangeEvent& ev)
  {
    if (ev.GetSource() == &m_tabPage->iUIPointNumber)
    {
      int iPointVal = ev.GetSource()->GetValue();
      //int iPointMax = ev.GetSource()->GetMax() ;

      DGSetItemValLong(InfoBoxPageId, m_tabPage->iUISliderId, iPointVal);
      //DGSetItemValLong(InfoBoxPageId, iUISliderId, iPointMax);

      m_tabPage->SetPointNumber(iPointVal/*, iPointMax*/);

    }
  }

  void PolygonReducerPageObserver::BarControlChanged(const DG::BarControlChangeEvent& ev)
  {
    if (ev.GetSource() == &m_tabPage->iSlider)
    {
      int iPointVal = ev.GetSource()->GetValue();

      DGSetItemValLong(InfoBoxPageId, m_tabPage->iUIPointNumberId, iPointVal);

      m_tabPage->SetPointNumber(iPointVal/*, iPointMax*/);
    }
  }

  void PolygonReducerPageObserver::ButtonClicked(const DG::ButtonClickEvent& ev)
  {
    if (ev.GetSource() == &m_tabPage->GDLButton)
    {
      GDLDialog* dialog = new GDLDialog();
      std::string _s = m_tabPage->m_currentPolygon->GetGDLcode();
      dialog->setEditText(_s);

      GDLDialogObserver observer(dialog);
      dialog->Invoke();
    }

    if (ev.GetSource() == &m_tabPage->SettingsButton)
    {
      SettingsDialog* dialog = new SettingsDialog();
      SettingsDialogObserver observer(dialog);
      dialog->Invoke();
    }
  }

  PolygonReducerPageObserver::PolygonReducerPageObserver(PolygonReducerInfoboxPage* testPage) :
    m_tabPage(testPage)
  {
    AttachToAllItems(*m_tabPage);
    m_tabPage->uiData->AttachObserver(this);

    TBUI::APIElemDefaultFieldMask mask;
    mask.SetAll();
    APIElementChanged(mask);
  }

  PolygonReducerPageObserver::~PolygonReducerPageObserver(void) {
    DetachFromAllItems(*m_tabPage);
    m_tabPage->uiData->DetachObserver(this);
    m_tabPage = NULL;
  }

  // --- PolygonReducerPage ---------------------------------------------------------

  PolygonReducerPanel::PolygonReducerPanel(Int32 refCon) :
    TBUI::APIToolUIPanel(refCon)
  {

  }

  PolygonReducerPanel::~PolygonReducerPanel() {

  }

  bool	PolygonReducerPanel::CreatePage(const DG::TabControl& tabControl, TBUI::IAPIToolUIData* data, DG::TabPage** tabPage) {
    this->tabPage = new PolygonReducerInfoboxPage(tabControl, data);
    *tabPage = this->tabPage;

    observer = new PolygonReducerPageObserver(dynamic_cast<PolygonReducerInfoboxPage*> (this->tabPage));

    return true;
  }

  void	PolygonReducerPanel::DestroyPage(void) {
    delete observer;		// this must be first
    observer = NULL;

    delete tabPage;			// this must be after destroying the observers
    tabPage = NULL;
  }
}

