# PCBRecheck  

基于Qt5、OpenCV4、Tesseract-OCR、ASIO的PCB缺陷复查系统

## 开发环境：  
Visual Studio 2017（需安装Qt VS Tools插件）  
Qt 5.11.1  
OpenCV 4.1.0  
Tesseract-OCR  
ASIO C++ Library （非Boost版）  

**说明：**
1. 本程序使用了C++ 17的filesystem，需要在VS设定语言标准
2. 由于ASIO的引入，需要在VS的预处理器选项中添加ASIO_STANDALONE和WIN32_LEAN_AND_MEAN
3. 虽然使用了OpenCV4，但是并没有涉及OpenCV4的新特性，使用OpenCV3也可以

## 文件说明：  

| 文件 | 格式 | 说明 |
|:----: | :----: | ------ |
| PCBRecheck | .h / .cpp / .ui | 主界面 |
| PCBRecheck.qrc | .h / .cpp | Qt界面使用的资源配置文件 |
|  |  |  |
| Configurator | .h / .cpp | 用户参数类和参数配置器 |
| RuntimeParams | .h / .cpp | 运行参数类 |
| pcbFuncLib | .h / .cpp | 函数库和若干类型定义 |
|                 |                 |                             |
| SysInitThread | .h / .cpp | 初始化线程 |
| SerialNumberUI | .h / .cpp/ .ui | 序号设置界面 |
| CameraControler | .h / .cpp | 相机控制器 |
| ExitQueryUI | .h / .cpp/ .ui | 退出询问界面 |
| FlickeringArrow | .h / .cpp | 主界面中PCB大图上闪烁的箭头 |
|  MyMessageBox   |    .h / .cpp    | 弹窗报错                    |
|                 |                 |                             |
| FileSyncThread  |    .h / .cpp    | 文件同步线程                |
| FileReceiver | .h / .cpp | 文件接收器 |
|   FileSender    |    .h / .cpp    | 文件发送器                  |

**说明：**
1. 相机控制器中使用OpenCV实现相机相关操作
2. OCR功能主要用于产品序号识别