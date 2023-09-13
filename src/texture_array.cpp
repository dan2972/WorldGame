#include <iostream>
#include "stb_image.h"
#include "texture_array.h"

#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF


TextureArray::TextureArray()
    : Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_NEAREST_MIPMAP_LINEAR), Filter_Max(GL_NEAREST)
{
    glGenTextures(1, &this->ID);
    stbi_set_flip_vertically_on_load(true);
}

void TextureArray::generate(unsigned int width, unsigned int height, const std::vector<unsigned char*>& data)
{
    this->Width = width;
    this->Height = height;
    // create Texture
    glBindTexture(GL_TEXTURE_2D_ARRAY, this->ID);
    GLint max_layers;
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &max_layers);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, this->Internal_Format, width, height, max_layers, 0, this->Image_Format, GL_UNSIGNED_BYTE, NULL);
    
    for(unsigned int i = 0; i < data.size(); i++)
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, this->Image_Format, GL_UNSIGNED_BYTE, data[i]);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 4);
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    // set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, this->Wrap_S);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, this->Wrap_T);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
    
    // unbind texture
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void TextureArray::bind() const
{
    glBindTexture(GL_TEXTURE_2D_ARRAY, this->ID);
}