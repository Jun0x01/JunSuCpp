/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.04.08
 */
#if !defined(SU_SCENECONTROL_INCLUDED)
#define SU_SCENECONTROL_INCLUDED

#include "Data/exportsDefine.h"

#include "MapEditor/UGMapEditorWnd.h"
#include "Graphics/UGGraphicsManager.h"
#include "Drawing/UGDrawParamaters.h"
#include "Data/Workspace.h"
#include "Scene/UGScene3D.h"
#include "SceneEditor/UGSceneEditorWnd.h"
#include "Scene/UGRoot3D.h"
#include "Graphics3D/UGRenderTarget.h"
#include "Scene/UGLayer3Ds.h"

#include "Layer3DDataset/UGLayer3DDatasetModelPro.h"
#include "Layer3DDataset/UGLayer3DDatasetVector.h"
#include "Layer3DDataset/UGLayer3DDatasetVectorPoint.h"
#include "Layer3DDataset/UGLayer3DDatasetVectorLR.h"

namespace UGC
{
	class UGWorkspace;
	class UGSceneEditorWnd;
	class UGRoot3D;
	class UGCameraWorld;
	class UGTrackingLayre3D;
}

using namespace UGC;
using namespace SuperMap;

namespace SuperMap
{
	//namespace Mapping
    //{
	   class SU_MAPPING_API SceneControl
	   {
		   //Constructor
	   public:
		   /*
		    *@en
			*@pWndHandle  window's handle, (HWND)CWnd::m_hWnd in MFC, (HWND)QWidget::winId() in Qt
			*@dpiX        logical dpi in horizontal
			*@dpiY        logical dpi in vertical
		   */
		   SceneControl(void* pWndHandle, int dpiX, int dpiY);
		   virtual ~SceneControl();

		   // members
	   private:
		   //@en the handle of view which providing a window for scene
		   void* m_pWnd;
		   
		   //@en the handle of Workspace, passed in through function SetWorkspace()  
		   Workspace* m_pWorkspace;

		   Workspace* m_pInnerWorkspace;

	   private:
		   // Scene window
		   UGSceneEditorWnd* m_pUGSceneWnd;
		   static UGRoot3D* m_pRoot3D;
		   UGCameraWorld* m_pCameraWorld;

		   // minimize the window
		   UGbool m_IsMinSized;
		   
		   bool mIsInWorkspace;

		   static bool mIsInitializeUGRoot3D;
		   // methods
	   private:
		   void Initialize(void* pWndHandle, int dpiX, int dpiY);
		   UGuint getKeyUGFlags(unsigned int flag);

	   // public functions
	   public:

	   private: // TODO: 
		   // Coordinate system conversion from pixel to scene on the window which contain the scene
		   UGPoint3D PixelToSceneGlobal(int x, int y);

		   // Coordinate system conversion from pixel to scene on the window which contain the scene.
		   UGPoint3D PixelToSceneGlobal(UGPoint pt);

		   // Coordinate system conversion from scene to pixel on the window which contain the scene.
		   UGPoint SceneGlobalToPixel(double x, double y, double z);

		   // Coordinate system conversion from scene to pixel on the window which contain the scene.
		   UGPoint SceneGlobalToPixel(UGPoint3D pnt);

	   public:
		   // Refresh the scene.
		   void Refresh();

		   // Call it in a timer to render the scene.
		   void RenderInTimer();

		   // Return the handle of UGSceneEditorWnd if need to use it, but don't delete it which is belong to SceneControl
		   UGSceneEditorWnd* GetSceneEditWnd();


	   // Called in window event. The pHDC is neccessary in MFC or .Net framework.
	   public:
		   // Called in OnPaint() or OnDraw() of client window to piant the scene on the window
		   void OnDraw(int rectLeft, int rectTop, int rectRight, int rectBottom, void* pHDC = NULL);

		   // Called when the window's size was changed.
		   void OnSizeChanged(int x, int y, void* pHDC=NULL);

		   // Called when left button down.
		   void OnLMouseDown(unsigned int nFlags, int x, int y, void* pHDC = NULL);

		   // Called when left button up.
		   void OnLMouseUp(unsigned int nFlags, int x, int y, void* pHDC = NULL);

		   // Called when Left button double clicked.
		   // Zoom in the scene if the scene is in pan mode.
		   void OnLMouseDbClick(unsigned int nFlags, int x, int y, void* pHDC = NULL);

		   // Called when mouse move.
		   // Pan the scene with left mouse button in pan mode.
		   // Pan the scene with middle mouse button in anyone mode if OnMidMouseDown() and OnMidMouseUp() is called when middle event accurs.
		   void OnMouseMove(unsigned int nFlags, int x, int y, void* pHDC = NULL);

		   // Called when middle button scrolled.
		   // Zoom in or Zoom out the scene
		   void OnMouseWheel(unsigned int nFlags, short zDelta, int x, int y);

		   // Called when right button down.
		   // Rigth button is used to finish current operation in edit or draw mode, so call this function when right button down
		   void OnRMouseDown(unsigned int nFlags, int x, int y, void* pHDC = NULL);

		   // Called when right button up.
		   // Rigth button is used to finish current operation in edit or draw mode, so call this function when right button up
		   void OnRMouseUp(unsigned int nFlags, int x, int y, void* pHDC = NULL);

		   // Called when middle buttun down if you need pan the scene with middle buttom
		   void OnMidMouseDown(unsigned int nFlags, int x, int y, void* pHDC = NULL);

		   // Called when middle buttun up if you need pan the scene with middle buttom
		   void OnMidMouseUp(unsigned int nFlags, int x, int y, void* pHDC = NULL);

		   // User interface
		public:
			/*
			 * @en
			 * @brief Set a Workspace handler, so that SceneControl can access scene data in the workspace. 
			 * @pWorkspace   pointer of Workspace which contains scene data will be shown on scene window by SceneControl
			 */
			void SetWorkspace(Workspace* pWorkspace);

			/*
			 * @en
			 * @brief Get UGLayers handler of current scense, which manager layers in the scense.
			 * @return pointer of UGLayers3Ds
			 */
			UGLayer3Ds* GetUGLayers();

		private: // TODO:
			/*
			 * @en
			 * @brief Set a layer editable or not in current scense.
			 * @pLayer       pointer of UGLayer
			 * @isEditable   true or false
			 */
			void SetEditableLayer(UGLayer* pLayer, bool isEditable);

       public:
			/*
			 * @en
			 * @brief       Open a scene according its name
			 * @scenseName  The scene's name
			 * @return      Return true if open, or false
			 */
			bool OpenScene(string scenseName);
			
			/*
			 * @en
			 * @brief   Save current scene.
			 * @return  Return true if saved or return false.
			 */
			bool Save();

			/*
			 * @en
			 * @brief       Save current scene with a new name.
			 * @scenseName  Scene's new name.
			 * @return      Return true if saved or return false.
			 */
			bool SaveAs(string scenseName);

			/*
			 * @en
			 * @brief Activate current scene for rendering, call it when you switch from one SceneControl to  another.
			 */
			void ActivateScene();

			/*
			 * @en
			 * @brief Add a dataset on scene according to its name and the datasource it belongs to.
			 * @datasourceName Name of datasource which contains the dataset.
			 * @datasetName    Dataset name
			 * @return         Return a pointer of UGLayer3D if the dataset was added, or return NULL.
			 */
			UGLayer3D* AddLayerFromDataset(string datasourceSName, string datasetName);

			UGLayer3D* AddLayerFromFile(string filePath);

			UGTerrainAccessor* AddTerrainLayerFromFile(string filePath);

			//UGLayer3D* AddLayerFromWeb();

			// 获取所有选中对象的选择集。通常情况下，使用点击选择对象，只有个图层有选中对象，因此只有一个选择集
			// 若没有选中对象，返回NULL; 不需要使用时，请先RemoveAll(), 再delete
			UGArray<UGSelection3D*>* GetGeoSelections();

			/*
			 * 将指定的选择集转为记录集。默认获得的记录集不能修改记录，只能用于获取字段值。
			 * 不需要使用得到的UGRecordset时，请通过如下代码释放内存：
			 *     pRecordset->GetDataset()->ReleaseRecordset(pRecordset)
			 *     pRecordset = NULL
			 *
			 * @pSelection  用于获得记录集的UGSelection3D*
			 * @isEditable  返回的记录集是否可修改，默认false, 不可修改记录。
			 */
			UGRecordset* ToRecordset(UGSelection3D* pSelection, bool isEditable=false);

	   };
	//}
}

#endif // !SU_MAPCONTROL_INCLUDED
