/*

===== IMPORTANT =====

This is sample code demonstrating API, technology or techniques in development.
Although this sample code has been reviewed for technical accuracy, it is not
final. Apple is supplying this information to help you plan for the adoption of
the technologies and programming interfaces described herein. This information
is subject to change, and software implemented based on this sample code should
be tested with final operating system software and final documentation. Newer
versions of this sample code may be provided with future seeds of the API or
technology. For information about updates to this and other developer
documentation, view the New & Updated sidebars in subsequent documentation
seeds.

=====================

File: AC3DTexture.h
Abstract: Convenience class that allows to create OpenGL 2D textures from
images, text or raw data.

Version: 1.1

Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple Inc.
("Apple") in consideration of your agreement to the following terms, and your
use, installation, modification or redistribution of this Apple software
constitutes acceptance of these terms.  If you do not agree with these terms,
please do not use, install, modify or redistribute this Apple software.

In consideration of your agreement to abide by the following terms, and subject
to these terms, Apple grants you a personal, non-exclusive license, under
Apple's copyrights in this original Apple software (the "Apple Software"), to
use, reproduce, modify and redistribute the Apple Software, with or without
modifications, in source and/or binary forms; provided that if you redistribute
the Apple Software in its entirety and without modifications, you must retain
this notice and the following text and disclaimers in all such redistributions
of the Apple Software.
Neither the name, trademarks, service marks or logos of Apple Inc. may be used
to endorse or promote products derived from the Apple Software without specific
prior written permission from Apple.  Except as expressly stated in this notice,
no other rights or licenses, express or implied, are granted by Apple herein,
including but not limited to any patent rights that may be infringed by your
derivative works or by other works in which the Apple Software may be
incorporated.

The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
COMBINATION WITH YOUR PRODUCTS.

IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR
DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF
CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF
APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Copyright (C) 2008 Apple Inc. All Rights Reserved.

*/

#import <UIKit/UIKit.h>
#import <OpenGLES/ES1/gl.h>

//CONSTANTS:

typedef enum {
	kAC3DTexturePixelFormat_Automatic = 0,
	kAC3DTexturePixelFormat_RGBA8888,
	kAC3DTexturePixelFormat_RGBA4444,
	kAC3DTexturePixelFormat_RGBA5551,
	kAC3DTexturePixelFormat_RGB565,
	kAC3DTexturePixelFormat_RGB888,
	kAC3DTexturePixelFormat_L8,
	kAC3DTexturePixelFormat_A8,
	kAC3DTexturePixelFormat_LA88,
	kAC3DTexturePixelFormat_RGB_PVRTC2,
	kAC3DTexturePixelFormat_RGB_PVRTC4,
	kAC3DTexturePixelFormat_RGBA_PVRTC2,
	kAC3DTexturePixelFormat_RGBA_PVRTC4
} AC3DTexturePixelFormat;

//CLASS INTERFACES:

/*
This class allows to easily create OpenGL 2D textures from images, text or raw data.
The created AC3DTexture object will always have power-of-two dimensions.
Depending on how you create the AC3DTexture object, the actual image area of the texture might be smaller than the texture dimensions i.e. "contentSize" != (pixelsWide, pixelsHigh) and (maxS, maxT) != (1.0, 1.0).
Be aware that the content of the generated textures will be upside-down!
*/
@interface AC3DTexture : NSObject
{
@private
	GLuint						_name;
	CGSize						_size;
	NSUInteger					_width,
								_height;
	AC3DTexturePixelFormat		_format;
	GLfloat						_maxS,
								_maxT;
}
- (id) initWithData:(const void*)data pixelFormat:(AC3DTexturePixelFormat)pixelFormat pixelsWide:(NSUInteger)width pixelsHigh:(NSUInteger)height contentSize:(CGSize)size;

@property(readonly) AC3DTexturePixelFormat pixelFormat;
@property(readonly) NSUInteger pixelsWide;
@property(readonly) NSUInteger pixelsHigh;

@property(readonly) GLuint name;

@property(readonly, nonatomic) CGSize contentSize;
@property(readonly) GLfloat maxS;
@property(readonly) GLfloat maxT;
@end

/*
Drawing extensions to make it easy to draw basic quads using a AC3DTexture object.
These functions require GL_TEXTURE_2D and both GL_VERTEX_ARRAY and GL_TEXTURE_COORD_ARRAY client states to be enabled.
*/
@interface AC3DTexture (Drawing)
- (void) preload; //Forces the texture to "preload" by drawing an invisible quad with it
- (void) drawAtPoint:(CGPoint)point;
- (void) drawAtPoint:(CGPoint)point depth:(CGFloat)depth;
- (void) drawInRect:(CGRect)rect;
- (void) drawInRect:(CGRect)rect depth:(CGFloat)depth;
@end

/*
Extensions to make it easy to create a AC3DTexture object from an image file.
Note that RGBA type textures will have their alpha premultiplied - use the blending mode (GL_ONE, GL_ONE_MINUS_SRC_ALPHA).
*/
@interface AC3DTexture (Image)
- (id) initWithImagePath:(NSString*)path; //If the path is not absolute, it is assumed to be relative to the main bundle's resources
- (id) initWithImagePath:(NSString*)path sizeToFit:(BOOL)sizeToFit; //For non-power-of-two images, if "sizeToFit" is YES, the image is scaled to power-of-two dimensions, otherwise extra margins are added
- (id) initWithImagePath:(NSString*)path sizeToFit:(BOOL)sizeToFit pixelFormat:(AC3DTexturePixelFormat)pixelFormat;

- (id) initWithCGImage:(CGImageRef)image orientation:(UIImageOrientation)orientation sizeToFit:(BOOL)sizeToFit pixelFormat:(AC3DTexturePixelFormat)pixelFormat; //Primitive
@end

/*
Extensions to make it easy to create a AC3DTexture object from a string of text.
Note that the generated textures are of type A8 - use the blending mode (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA).
*/
@interface AC3DTexture (Text)
- (id) initWithString:(NSString*)string dimensions:(CGSize)dimensions alignment:(UITextAlignment)alignment fontName:(NSString*)name fontSize:(CGFloat)size;
- (id) initWithString:(NSString*)string dimensions:(CGSize)dimensions alignment:(UITextAlignment)alignment font:(UIFont*)font;
@end