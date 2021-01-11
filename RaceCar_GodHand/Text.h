#pragma once
#include "Color.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "glut/glut.h"

enum class Font
{
    BY_15 = 2,
    BY_13,
    TIMES_ROMAN_10,
    TIMES_ROMAN_24,
    HELVETICA_10,
    HELVETICA_12,
    HELVETICA_18
};

class Text
{
public:
    void SetFont(Font font)
    {
        switch (font)
        {
        case Font::BY_15:
            this->font = GLUT_BITMAP_9_BY_15;
            break;
        case Font::BY_13:
            this->font = GLUT_BITMAP_8_BY_13;
            break;
        case Font::TIMES_ROMAN_10:
            this->font = GLUT_BITMAP_TIMES_ROMAN_10;
            break;
        case Font::TIMES_ROMAN_24:
            this->font = GLUT_BITMAP_TIMES_ROMAN_24;
            break;
        case Font::HELVETICA_10:
            this->font = GLUT_BITMAP_HELVETICA_10;
            break;
        case Font::HELVETICA_12:
            this->font = GLUT_BITMAP_HELVETICA_12;
            break;
        case Font::HELVETICA_18:
            this->font = GLUT_BITMAP_HELVETICA_18;
            break;
        default:
            this->font = GLUT_BITMAP_8_BY_13;
            break;
        }
    };

    void PrintTextOnMap(float x, float y, float z, Color color, Font font, const char* string)
    {
        glColor3f(color.r, color.g, color.b);
        glRasterPos3f(x, y, z);
        int len, i;
        len = (int)strlen(string);
        SetFont(font);
        for (i = 0; i < len; ++i)
        {
            glutBitmapCharacter(this->font, string[i]);
        }
    }

    void* font;
};