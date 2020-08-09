#pragma once

#include "Cmn/SaveData.h"
#include "memory.hpp"

namespace save {

static constexpr auto SP_SAVEDATA_OFFSET = 0x41C4828;  // TODO: make versions

static inline auto getSaveData() {
    return MemoryReader::readDataFromAddr<Cmn::SaveData>(
        *reinterpret_cast<Cmn::SaveData**>(MemoryReader::readDataFromMain<Cmn::SaveData*>(SP_SAVEDATA_OFFSET).data()));
}

static inline auto getSaveDataCmnData() {
    return MemoryReader::readBufferFromAddr<Cmn::SaveDataCmn::SaveDataCmnData>(
        reinterpret_cast<Cmn::SaveDataCmn*>(MemoryReader::readDataFromAddr<Cmn::SaveDataCmn>(
                                                reinterpret_cast<Cmn::SaveData*>(getSaveData().data())->pSaveDataCmn)
                                                .data())
            ->pData);
}

}  // namespace save
