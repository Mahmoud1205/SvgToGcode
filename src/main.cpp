#include "defines.h"

#define NANOSVG_IMPLEMENTATION
#include "nanosvg.h"
#include <cstdarg>
#include <cstring>
#include <cmath>

/**
 * G-CODE COMMANDS REFERENCE
 * - G90:		absolute positioning (relative to origin point)
 * - G91:		relative positioning (relative to current tool position)
 * - G0:		rapid movement (fastest that the cnc can do)
 * - G1:		straight line (set feedrate using F, e.g. F500 for 500unit/m)
 * - G21:		set millimeters mode
 * - X, Y, Z, and F: specify coordinated and feedrate
 * - if F is defined once, in any line, it stays like that until changed
 * - F can be used as a standalone command. e.g. one line that only has the feedrate
 * - M30:		end program
 */

// TODO: argument to give the program a specific size and it sets the DPI accordingly

struct
{
	FILE*			mOutFile = nullptr;
	const char*		mInFileName = nullptr;
	NSVGimage*		mSvg = nullptr;
	F32				mDPI = 96.0f;
	F32				mDepthZ = -2.0f;
	F32				mFeedrateXY = 3000.0f;
} gState;

void insertCmd(const char* inCmd, ...);
void usageExit(const char* inArgv0);
void handleArgs(I32 inArgc, char* inArgv[]);

I32 main(I32 argc, char* argv[])
{
	handleArgs(argc, argv);

	NSVGimage* svg = gState.mSvg;

	for (NSVGshape* shape = svg->shapes; shape != nullptr; shape = shape->next)
	{
		for (NSVGpath* path = shape->paths; path != nullptr; path = path->next)
		{
			F32 startX = path->pts[0];
			F32 startY = path->pts[1];
			startY = svg->height - startY - svg->height;

			insertCmd("G90");
			insertCmd("G0 Z0.5");
			insertCmd("G0 X%f Y%f", startX, startY);
			insertCmd("G0 Z0");

			insertCmd("G91");
			insertCmd("G1 Z%f F1000", gState.mDepthZ);

			insertCmd("G90");
			for (U32 i = 0; i < path->npts; i++)
			{
				F32 x = path->pts[i * 2];
				F32 y = path->pts[i * 2 + 1];
				y = svg->height - y - svg->height;

				insertCmd("G1 X%f Y%f F%f", x, y, gState.mFeedrateXY);
			}
		}
	}

	insertCmd("G90");
	insertCmd("G0 Z5");
	insertCmd("M30");

	nsvgDelete(svg);
	fclose(gState.mOutFile);

	return 0;
}

void insertCmd(const char* inCmd, ...)
{
	assert(gState.mOutFile);

	va_list list;
	va_start(list, inCmd);
	vfprintf(gState.mOutFile, inCmd, list);
	fputc('\n', gState.mOutFile);
	va_end(list);
}

void usageExit(const char* inArgv0)
{
	printf("Usage: %s -i [inFileName.svg] -o [outFileName.nc]\n", inArgv0);
	puts("'-f' [optional, default: 3000.0] can be used to set the XY feedrate in mm/min.");
	puts("'-z' [optional, default: -2.0] can be used to set the z depth to move while drawing, in millimeters.");
	puts("'-d' [optional, default: 96.0] can be used to set the DPI (dots per inch) to parse the input SVG with.");
	exit(1);
}

void handleArgs(I32 inArgc, char* inArgv[])
{
	U32 numRequiredArgs = 0;
	const U32 kExpectedNumRequiredArgs = 2; // -i and -o

	for (I32 i = 1; i < inArgc; i++)
	{
		const char* arg = inArgv[i];
		if (strncmp(arg, "-i", 2) == 0)
		{
			if (++i == inArgc)
			{
				eputs("Error: Incomplete argument '-i'.");
				usageExit(inArgv[0]);
			}

			gState.mInFileName = inArgv[i];

			// we just open and close it here to check if it exists at all or not,
			// before parsing it. we parse it later so we have the DPI
			FILE* input = fopen(inArgv[i], "r");
			if (!input)
			{
				eprintf("Error: Input file does not exist: '%s'.\n", inArgv[i]);
				usageExit(inArgv[0]);
			}
			fclose(input);

			numRequiredArgs++;

			continue;
		}

		if (strncmp(arg, "-o", 2) == 0)
		{
			if (++i == inArgc)
			{
				eputs("Error: Incomplete argument '-o'.");
				usageExit(inArgv[0]);
			}

			gState.mOutFile = fopen(inArgv[i], "wb");
			if (!gState.mOutFile)
			{
				eprintf("Error: Failed to create file: '%s'.\n", inArgv[i]);
				usageExit(inArgv[0]);
			}

			numRequiredArgs++;

			continue;
		}

		if (strncmp(arg, "-d", 2) == 0)
		{
			if (++i == inArgc)
			{
				eputs("Error: Incomplete argument '-d'.");
				usageExit(inArgv[0]);
			}

			// we use nsvg's atof function instead of std's to avoid locale issues
			gState.mDPI = (F32)nsvg__atof(inArgv[i]);
			if (isnan(gState.mDPI) || isinf(gState.mDPI) || gState.mDPI <= 0.0f)
			{
				eprintf("Error: DPI was parsed as NaN, Inf, zero, or a negative value. ");
				eputs("This can happen if the DPI argument is invalid, or was set as zero or a negative value.");
				usageExit(inArgv[0]);
			}

			continue;
		}

		if (strncmp(arg, "-z", 2) == 0)
		{
			if (++i == inArgc)
			{
				eputs("Error: Incomplete argument '-z'.");
				usageExit(inArgv[0]);
			}

			// we use nsvg's atof function instead of std's to avoid locale issues
			gState.mDepthZ = (F32)nsvg__atof(inArgv[i]);
			if (isnan(gState.mDepthZ) || isinf(gState.mDepthZ) || gState.mDepthZ >= 0.0f)
			{
				eprintf("Error: Z depth was parsed as NaN, Inf, zero, or a positive value. ");
				eprintf("This can happen if the Z depth argument is invalid, or was set as zero or a positive value. ");
				eputs("The Z depth must be a negative value so that the pen goes down into the paper.");
				usageExit(inArgv[0]);
			}

			continue;
		}

		if (strncmp(arg, "-f", 2) == 0)
		{
			if (++i == inArgc)
			{
				eputs("Error: Incomplete argument '-f'.");
				usageExit(inArgv[0]);
			}

			// we use nsvg's atof function instead of std's to avoid locale issues
			gState.mFeedrateXY = (F32)nsvg__atof(inArgv[i]);
			if (isnan(gState.mFeedrateXY) || isinf(gState.mFeedrateXY) || gState.mFeedrateXY <= 0.0f)
			{
				eprintf("Error: XY feedrate was parsed as NaN, Inf, zero, or a negative value. ");
				eprintf("This can happen if the feedrate argument is invalid, or was set as zero or a positive value. ");
				eputs("The XY feedrate must be a positive value.");
				usageExit(inArgv[0]);
			}

			continue;
		}
	}

	if (numRequiredArgs != kExpectedNumRequiredArgs)
	{
		eputs("Error: Missing arguments. -i and -o are required.");
		usageExit(inArgv[0]);
	}

	gState.mSvg = nsvgParseFromFile(gState.mInFileName, "mm", gState.mDPI);
}
