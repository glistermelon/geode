#include <Geode/Geode.hpp>

#ifdef GEODE_IS_WINDOWS

int __stdcall AddFontResourceWHook(LPCWSTR p0);
int __stdcall RemoveFontResourceWHook(LPCWSTR p0);

static void patchCall(uintptr_t addr, uintptr_t newCall) {
    std::vector<uint8_t> patch = { 0xE8 };
    addr += reinterpret_cast<uintptr_t>(geode::base::getCocos());
    uintptr_t callAddr = newCall - (addr + 5);
    for (auto i = 0; i < sizeof(int); ++i)
        patch.push_back(callAddr >> (8 * i));
    patch.push_back(0x90);
    (void)Mod::get()->patch(reinterpret_cast<void*>(addr), patch);
}

$execute {
    patchCall(0xC9A56, reinterpret_cast<uintptr_t>(&RemoveFontResourceWHook));
    patchCall(0xCB5BC, reinterpret_cast<uintptr_t>(&RemoveFontResourceWHook));
    patchCall(0xCB642, reinterpret_cast<uintptr_t>(&AddFontResourceWHook));
};

#endif
