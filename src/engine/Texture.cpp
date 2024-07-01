//
// Created by gbian on 29/06/2024.
//

#include "Texture.hpp"

Texture::Texture(const fs::path &image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType) {
    type = texType;

    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *bytes = stbi_load(image.string().c_str(), &widthImg, &heightImg, &numColCh, 0);

    glGenTextures(1, &ID);
    glActiveTexture(slot);
    glBindTexture(texType, ID);

    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

    std::array<float, 3> flatColor = {1.0F, 1.0F, 1.0F};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor.data());

    glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
    glGenerateMipmap(texType);

    stbi_image_free(bytes);

    glBindTexture(texType, 0);
}

void Texture::texUnit(const Shader &shader, const char *uniform, GLuint unit) noexcept {
    const GLuint texUni = glGetUniformLocation(shader.ID, uniform);
    shader.Activate();
    glUniform1i(texUni, unit);
}

void Texture::Bind() const noexcept { glBindTexture(type, ID); }

void Texture::Unbind() const noexcept { glBindTexture(type, 0); }

void Texture::Delete() const noexcept { glDeleteTextures(1, &ID); }
