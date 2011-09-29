/*
 * Texture image definitions from Chapter 8.
 *
 * Written by Michael Sweet
 */

/*
 * Include necessary headers.
 */

#ifndef _TEXTURE_H_
#  define _TEXTURE_H_

#  include "bitmap.h"


/*
 * Make this header file work with C and C++ source code...
 */

#  ifdef __cplusplus
extern "C" {
#  endif /* __cplusplus */

extern GLuint TextureLoad(char *filename, GLboolean alpha,
                          GLenum minfilter, GLenum magfilter,
			  GLenum wrap);

#  ifdef __cplusplus
}
#  endif /* __cplusplus */
#endif /* !_TEXTURE_H_ */

/*
 * End of "$Id: texture.h,v 1.2 2006/04/13 21:34:20 snagappa Exp $".
 */
