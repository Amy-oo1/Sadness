module;
#include "Runtime/Core/Utility/Include/UtilityMacros.h"

export module FileSystemLayer;

import Platform;
import Utility;
import Container;
import String;
import SyncIO;

export class FileSystem final : public Singleton {
private:
	FileSystem(void) = default;

public:
	~FileSystem(void) = default;

	static FileSystem& Get(void) {
		static FileSystem instance{};

		return instance;
	}

	Pair<Uint8*, Uint64> RenderFileSync(const Path& file) {
		if (auto It = this->m_FileCache.find(file); It != this->m_FileCache.end())
			return { It->second.first.get(),It->second.second };

		IFstream File { file, Ios::binary | Ios::ate };
		auto DataSize { File.tellg() };

		auto data { MakeShared<Uint8[]>(DataSize) };
		File.read(reinterpret_cast<char*>(data.get()), DataSize);
		File.clear();

		this->m_FileCache[file] = { data, DataSize };

		return { data.get(),DataSize };
	}


public:
	UnorderedMap<Path,Pair<SharedPtr<Uint8[]>,Uint64>> m_FileCache;

};