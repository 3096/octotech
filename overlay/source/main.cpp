#include "lx/debug.hpp"
#include "lx/overlay.hpp"
#include "lx/ui/controller.hpp"
#include "lx/util.hpp"
// #include "screens/main_screen.hpp"
#include "screens/gear_screen.hpp"

extern "C" {
u32 __nx_applet_type = AppletType_None;
char nx_inner_heap[INNER_HEAP_SIZE];

u32 __nx_nv_transfermem_size = 0x15000;

void __libnx_initheap(void);
void __libnx_init_time(void);
void __appInit(void);
void __appExit(void);
}

void __libnx_initheap(void) {
    extern char* fake_heap_start;
    extern char* fake_heap_end;
    fake_heap_start = nx_inner_heap;
    fake_heap_end = nx_inner_heap + sizeof(nx_inner_heap);
}

void __appInit(void) {
    // Init services
    TRY_FATAL(smInitialize());
    TRY_FATAL(hidInitialize());
    TRY_FATAL(fsInitialize());
    TRY_FATAL(fsdevMountSdmc());
    TRY_FATAL(romfsMountFromFsdev("sdmc:/atmosphere/contents/020000100D0A0906/romfs.bin", 0, "romfs"));

    auto socketConfig = SocketInitConfig{.bsdsockets_version = 1,
                                         .tcp_tx_buf_size = 0x800,
                                         .tcp_rx_buf_size = 0x1000,
                                         .tcp_tx_buf_max_size = 0,
                                         .tcp_rx_buf_max_size = 0,
                                         .udp_tx_buf_size = 0x2400,
                                         .udp_rx_buf_size = 0xA500,
                                         .sb_efficiency = 1};
    TRY_FATAL(socketInitialize(&socketConfig));

    lx::debugInit();
    lx::Overlay::instantiate();
}

void __appExit(void) {
    // Cleanup services.
    lx::debugExit();
    socketExit();
    romfsExit();
    fsdevUnmountAll();
    fsExit();
    hidExit();
    smExit();
}

int main(int argc, char* argv[]) {
    LOG("Main start");

    try {
        lx::ui::Controller::show(GearScreen::getInstance());
    } catch (std::runtime_error& e) {
        LOG("runtime_error: %s", e.what());
    }

    LOG("Main exit");
    return 0;
}
