// Header Files
//=============

#include "cMeshBuilder.h"

#include <cstdio>
#include <cassert>
#include <sstream>
#include "../../Engine/Windows/WindowsFunctions.h"
// Interface
//==========

// Build
//------

//Helper Functions
bool LoadTableWithKey(lua_State& io_luaState, const char * key);
bool LoadTableWithIndex(lua_State& io_luaState, const int index);
bool LoadValueWithIndex(lua_State& io_luaState, const int index);
eae6320::cMeshBuilder::sVertex GetVertexData(lua_State& io_luaState);



bool eae6320::cMeshBuilder::Build(const std::vector<std::string>&)
{
	bool wereThereErrors = false;

	// Create a new Lua state
	lua_State* luaState = NULL;
	{
		luaState = luaL_newstate();
		if (!luaState)
		{
			wereThereErrors = true;
			eae6320::OutputErrorMessage("Failed to create a new Lua state", __FILE__);
			goto OnExit;
		}
	}

	// Load the asset file as a "chunk",
	// meaning there will be a callable function at the top of the stack
	{
		const int luaResult = luaL_loadfile(luaState, m_path_source);
		if (luaResult != LUA_OK)
		{
			wereThereErrors = true;
			eae6320::OutputErrorMessage(lua_tostring(luaState, -1), __FILE__);
			// Pop the error message
			lua_pop(luaState, 1);
			goto OnExit;
		}
	}
	// Execute the "chunk", which should load the asset
	// into a table at the top of the stack
	{
		const int argumentCount = 0;
		const int returnValueCount = LUA_MULTRET;	// Return _everything_ that the file returns
		const int noMessageHandler = 0;
		const int luaResult = lua_pcall(luaState, argumentCount, returnValueCount, noMessageHandler);
		if (luaResult == LUA_OK)
		{
			// A well-behaved asset file will only return a single value
			const int returnedValueCount = lua_gettop(luaState);
			if (returnedValueCount == 1)
			{
				// A correct asset file _must_ return a table
				if (!lua_istable(luaState, -1))
				{
					wereThereErrors = true;
					std::stringstream errorMessage;
					errorMessage << "Asset files must return a table (instead of a " <<
						luaL_typename(luaState, -1) << ")\n";
					eae6320::OutputErrorMessage(errorMessage.str().c_str(),__FILE__);
					// Pop the returned non-table value
					lua_pop(luaState, 1);
					goto OnExit;
				}
			}
			else
			{
				wereThereErrors = true;
				std::stringstream errorMessage;
				errorMessage << "Asset files must return a single table (instead of " <<
					returnedValueCount << " values)"
					"\n";
				eae6320::OutputErrorMessage(errorMessage.str().c_str(),__FILE__);
				// Pop every value that was returned
				lua_pop(luaState, returnedValueCount);
				goto OnExit;
			}
		}
		else
		{
			wereThereErrors = true;
			std::stringstream errorMessage;
			errorMessage << lua_tostring(luaState, -1);
			eae6320::OutputErrorMessage(errorMessage.str().c_str(),__FILE__);
			// Pop the error message
			lua_pop(luaState, 1);
			goto OnExit;
		}
	}

	// If this code is reached the asset file was loaded successfully,
	// and its table is now at index -1
	if (!ProcessMeshData(*luaState))
	{
		wereThereErrors = true;
	}

	// Pop the table
	lua_pop(luaState, 1);


	//Write to file
	if (!wereThereErrors)
	{
		FILE * oFile;
		fopen_s(&oFile, m_path_target, "wb");
		if (oFile != NULL)
		{
			fwrite(&mVertexCount, sizeof(uint32_t), 1, oFile);
			fwrite(&mIndexCount, sizeof(uint32_t), 1, oFile);
			fwrite(mVertexData, sizeof(sVertex), mVertexCount, oFile);
			fwrite(mIndexData, sizeof(uint32_t), mIndexCount, oFile);
			fclose(oFile);
		}
	}
OnExit:

	if (luaState)
	{
		// If I haven't made any mistakes
		// there shouldn't be anything on the stack,
		// regardless of any errors encountered while loading the file:
		assert(lua_gettop(luaState) == 0);

		lua_close(luaState);
		luaState = NULL;
	}
	if (mVertexData)
		delete mVertexData;
	if (mIndexData)
		delete mIndexData;

	return !wereThereErrors;
}

bool eae6320::cMeshBuilder::ProcessMeshData(lua_State& io_luaState)
{
	//Add Vertices table to stack
	if (!LoadTableWithKey(io_luaState, "vertices"))
	{
		return false;
	}

	//Get number of vertices
	mVertexCount = luaL_len(&io_luaState, -1);

	mVertexData = new sVertex[mVertexCount];

	//Iterate through all the vertices

	for (unsigned int i = 1; i <= mVertexCount; i++)
	{
		//Load 'i'th vertex table
		if (!LoadTableWithIndex(io_luaState, i))
		{
			return false;
		}
		mVertexData[i - 1] = GetVertexData(io_luaState);
		//Pop 'i'th vertex table from stack
		lua_pop(&io_luaState, 1);
	}

	//Pop Vertices table from stack
	lua_pop(&io_luaState, 1);


	//Add Indices table to stack
	if (!LoadTableWithKey(io_luaState, "indices"))
	{
		return false;
	}

	//Get number of Indices

	mIndexCount = luaL_len(&io_luaState, -1);

	mIndexData = new uint32_t[mIndexCount];

	//Iterate through all the indices

	for (unsigned int i = 1; i <= mIndexCount; i++)
	{
		//Load 'i'th index value
		if (!LoadValueWithIndex(io_luaState, i))
		{
			return false;
		}
		int x = i - 1;
#if defined EAE6320_PLATFORM_D3D //Left handed index winding
		if (x % 3 == 1)
		{
			x++;
		}
		else if (x % 3 == 2)
		{
			x--;
		}
#endif
		mIndexData[x] = (uint32_t)lua_tointeger(&io_luaState, -1);
		//Pop 'i'th index vaue from stack
		lua_pop(&io_luaState, 1);
	}

	//Pop Vertices table from stack
	lua_pop(&io_luaState, 1);

	return true;
}
eae6320::cMeshBuilder::sVertex GetVertexData(lua_State& io_luaState)
{
	//Vertex i table is on top of stack
	eae6320::cMeshBuilder::sVertex vertex;
	if (LoadTableWithKey(io_luaState, "position"))
	{
		int a = luaL_len(&io_luaState, -1);
		//Position Table is on top of stack
		LoadValueWithIndex(io_luaState, 1);
		vertex.x = (float)lua_tonumber(&io_luaState, -1);
		lua_pop(&io_luaState, 1);
		LoadValueWithIndex(io_luaState, 2);
		vertex.y = (float)lua_tonumber(&io_luaState, -1);
		lua_pop(&io_luaState, 1);
		//Pop Position Table from stack
		lua_pop(&io_luaState, 1);
	}
	if (LoadTableWithKey(io_luaState, "color"))
	{
		//Color Table is top of stack
		LoadValueWithIndex(io_luaState, 1);
		vertex.r = (uint8_t)(255 * lua_tonumber(&io_luaState, -1));
		lua_pop(&io_luaState, 1);
		LoadValueWithIndex(io_luaState, 2);
		vertex.g = (uint8_t)(255 * lua_tonumber(&io_luaState, -1));
		lua_pop(&io_luaState, 1);
		LoadValueWithIndex(io_luaState, 3);
		vertex.b = (uint8_t)(255 * lua_tonumber(&io_luaState, -1));
		lua_pop(&io_luaState, 1);
		LoadValueWithIndex(io_luaState, 4);
		vertex.a = (uint8_t)(255 * lua_tonumber(&io_luaState, -1));
		lua_pop(&io_luaState, 1);
		//Pop Color Table from stack
		lua_pop(&io_luaState, 1);
	}
	return vertex;
}

bool LoadTableWithKey(lua_State& io_luaState, const char * key)
{
	lua_pushstring(&io_luaState, key);
	lua_gettable(&io_luaState, -2);
	if (!lua_istable(&io_luaState, -1))
	{
		std::stringstream errorMessage;
		errorMessage << "The value at \"" << key << "\" must be a table "
			"(instead of a " << luaL_typename(&io_luaState, -1) << ")\n";
		eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
		//Pop the faulty entry
		lua_pop(&io_luaState, 1);
		return false;
	}
	return true;
}
bool LoadTableWithIndex(lua_State& io_luaState, const int index)
{
	lua_pushinteger(&io_luaState, index);
	lua_gettable(&io_luaState, -2);
	if (!lua_istable(&io_luaState, -1))
	{
		std::stringstream errorMessage;
		errorMessage << "The value at \"" << index << "\" must be a table "
			"(instead of a " << luaL_typename(&io_luaState, -1) << ")\n";
		eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
		//Pop the faulty entry
		lua_pop(&io_luaState, 1);
		return false;
	}
	return true;
}
bool LoadValueWithIndex(lua_State& io_luaState, const int index)
{
	lua_pushinteger(&io_luaState, index);
	lua_gettable(&io_luaState, -2);
	return true;
}
