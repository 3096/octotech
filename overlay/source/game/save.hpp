#pragma once

#include "Cmn/SaveData.h"
#include "memory.hpp"

namespace save {

static constexpr auto SP_SAVEDATA_OFFSET_310 = 0x41C4828;
static constexpr auto SP_SAVEDATA_OFFSET_522 = 0x2D669F8;
static constexpr auto SP_SAVEDATA_OFFSET = SP_SAVEDATA_OFFSET_522;  // TODO: make versions

static inline auto getSaveData() {
    return MemoryReader::readDataFromAddr<Cmn::SaveData>(
        *reinterpret_cast<Cmn::SaveData**>(MemoryReader::readDataFromMain<Cmn::SaveData*>(SP_SAVEDATA_OFFSET).data()));
}

static inline auto getSaveDataCmnDataAddress() {
    return reinterpret_cast<Cmn::SaveDataCmn*>(
               MemoryReader::readDataFromAddr<Cmn::SaveDataCmn>(
                   reinterpret_cast<Cmn::SaveData*>(getSaveData().data())->pSaveDataCmn)
                   .data())
        ->pData;
}

static inline auto getSaveDataCmnData() {
    return MemoryReader::readBufferFromAddr<Cmn::SaveDataCmn::SaveDataCmnData>(getSaveDataCmnDataAddress());
}

}  // namespace save
