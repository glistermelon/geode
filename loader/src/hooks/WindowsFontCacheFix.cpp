#include <Geode/Geode.hpp>

#ifdef GEODE_IS_WINDOWS

using namespace geode::prelude;

const int PATCH_OFFSET_1 = 0xCB5BC;
const int PATCH_OFFSET_2 = 0xCB642;

int AddFontResourceWHook(LPCWSTR p0) {
    return AddFontResourceExW(p0, FR_PRIVATE, 0);
}

int RemoveFontResourceWHook(LPCWSTR p0) {
    return RemoveFontResourceExW(p0, FR_PRIVATE, 0);
}

void applyPatch(void* dst, unsigned int hookAddr) {
    ByteVector patch = { 0xE8 };
    unsigned int callAddr = hookAddr - ((unsigned int)dst + 5);
    for (unsigned char i = 0; i < sizeof(int); ++i)
        patch.push_back(callAddr >> (8 * i));
    patch.push_back(0x90);
    Mod::get()->patch(dst, patch);
}

$execute {
    void* dst1 = reinterpret_cast<void*>(geode::base::getCocos() + PATCH_OFFSET_1);
    applyPatch(dst1, (unsigned int)&RemoveFontResourceWHook);

    void* dst2 = reinterpret_cast<void*>(geode::base::getCocos() + PATCH_OFFSET_2);
    applyPatch(dst2, (unsigned int)&AddFontResourceWHook);
};

#endif
