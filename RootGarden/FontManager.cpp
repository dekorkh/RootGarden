#include "FontManager.h"

#include <iostream>
#include <assert.h>

using namespace std;

FontManager::FontManager()
{
	// Initialize the FreeType library.
	Error = FT_Init_FreeType(&Library);
	if (Error)
	{
		cout << "FreeType err: Failed to initialize library.";
		assert(!Error);
	}

	// Load a font int a font face object
	Error = FT_New_Face(Library, "%WINDIR%/Fonts/Consolas.ttf", 0, &FontFace);
	if (Error = FT_Err_Unknown_File_Format)
	{
		cout << "A font was located but it's format is not supported.";
		assert(!Error);
	}

	// Set sensible font params
	Error = FT_Set_Char_Size(FontFace, 8, 0, 72, 72);
	if (Error)
	{
		cout << "FreeType err: Failed to set font's character size.";
		assert(!Error);
	}

	FT_UInt16 Glyph_Index = FT_Get_Char_Index(FontFace, 'A');

	// Load the glyph into the FaceFont slot.
	Error = FT_Load_Glyph(FontFace, Glyph_Index, FT_LOAD_DEFAULT);
	if (Error)
	{
		cout << "FreeType err: Failed to set font's character size.";
		assert(!Error);
	}

	// If the glyph was loaded in a format other than bitmap, render it.
	if (FontFace->glyph->format != FT_GLYPH_FORMAT_BITMAP)
		FT_Render_Glyph(FontFace->glyph, FT_RENDER_MODE_NORMAL);
}


FT_Bitmap FontManager::GetBitmap() const
{
	return FontFace->glyph->bitmap;
}
