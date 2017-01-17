#pragma once

#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std;

class FontManager
{
	FontManager();

	FT_Bitmap GetBitmap() const;

private:
	FT_Error Error;
	FT_Library Library;
	FT_Face FontFace;
};