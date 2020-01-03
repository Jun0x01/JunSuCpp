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

		   // ��̬��
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
			 * ��γ������ֵת����Ļ����, ��ȻҲ֧�ִ���ͶӰ���ֻ꣬�轫srcEPSGCode����Ϊ��Ӧ��ֵ����
			 * @langitude     ���Ȼ��������ֵ
			 * @latitude      γ�Ȼ���������ֵ
			 * @srcEPSGCode   ����ֵ��Ӧ������ϵ��EPSG Code��Ĭ��4326,��WGS1984����ϵ����ʹ��China2000,����4490
			 */
			UGPoint GeoCoordToPixel(double longitude, double latitude, int srcEPSGCode = 4326);

			/*
			 * ��Ļ����ת���ɾ�γ������, ��ȻҲ֧�ִ���ͶӰ���ֻ꣬�轫destEPSGCode����Ϊ��Ӧ��ֵ����
			 * @x             ��Ļ����X
			 * @y             ��Ļ����Y
			 * @destEPSGCode  ת����ľ�γ��ֵ��Ӧ������ϵ��EPSG Code��Ĭ��4326,��WGS1984����ϵ����ʹ��China2000,����4490
			 */
			UGPoint2D PixelToGeoCoord(int x, int y, int destEPSGCode = 4326);

			/*
			 * �������ĵ㴮ת��Ϊ��ͼ����, ���ڴ�����̬�㣬���ٲ��ϵĶ���ʱ�����ô���ӦΪ��Щ��ʱ����ʹ�õ�ͼ���꣬��Ȼ�����Ҫ��ӵĵ��Ѿ��ǵ�ͼ�����˾Ͳ���Ҫת���ˡ�
			 * @pts           ��Ҫת����һϵ�е㣬��Щ���ǵ��������ͶӰ����ĵ�
			 * @srcEPSGCode   ����㴮��Ӧ������ϵ��EPSG Code��Ĭ��4326,��WGS1984����ϵ����ʹ��China2000,����4490
 			 */
			bool ToMapCoords(UGPoint2Ds& pts, int srcEPSGCode = 4326);

			/*
			 * �������ĵ㴮ת��Ϊ��Ļ����
			 * @pts           ��Ҫת����һϵ�е㣬��Щ���ǵ��������ͶӰ����ĵ�
			 * @outPts        ת�������Ļ������
			 * @srcEPSGCode   ����㴮��Ӧ������ϵ��EPSG Code��Ĭ��4326,��WGS1984����ϵ����ʹ��China2000,����4490
			 */
			bool ToPixels(const UGPoint2Ds& pts, UGPoints& outPts, int srcEPSGCode = 4326);

			/*
			 * �������ĵ㴮ת����ָ������ϵ
			 * @pts           ��Ҫת����һϵ�е�,��Щ������Ļ����ĵ�
			 * @outPts        ת�����������
			 * @srcEPSGCode   ����㴮��Ӧ������ϵ��EPSG Code��Ĭ��4326,��WGS1984����ϵ����ʹ��China2000,����4490
			 */
			bool ToGeoCoords(const UGPoints& pts, UGPoint2Ds& outPts, int destEPSGCode = 4326);

			/*
			 * ����������ֵ�ĵ���ӵ���̬��
			 * @keyName       ��ӵ���̬���������ƣ���������
			 * @x             ���Ȼ��������ֵ
			 * @y             γ�Ȼ���������ֵ
			 * @size          ����ŵĴ�С��Ĭ��Ϊ4mm
			 * @color         ����ŵ���ɫֵ���Թ̶���ɫ�ķ��ź�ͼƬ������Ч; ��ɫֵ��ʽΪBGR(0xFFFFFF)��Ĭ��ֵΪ��ɫ
			 * @symbolID      ����ſ�ID, ����ָ�������ʾ��״��Ĭ��ֵΪ0(���õ�ϵͳ���ſ�id)������Բ��; 
			 *                �����ռ���Դ�б����˷��ſ⣬��ʹ��iDesktop����鿴���Զ��塣
			 * @srcEPSGCode   ����ľ�γ��ֵ��Ӧ������ϵ��EPSG Code��Ĭ��4326,��WGS1984����ϵ����ʹ��China2000,����4490
			 */
			bool AddDynamicPoint(const UGString& keyName, double x, double y, int color = 0, double size = 4, int symbolID = 0, int srcEPSGCode = 4326);

			/*
			 * �������������������ֵ��Ϊ�ߵ������˵㣬����ӵ���̬��
			 * @keyName       ��ӵ���̬���������ƣ���������
			 * @x1, y1        ��һ���������ֵ
			 * @x2, y2        �ڶ����������ֵ
			 * @color         �ߵ���ɫ���Թ̶���ɫ�ķ�����Ч; ��ɫֵ��ʽΪBGR(0xFFFFFF)��Ĭ��ֵΪ��ɫ
			 * @width         �߿�Ĭ��0.1mm;�Թ̶��߿�ķ�����Ч
			 * @symbolID      �߷��ſ�ID, ����ָ���ߵ���ʾ��״��Ĭ��ֵΪ0(���õ�ϵͳ���ſ�id)������ʵ��; 1��������
			 *                �����ռ���Դ�б����˷��ſ⣬��ʹ��iDesktop����鿴���Զ��塣
			 * @srcEPSGCode   ����ֵ��Ӧ������ϵ��EPSG Code��Ĭ��4326,��WGS1984����ϵ����ʹ��China2000,����4490
			 */
			bool AddDynamicLine(const UGString& keyName, double x1, double y1, double x2, double y2, int color=0, double width=0.1, int symbolID=0, int srcEPSGCode = 4326);

			/*
			 * �������ĵ㴮��ӵ���̬��
			 * @keyName       ��ӵ���̬���������ƣ���������
			 * @pts           ��Ҫ��ӵĵ㴮
			 * @color         �ߵ���ɫ��Ĭ��ֵΪ��ɫ���Էǹ̶������߷�����Ч
			 * @width         �߿�Ĭ��0.1mm
			 * @symbolID      �߷��ſ�ID, ����ָ���ߵ���ʾ��״��Ĭ��ֵΪ0(���õ�ϵͳ���ſ�id)������ʵ��; 1��������
			 *                �����ռ���Դ�б����˷��ſ⣬��ʹ��iDesktop����鿴���Զ��塣
			 * @srcEPSGCode   ����㴮��Ӧ������ϵ��EPSG Code��Ĭ��4326,��WGS1984����ϵ����ʹ��China2000,����4490
			 */
			bool AddDynamicLine(const UGString& keyName, const UGPoint2Ds pts, int color = 0, double width = 0.1, int symbolID = 0, int srcEPSGCode = 4326);

			/*
			 * ���ö�̬���еĶ����Ƿ�ɼ�
			 * @keyName       ��ӵ���̬��Ķ��������
			 * @isVisible     �����Ƿ�ɼ�
			 * @needRefresh   �Ƿ�����ˢ�µ�ͼ��Ĭ��true.
			 */
			void SetDynamicGeoVisible(const UGString& keyName, bool isVisible, bool needRefresh = true);

			/*
			 * �����Ƿ�ʹ�ö�̬ͶӰ��ʾ��ͼ, �޸ĺ������ˢ�µ�ͼ
			 */
			void SetMapDynamicProjection(bool isDynamicPrjection);

			/*
			 * �رյ�ǰ��ͼ
			 */
			void CloseMap();

			// ��ȡ����ѡ�ж����ѡ�񼯡�ͨ������£�ʹ�õ��ѡ�����ֻ�и�ͼ����ѡ�ж������ֻ��һ��ѡ��
			// ��û��ѡ�ж��󣬷���NULL; ����Ҫʹ��ʱ������RemoveAll(), ��delete
			UGArray<UGSelection*>* GetGeoSelections();

			/*
			 * ��ָ����ѡ��תΪ��¼����Ĭ�ϻ�õļ�¼�������޸ļ�¼��ֻ�����ڻ�ȡ�ֶ�ֵ��
			 * ����Ҫʹ�õõ���UGRecordsetʱ����ͨ�����´����ͷ��ڴ棺
			 *     pRecordset->GetDataset()->ReleaseRecordset(pRecordset)
			 *     pRecordset = NULL
			 *
			 * @pSelection  ���ڻ�ü�¼����UGSelection*
			 * @isEditable  ���صļ�¼���Ƿ���޸ģ�Ĭ��false, �����޸ļ�¼��
			 */
			UGRecordset* ToRecordset(UGSelection* pSelection, bool isEditable = false);
	   };
	//}
}

#endif // !SU_MAPCONTROL_INCLUDED
