#pragma warning(disable : 4996)
#pragma warning(disable : 6386)
#pragma warning(disable : 6031)
#pragma warning(disable : 6387)


#include <stdio.h>
#include "data_storage.h"
#include "endianity_bl.h"

//function for storage of data to 3D array.
//xDim is the x dimension, yDim is the y dimension and zDim is the z dimension
//pathPtr is pointer to file path
bool store3dDataArrayUC(unsigned char ** array3DPtr, const size_t xDim, const size_t yDim,
	const size_t zDim, unsigned char * pathPtr, bool flagMode)
{
	const size_t dimXY = xDim * yDim;
	size_t k;//loop counter for z dimension
	FILE *cfPtr;

	//checks if the memory was allocated
	if (array3DPtr == NULL)
		return false;

	if (flagMode == true)
	{
		//writing binary data to file
		if ((fopen_s(&cfPtr, pathPtr, "ab")) != 0)
			return false;
	}
	else
	{
		//writing binary data to file
		if ((fopen_s(&cfPtr, pathPtr, "wb")) != 0)
			return false;
	}
	for (k = 0; k < zDim; k++)
	{
		fwrite(array3DPtr[k], sizeof(unsigned char), dimXY, cfPtr);
	}

	fclose(cfPtr);

	return true;
}

//function for initialization of 3D array with some constant value.
//xDim is the x dimension, yDim is the y dimension and zDim is the z dimension
//value is the initial constant value
bool store3dDataVtkUC(unsigned char ** array3DPtr, const size_t xDim, const size_t yDim,
	const size_t zDim, unsigned char * pathPtr, double h)
{
	FILE * outputfile; //file stream
	size_t dimXYZ = xDim * yDim * zDim;
	double sx = h, sy = h, sz = h;
	//checks if the memory was allocated
	if (array3DPtr == NULL)
		return false;

	//checks if the file was sucessfully opened
	if ((fopen_s(&outputfile, pathPtr, "w")) != 0) {
		puts("File could not be opened.");
	}
	else
	{
		fprintf(outputfile, "# vtk DataFile Version 3.0\n");
		fprintf(outputfile, "file in binary format\n");
		fprintf(outputfile, "BINARY\n");
		fprintf(outputfile, "DATASET STRUCTURED_POINTS\n");
		fprintf(outputfile, "DIMENSIONS %zd %zd %zd\n", xDim, yDim, zDim);

		fprintf(outputfile, "ORIGIN %f %f %f\n", (-1.25 + h / 2.), (-1.25 + h / 2.), (-1.25 + h / 2.));
		fprintf(outputfile, "SPACING %f %f %f\n", sx, sy, sz);
		fprintf(outputfile, "POINT_DATA %zd\n", dimXYZ);
		fprintf(outputfile, "SCALARS scalars unsigned_char\n");
		fprintf(outputfile, "LOOKUP_TABLE default\n");
	}
	fclose(outputfile);
	// writing data to vtk file
	store3dDataArrayUC(array3DPtr, xDim, yDim, zDim, pathPtr, true);
	return true;
}

//function for storage of data in 3D binary format.
bool store3dDataArrayD(dataType ** array3DPtr, const size_t xDim, const size_t yDim,
	const size_t zDim, unsigned char * pathPtr, Storage_Flags flags)
{
	size_t i, j, k;
	const size_t dimXY = xDim * yDim;
	FILE *cfPtr;

	//checks if the memory was allocated
	if (array3DPtr == NULL)
		return false;

	if (flags.appendToFile == true)
	{
		//writing binary data to file
		if ((fopen_s(&cfPtr, pathPtr, "ab")) != 0)
			return false;
	}
	else
	{
		//writing binary data to file
		if ((fopen_s(&cfPtr, pathPtr, "wb")) != 0)
			return false;
	}

	if (flags.revertDataBytes)
	{
		for (k = 0; k < zDim; k++)
		{
			for (i = 0; i < dimXY; i++)
			{
				dataType tmp = array3DPtr[k][i];
				revertBytes(&tmp, sizeof(dataType));
				fwrite(&tmp, sizeof(dataType), dimXY, cfPtr);
			}
		}
	}
	else
	{
		for (k = 0; k < zDim; k++) {
			for (i = 0; i < xDim; i++) {
				for (j = 0; j < yDim; j++) {
					fwrite(&array3DPtr[k][x_new(i, j, xDim)], sizeof(dataType), 1, cfPtr);
				}
			}
		}
	}

	fclose(cfPtr);

	return true;
}

//function for storage of data in 3D ASCII format.
bool store3dDataArrayASCII(dataType ** array3DPtr, const size_t xDim, const size_t yDim,
	const size_t zDim, unsigned char * pathPtr, Storage_Flags flags)
{
	const size_t dim2D = xDim * yDim;
	FILE *cfPtr;

	//checks if the memory was allocated
	if (array3DPtr == NULL)
		return false;

	if (flags.appendToFile == true)
	{
		//writing binary data to file
		if ((fopen_s(&cfPtr, pathPtr, "a")) != 0)
			return false;
	}
	else
	{
		//writing binary data to file
		if ((fopen_s(&cfPtr, pathPtr, "w")) != 0)
			return false;
	}

	if (flags.revertDataBytes)
	{
		for (size_t k = 0; k < zDim; k++)
		{
			for (size_t j = 0; j < dim2D; j++)
			{
				double tmp = array3DPtr[k][j];
				revertBytes(&tmp, sizeof(double));
				fprintf(cfPtr, "%.16lf \n", tmp);
			}
		}
	}
	else
	{
		for (size_t k = 0; k < zDim; k++)
		{
			for (size_t i = 0; i < dim2D; i++)
			{
				fprintf(cfPtr, "%.16lf \n", array3DPtr[k][i]);
			}
		}
	}

	fclose(cfPtr);

	return true;
}

//xDim is the x dimension, yDim is the y dimension and zDim is the z dimension
//value is the initial constant value
bool store3dDataVtkD(dataType ** array3DPtr, const size_t xDim, const size_t yDim,
	const size_t zDim, unsigned char * pathPtr, double h, Storage_Flags flags)
{
	FILE * outputfile; //file stream
	size_t dimXYZ = xDim * yDim * zDim;
	double sx = h, sy = h, sz = h;
	//checks if the memory was allocated
	if (array3DPtr == NULL)
		return false;

	//checks if the file was sucessfully opened
	if ((fopen_s(&outputfile, pathPtr, "w")) != 0) {
		return false;
	}
	else
	{
		fprintf(outputfile, "# vtk DataFile Version 3.0\n");
		fprintf(outputfile, "file in binary format\n");
		fprintf(outputfile, "BINARY\n");
		fprintf(outputfile, "DATASET STRUCTURED_POINTS\n");
		fprintf(outputfile, "DIMENSIONS %zd %zd %zd\n", xDim, yDim, zDim);

		//fprintf(outputfile, "ORIGIN %f %f %f\n", (-1.25 + h / 2.), (-1.25 + h / 2.), (-1.25 + h / 2.));
		fprintf(outputfile, "ORIGIN %f %f %f\n", 0, 0, 0);
		fprintf(outputfile, "SPACING %f %f %f\n", sx, sy, sz);
		fprintf(outputfile, "POINT_DATA %zd\n", dimXYZ);
		fprintf(outputfile, "SCALARS scalars double\n");
		fprintf(outputfile, "LOOKUP_TABLE default\n");
	}
	fclose(outputfile);
	// writing data to vtk file
	store3dDataArrayD(array3DPtr, xDim, yDim, zDim, pathPtr, flags);
	return true;
}

bool store3dRealDataVtkD(dataType ** array3DPtr, const size_t imageLength, const size_t imageWidth,
	const size_t imageHeight, unsigned char * pathPtr, VTK_Header_Lines * lines, Storage_Flags flags)
{
	//checks if the memory was allocated
	if (array3DPtr == NULL)
		return false;

	// imageLength == xDim, imageWidth == yDim, imageHeight == zDim
	FILE * outputfile; //file stream

					   //checks if the file was sucessfully opened
	if ((fopen_s(&outputfile, pathPtr, "w")) != 0) {
		return false;
	}
	else
	{
		//Write header
		fprintf(outputfile, "%s", lines->line1);
		fprintf(outputfile, "%s", lines->line2);
		fprintf(outputfile, "%s", lines->line3);
		fprintf(outputfile, "%s", lines->line4);
		fprintf(outputfile, "%s", lines->line5);
		fprintf(outputfile, "%s", lines->line6);
		fprintf(outputfile, "%s", lines->line7);
		fprintf(outputfile, "%s", lines->line8);
		fprintf(outputfile, "SCALARS scalars double\n");
		fprintf(outputfile, "%s", lines->line10);
		//fprintf(outputfile, "SCALARS scalars double\n");
		//fprintf(outputfile, "LOOKUP_TABLE default\n");
	}
	fclose(outputfile);
	// writing data to vtk file
	store3dDataArrayD(array3DPtr, imageLength, imageWidth, imageHeight, pathPtr, flags);
	return true;
}

bool store3dRealDataVtkUC(unsigned char ** array3DPtr, const size_t imageLength, const size_t imageWidth,
	const size_t imageHeight, unsigned char * pathPtr, VTK_Header_Lines * lines)
{
	//checks if the memory was allocated
	if (array3DPtr == NULL)
		return false;

	// imageLength == xDim, imageWidth == yDim, imageHeight == zDim
	FILE * outputfile; //file stream

					   //checks if the file was sucessfully opened
	if ((fopen_s(&outputfile, pathPtr, "w")) != 0) {
		return false;
	}
	else
	{
		//Write header
		fprintf(outputfile, "%s", lines->line1);
		fprintf(outputfile, "%s", lines->line2);
		fprintf(outputfile, "%s", lines->line3);
		fprintf(outputfile, "%s", lines->line4);
		fprintf(outputfile, "%s", lines->line5);
		fprintf(outputfile, "%s", lines->line6);
		fprintf(outputfile, "%s", lines->line7);
		fprintf(outputfile, "%s", lines->line8);
		fprintf(outputfile, "%s", lines->line9);
		fprintf(outputfile, "%s", lines->line10);
	}
	fclose(outputfile);
	// writing data to vtk file
	store3dDataArrayUC(array3DPtr, imageLength, imageWidth, imageHeight, pathPtr, true);
	return true;
}

//
//Store 2D (.pgm) image ascii
bool save2dPGM(dataType** imageDataPtr, const size_t xDim, const size_t yDim, const char* pathPtr)
{
	if (imageDataPtr == NULL)
		return false;

	size_t i, j;

	FILE* file;
	if (fopen_s(&file, pathPtr, "w") != 0) {
		printf("Unable to open the file");
		return false;
	}

	fprintf(file, "P2\n");
	fprintf(file, "%d %d\n%d\n", xDim, yDim, 255);

	for (i = 0; i < xDim; i++) {
		for (j = 0; j < yDim; j++) {
			fprintf(file, "%d ", (int)imageDataPtr[i][j]);
		}
	}
	fclose(file);

	return true;
}

//Sotore 2D (.vtk) image ascii
bool storeVTK2d(int** imageData, const size_t xDim, const size_t yDim, const char* pathPtr)
{
	if (imageData == NULL)
		return false;

	double sx = 1., sy = 1.;
	int i, j;

	FILE* f;
	if (fopen_s(&f, pathPtr, "w") != 0) {
		printf("Unable to open the file");
		return false;
	}

	fprintf(f, "# vtk DataFile Version 3.0\n");
	fprintf(f, "file in ascii format\n");
	fprintf(f, "ASCII\n");
	fprintf(f, "DATASET STRUCTURED_POINTS\n");
	fprintf(f, "DIMENSIONS %d %d 1\n", xDim, yDim);
	fprintf(f, "ORIGIN 0 0  0\n");
	fprintf(f, "SPACING %f %f 1\n", sx, sy);
	fprintf(f, "POINT_DATA %d\n", (xDim) * (yDim));
	fprintf(f, "SCALARS scalars float\n");
	fprintf(f, "LOOKUP_TABLE default\n");

	for (i = 0; i < xDim; i++) {
		for (j = 0; j < yDim; j++) {
			fprintf(f, "%d ", imageData[i][j]);
			fprintf(f, "\n");
		}
	}
	fclose(f);

	return true;
}

bool storeVTK3D(dataType** array3DPtr, const size_t xDim, const size_t yDim, const size_t zDim, unsigned char* pathPtr, double h)
{
	FILE* outputfile; //file stream
	size_t i, j, k, dimXYZ = xDim * yDim * zDim;
	double sx = h, sy = h, sz = h;
	//checks if the memory was allocated
	if (array3DPtr == NULL)
		return false;

	//checks if the file was sucessfully opened
	if ((fopen_s(&outputfile, pathPtr, "w")) != 0) {
		return false;
	}
	else
	{
		fprintf(outputfile, "# vtk DataFile Version 3.0\n");
		fprintf(outputfile, "file in binary format\n");
		fprintf(outputfile, "BINARY\n");
		fprintf(outputfile, "DATASET STRUCTURED_POINTS\n");
		fprintf(outputfile, "DIMENSIONS %d %d %d\n", xDim, yDim, zDim);
		fprintf(outputfile, "ORIGIN %f %f %f\n", 0., 0., 0.);
		fprintf(outputfile, "SPACING %f %f %f\n", sx, sy, sz);
		fprintf(outputfile, "POINT_DATA %d\n", dimXYZ);
		fprintf(outputfile, "SCALARS scalars double\n");
		fprintf(outputfile, "LOOKUP_TABLE default\n");
	}
	
	// writing data to vtk file
	for (k = 0; k < zDim; k++) {
		for (i = 0; i < xDim; i++) {
			for (j = 0; j < yDim; j++) {
				fprintf(outputfile, "%.4lf \n", array3DPtr[k][x_new(i, j, xDim)]);
			}
		}
	}
	fclose(outputfile);
	
	return true;
}