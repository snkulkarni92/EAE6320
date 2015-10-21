#include "Mesh.h"

#include <cstdio>
#include <cassert>
#include <sstream>
#include "../UserOutput/UserOutput.h"

namespace eae6320
{
	namespace Graphics
	{
		//Helper Functionss

		Mesh::Mesh()
		{

			mVertexData = NULL;
			mIndexData = NULL;
			mVertexCount = 0;
			mIndexCount = 0;
		}


		void * Mesh::LoadMesh(const char * i_path)
		{
			FILE * iFile;
			void * buffer;
			size_t fileSize;

			fopen_s(&iFile, i_path, "rb");
			if (iFile != NULL)
			{
				fseek(iFile, 0, SEEK_END);
				fileSize = ftell(iFile);
				rewind(iFile);

				buffer = (void *)malloc(fileSize);

				size_t result = fread(buffer, 1, fileSize, iFile);
				if (result != fileSize)
				{
					eae6320::UserOutput::Print("Error reading file");
					free(buffer);
					return NULL;
				}
				char * iPointer = reinterpret_cast<char *>(buffer);
				mVertexCount = *reinterpret_cast<uint32_t *>(iPointer);
				iPointer += sizeof(mVertexCount);
				mIndexCount = *reinterpret_cast<uint32_t *>(iPointer);
				iPointer += sizeof(mIndexCount);
				mVertexData = reinterpret_cast<sVertex *>(iPointer);
				iPointer += sizeof(sVertex) * mVertexCount;
				mIndexData = reinterpret_cast<uint32_t *>(iPointer);
				iPointer += sizeof(uint32_t) * mIndexCount;

				fclose(iFile);
				return buffer;
			}
			return NULL;
		}
	}
}