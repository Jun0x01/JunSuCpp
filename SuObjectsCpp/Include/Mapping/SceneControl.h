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
			*@pInvalidateCallBack  A callback function pointer through which to invalidate the Window if map content is updated
			*@pWnd  the Window which owns the invalidate callback function
		   */
		   SceneControl(void* pWndHandle, int dpiX, int dpiY);
		   virtual ~SceneControl();

		   // members
	   private:
		   //@en the handle of view which providing a window for map
		   void* m_pWnd;
		   
		   //@en the handle of Workspace, passed in through function SetWorkspace()  
		   Workspace* m_pWorkspace;

	   private:
		   // Map window
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

		   // Coordinate system conversion from pixel to map on the window which contain the map
		   UGPoint2D PixelToMap(int x, int y);

		   // Coordinate system conversion from pixel to map on the window which contain the map.
		   UGPoint2D PixelToMap(UGPoint pt);

		   // Coordinate system conversion from map to pixel on the window which contain the map.
		   UGPoint MapToPixel(double x, double y);

		   // Coordinate system conversion from map to pixel on the window which contain the map.
		   UGPoint MapToPixel(UGPoint2D pnt);

		   // Refresh the map.
		   void Refresh();

		   // Call it in a timer to render the scene.
		   void RenderInTimer();

		   // Return the handle of UGSceneEditorWnd if need to use it, but don't delete it which is belong to SceneControl
		   UGSceneEditorWnd* GetSceneEditWnd();


	   // Called in window event. The pHDC is neccessary in MFC or .Net framework.
	   public:
		   // Called in OnPaint() or OnDraw() of client window to piant the map on the window
		   void OnDraw(int rectLeft, int rectTop, int rectRight, int rectBottom, void* pHDC = NULL);

		   // Called when the window's size was changed.
		   void OnSizeChanged(int x, int y, void* pHDC=NULL);

		   // Called when left button down.
		   void OnLMouseDown(unsigned int nFlags, int x, int y, void* pHDC = NULL);

		   // Called when left button up.
		   void OnLMouseUp(unsigned int nFlags, int x, int y, void* pHDC = NULL);

		   // Called when Left button double clicked.
		   // Zoom in the map if the map is in pan mode.
		   void OnLMouseDbClick(unsigned int nFlags, int x, int y, void* pHDC = NULL);

		   // Called when mouse move.
		   // Pan the map with left mouse button in pan mode.
		   // Pan the map with middle mouse button in anyone mode if OnMidMouseDown() and OnMidMouseUp() is called when middle event accurs.
		   void OnMouseMove(unsigned int nFlags, int x, int y, void* pHDC = NULL);

		   // Called when middle button scrolled.
		   // Zoom in or Zoom out the map
		   void OnMouseWheel(unsigned int nFlags, short zDelta, int x, int y);

		   // Called when right button down.
		   // Rigth button is used to finish current operation in edit or draw mode, so call this function when right button down
		   void OnRMouseDown(unsigned int nFlags, int x, int y, void* pHDC = NULL);

		   // Called when right button up.
		   // Rigth button is used to finish current operation in edit or draw mode, so call this function when right button up
		   void OnRMouseUp(unsigned int nFlags, int x, int y, void* pHDC = NULL);

		   // Called when middle buttun down if you need pan the map with middle buttom
		   void OnMidMouseDown(unsigned int nFlags, int x, int y, void* pHDC = NULL);

		   // Called when middle buttun up if you need pan the map with middle buttom
		   void OnMidMouseUp(unsigned int nFlags, int x, int y, void* pHDC = NULL);

		   // User interface
		public:
			/*
			 * @en
			 * @brief Set a Workspace handler, so that SceneControl can access map data in the workspace. 
			 * @pWorkspace   pointer of Workspace which contains map data will be shown on map window by SceneControl
			 */
			void SetWorkspace(Workspace* pWorkspace);

			/*
			 * @en
			 * @brief Get UGLayers handler of current map, which manager layers in the map.
			 * @return pointer of UGLayers
			 */
			UGLayer3Ds* GetUGLayers();

			/*
			 * @en
			 * @brief Set a layer editable or not in current map.
			 * @pLayer       pointer of UGLayer
			 * @isEditable   true or false
			 */
			void SetEditableLayer(UGLayer* pLayer, bool isEditable);

			/*
			 * @en
			 * @brief    Open a map according its name
			 * @mapName  The map's name
			 * @return   Return true if open, or false
			 */
			bool OpenScene(string mapName);
			
			/*
			 * @en
			 * @brief Add a dataset on map according to its name and the datasource it belongs to.
			 * @datasourceName Name of datasource which contains the dataset.
			 * @datasetName    Dataset name
			 * @bAddToHead     If true, the dataset will be add to the top of map; otherwise it will add to the end.
			 * @return         Return a pointer of UGLayer if the dataset was added on map, or return NULL.
			 */
			UGLayer* AddDataset(string datasourceSName, string datasetName, bool bAddToHead = true);

			/*
			 * @en
			 * @brief   Save current map.
			 * @return  Return true if saved or return false.
			 */
			bool Save();

			/*
			 * @en
			 * @brief    Save current map with a new name.
			 * @mapName  Map's new name.
			 * @return   Return true if saved or return false.
			 */
			bool SaveAs(string mapName);

			/*
			 *
			 */
			void ActiveScene();
	   };
	//}
}

#endif // !SU_MAPCONTROL_INCLUDED
