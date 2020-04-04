# JunSuCpp

### 项目描述
 该项目展示了如何使用SuperMap iObjects for C++ 进行二次开发，并对一些不易使用的功能进行封装，如用于地图窗口MapControl，用于场景窗口的SceneControl等。封装的工程在SuObjectCpp目录，这些类都支持在VS和Qt项目中使用，以及Linux系统下Qt开发使用。

### SuperMap iObjects for C++ 进行二次开发博客
1. [SuperMap iObjects for C++ 入门详解（VS + Qt）](https://blog.csdn.net/u012824671/article/details/104510776)
2. [SuperMap iObjects for C++ 构建三维场景与加载模型数据](https://blog.csdn.net/u012824671/article/details/104913793)

#### 项目地址
1. [GitHub: JunSuCpp](https://github.com/Jun0x01/JunSuCpp), 原始项目。
2. [Gitee码云: JunSuCpp](https://gitee.com/Jun0x01/JunSuCpp), 同步自Github的项目，下载速度较好。
### 应用截图
1. JunSuMFCDialog二维地图添加标绘符号
   ![如图](https://github.com/Jun0x01/JunSuCpp/blob/master/Images/App_Plot.png)
2. JunSuCpp二维地图绘制面对象
   ![如图](https://github.com/Jun0x01/JunSuCpp/blob/master/Images/App_Map_Draw.png)
3. JunSuCpp打开CBD场景展示模型数据
   ![如图](https://github.com/Jun0x01/JunSuCpp/blob/master/Images/App_Scene_CBD.png)
4. JunSuCpp打开MaSai场景展示OSGB数据
   ![如图](https://github.com/Jun0x01/JunSuCpp/blob/master/Images/App_Scene_OSGB.png)

注：TestData目录中仅有二维地图数据，因三维数据较大，没有上传GitHub。应用中采用选择文件方式打开，因此可以使用自己制作的工作空间数据，或是超图软件其他产品提供的示范数据。

## 主要功能

### Project: JunSuMFCDialog
1. 实现地图窗口，实现地图交互操作
2. 打开工作空间，显示地图
3. 绘制点、线、面、态势标绘对象
4. 获得选中对象
5. 动态层示例：选中对象时，在点击位置添加一条线到动态层显示
6. 态势标绘代码示例：选中对象时，在点击位置添加一个态势标绘对象到CAD数据集
7. 态势推演示例：选中点类型的标绘对象，创建比例动画

### Project: JunSuCpp
1. 同时支持二维地图和三维场景
2. 选择文件打开工作空间(*.smwu, *.sxwu)
3. 选择文件打开数据源(*.udb)
4. 选择文件打开场景缓存文件(*.scp)
5. 从数据库打开工作空间或数据源(数据库包括：MySQL, SQLPlus, PostgreSQL, PostGIS，达梦等)
6. 多窗口模式，支持打开多幅地图或多个场景
7. 支持选中数据集右键添加到二维地图
8. 支持选中二维地图图层设置是否可选，是否可见，是否可编辑
9. 支持二维地图上绘制点、线、面
10. 支持二维地图上编辑选择对象的节点

### SuObjectCpp
包含Data和Mapping两个工程，用于封装SuperMap iObjects C++中较复杂的功能接口。Data中的Workspace实现对工作空间和数据源的管理。Mapping中的MapControl和SceneControl分别实现对二维地图和三维场景的窗口连接，以及图层管理等功能。JunSuMFCDialog和JunSuCpp就是基于这些封装实现的。 



## 目录组织结构
``` lua
JunSuCpp
   |—— PropertySheets   -- MFC 项目属性配置文件
   |—— Qt_pros          -- Qt 项目配置文件,Qt Creator 打开该目录下的.pro文件即可
   |—— SuObjectsCpp     -- 部分封装SuperMap Objects C++常用功能方法的C++库,尤其是Mapping/MapControl是一个具有通用性的实现地图窗口的类
   |—— TestData         -- 测试数据
   |—— VSApps           -- 使用SuObjectsCpp中的库和SuperMap原有库构建的应用，分MFC和Qt框架两类应用
   |—— VSSolution_2017  -- VS2017解决方案文件，VS2017为当前项目使用的版本
   |—— What's New.md    -- 项目功能更新记录
```

## 开发注意事项
0. 验证过的开发工具及版本
- VS2012 + Qt5.2(Qt addin 1.2.5)
- VS2017 + Qt5.2 及以上（Qt addin 2.3.2, 使用过的Qt有Qt5.2、Qt5.6.3、Qt5.9.3、Qt5.12
- Windows, Qt Creator 5.2及以上
- Unbuntu, Qt Creator 4.8.6, 5.2; 由于SuperMap iObjects C++是基于Qt4.8.6编译, 且Qt5不兼容Qt4, 因此Linux系统上需使用4.8.6版本的Qt库编译。

1. PropertySheets中添加顺序，以Debug_x64为例
``` lua
   本项目都使用这些配置文件来配置：
   1) PropertySheet_x64d.props     -- 设置SuperMap Objects C++链接库目录和链接库名称
   2) PropertySheets_Link_SuObjectsCpp.props -- 引入SuObjectsCpp中封装的库
   3) PropertySheet_General.props  -- 定义宏名"SuObjectsDir"指向SuperMap Objects C++ 产品包目录，并配置同文件目录; 头文件包括include,include/private 两个目录
   如图：
```
![如图](https://github.com/Jun0x01/JunSuCpp/blob/master/Images/PropertyManager.png)

2. VS开发，以VS2017为例，说明打开解决方案后，需要修改的配置
- 打开属性管理器，在任意一个项目配置中打开"PropertySheet_General.props"配置文件，修改“自定义宏” -> "SuObjectsDir"指定的SuperMap iObjects for C++产品包目录。
如图：
![如图](https://github.com/Jun0x01/JunSuCpp/blob/master/Images/Property-SuObjectsDir.png)

- 打开工程属性，配置编译工具版本，如图：
![如图](https://github.com/Jun0x01/JunSuCpp/blob/master/Images/Property-Platform_Toolset.png)

- Qt工程，右键工程 -> Qt工程设置，检查设置Qt版本以及使用的模块，本项目只使用Core, GUI, Widgets, 如图:
  ![如图](https://github.com/Jun0x01/JunSuCpp/blob/master/Images/Property-QtProjectSettings.png)

- 运行库配置，该项目中直接在工程属性中进行配置，可不依赖系统环境Path, 如图配置应用工程:
  ![如图](https://github.com/Jun0x01/JunSuCpp/blob/master/Images/Property-RuntimeLibs.png)

- 若使用VS2012打开解决方案，其配置方法类似

3. Qt Creator开发，以Qt 5.9.3为例，说明配置方法
- 使用nmake编译，工具 -> 选项 -> 编译&运行 -> 常规设置 -> 不勾选 “使用jom 代替nmake”, 如图:
  ![如图](https://github.com/Jun0x01/JunSuCpp/blob/master/Images/Qt_Option_namake.png)

- 项目属性，编译环境配置SuObjectsDir变量，设置SuperMap iObjects for C+产品目录，如图：
  ![如图](https://github.com/Jun0x01/JunSuCpp/blob/master/Images/Qt_BuildEnvironment.png)

- 项目属性，运行环境中，将SuperMap iObjects for C++ 运行库目录添加到Path, 如图：
  ![如图](https://github.com/Jun0x01/JunSuCpp/blob/master/Images/Qt_RunEnvironment.png)
