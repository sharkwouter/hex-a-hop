#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <errno.h>
#include "i18n.h"
#include "text.h"
#include "video.h"

#include <SDL_ttf.h>

static TTF_Font* font;

/**
 * \brief Splits a string into one or more lines.
 *
 * The string is altered by the algorithm and must thus be writable. This is
 * the case since the UTF8 conversion is done to all strings passed to this.
 *
 * \param str UTF8 string.
 * \param width Maximum line width.
 * \param split True if should interpret double spaces as line breaks.
 * \return Array of lines.
 */
static std::vector<std::string> TextWrapString(char* str, int width, bool split)
{
	int val;
	char tmp;
	char* start;
	char* end;
	char* word;
	std::vector<std::string> lines;

	if (*str == '\0')
		return lines;
	word = NULL;
	start = str;
	end = start + 1;
	while (*end != '\0')
	{
		tmp = *end;
		*end = '\0';

		if (tmp == '\n')
		{
			// Normal newline.
			lines.push_back(std::string(start));
			start = end;
			word = NULL;
		}
		else if (split && tmp == ' ' && end[1] == ' ')
		{
			// Double space break.
			// FIXME: Should really get rid of this and just use newlines.
			lines.push_back(std::string(start));
			//lines.push_back(std::string(""));
			start = end;
			word = NULL;
			*end = tmp;
			end += 2;
			continue;
		}
		else
		{
			// Character wrap.
			if (tmp == ' ')
				word = end;
			TTF_SizeUTF8(font, start, &val, NULL);
			if (val > width)
			{
				if (word != NULL)
				{
					*end = tmp;
					end = word;
					tmp = *end;
					*end = '\0';
				}
				lines.push_back(std::string(start));
				start = end;
				word = NULL;
			}
		}

		*end = tmp;
		end++;
	}
	lines.push_back(std::string(start));

	return lines;
}

static void TextPrintUTF8(int x, int y, const char* str)
{
	SDL_Color fg = { 255, 255, 255, 255 };
	SDL_Surface* surface = TTF_RenderUTF8_Blended (font, str, fg);
	SDL_Rect dst = {x, y, 1, 1};
	SDL_Texture* texture = SDL_CreateTextureFromSurface(screenRenderer, surface);
	SDL_RenderCopy(screenRenderer, texture, NULL, &dst);
	SDL_FreeSurface(surface);
}

static void TextPrintRAW(int x, int y, const char* str)
{
	char tmp[5000];

	ConvertToUTF8(str, tmp, 5000);
	SDL_Color fg = { 255, 255, 255, 255 };
	SDL_Surface* surface = TTF_RenderUTF8_Blended (font, tmp, fg);
	SDL_Rect dst = {x, y, 1, 1};
	SDL_Texture* texture = SDL_CreateTextureFromSurface(screenRenderer, surface);
	SDL_RenderCopy(screenRenderer, texture, NULL, &dst);
	SDL_FreeSurface(surface);
}

bool TextInit(const char* base)
{
	std::string dir(base);

#ifdef FONT_PATH
	std::string name;
	std::string fontname(FONT_PATH);
#ifdef WIN32
	if (fontname[0]=='/' || fontname[0]=='\\')
#else
	if (fontname[0]=='/')
#endif
		name = fontname;
	else
		name = dir + "/" + fontname;
#else
#error "Font path not configured, please use the --with-font-path configure argument"
#endif

	TTF_Init();
	font = TTF_OpenFont(name.c_str(), 16);
	if (font == NULL)
	{
		fprintf (stderr, "Cannot load font `%s'.\n", name.c_str());
		return false;
	}
	TTF_SetFontStyle(font, TTF_STYLE_BOLD);

	return true;
}

void TextFree()
{
	TTF_CloseFont(font);
	TTF_Quit();
}

int TextWidth(const std::string &text_utf8)
{
	int val;

	TTF_SizeUTF8(font, text_utf8.c_str(), &val, NULL);
	return val;
}

int TextHeight(const std::string &text_utf8, int width)
{
	int h;
	int val = 0;
	char* tmp = strdup(text_utf8.c_str());

	std::vector<std::string> lines = TextWrapString(tmp, width, false);
	std::vector<std::string>::iterator iter;
	for (iter = lines.begin() ; iter != lines.end() ; iter++)
	{
		TTF_SizeUTF8(font, iter->c_str(), NULL, &h);
		val += h;
	}
	free (tmp);

	return val;
}

/// Prints a left aligned string (a single line) beginning at (x,y)
// TODO: Check that the maximal text width is already set
void Print(int x, int y, const char * string, ...)
{
	va_list marker;
	va_start( marker, string );     /* Initialize variable arguments. */

	char tmp[1000];
	vsnprintf((char*)tmp, 1000, string, marker);

	TextPrintRAW(x, y, tmp);

	va_end( marker );              /* Reset variable arguments.      */
}

/// Prints a string right aligned so that it ends at (x,y)
// TODO: Check that the maximal text width is already set
void PrintR(int x, int y, const char * string, ...)
{
	va_list marker;
	va_start( marker, string );     /* Initialize variable arguments. */

	char tmp[1000];
	vsnprintf((char*)tmp, 1000, string, marker);

	TextPrintRAW(x-TextWidth(tmp), y, tmp);

	va_end( marker );              /* Reset variable arguments.      */
}

/** \brief Prints a string horizontally centered around (x,y)
 *
 *  "  " in the string is interpreted as linebreak
*/
void Print_Aligned(bool split, int x, int y, int width, const char * string, int align)
{
	int h;
	int w;
	int off;
	char tmp_utf8[5000]; // FIXME: Check this limit
	
	ConvertToUTF8(string, tmp_utf8, sizeof(tmp_utf8)/sizeof(char));
	std::vector<std::string> lines = TextWrapString(tmp_utf8, width, split);
	std::vector<std::string>::iterator iter;
	for (iter = lines.begin() ; iter != lines.end() ; iter++)
	{
		TTF_SizeUTF8(font, iter->c_str(), &w, &h);
		switch (align)
		{
			case 0: off = 0; break;
			case 2: off = -w; break;
			default: off = -w / 2; break;
		}
		TextPrintUTF8(x + off, y, iter->c_str());
		y += h;
	}
}

void PrintC(bool split, int x, int y, const char * string, ...)
{
	va_list marker;
	va_start( marker, string );     /* Initialize variable arguments. */

	char tmp[1000];
	vsnprintf((char*)tmp, 1000, string, marker);

	va_end( marker );              /* Reset variable arguments.      */

	static bool print = true; // avoid flickering!
	if (print) {
		std::cerr << "Warning: don't know window width for message:\n" << tmp << "\n";
		for (unsigned int i=0; i<strlen(tmp); ++i)
			if (!std::isspace(tmp[i]))
				print = false;
	}
	Print_Aligned(split, x, y, 2*std::min(x, SCREEN_W-x), tmp, 1);
}

void ConvertToUTF8(const std::string &text_locally_encoded, char *text_utf8, size_t text_utf8_length)
{
#ifdef ENABLE_NLS
	// Is this portable?
	size_t text_length = text_locally_encoded.length()+1;
	errno = 0;
	static const char *locale_enc = gettext_init.GetEncoding();
	iconv_t cd = iconv_open("UTF-8", locale_enc);
	ICONV_CONST char *in_buf = const_cast<char *>(&text_locally_encoded[0]);
	char *out_buf = &text_utf8[0];
	iconv(cd, &in_buf, &text_length, &out_buf, &text_utf8_length);
	iconv_close(cd);
	if (errno != 0)
		std::cerr << "An error occurred recoding " << text_locally_encoded << " to UTF8" << std::endl;
#else
	strcpy (text_utf8, text_locally_encoded.c_str ());
#endif
}
