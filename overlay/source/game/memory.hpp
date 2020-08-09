#pragma once

#include <switch.h>

#include <array>
#include <memory>

#include "lx/util.hpp"

using BuildId = std::array<uint8_t, 0x20>;

extern "C" {
typedef struct {
    uint64_t base;
    uint64_t size;
} DmntMemoryRegionExtents;

typedef struct {
    uint64_t process_id;
    uint64_t title_id;
    DmntMemoryRegionExtents main_nso_extents;
    DmntMemoryRegionExtents heap_extents;
    DmntMemoryRegionExtents alias_extents;
    DmntMemoryRegionExtents address_space_extents;
    BuildId main_nso_build_id;
} DmntCheatProcessMetadata;

Result dmntchtInitialize();
void dmntchtExit();
Result dmntchtHasCheatProcess(bool* out);
Result dmntchtForceOpenCheatProcess();
Result dmntchtGetCheatProcessMetadata(DmntCheatProcessMetadata* out_metadata);
Result dmntchtReadCheatProcessMemory(uint64_t address, void* buffer, size_t size);
Result dmntchtWriteCheatProcessMemory(uint64_t address, void* buffer, size_t size);
}

class MemoryReader {
   private:
    MemoryReader();
    MemoryReader(const MemoryReader&) = delete;
    ~MemoryReader();
    static inline auto& getInstance() {
        static MemoryReader s_instance;
        return s_instance;
    }

    static constexpr auto JP_TID = 0x01003C700009C000;
    static constexpr auto US_TID = 0x01003BC0000A0000;
    static constexpr auto EU_TID = 0x0100F8F0000A2000;

    DmntCheatProcessMetadata m_dmntCheatProcessMetadata;

    auto ensureHasCheatProcess_() -> bool;

    template <typename T>
    inline auto readData_(size_t offset) {
        auto result = std::array<uint8_t, sizeof(T)>{};
        TRY_THROW(dmntchtReadCheatProcessMemory(offset, result.data(), sizeof(T)));
        return result;
    }

    template <typename T>
    inline auto readBuffer_(size_t offset) {
        auto result = std::make_shared<uint8_t>(sizeof(T));
        TRY_THROW(dmntchtReadCheatProcessMemory(offset, result.get(), sizeof(T)));
        return result;
    }

   public:
    static inline auto gameIsRunning() { return getInstance().ensureHasCheatProcess_(); }

    template <typename T>
    static inline auto readDataFromAddr(void* address) {
        return getInstance().readData_<T>(reinterpret_cast<size_t>(address));
    }

    template <typename T>
    static inline auto readDataFromMain(size_t offset) {
        auto& s_instance = getInstance();
        return s_instance.readData_<T>(s_instance.m_dmntCheatProcessMetadata.main_nso_extents.base + offset);
    }

    template <typename T>
    static inline auto readBufferFromAddr(void* address) {
        return getInstance().readBuffer_<T>(reinterpret_cast<size_t>(address));
    }
};
