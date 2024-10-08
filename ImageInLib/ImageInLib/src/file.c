#include "file.h"
#include "data_load.h"
#include "data_storage.h"

bool manageFile(void* imageDataPtr, const size_t length, const size_t width,
	const size_t height, unsigned char * pathPtr, OperationType operation, LoadDataType dType, Storage_Flags flags)
{
	bool status = false; // Initial Status, only changed to true if the operation is successful
    switch (operation)
    {
        case LOAD_DATA_RAW:
            status = load3dDataArrayRAW((dataType**)imageDataPtr, length, width, height, pathPtr, dType);
            break;
        case LOAD_2D_DATA_RAW:
            status = load2dArrayRAW((dataType*)imageDataPtr, width, height, pathPtr, dType);
            break;
        case LOAD_2D_DATA_PGM:
            status = load2dPGM((dataType*)imageDataPtr, width, height, pathPtr);
            break;
        case STORE_DATA_RAW:
            switch (dType)
            {
                case BINARY_DATA:
                    status = store3dDataArrayD((dataType**)imageDataPtr, length, width, height, pathPtr, flags);
                    break;
                case ASCII_DATA:
                    status = store3dDataArrayASCII((dataType**)imageDataPtr, length, width, height, pathPtr, flags);
                    break;
                default:
                    break;
            }
            break;
        case STORE_2D_RAW_DATA:
            store2dRawData((dataType*)imageDataPtr, width, height, pathPtr, flags);
            break;
        case STORE_2D_DATA_PGM:
            status = store2dPGM((dataType*)imageDataPtr, width, height, pathPtr, dType == BINARY_DATA);
            break;
        case STORE_CSV:
            status = storeCSV((dataType**)imageDataPtr, width, height, pathPtr);
            break;
        case LOAD_CSV:
            status = loadCSV((dataType**)imageDataPtr, width, height, pathPtr);
            break;
        default:
            break;
    }
	return status;
}

void convertTodataType(unsigned char ** dataPtrUC, dataType ** dataPtrD, const size_t dimXY, const size_t height)
{
	for (size_t k = 0; k < height; k++)
	{
		for (size_t i = 0; i < dimXY; i++)
		{
			dataPtrD[k][i] = (dataType)dataPtrUC[k][i];
		}
	}
}

void convertFromShortTodatatype(short** dataPtrSH, dataType** dataPtrF, const size_t dimXY, const size_t height)
{
	for (size_t k = 0; k < height; k++)
	{
		for (size_t i = 0; i < dimXY; i++)
		{
			dataPtrF[k][i] = (dataType)dataPtrSH[k][i];
		}
	}
}
