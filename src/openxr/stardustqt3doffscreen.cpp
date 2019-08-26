#include "stardustqt3doffscreen.h"

StardustQt3DOffscreen::StardustQt3DOffscreen(QObject *parent) : QObject(parent) {

}

void StardustQt3DOffscreen::initialize() {
    //Get the render size from OpenXRGraphics
    eyeDimensions = graphics->eyeDimensions;

    //Create the cameras
    leftEyeEngine = new OffscreenEngine(leftEye, eyeDimensions);
    rightEyeEngine = new OffscreenEngine(rightEye, eyeDimensions);

    //Add the scene root to be rendered to each eye
    leftEyeEngine->setSceneRoot(sceneRoot);
    rightEyeEngine->setSceneRoot(sceneRoot);

    //Create the frameAction and connect it to capturing the frames
//    frameAction = new Qt3DLogic::QFrameAction(sceneRoot);
//    connect(frameAction, &Qt3DLogic::QFrameAction::triggered, this, &StardustQt3DOffscreen::captureFrames);
    connect(graphics->frameWorker, &StardustOpenXRFrameWorker::renderFrame, this, &StardustQt3DOffscreen::captureFrames);
}

void StardustQt3DOffscreen::captureFrames(float dt) {
    Q_UNUSED(dt);
//    if(leftEyeCapture != nullptr)
//        delete leftEyeCapture;
//    if(rightEyeCapture != nullptr)
//        delete rightEyeCapture;

    leftEyeCapture = leftEyeEngine->getRenderCapture()->requestCapture();
    rightEyeCapture = rightEyeEngine->getRenderCapture()->requestCapture();
    connect(leftEyeCapture, &Qt3DRender::QRenderCaptureReply::completed, this, &StardustQt3DOffscreen::onLeftEyeFrameRendered);
    connect(rightEyeCapture, &Qt3DRender::QRenderCaptureReply::completed, this, &StardustQt3DOffscreen::onRightEyeFrameRendered);
}

void StardustQt3DOffscreen::onFrameRendered(Qt3DRender::QRenderCaptureReply *capture, VkImage *image) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands(3);
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VkDeviceSize imageSize = eyeDimensions.width()*eyeDimensions.height()*4;
    VkMemoryRequirements memRequirements;

    createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT, stagingBuffer, stagingBufferMemory, memRequirements);

    VkImageSubresourceRange range;
    range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    range.baseArrayLayer = 0;
    range.layerCount = 1;
    range.baseMipLevel = 0;
    range.levelCount = 1;

    VkImageMemoryBarrier layoutTransition = {
        VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        nullptr,
        VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
        VK_ACCESS_TRANSFER_WRITE_BIT,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        graphics->openxr->vulkan->queueFamilyIndex,
        graphics->openxr->vulkan->queueFamilyIndex,
        *image,
        range
    };

    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_DEPENDENCY_BY_REGION_BIT, 0, nullptr, 0, nullptr, 1, &layoutTransition);

//    capture->image().convertTo(QImage::Format_RGB888);

    const int align_mod = memRequirements.size % memRequirements.alignment;
    const int aligned_size = ((memRequirements.size % memRequirements.alignment) == 0)
                             ? memRequirements.size
                             : (memRequirements.size + memRequirements.alignment - align_mod);

    uint *imgData = (uint*) malloc(sizeof (uint) * imageSize); // TODO: free somewhere

    memset(imgData, 255, sizeof(uint) * imageSize);

//    QByteArray arr;
//    QBuffer buffer(&arr);
//    buffer.open(QIODevice::WriteOnly);
//    capture->image().save(&buffer);

    void* data = nullptr;
    vkMapMemory(graphics->openxr->vulkan->device, stagingBufferMemory, 0, imageSize, 0, &data);
        memcpy(data, imgData, imageSize);
    vkUnmapMemory(graphics->openxr->vulkan->device, stagingBufferMemory);

    free(imgData);

    VkBufferImageCopy region = {};
    region.bufferOffset = 0;
    region.bufferRowLength = graphics->imageExtent.width;
    region.bufferImageHeight = graphics->imageExtent.height;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = graphics->imageExtent;

    vkCmdCopyBufferToImage(
        commandBuffer,
        stagingBuffer,
        *image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region
    );

    layoutTransition.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    layoutTransition.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
    layoutTransition.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    layoutTransition.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_DEPENDENCY_BY_REGION_BIT, 0, nullptr, 0, nullptr, 1, &layoutTransition);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo = {
        VK_STRUCTURE_TYPE_SUBMIT_INFO,
        nullptr
    };
    submitInfo.commandBufferCount = 1;
    VkCommandBuffer cmdBuffers[1] = {commandBuffer};
    submitInfo.pCommandBuffers = cmdBuffers;

    vkQueueSubmit(*graphics->openxr->vulkan->queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(*graphics->openxr->vulkan->queue);
}

void StardustQt3DOffscreen::onLeftEyeFrameRendered() {
    onFrameRendered(leftEyeCapture, graphics->leftEyeImage);
}

void StardustQt3DOffscreen::onRightEyeFrameRendered() {
    onFrameRendered(rightEyeCapture, graphics->rightEyeImage);
}
