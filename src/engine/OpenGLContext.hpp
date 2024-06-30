//
// Created by gbian on 27/06/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "SGLDM/Core.hpp"

DISABLE_WARNINGS_PUSH(26446 26482)
class OpenGLContext {
public:
    OpenGLContext() noexcept { retrieveOpenGLInfo(); }

    void retrieveOpenGLInfo() {
        vnd::AutoTimer timer("retrieveOpenGLInfo");
        vendor = std::bit_cast<const char *>(glGetString(GL_VENDOR));
        renderer = std::bit_cast<const char *>(glGetString(GL_RENDERER));
        version = std::bit_cast<const char *>(glGetString(GL_VERSION));
        glslVersion = std::bit_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));

        glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
        glGetIntegerv(GL_MAX_VIEWPORT_DIMS, maxViewportDims.data());
        glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &maxUniformComponents);
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureImageUnits);

        for(int i = 0; i < 3; ++i) {
            glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, i, &maxComputeWorkGroupCount[i]);
            glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, i, &maxComputeWorkGroupSize[i]);
        }

        glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_COMPONENTS, &maxComputeUniformComponents);
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs);
        glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuffers);
        glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);
        glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxCombinedTextureUnits);
        glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);
        glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &maxFramebufferWidth);
        glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &maxFramebufferHeight);
        glGetIntegerv(GL_MAX_FRAMEBUFFER_SAMPLES, &maxFramebufferSamples);
        glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize);
        glGetIntegerv(GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS, &maxVertexShaderStorageBlocks);
        glGetIntegerv(GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS, &maxFragmentShaderStorageBlocks);
        glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &maxShaderStorageBlockSize);
        glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS, &maxTransformFeedbackSeparateComponents);
        glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS, &maxTransformFeedbackInterleavedComponents);
    }

    void logOpenGLInfo() {
        LINFO("OpenGL Vendor: {}", vendor);
        LINFO("OpenGL Renderer: {}", renderer);
        LINFO("OpenGL Version: {}", version);
        LINFO("GLSL Version: {}", glslVersion);
        LINFO("Number of Extensions: {}", numExtensions);
        LINFO("Max Texture Size: {}", maxTextureSize);
        LINFO("Max Viewport Dimensions: {} x {}", maxViewportDims[0], maxViewportDims[1]);
        LINFO("Max Vertex Uniform Components: {}", maxUniformComponents);
        LINFO("Max Texture Image Units: {}", maxTextureImageUnits);
        LINFO("Max Compute Work Group Count: {}", maxComputeWorkGroupCount);
        LINFO("Max Compute Work Group Size: {}", maxComputeWorkGroupSize);
        LINFO("Max Compute Uniform Components: {}", maxComputeUniformComponents);
        LINFO("Max Vertex Attribs: {}", maxVertexAttribs);
        LINFO("Max Draw Buffers: {}", maxDrawBuffers);
        LINFO("Max Color Attachments: {}", maxColorAttachments);
        LINFO("Max Combined Texture Units: {}", maxCombinedTextureUnits);
        LINFO("Max Samples: {}", maxSamples);
        LINFO("Max Anisotropy: {:.F}", maxAnisotropy);
        LINFO("Max Framebuffer Width: {}", maxFramebufferWidth);
        LINFO("Max Framebuffer Height: {}", maxFramebufferHeight);
        LINFO("Max Framebuffer Samples: {}", maxFramebufferSamples);
        LINFO("Max Renderbuffer Size: {}", maxRenderbufferSize);
        LINFO("Max Vertex Shader Storage Blocks: {}", maxVertexShaderStorageBlocks);
        LINFO("Max Fragment Shader Storage Blocks: {}", maxFragmentShaderStorageBlocks);
        LINFO("Max Shader Storage Block Size: {}", maxShaderStorageBlockSize);
        LINFO("Max Transform Feedback Separate Components: {}", maxTransformFeedbackSeparateComponents);
        LINFO("Max Transform Feedback Interleaved Components: {}", maxTransformFeedbackInterleavedComponents);
    }

private:
    const char *vendor;
    const char *renderer;
    const char *version;
    const char *glslVersion;
    int numExtensions;
    int maxTextureSize;
    std::array<int, 2> maxViewportDims;
    int maxUniformComponents;
    int maxTextureImageUnits;
    std::array<int, 3> maxComputeWorkGroupCount;
    std::array<int, 3> maxComputeWorkGroupSize;
    int maxComputeUniformComponents;
    int maxVertexAttribs;
    int maxDrawBuffers;
    int maxColorAttachments;
    int maxCombinedTextureUnits;
    int maxSamples;
    float maxAnisotropy;
    int maxFramebufferWidth;
    int maxFramebufferHeight;
    int maxFramebufferSamples;
    int maxRenderbufferSize;
    int maxVertexShaderStorageBlocks;
    int maxFragmentShaderStorageBlocks;
    int maxShaderStorageBlockSize;
    int maxTransformFeedbackSeparateComponents;
    int maxTransformFeedbackInterleavedComponents;
};
DISABLE_WARNINGS_POP()
// NOLINTEND(*-include-cleaner)
