/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.04.08
 */
#if !defined(SU_MAPCONTROL_INCLUDED)
#define SU_MAPCONTROL_INCLUDED

#include "Data/exportsDefine.h"

#include "MapEditor/UGMapEditorWnd.h"
#include "Graphics/UGGraphicsManager.h"
#include "Drawing/UGDrawParamaters.h"
#include "Data/Workspace.h"
#include "Map/UGDynamicLayers.h"
#include "Projection/UGRefTranslator.h"

#include "Geometry/UGGeoPoint.h"
#include "Geometry/UGGeoLine.h"
#include "Map/UGLayerGroup.h"


using namespace UGC;
using namespace SuperMap;

namespace SuperMap
{
	//namespace Mapping
	//{
	   class SU_MAPPING_API MapControl
	   {
		   //Constructor
	   public:
		   /*
		    *@en
			*@pInvalidateCallBack  A callback function pointer through which to invalidate the Window if map content is updated
			*@pWnd  the Window which owns the invalidate callback function
		   */
		   MapControl(INVALIDATEPROC pInvalidateCallBack, void* pWnd);
		   virtual ~MapControl();

		   // members
	   private:
		   //@en the handle of view which providing a window for map
		   void* m_pWnd;

		   Workspace* m_pInnerWorkspace;

		   //@en the handle of Workspace, passed in through function SetWorkspace()  
		   Workspace* m_pWorkspace;

		   // 动态层
		   UGDynamicLayer* pUGDynLayer;
	   private:
		   // Map window
		   UGMapEditorWnd* m_pUGMapWnd;
		   // minimize the window
		   UGbool m_IsMinSized;


		// Image Graphics used in Qt, no pHDC handle
	   private:
		   
		   // drawing on this image
		   UGImage* m_pGraphicsImage;
		   // old image
		   UGImage* m_pGraphicsImageOld;
		   bool mNeedRedraw;

		   bool mIsInWorkspace;

		   // methods
	   private:
		   void Initialize(INVALIDATEPROC pCallBack, void* pView);
		   //pHDC: the handle to the device context associated with System::Drawing:Graphics
		   UGGraphics* CreateUGGraphics(void* pHDC);

		   void DisposeUGGrapics(UGGraphics* &pGraphics);
		   UGuint getKeyUGFlags(unsigned int flag);
		   //void Invalidate();
		   void MouseScale(double ratio, int x, int y, bool delayRefresh = false);

		   bool isExecuteRButton();

		   void GetSelectionOfLayer(UGLayer* pLayer, UGArray<UGSelection*>* pSelectionArr);

		   // pointers of callback function
	   private:
		   // the pointer of invalidateCallback function
		   INVALIDATEPROC m_pInvalidateCallback;

	   // public functions
	   public:
		   // Normally used by MapControl self to notice the window update.
		   void Invalidate(bool reDraw = true);

		   //Used in Qt, get the image bytes which will be draw in the window.
		   UGbyte* GetImageBytes();

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

		   // Return the handle of UGMapEditorWnd if need to use it, but don't delete it which is belong to MapControl
		   UGMapEditorWnd* GetMapEditWnd();  


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
			 * @brief Set a Workspace handler, so that MapControl can access map data in the workspace. 
			 * @pWorkspace   pointer of Workspace which contains map data will be shown on map window by MapControl
			 */
			void SetWorkspace(Workspace* pWorkspace);

			// Return the related Workspace's pointer.
			Workspace* GetWorkspace();

			/*
			 * @en
			 * @brief Get UGLayers handler of current map, which manager layers in the map.
			 * @return pointer of UGLayers
			 */
			UGLayers* GetUGLayers();

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
			bool OpenMap(string mapName);
			
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
			 * 经纬度坐标值转成屏幕坐标, 当然也支持传入投影坐标，只需将srcEPSGCode设置为响应的值即可
			 * @langitude     经度或横向坐标值
			 * @latitude      纬度或纵向坐标值
			 * @srcEPSGCode   传入值对应的坐标系的EPSG Code，默认4326,即WGS1984坐标系；若使用China2000,则传入4490
			 */
			UGPoint GeoCoordToPixel(double longitude, double latitude, int srcEPSGCode = 4326);

			/*
			 * 屏幕坐标转换成经纬度坐标, 当然也支持传入投影坐标，只需将destEPSGCode设置为响应的值即可
			 * @x             屏幕坐标X
			 * @y             屏幕坐标Y
			 * @destEPSGCode  转换后的经纬度值对应的坐标系的EPSG Code，默认4326,即WGS1984坐标系；若使用China2000,则传入4490
			 */
			UGPoint2D PixelToGeoCoord(int x, int y, int destEPSGCode = 4326);

			/*
			 * 将给定的点串转换为地图坐标, 对于创建动态层，跟踪层上的对象时很有用处，应为这些临时层需使用地图坐标，当然如果需要添加的点已经是地图坐标了就不需要转换了。
			 * @pts           需要转换的一系列点，这些点是地理坐标或投影坐标的点
			 * @srcEPSGCode   传入点串对应的坐标系的EPSG Code，默认4326,即WGS1984坐标系；若使用China2000,则传入4490
 			 */
			bool ToMapCoords(UGPoint2Ds& pts, int srcEPSGCode = 4326);

			/*
			 * 将给定的点串转换为屏幕坐标
			 * @pts           需要转换的一系列点，这些点是地理坐标或投影坐标的点
			 * @outPts        转换后的屏幕坐标结果
			 * @srcEPSGCode   传入点串对应的坐标系的EPSG Code，默认4326,即WGS1984坐标系；若使用China2000,则传入4490
			 */
			bool ToPixels(const UGPoint2Ds& pts, UGPoints& outPts, int srcEPSGCode = 4326);

			/*
			 * 将给定的点串转换到指定坐标系
			 * @pts           需要转换的一系列点,这些点是屏幕坐标的点
			 * @outPts        转换后的坐标结果
			 * @srcEPSGCode   传入点串对应的坐标系的EPSG Code，默认4326,即WGS1984坐标系；若使用China2000,则传入4490
			 */
			bool ToGeoCoords(const UGPoints& pts, UGPoint2Ds& outPts, int destEPSGCode = 4326);

			/*
			 * 将给定坐标值的点添加到动态层
			 * @keyName       添加到动态层对象的名称，不可重名
			 * @x             经度或横向坐标值
			 * @y             纬度或纵向坐标值
			 * @size          点符号的大小，默认为4mm
			 * @color         点符号的颜色值，对固定颜色的符号和图片符号无效; 颜色值格式为BGR(0xFFFFFF)，默认值为黑色
			 * @symbolID      点符号库ID, 用于指定点的显示形状，默认值为0(内置的系统符号库id)，代表圆点; 
			 *                工作空间资源中保存了符号库，可使用iDesktop软件查看或自定义。
			 * @srcEPSGCode   传入的经纬度值对应的坐标系的EPSG Code，默认4326,即WGS1984坐标系；若使用China2000,则传入4490
			 */
			bool AddDynamicPoint(const UGString& keyName, double x, double y, int color = 0, double size = 4, int symbolID = 0, int srcEPSGCode = 4326);

			/*
			 * 将给定的两个点的坐标值作为线的两个端点，并添加到动态层
			 * @keyName       添加到动态层对象的名称，不可重名
			 * @x1, y1        第一个点的坐标值
			 * @x2, y2        第二个点的坐标值
			 * @color         线的颜色，对固定颜色的符号无效; 颜色值格式为BGR(0xFFFFFF)，默认值为黑色
			 * @width         线宽，默认0.1mm;对固定线宽的符号无效
			 * @symbolID      线符号库ID, 用于指定线的显示形状，默认值为0(内置的系统符号库id)，代表实线; 1代表虚线
			 *                工作空间资源中保存了符号库，可使用iDesktop软件查看或自定义。
			 * @srcEPSGCode   传入值对应的坐标系的EPSG Code，默认4326,即WGS1984坐标系；若使用China2000,则传入4490
			 */
			bool AddDynamicLine(const UGString& keyName, double x1, double y1, double x2, double y2, int color=0, double width=0.1, int symbolID=0, int srcEPSGCode = 4326);

			/*
			 * 将给定的点串添加到动态层
			 * @keyName       添加到动态层对象的名称，不可重名
			 * @pts           需要添加的点串
			 * @color         线的颜色，默认值为黑色，对非固定风格的线符号有效
			 * @width         线宽，默认0.1mm
			 * @symbolID      线符号库ID, 用于指定线的显示形状，默认值为0(内置的系统符号库id)，代表实线; 1代表虚线
			 *                工作空间资源中保存了符号库，可使用iDesktop软件查看或自定义。
			 * @srcEPSGCode   传入点串对应的坐标系的EPSG Code，默认4326,即WGS1984坐标系；若使用China2000,则传入4490
			 */
			bool AddDynamicLine(const UGString& keyName, const UGPoint2Ds pts, int color = 0, double width = 0.1, int symbolID = 0, int srcEPSGCode = 4326);

			/*
			 * 设置动态层中的对象是否可见
			 * @keyName       添加到动态层的对象的名称
			 * @isVisible     对象是否可见
			 * @needRefresh   是否立即刷新地图，默认true.
			 */
			void SetDynamicGeoVisible(const UGString& keyName, bool isVisible, bool needRefresh = true);

			/*
			 * 设置是否使用动态投影显示地图, 修改后会立即刷新地图
			 */
			void SetMapDynamicProjection(bool isDynamicPrjection);

			/*
			 * 关闭当前地图
			 */
			void CloseMap();

			// 获取所有选中对象的选择集。通常情况下，使用点击选择对象，只有个图层有选中对象，因此只有一个选择集
			// 若没有选中对象，返回NULL; 不需要使用时，请先RemoveAll(), 再delete
			UGArray<UGSelection*>* GetGeoSelections();

			/*
			 * 将指定的选择集转为记录集。默认获得的记录集不能修改记录，只能用于获取字段值。
			 * 不需要使用得到的UGRecordset时，请通过如下代码释放内存：
			 *     pRecordset->GetDataset()->ReleaseRecordset(pRecordset)
			 *     pRecordset = NULL
			 *
			 * @pSelection  用于获得记录集的UGSelection*
			 * @isEditable  返回的记录集是否可修改，默认false, 不可修改记录。
			 */
			UGRecordset* ToRecordset(UGSelection* pSelection, bool isEditable = false);
	   };
	//}
}

#endif // !SU_MAPCONTROL_INCLUDED
