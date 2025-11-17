#include "InfoboxPanel.hpp"
//#include "../PolygonReducer_Resource.h"
#include "../APIEnvir.h"
#include "ACAPinc.h"
#include "GDLDialog.hpp"
#include "SettingsDialog.hpp"
#include "../PolygonReducer.template.hpp"
#include "../Utils/Utils.hpp"
#include "Array.hpp"
#include <APIdefs_Elements.h>
#include "../Geometry/Polygon.hpp"
#include <functional>

//-----------------------   Utility functions //-----------------------

static void SetOriginalPolyGUID(const API_Guid i_elemGuid, const API_Guid i_originalGuid, GSErrCode* o_err = nullptr)
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

static std::optional<API_Guid> GetOriginalPolyGUID(const API_Guid& i_guid, GSErrCode* o_err = nullptr)
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

static S::Polygon* GetPolygonByGUID(const API_Guid i_guid, GSErrCode* o_err = nullptr)
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

static S::Polygon* GetFirstPolygonFromSelection(API_Guid* o_guid = nullptr)
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

static std::optional<API_Guid> GetFirstPolygonGUIDFromSelection(GSErrCode* o_err = nullptr)
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

static GSErrCode _Wrapper(std::function<void(S::Polygon&)> i_func)
{
  GSErrCode   err;
  API_Guid guid{};
  std::optional<API_Guid> original_guid;
  API_ElementMemo memo{};
  API_Element element{}, mask;

  if (auto guidOpt = GetFirstPolygonGUIDFromSelection())
    guid = *guidOpt;
  else
    return APIERR_NOSEL;

  original_guid = GetOriginalPolyGUID(guid);

  S::Polygon pgon;

  if (original_guid) {
    pgon = S::Polygon(&*original_guid);
  }
  else {
    pgon = S::Polygon(&guid);
  }

  logger.Log(GS::UniString("Original polygon: ") + pgon.ToString());

  i_func(pgon);

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

  err = ACAPI_CallUndoableCommand("Optimize polygons",
    [&]() -> GSErrCode {
      if (original_guid) {
        _err = ACAPI_Element_Change(&element, &mask, &memo, APIMemoMask_Polygon, true);
      }
      else
      {
        _err = ACAPI_Element_Create(&element, &memo);

        SetOriginalPolyGUID(element.header.guid, guid);

        ACAPI_Selection_DeselectAll();

        GS::Array<API_Neig> selNeigs;
        API_Neig neig(element.header.guid);
        selNeigs.Push(neig);

        return ACAPI_Selection_Select(selNeigs, true);
      }

      return _err;
    });

  return err;
}

//----------------------- / Utility functions //-----------------------

namespace PolygonReducer {
  void PolygonReducerInfoboxPage::SetControls()
  {
    auto _originalGuid = GetOriginalPolyGUID(m_currentPolygonGUID);
    API_Guid originalGuid = _originalGuid.value_or(m_currentPolygonGUID);

    S::Polygon* currentPgon = GetPolygonByGUID(m_currentPolygonGUID), * originalPgon = GetPolygonByGUID(originalGuid);
    
    iUIPointNumber.SetValue(currentPgon->GetPointCount());

    DGSetItemValLong(InfoBoxPageId, iUISliderId, currentPgon->GetPointCount());
    DGSetItemMaxLong(InfoBoxPageId, iUISliderId, originalPgon->GetPointCount());

    iUISmallestLength.SetValue(currentPgon->GetShortestEdgeLength());
  }

  PolygonReducerInfoboxPage::PolygonReducerInfoboxPage(const DG::TabControl& tabControl, TBUI::IAPIToolUIData* p_uiData)
    :DG::TabPage(tabControl, 1, ACAPI_GetOwnResModule(), InfoBoxPageId, InvalidResModule)
    , iUIPointNumber(GetReference(), iUIPointNumberId)
    , iSlider(GetReference(), iUISliderId)
    , GDLButton(GetReference(), GDLButtonId)
    , SettingsButton(GetReference(), SettingsButtonId)
    , iUISmallestLength(GetReference(), iUISmallestLengthId)
    , uiData(p_uiData)
    , m_currentPolygonGUID()
  {
    auto FirstGUID = GetFirstPolygonGUIDFromSelection();

    if (FirstGUID)
    {
      SetCurrentPolyGUID(*FirstGUID);
    }
  }

  PolygonReducerInfoboxPage::~PolygonReducerInfoboxPage() {
    uiData = NULL;
  }

  // Getters / Setters

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

  GSErrCode PolygonReducerInfoboxPage::SetPointNumber(const int i_iPoint)
  {
    auto func = [i_iPoint](S::Polygon& i_poly) -> void {
      return i_poly.SetPointCount(i_iPoint);
      };

    return _Wrapper(func);
  }

  double PolygonReducerInfoboxPage::GetSmallestLength()
  {
    S::Polygon* pgon = GetFirstPolygonFromSelection();

    if (pgon != NULL) {
      double _sel = pgon->GetShortestEdgeLength();
      delete pgon;

      return _sel;
    }
    else {
      return 0.00; // no polygon found in selection
    }
  }

  GSErrCode PolygonReducerInfoboxPage::SetSmallestLength(double i_length)
  {
    auto func = [i_length](S::Polygon& i_poly) -> void {
      return i_poly.SetShortestEdgeLength(i_length);
      };

    return _Wrapper(func);
  }

  void PolygonReducerInfoboxPage::SetCurrentPolyGUID(API_Guid i_currentPolygonGUID)
  {
    m_currentPolygonGUID = i_currentPolygonGUID;

    SetControls();
  }

  // --- PolygonReducerPageObserver -------------------------------------------------

  void PolygonReducerPageObserver::APIElementChanged(const TBUI::APIElemDefaultFieldMask& fieldMask) {
    if (fieldMask.GetRegDataChanged()) {
      auto FirstGUID = GetFirstPolygonGUIDFromSelection();

      if (FirstGUID)
      {
        m_tabPage->SetCurrentPolyGUID(*FirstGUID);
      }
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
      S::Polygon* pgon = GetPolygonByGUID(m_tabPage->m_currentPolygonGUID);
      std::string _s = pgon->GetGDLcode();
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

