#pragma once
#pragma comment(lib, "detours.lib")

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <type_traits>
#include <vector>
#include "detours/detours.h"

using namespace std;

enum class convention_type
{
	stdcall_t,
	cdecl_t,
	thiscall_t,
	fastcall_t,
	winapi_t,
	thiscall_r
};

enum class convention_type_n
{
	winapi_t
};

template <convention_type tp, typename retn, typename convention, typename ...args>
struct convention;

template <typename retn, typename ...args>
struct convention<convention_type::stdcall_t, retn, args...>
{
	typedef retn(__stdcall* type)(args ...);
};

template <typename retn, typename ...args>
struct convention<convention_type::cdecl_t, retn, args...>
{
	typedef retn(__cdecl* type)(args ...);
};

template <typename retn, typename ...args>
struct convention<convention_type::thiscall_t, retn, args...>
{
	typedef retn(__stdcall* type)(args ...);
};

template <typename retn, typename ...args>
struct convention<convention_type::fastcall_t, retn, args...>
{
	typedef retn(__fastcall* type)(args ...);
};

template <typename retn, typename ...args>
struct convention<convention_type::winapi_t, retn, args...>
{
	typedef retn(WINAPI* type)(args ...);
};

template <typename retn, typename ...args>
struct convention<convention_type::thiscall_r, retn, args...>
{
	typedef retn(__thiscall* type)(args ...);
};

typedef struct DetourEntry
{
	void* orig;
	void* target;
	void* detour;
	char* fncName;

	DetourEntry(void* o, void* t, void* d, char* f) : orig(o), target(t), detour(d), fncName(f)
	{
	}
} DetourEntry;

class Detour
{
private:
	std::vector<DetourEntry> m_detourList;
public:
	void* Install(void* target, void* detour, char* fncName)
	{
		auto origFunc = DetourFunction((PBYTE)target, (PBYTE)detour);
		m_detourList.push_back(DetourEntry(origFunc, target, detour, fncName));
		return origFunc;
	}
	bool RemoveHooks() const
	{
		for (auto hookEntry : m_detourList)
		{
			if (!DetourRemove((PBYTE)hookEntry.orig, (PBYTE)hookEntry.detour))
				//if (!DetourRemove((PBYTE)hookEntry.detour))
			{
				//Console::PrintLn("Failed to remove Hook: %p -> %p (%s) - %08x", hookEntry.detour, hookEntry.target, hookEntry.fncName, GetLastError());
			}
		}


		return true;
	}
};

template <DWORD ptr, convention_type tp, typename retn, typename ...args>
class MAKE_CALL
{
	typedef typename convention<tp, retn, args...>::type type;
public:
	type _orig;

	MAKE_CALL() : _orig(0)
	{
		DWORD mdd = (DWORD)GetModuleHandleA(0) + ptr;
		_orig = (type)mdd;
	}

	retn Call(args ... p)
	{
		return _orig(p...);
	}
};

template <convention_type tp, typename retn, typename ...args>
class MAKE_HOOK
{
	typedef typename convention<tp, retn, args...>::type type;


	bool _isApplied;
public:
	type _orig;
	type _detour;

	MAKE_HOOK() : _isApplied(false), _orig(0), _detour(0)
	{
	}

	template <typename T>
	MAKE_HOOK(T pFunc, type detour)
	{
		Apply<T>(pFunc, detour);
	}

	~MAKE_HOOK()
	{
		Remove();
	}

	template <typename T>
	void Apply(T pFunc, type detour)
	{
		static auto Instance = new Detour();
		_detour = detour;
		_orig = (type)Instance->Install((void*)pFunc, (void*)_detour, (char*)"null_xdkappa");
		_isApplied = _orig != nullptr;

		if (_orig == nullptr)
		{
			//Console::PrintLn("Faulty Hook, aborting: %s @%08x-%08x", __FUNCTION__, pFunc, _detour);
		}
	}


	bool Remove()
	{
		if (!_isApplied)
			return false;

		_isApplied = false;
		//return DetourRemove((PBYTE)_orig, (PBYTE)_detour) > 0;
		return true;
	}

	retn CallOriginal(args ... p)
	{
		return _orig(p...);
	}

	retn CallDetour(args ... p)
	{
		return _detour(p...);
	}

	bool IsApplied() const
	{
		return _isApplied;
	}
};
