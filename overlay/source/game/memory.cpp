#include "memory.hpp"

MemoryReader::MemoryReader() {
    TRY_THROW(pmdmntInitialize());
    TRY_THROW(pminfoInitialize());
    TRY_THROW(dmntchtInitialize());
    if (R_SUCCEEDED(dmntchtForceOpenCheatProcess())) {
        TRY_THROW(dmntchtGetCheatProcessMetadata(&m_dmntCheatProcessMetadata));
    }
}

MemoryReader::~MemoryReader() {
    dmntchtExit();
    pminfoExit();
    pmdmntExit();
}

auto MemoryReader::ensureHasCheatProcess_() -> bool {
    bool hasCheatProcess;
    TRY_THROW(dmntchtHasCheatProcess(&hasCheatProcess));
    if (not hasCheatProcess) {
        if (R_FAILED(dmntchtForceOpenCheatProcess())) {
            return false;
        }
        TRY_THROW(dmntchtGetCheatProcessMetadata(&m_dmntCheatProcessMetadata));
    }
    return m_dmntCheatProcessMetadata.title_id == US_TID or m_dmntCheatProcessMetadata.title_id == JP_TID or
           m_dmntCheatProcessMetadata.title_id == EU_TID;
}
