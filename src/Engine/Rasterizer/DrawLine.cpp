//
//  DrawLine.cpp
//   2D Rasterizer
//
//  Created by Diego Revilla
//  Copyright � 2020 . All rights reserved.
//

#include <AEEngine.h>
#include "../Rasterizer/Rasterizer.h"

namespace Rasterizer {

    static EDrawLineMethod Sline_method = eDL_NAIVE;

    // ------------------------------------------------------------------------
    /*! Draw Horizontal Line
    *
    *   Draws a Horizontal Line
    */ // --------------------------------------------------------------------
    void DrawHorizontalLine(const AEVec2& p1, const AEVec2& p2, const Color& c) {

        //Calculate the Offset
        int offset = abs(static_cast<int>(p1.x) - static_cast<int>(p2.x));

        //Draw all the pixels in both points (as a horizontal line)
        for (float i = 0; i <= offset; i++) {

            //Set the Pixel
            FrameBuffer::SetPixel(static_cast<int>(
                p1.x + i / offset * (p2.x - p1.x)), 
                static_cast<u32>(p1.y), c);

        }
    }

    // ------------------------------------------------------------------------
    /*! Draw Vertical Line
    *
    *   Draws a Vertical Line
    */ // --------------------------------------------------------------------
    void DrawVerticalLine(const AEVec2& p1, const AEVec2& p2, const Color& c) {

        //Calculate the Offset
        int offset = abs(static_cast<int>(p1.y) - static_cast<int>(p2.y));

        //Draw all the pixels in both points (as a vertical line)
        for (float i = 0; i <= offset; i++) {

            //Set the Pixel
            FrameBuffer::SetPixel(static_cast<u32>(p1.x), static_cast<int>(
                    p1.y + i/ offset * (p2.y - p1.y)), c);

        }
    }

    // ------------------------------------------------------------------------
    /*! Draw Diagonal Line
    *
    *   Draws a Diagonal Line
    */ // --------------------------------------------------------------------
    void DrawDiagonalLine(const AEVec2& p1, const AEVec2& p2, const Color& c) {

        //Calculate the Offset
        int offset = static_cast<int>(sqrt(abs(pow(static_cast<int>(p1.y), 2) - 
                pow(static_cast<int>(p2.y), 2))));

        //Create a Temporary Point
        AEVec2 Point = p1;

        //Draw all the pixels in both points (as a line)
        for (int i = 0; i < offset; i++) {

            //Set the Pixel
            FrameBuffer::SetPixel(static_cast<int>(Point.x), static_cast<int>(Point.y), c);
            Point.x++;
            Point.y++;

        }
    }

    // ------------------------------------------------------------------------
    /*! Draw Rect
    *
    *   Draws a Rectangle
    */ // --------------------------------------------------------------------
    void DrawRect(const AEVec2& p, const AEVec2& s, const Color& c) {

        //Calculate the Top Left Corner Point
        AEVec2 TopLeftCorner(p.x - (s.x / 2), p.y + (s.y / 2));

        //Calculate the Top Right Corner Point
        AEVec2 TopRightCorner(p.x + (s.x / 2), p.y + (s.y / 2));
        
        //Calculate the Bottom Left Corner Point
        AEVec2 BottomLeftCorner(p.x - (s.x / 2), p.y - (s.y / 2));

        //Calculate the Bottom Right Corner Point
        AEVec2 BottomRightCorner(p.x + (s.x / 2), p.y - (s.y / 2));

        //Draw the Left Line
        DrawVerticalLine(BottomLeftCorner, TopLeftCorner, c);
        
        //Draw the Right Line
        DrawVerticalLine(BottomRightCorner, TopRightCorner, c);

        //Draw the Bottom Line
        DrawHorizontalLine(BottomLeftCorner, BottomRightCorner, c);

        //Draw the Top Line
        DrawHorizontalLine(TopLeftCorner, TopRightCorner, c);

    }

    // ------------------------------------------------------------------------
    /*! Draw Line
    *
    *   Draws a Line with the fastest Method
    */ // --------------------------------------------------------------------
    void DrawLine(const AEVec2& p1, const AEVec2& p2, const Color& c) {

        f32 dx = p2.x - p1.x;
        f32 dy = p2.y - p1.y;

        DrawLineBresenham(p1, p2, c);
        return;

        //if we have 0 slope
        if (!dx) {

            //Draw a Vertical Line
            DrawVerticalLine(p1, p2, c);

        //if we have infinite slope
        } else if (!dy) {

            //Draw a Horizontal Line
            DrawHorizontalLine(p1, p2, c);

        //if the slope is 1
        } else if (fabs(dx) == fabs(dy)) {

            //Draw Diagonal Line
            DrawDiagonalLine(p1, p2, c);

        //else
        } else {

            //choose the selected drawing method
            switch (Sline_method) {

            //case NAIVE
            case eDL_NAIVE:

                //Draw a Naive Line
                DrawLineNaive(p1, p2, c);

                //break
                break;

            //case OPTIMAL
            case eDL_DDA:

                //Draw the Digital Diagonal Analysis Line
                DrawLineDDA(p1, p2, c);
                
                //break
                break;

            case eDL_BRESENHAM:

                DrawLineBresenham(p1, p2, c);

                break;

            }
        }
    }

    // ------------------------------------------------------------------------
    /*! Get Draw Line Method
    *
    *   Gets the Method in which we are going to Draw
    */ // --------------------------------------------------------------------
    EDrawLineMethod GetDrawLineMethod() {

        //retutn the Method that we use
        return Sline_method;

    }

    // ------------------------------------------------------------------------
    /*! Set Draw Line Method
    *
    *   Sets the Method in which we are going to Draw
    */ // --------------------------------------------------------------------
    void SetDrawLineMethod(EDrawLineMethod dlm) {

        //Set the Method
        Sline_method = dlm;

    }

    // ------------------------------------------------------------------------
    /*! Draw Line Naive
    *
    *   Draws Line with the Naive Method
    */ // --------------------------------------------------------------------
    void DrawLineNaive(const AEVec2& p1, const AEVec2& p2, const Color& c) {

        //find the slope for this line
        float m =(p2.y - p1.y) /(p2.x - p1.x);

        //find the bias for this line
        float b = m * p1.x - p1.y;

        //if we have a slope that is lower than 1
        if (fabs(m) < 1) {

            //find the starting point for X
            u32 StartX = static_cast<u32>(min(p1.x, p2.x));

            //start the ending point for X
            u32 MaxX = static_cast<u32>(max(p2.x, p1.x));

            //loop over the X axis
            for (u32 x = StartX; x <= MaxX;) {

                //Set the right pixel
                FrameBuffer::SetPixel(x, Round(m * x - b), c);
                _asm inc x

            }
            
        //else
        } else {

            //find the starting point for Y
            u32 StartY = static_cast<u32>(min(p1.y, p2.y));

            //start the ending point for Y
            u32 MaxY = static_cast<u32>(max(p2.y, p1.y));

            //loop over the Y axis
            for (u32 y = StartY; y <= MaxY;) {

                //Set the right pixel
                FrameBuffer::SetPixel(Round((b + y)/m), y, c);
                _asm inc y

            }
        }
    }

    // ------------------------------------------------------------------------
    /*! Draw Line DDA
    *
    *   Draws Line with the Digital Diagonal Analysis method
    */ // --------------------------------------------------------------------
    void DrawLineDDA(const AEVec2& p1, const AEVec2& p2, const Color& c) {

        //find the slope for this line
        float m = (p2.y - p1.y) / (p2.x - p1.x);

        //if we have a slope that is lower than 1
        if (fabs(m) < 1) {

            //find the starting point of the line on X
            u32 StartX = static_cast<u32>(min(p1.x, p2.x));

            //find the ending point of the line on X
            u32 MaxX = static_cast<u32>(max(p2.x, p1.x));

            //Find a first value for Y
            float y = m > 0 ? min(fabs(p1.y), fabs(p2.y)) : max(fabs(p1.y), fabs(p2.y));

            //loop over the x axis
            for (unsigned x = StartX; x <= MaxX;) {

                //Set the Right Pixel
                FrameBuffer::SetPixel(x, Round(y), c);
                
                //Increment Y
                y += m;
                _asm inc x;

            }

        //else
        } else {

            //find the starting point on the line on Y
            u32 StartY = static_cast<u32>(min(p1.y, p2.y));     

            //find the ending point of the line on Y
            u32 MaxY = static_cast<u32>(max(p2.y, p1.y));

            //find the inverse slope
            m = (p2.x - p1.x) / (p2.y - p1.y);

            //find the initial value for x
            float x = m > 0 ? min(fabs(p1.x), fabs(p2.x)) : max(fabs(p1.x), fabs(p2.x));

            //loop over the y axis
            for (unsigned long y = StartY; y <= MaxY;) {

                //set the right pixel
                FrameBuffer::SetPixel(Round(x), y, c);

                //Increment X
                x += m;
                _asm inc y

            }
        }
    }

    // ------------------------------------------------------------------------
    /*! Draw Line Bresenham
    *
    *   Draws Line using the Bresenham method
    */ // --------------------------------------------------------------------
    void DrawLineBresenham(const AEVec2& p1, const AEVec2& p2, const Color& c) {

        const float dx = p2.x - p1.x, dy = p2.y - p1.y;        
        const u8 r = static_cast<u8>(c.r * 255), g = static_cast<u8>(c.g * 255),
            b = static_cast<u8>(c.b * 255), a = static_cast<u8>(c.a * 255);
        const int stepX = dx > 0 ? 1 : -1, stepY = dy > 0 ? 1 : -1;
        const unsigned Dx_ = abs(Round(dx)), Dy_ = abs(Round(dy));

        //if the slope is less than 1
        if (abs(dy / dx) < 1) {

            int ErrMg_ = 2 * Dy_ - Dx_;
            const u32 eX_ = Round(p2.x);
            const unsigned twoDiffY_ = 2 * Dy_, diffDP_ = twoDiffY_ - 2 * Dx_;

            //Until we reach the end of our line
            for (u32 x = Round(p1.x), y = Round(p1.y); x != eX_; x += stepX) {

                FrameBuffer::SetPixel(x, y, r, g, b, a);

                //if the Error Margin is greater than 0
                if (ErrMg_ > 0) {

                    y += stepY;
                    ErrMg_ += diffDP_;

                } else {

                    ErrMg_ += twoDiffY_;

                }
            }
        } else {

            //Calculate the starting Error Margin
            int ErrMg_ = 2 * Dx_ - Dy_;

            //Calculate the Ending Point
            const u32 eY_ = Round(p2.y);

            //Calculate 2Dx and 2Dx - 2Dy to avoid computing it everytime is needed
            const unsigned twoDiffX_ = 2 * Dx_, diffDP_ = twoDiffX_ - 2 * Dy_;

            //Until we reach the end of the line
            for (u32 x = Round(p1.x), y = Round(p1.y); y != eY_; y += stepY) {

                //set the corresponding pixel
                FrameBuffer::SetPixel(x, y, r, g, b, a);

                //if the error margin is greater than 0
                if (ErrMg_ > 0) {

                    x += stepX;
                    ErrMg_ += diffDP_;

                //else
                } else {

                    ErrMg_ += twoDiffX_;

                }
            }
        }
    }

    void DrawLine(const AEVec2& p1, const Color& c1, const AEVec2& p2, const Color& c2) {

        float dx = (p2.x - p1.x);

        //find the slope for this line
        float m = (p2.y - p1.y) / (p2.x - p1.x);

        //if we have a slope that is lower than 1
        if (fabs(m) < 1) {

            //find the starting point of the line on X
            u32 StartX = static_cast<u32>(min(p1.x, p2.x));

            //find the ending point of the line on X
            u32 MaxX = static_cast<u32>(max(p2.x, p1.x));
            
            const float diff = 1.f / static_cast<float>(MaxX);

            //Find a first value for Y
            float y = m > 0 ? min(fabs(p1.y), fabs(p2.y)) : max(fabs(p1.y), fabs(p2.y));
            float t = diff;

            //loop over the x axis
            for (unsigned x = StartX; x <= MaxX;) {


                //Set the Right Pixel
                FrameBuffer::SetPixel(x, Round(y), Color(c1.r + (t) * (c2.r - c1.r)
                , c1.g + (t) * (c2.g - c1.g), c1.b + (t) * (c2.b - c1.b), 
                    c1.a + (t) * (c2.a - c1.a)));


                t += diff;
                //Increment Y
                y += m;
                _asm inc x;

            }

            //else
        } else {

            //find the starting point on the line on Y
            u32 StartY = static_cast<u32>(min(p1.y, p2.y));

            //find the ending point of the line on Y
            u32 MaxY = static_cast<u32>(max(p2.y, p1.y));

            //find the inverse slope
            m = (p2.x - p1.x) / (p2.y - p1.y);

            //find the initial value for x
            float x = m > 0 ? min(fabs(p1.x), fabs(p2.x)) : max(fabs(p1.x), fabs(p2.x));

            const float diff = 1.f / static_cast<float>(MaxY);
            float t = diff;

            //loop over the y axis
            for (unsigned long y = StartY; y <= MaxY;) {

                const float diff = 
                static_cast<float>(y) / static_cast<float>(MaxY);

                //Set the Right Pixel
                FrameBuffer::SetPixel(static_cast<unsigned long>(x), static_cast<unsigned long>(y), Color(c1.r + t * (c2.r - c1.r)
                    , c1.g + t * (c2.g - c1.g), c1.b + t * (c2.b - c1.b),
                    c1.a + t * (c2.a - c1.a)));


                t += diff;
                //Increment X
                x += m;
                _asm inc y

            }
        }
    }

    void DrawHorizontalLine(const AEVec2& p1, const Color& c1, const AEVec2& p2, const Color& c2) {



    }

    void DrawVerticalLine(const AEVec2& p1, const Color& c1, const AEVec2& p2, const Color& c2) {



    }
}