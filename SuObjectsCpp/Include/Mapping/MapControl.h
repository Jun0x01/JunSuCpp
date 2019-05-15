#if !defined(SU_MAPCONTROL_INCLUDED)
#define SU_MAPCONTROL_INCLUDED

#include "Data/exportsDefine.h"

#include "MapEditor/UGMapEditorWnd.h"
#include "Graphics/UGGraphicsManager.h"
#include "Drawing/UGDrawParamaters.h"

using namespace UGC;

namespace SuperMap
{
	//namespace Mapping
	//{
	   class SU_MAPPING_API MapControl
	   {
		   //Constructor
	   public:
		   MapControl(INVALIDATEPROC pInvalidateCallBack, void* pView);
		   virtual ~MapControl();

		   // members
	   private:
		   // the handle of view which providing a window for map
		   void* m_pWnd;

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
		   
	   };
	//}
}

#endif // !SU_MAPCONTROL_INCLUDED
