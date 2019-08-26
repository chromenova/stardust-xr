#ifndef STARDUSTOPENXRGRAPHICS_H
#define STARDUSTOPENXRGRAPHICS_H

#include <QSize>
#include <QThread>
#include <QVector3D>
#include <QQuaternion>
#include <QOpenGLFramebufferObject>
#include <QMetaObject>
#include <QQuickItem>

#include "stardustopenxr.h"

class StardustOpenXRFrame;

class StardustOpenXRGraphics : public QObject {
    Q_OBJECT

    Q_PROPERTY(StardustOpenXR *openxr MEMBER openxr)

    Q_PROPERTY(QSize eyeDimensions MEMBER eyeDimensions NOTIFY eyeDimensionsChanged)

    Q_PROPERTY(QQuickItem *leftView MEMBER leftView)
    Q_PROPERTY(QQuickItem *rightView MEMBER rightView)
    Q_PROPERTY(QObject *leftEye MEMBER leftEye)
    Q_PROPERTY(QObject *rightEye MEMBER rightEye)
public:
    explicit StardustOpenXRGraphics(QObject *parent = nullptr);
    ~StardustOpenXRGraphics();

    StardustOpenXR *openxr = nullptr;
    Q_INVOKABLE void initialize();

    uint displayFPS = 0;

    XrViewConfigurationProperties viewProperties{XR_TYPE_VIEW_CONFIGURATION_PROPERTIES};
    XrViewConfigurationView eyeData[2];

    XrSwapchainCreateInfo swapInfo = {
        XR_TYPE_SWAPCHAIN_CREATE_INFO,                                                      //XrStructureType           type;
        nullptr,                                                                            //const void*               next;
        0,                                                                                  //XrSwapchainCreateFlags    createFlags;
        XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT | XR_SWAPCHAIN_USAGE_TRANSFER_DST_BIT,      //XrSwapchainUsageFlags     usageFlags;
        VK_FORMAT_R8G8B8A8_UNORM,                                                           //int64_t                   format;
        1,                                                                                  //uint32_t                  sampleCount;
        1,                                                                                  //uint32_t                  width;
        1,                                                                                  //uint32_t                  height;
        1,                                                                                  //uint32_t                  faceCount;
        1,                                                                                  //uint32_t                  arraySize;
        1                                                                                   //uint32_t                  mipCount;
    };
    XrSwapchain swapchains[2] = {
        XrSwapchain {},
        XrSwapchain {}
    };

    VkExtent3D imageExtents[2] = {
        VkExtent3D{0, 0, 1},
        VkExtent3D{0, 0, 1}
    };

    uint32_t swapchainImageCount = 0;
    XrSwapchainImageVulkanKHR swapchainImageTemplate = {
        XR_TYPE_SWAPCHAIN_IMAGE_VULKAN_KHR,
        nullptr,
        VkImage()
    };
    std::vector<XrSwapchainImageVulkanKHR> swapchainImages[2];
    uint32_t swapchainImageIndex;

    std::vector<uint32_t> swapchainImageIndices = std::vector<uint32_t>(2);

    XrReferenceSpaceCreateInfo refSpaceInfo = {
        XR_TYPE_REFERENCE_SPACE_CREATE_INFO,
        nullptr,
        XR_REFERENCE_SPACE_TYPE_STAGE,
        XrPosef {
            XrQuaternionf {0.0f,0.0f,1.0f,0.0f},
            XrVector3f {0.0f,0.0f,0.0f}
        }
    };
    XrSpace refSpace = XR_NULL_HANDLE;

    XrViewLocateInfo viewLocateInfo = {
        XR_TYPE_VIEW_LOCATE_INFO,
        nullptr
    };
    XrViewState viewState = {
        XR_TYPE_VIEW_STATE,
        nullptr
    };

    XrRect2Di eyeRects[2] = {
        XrRect2Di {
            XrOffset2Di {0,0},
            XrExtent2Di {1,1}
        },
        XrRect2Di {
            XrOffset2Di {0,0},
            XrExtent2Di {1,1}
        }
    };

    XrCompositionLayerProjectionView stardustLayerViews[2] = {
        XrCompositionLayerProjectionView {
            XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW,
            nullptr
        },
        XrCompositionLayerProjectionView {
            XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW,
            nullptr
        }
    };

    QOpenGLFramebufferObject *fbo;

    QThread *frameThread = nullptr;
    StardustOpenXRFrame *frame = nullptr;

    QSize eyeDimensions = QSize(900, 900);
    std::vector<XrView> views;

    QObject *leftEye = nullptr;
    QObject *rightEye = nullptr;

    QQuickItem *leftView = nullptr;
    QQuickItem *rightView = nullptr;

    QQuickItem *surfaces[2];

    VkImage *vulkanImages[2] = {
        VK_NULL_HANDLE,
        VK_NULL_HANDLE
    };

    XrFrameWaitInfo frameWaitInfo{XR_TYPE_FRAME_WAIT_INFO};
    XrFrameState frameState{XR_TYPE_FRAME_STATE};

signals:
    void eyeDimensionsChanged();
    void startFrameLoop();
};

#endif // STARDUSTOPENXRGRAPHICS_H
