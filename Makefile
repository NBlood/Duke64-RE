OUTPUT := build

AS := mips-linux-gnu-as
ASFLAGS := -mips3 -march vr4300 -I include
OBJCOPY := mips-linux-gnu-objcopy
LD := mips-linux-gnu-ld
CC := ido-static-recomp-main/build/5.3/out/cc
OPT_FLAGS := -O2 -mips2
$(OUTPUT)/src/libultra/io/%.o: OPT_FLAGS := -O1 -mips2
$(OUTPUT)/src/libultra/os/%.o: OPT_FLAGS := -O1 -mips2
$(OUTPUT)/src/libultra/libc/%.o: OPT_FLAGS := -O3 -mips2
$(OUTPUT)/src/libultra/libc/ll.o: OPT_FLAGS := -O1 -mips3 -o32
$(OUTPUT)/src/libultra/libc/llcvt.o: OPT_FLAGS := -O1 -mips3 -o32
$(OUTPUT)/src/libultra/gu/%.o: OPT_FLAGS := -O3 -mips2
$(OUTPUT)/src/libultra/audio/%.o: OPT_FLAGS := -O3 -mips2
CCFLAGS = -Xcpluscomm $(OPT_FLAGS) -I include -I include/libultra -I include/libultra/PR -DTARGET_N64 -D_FINALROM -DF3DEX_GBI -DNDEBUG -non_shared -c -Wab,-r4300_mul -G 0
PYTHON := python3

OBJECTS := \
    $(OUTPUT)/header.o \
    $(OUTPUT)/assets/boot.o \
    $(OUTPUT)/src/entry.o \
    $(OUTPUT)/src/main.o \
    $(OUTPUT)/src/sched.o \
    $(OUTPUT)/asm/control.o \
    $(OUTPUT)/asm/game1.o \
    $(OUTPUT)/asm/control2.o \
    $(OUTPUT)/asm/game2.o \
    $(OUTPUT)/asm/logo.o \
    $(OUTPUT)/asm/boss2.o \
    $(OUTPUT)/asm/sound1.o \
    $(OUTPUT)/asm/sound2.o \
    $(OUTPUT)/asm/rend1.o \
    $(OUTPUT)/asm/rend2.o \
    $(OUTPUT)/asm/getangle.o \
    $(OUTPUT)/asm/draw.o \
    $(OUTPUT)/asm/rnc.o \
    $(OUTPUT)/asm/display1.o \
    $(OUTPUT)/src/cache1d.o \
    $(OUTPUT)/asm/display2.o \
    $(OUTPUT)/asm/actors1.o \
    $(OUTPUT)/asm/actors2.o \
    $(OUTPUT)/asm/actors3.o \
    $(OUTPUT)/asm/gamedef.o \
    $(OUTPUT)/src/engine.o \
    $(OUTPUT)/asm/game3.o \
    $(OUTPUT)/asm/game4.o \
    $(OUTPUT)/asm/player1.o \
    $(OUTPUT)/asm/player2.o \
    $(OUTPUT)/asm/player3.o \
    $(OUTPUT)/asm/premap.o \
    $(OUTPUT)/asm/sector.o \
    $(OUTPUT)/asm/board.o \
    $(OUTPUT)/asm/tile.o \
    $(OUTPUT)/asm/dukebot.o \
    $(OUTPUT)/asm/menues.o \
    $(OUTPUT)/src/libultra/io/viblack.o \
    $(OUTPUT)/src/libultra/os/initialize.o \
    $(OUTPUT)/src/libultra/os/createthread.o \
    $(OUTPUT)/src/libultra/os/startthread.o \
    $(OUTPUT)/src/libultra/io/pimgr.o \
    $(OUTPUT)/src/libultra/os/setthreadpri.o \
    $(OUTPUT)/asm/libultra/os/writebackdcacheall.o \
    $(OUTPUT)/src/libultra/io/pigetstat.o \
    $(OUTPUT)/src/libultra/io/pidma.o \
    $(OUTPUT)/src/libultra/os/recvmesg.o \
    $(OUTPUT)/src/libultra/os/gettime.o \
    $(OUTPUT)/src/libultra/os/sendmesg.o \
    $(OUTPUT)/src/libultra/os/createmesgqueue.o \
    $(OUTPUT)/src/libultra/libc/ll.o \
    $(OUTPUT)/src/libultra/io/visetyscale.o \
    $(OUTPUT)/src/libultra/io/vimgr.o \
    $(OUTPUT)/asm/libultra/unk1.o \
    $(OUTPUT)/src/libultra/io/vitbl.o \
    $(OUTPUT)/src/libultra/io/visetmode.o \
    $(OUTPUT)/src/libultra/io/visetspecial.o \
    $(OUTPUT)/src/libultra/os/seteventmesg.o \
    $(OUTPUT)/src/libultra/io/visetevent.o \
    $(OUTPUT)/asm/libultra/os/setintmask.o \
    $(OUTPUT)/src/libultra/io/sptaskyielded.o \
    $(OUTPUT)/src/libultra/io/vigetcurrframebuf.o \
    $(OUTPUT)/src/libultra/io/vigetnextframebuf.o \
    $(OUTPUT)/src/libultra/io/viswapbuf.o \
    $(OUTPUT)/src/libultra/io/sptask.o \
    $(OUTPUT)/src/libultra/io/dpsetnextbuf.o \
    $(OUTPUT)/src/libultra/io/sptaskyield.o \
    $(OUTPUT)/src/libultra/io/contreaddata.o \
    $(OUTPUT)/src/libultra/io/controller.o \
    $(OUTPUT)/src/libultra/io/pfsinitpak.o \
    $(OUTPUT)/src/libultra/io/pfsnumfiles.o \
    $(OUTPUT)/src/libultra/io/pfsfreeblocks.o \
    $(OUTPUT)/src/libultra/io/motor.o \
    $(OUTPUT)/src/libultra/gu/translate.o \
    $(OUTPUT)/src/libultra/gu/scale.o \
    $(OUTPUT)/src/libultra/libc/sprintf.o \
    $(OUTPUT)/src/libultra/libc/string.o \
    $(OUTPUT)/src/libultra/io/pfsfilestate.o \
    $(OUTPUT)/src/libultra/os/yieldthread.o \
    $(OUTPUT)/src/libultra/io/pfsdeletefile.o \
    $(OUTPUT)/src/libultra/io/pfsallocatefile.o \
    $(OUTPUT)/src/libultra/io/pfsreadwritefile.o \
    $(OUTPUT)/src/libultra/io/pfssearchfile.o \
    $(OUTPUT)/src/libultra/io/pfsrepairid.o \
    $(OUTPUT)/src/libultra/gu/rotate.o \
    $(OUTPUT)/src/libultra/os/virtualtophysical.o \
    $(OUTPUT)/src/libultra/audio/sl.o \
    $(OUTPUT)/src/libultra/audio/heapalloc.o \
    $(OUTPUT)/src/libultra/audio/heapinit.o \
    $(OUTPUT)/src/libultra/io/aisetfreq.o \
    $(OUTPUT)/src/libultra/audio/sndplayer.o \
    $(OUTPUT)/src/libultra/audio/bnkf.o \
    $(OUTPUT)/src/libultra/audio/sndpallocate.o \
    $(OUTPUT)/src/libultra/audio/csplayer.o \
    $(OUTPUT)/src/libultra/audio/seqpsetbank.o \
    $(OUTPUT)/src/libultra/audio/cspsetvol.o \
    $(OUTPUT)/src/libultra/audio/sndpsetsound.o \
    $(OUTPUT)/src/libultra/audio/sndpgetstate.o \
    $(OUTPUT)/src/libultra/audio/sndpsetpitch.o \
    $(OUTPUT)/src/libultra/audio/sndpsetpan.o \
    $(OUTPUT)/src/libultra/audio/sndpsetvol.o \
    $(OUTPUT)/src/libultra/audio/sndpplay.o \
    $(OUTPUT)/src/libultra/audio/cspstop.o \
    $(OUTPUT)/src/libultra/audio/cseq.o \
    $(OUTPUT)/src/libultra/audio/cspsetseq.o \
    $(OUTPUT)/src/libultra/audio/cspplay.o \
    $(OUTPUT)/src/libultra/audio/sndpstop.o \
    $(OUTPUT)/src/libultra/audio/synthesizer.o \
    $(OUTPUT)/src/libultra/io/aisetnextbuf.o \
    $(OUTPUT)/src/libultra/gu/sinf.o \
    $(OUTPUT)/src/libultra/gu/cosf.o \
    $(OUTPUT)/src/libultra/libc/llcvt.o \
    $(OUTPUT)/src/libultra/gu/perspective.o \
    $(OUTPUT)/src/libultra/gu/lookat.o \
    $(OUTPUT)/src/libultra/gu/lookatref.o \
    $(OUTPUT)/src/libultra/gu/mtxcatf.o \
    $(OUTPUT)/asm/libultra/gu/sqrtf.o \
    $(OUTPUT)/asm/libultra/os/interrupt.o \
    $(OUTPUT)/src/libultra/io/vi.o \
    $(OUTPUT)/asm/libultra/os/setsr.o \
    $(OUTPUT)/asm/libultra/os/getsr.o \
    $(OUTPUT)/asm/libultra/os/setfpccsr.o \
    $(OUTPUT)/src/libultra/io/sirawread.o \
    $(OUTPUT)/src/libultra/io/sirawwrite.o \
    $(OUTPUT)/asm/libultra/os/exceptasm.o \
    $(OUTPUT)/asm/libultra/os/writebackdcache.o \
    $(OUTPUT)/asm/libultra/os/invalicache.o \
    $(OUTPUT)/asm/libultra/os/maptlbrdb.o \
    $(OUTPUT)/src/libultra/io/pirawread.o \
    $(OUTPUT)/asm/libultra/libc/bzero.o \
    $(OUTPUT)/src/libultra/os/thread.o \
    $(OUTPUT)/src/libultra/io/cartrominit.o \
    $(OUTPUT)/src/libultra/io/leodiskinit.o \
    $(OUTPUT)/src/libultra/io/piacs.o \
    $(OUTPUT)/src/libultra/os/getthreadpri.o \
    $(OUTPUT)/src/libultra/io/pirawdma.o \
    $(OUTPUT)/src/libultra/io/epirawdma.o \
    $(OUTPUT)/src/libultra/io/devmgr.o \
    $(OUTPUT)/src/libultra/os/jammesg.o \
    $(OUTPUT)/src/libultra/io/pigetcmdq.o \
    $(OUTPUT)/asm/libultra/os/getcount.o \
    $(OUTPUT)/src/libultra/os/timerintr.o \
    $(OUTPUT)/src/libultra/io/vigetcurrcontext.o \
    $(OUTPUT)/src/libultra/io/viswapcontext.o \
    $(OUTPUT)/src/libultra/io/spgetstat.o \
    $(OUTPUT)/asm/libultra/libc/bcopy.o \
    $(OUTPUT)/src/libultra/io/spsetstat.o \
    $(OUTPUT)/src/libultra/io/spsetpc.o \
    $(OUTPUT)/src/libultra/io/sprawdma.o \
    $(OUTPUT)/src/libultra/io/sp.o \
    $(OUTPUT)/src/libultra/io/dp.o \
    $(OUTPUT)/src/libultra/io/siacs.o \
    $(OUTPUT)/src/libultra/io/sirawdma.o \
    $(OUTPUT)/src/libultra/os/settimer.o \
    $(OUTPUT)/src/libultra/io/pfsgetstatus.o \
    $(OUTPUT)/src/libultra/io/contpfs.o \
    $(OUTPUT)/src/libultra/io/contramread.o \
    $(OUTPUT)/src/libultra/io/pfschecker.o \
    $(OUTPUT)/src/libultra/io/pfsisplug.o \
    $(OUTPUT)/src/libultra/io/crc.o \
    $(OUTPUT)/src/libultra/io/contramwrite.o \
    $(OUTPUT)/src/libultra/gu/mtxutil.o \
    $(OUTPUT)/src/libultra/libc/xprintf.o \
    $(OUTPUT)/src/libultra/gu/normalize.o \
    $(OUTPUT)/asm/libultra/os/probetlb.o \
    $(OUTPUT)/src/libultra/audio/syndelete.o \
    $(OUTPUT)/src/libultra/audio/event.o \
    $(OUTPUT)/src/libultra/audio/synaddplayer.o \
    $(OUTPUT)/src/libultra/audio/synallocvoice.o \
    $(OUTPUT)/src/libultra/audio/synstartvoice.o \
    $(OUTPUT)/src/libultra/audio/synsetpan.o \
    $(OUTPUT)/src/libultra/audio/synsetvol.o \
    $(OUTPUT)/src/libultra/audio/synsetpitch.o \
    $(OUTPUT)/src/libultra/audio/synsetfxmix.o \
    $(OUTPUT)/src/libultra/audio/synstopvoice.o \
    $(OUTPUT)/src/libultra/audio/synfreevoice.o \
    $(OUTPUT)/src/libultra/audio/seqplayer.o \
    $(OUTPUT)/src/libultra/audio/cents2ratio.o \
    $(OUTPUT)/src/libultra/audio/synstartvoiceparam.o \
    $(OUTPUT)/src/libultra/audio/drvrNew.o \
    $(OUTPUT)/src/libultra/audio/synallocfx.o \
    $(OUTPUT)/src/libultra/audio/mainbus.o \
    $(OUTPUT)/src/libultra/audio/load.o \
    $(OUTPUT)/src/libultra/audio/resample.o \
    $(OUTPUT)/src/libultra/audio/env.o \
    $(OUTPUT)/src/libultra/audio/auxbus.o \
    $(OUTPUT)/src/libultra/audio/save.o \
    $(OUTPUT)/src/libultra/io/ai.o \
    $(OUTPUT)/src/libultra/io/si.o \
    $(OUTPUT)/src/libultra/os/kdebugserver.o \
    $(OUTPUT)/src/libultra/io/leointerrupt.o \
    $(OUTPUT)/src/libultra/os/destroythread.o \
    $(OUTPUT)/src/libultra/os/resetglobalintmask.o \
    $(OUTPUT)/src/libultra/io/epirawwrite.o \
    $(OUTPUT)/src/libultra/io/epirawread.o \
    $(OUTPUT)/src/libultra/os/setglobalintmask.o \
    $(OUTPUT)/asm/libultra/os/setcompare.o \
    $(OUTPUT)/asm/libultra/os/invaldcache.o \
    $(OUTPUT)/src/libultra/io/vimodepallan1.o \
    $(OUTPUT)/src/libultra/io/vimodempallan1.o \
    $(OUTPUT)/src/libultra/io/vimodentsclan1.o \
    $(OUTPUT)/src/libultra/libc/xlitob.o \
    $(OUTPUT)/src/libultra/libc/xldtob.o \
    $(OUTPUT)/src/libultra/audio/copy.o \
    $(OUTPUT)/src/libultra/audio/seq.o \
    $(OUTPUT)/src/libultra/audio/synsetpriority.o \
    $(OUTPUT)/src/libultra/audio/filter.o \
    $(OUTPUT)/src/libultra/audio/reverb.o \
    $(OUTPUT)/src/libultra/libc/ldiv.o \
    $(OUTPUT)/data/src/main.data.o \
    $(OUTPUT)/data/src/sched.data.o \
    $(OUTPUT)/data/asm/control.data.o \
    $(OUTPUT)/data/asm/game1.data.o \
    $(OUTPUT)/data/asm/control2.data.o \
    $(OUTPUT)/data/asm/game2.data.o \
    $(OUTPUT)/data/asm/logo.data.o \
    $(OUTPUT)/data/asm/boss2.data.o \
    $(OUTPUT)/data/asm/sound1.data.o \
    $(OUTPUT)/data/asm/sound2.data.o \
    $(OUTPUT)/data/asm/rend1.data.o \
    $(OUTPUT)/data/asm/rend2.data.o \
    $(OUTPUT)/data/asm/getangle.data.o \
    $(OUTPUT)/data/asm/draw.data.o \
    $(OUTPUT)/data/asm/tileinfo.data.o \
    $(OUTPUT)/data/asm/display1.data.o \
    $(OUTPUT)/data/src/cache1d.data.o \
    $(OUTPUT)/data/asm/display2.data.o \
    $(OUTPUT)/data/asm/actors1.data.o \
    $(OUTPUT)/data/asm/actors2.data.o \
    $(OUTPUT)/data/asm/actors3.data.o \
    $(OUTPUT)/data/asm/script.data.o \
    $(OUTPUT)/data/src/engine.data.o \
    $(OUTPUT)/data/asm/game3.data.o \
    $(OUTPUT)/data/asm/player1.data.o \
    $(OUTPUT)/data/asm/player3.data.o \
    $(OUTPUT)/data/asm/sector.data.o \
    $(OUTPUT)/data/asm/tables.data.o \
    $(OUTPUT)/data/asm/dukebot.data.o \
    $(OUTPUT)/data/asm/menues.data.o \
    $(OUTPUT)/data/asm/global.data.o \
    $(OUTPUT)/data/asm/libultra/os/exceptasm.data.o \
    $(OUTPUT)/data/asm/control.rodata.o \
    $(OUTPUT)/data/asm/game1.rodata.o \
    $(OUTPUT)/data/asm/control2.rodata.o \
    $(OUTPUT)/data/asm/game2.rodata.o \
    $(OUTPUT)/data/asm/logo.rodata.o \
    $(OUTPUT)/data/asm/boss2.rodata.o \
    $(OUTPUT)/data/asm/sound1.rodata.o \
    $(OUTPUT)/data/asm/rend1.rodata.o \
    $(OUTPUT)/data/asm/rend2.rodata.o \
    $(OUTPUT)/data/asm/getangle.rodata.o \
    $(OUTPUT)/data/asm/draw.rodata.o \
    $(OUTPUT)/data/asm/display2.rodata.o \
    $(OUTPUT)/data/asm/actors1.rodata.o \
    $(OUTPUT)/data/asm/actors2.rodata.o \
    $(OUTPUT)/data/asm/actors3.rodata.o \
    $(OUTPUT)/data/asm/gamedef.rodata.o \
    $(OUTPUT)/data/asm/script.rodata.o \
    $(OUTPUT)/data/asm/game3.rodata.o \
    $(OUTPUT)/data/asm/game4.rodata.o \
    $(OUTPUT)/data/asm/player1.rodata.o \
    $(OUTPUT)/data/asm/player2.rodata.o \
    $(OUTPUT)/data/asm/player3.rodata.o \
    $(OUTPUT)/data/asm/premap.rodata.o \
    $(OUTPUT)/data/asm/sector.rodata.o \
    $(OUTPUT)/data/asm/board.rodata.o \
    $(OUTPUT)/data/asm/menues.rodata.o \
    $(OUTPUT)/data/asm/libultra/os/setintmask.rodata.o \
    $(OUTPUT)/data/asm/libultra/os/exceptasm.rodata.o \
    $(OUTPUT)/data/asm/libultra/gu/libm_vals.rodata.o \
    $(OUTPUT)/data/asm/libultra/libc/xldtob.rodata.o \
    $(OUTPUT)/data/asm/libultra/audio/seq.rodata.o \
    $(OUTPUT)/data/asm/libultra/audio/reverb.rodata.o \
    $(OUTPUT)/data/src/main.bss.o \
    $(OUTPUT)/data/asm/control.bss.o \
    $(OUTPUT)/data/asm/game1.bss.o \
    $(OUTPUT)/data/asm/control2.bss.o \
    $(OUTPUT)/data/asm/game2.bss.o \
    $(OUTPUT)/data/asm/boss2.bss.o \
    $(OUTPUT)/data/asm/sound1.bss.o \
    $(OUTPUT)/data/asm/rend1.bss.o \
    $(OUTPUT)/data/asm/rend2.bss.o \
    $(OUTPUT)/data/asm/draw.bss.o \
    $(OUTPUT)/data/asm/rnc.bss.o \
    $(OUTPUT)/data/src/cache1d.bss.o \
    $(OUTPUT)/data/asm/actors1.bss.o \
    $(OUTPUT)/data/asm/gamedef.bss.o \
    $(OUTPUT)/data/src/engine.bss.o \
    $(OUTPUT)/data/asm/game4.bss.o \
    $(OUTPUT)/data/asm/player1.bss.o \
    $(OUTPUT)/data/asm/dukebot.bss.o \
    $(OUTPUT)/data/asm/menues.bss.o \
    $(OUTPUT)/data/asm/global.bss.o \
    $(OUTPUT)/data/asm/global2.bss.o \
    $(OUTPUT)/data/asm/libultra/io/leointerrupt.bss.o \
    $(OUTPUT)/assets/palette.o \
    $(OUTPUT)/src/dlstatic.o \
    $(OUTPUT)/assets/tileblob.o \
    $(OUTPUT)/assets/board01.o \
    $(OUTPUT)/assets/board02.o \
    $(OUTPUT)/assets/board03.o \
    $(OUTPUT)/assets/board04.o \
    $(OUTPUT)/assets/board06.o \
    $(OUTPUT)/assets/board05.o \
    $(OUTPUT)/assets/board07.o \
    $(OUTPUT)/assets/board09.o \
    $(OUTPUT)/assets/board10.o \
    $(OUTPUT)/assets/board11.o \
    $(OUTPUT)/assets/board12.o \
    $(OUTPUT)/assets/board13.o \
    $(OUTPUT)/assets/board14.o \
    $(OUTPUT)/assets/board15.o \
    $(OUTPUT)/assets/board16.o \
    $(OUTPUT)/assets/board17.o \
    $(OUTPUT)/assets/board18.o \
    $(OUTPUT)/assets/board19.o \
    $(OUTPUT)/assets/board20.o \
    $(OUTPUT)/assets/board21.o \
    $(OUTPUT)/assets/board22.o \
    $(OUTPUT)/assets/board23.o \
    $(OUTPUT)/assets/board24.o \
    $(OUTPUT)/assets/board25.o \
    $(OUTPUT)/assets/board26.o \
    $(OUTPUT)/assets/board27.o \
    $(OUTPUT)/assets/board28.o \
    $(OUTPUT)/assets/board30.o \
    $(OUTPUT)/assets/board08.o \
    $(OUTPUT)/assets/board29.o \
    $(OUTPUT)/assets/board31.o \
    $(OUTPUT)/assets/board32.o \
    $(OUTPUT)/assets/board33.o \
    $(OUTPUT)/assets/board34.o \
    $(OUTPUT)/assets/soundctl.o \
    $(OUTPUT)/assets/soundtbl.o \
    $(OUTPUT)/assets/musicseq.o \
    $(OUTPUT)/assets/musicctl.o \
    $(OUTPUT)/assets/musictbl.o \
    $(OUTPUT)/data/ucode/rdp.o \
    $(OUTPUT)/data/ucode/asp.o \
    $(OUTPUT)/data/ucode/rdp_data.o \
    $(OUTPUT)/data/ucode/asp_data.o \
    $(OUTPUT)/data/ucode/boot.o \


dukenukem.bin : $(OUTPUT)/dukenukem.elf
	$(OBJCOPY) -O binary $< $@
	
$(OUTPUT)/%.o : %.c
	@mkdir -p $(@D)
	$(PYTHON) ./asm-processor/build.py $(CC) -- $(AS) $(ASFLAGS) -- $(CCFLAGS) -o $@ $<
	
$(OUTPUT)/src/libultra/gu/%.o : src/libultra/gu/%.c
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) -o $@ $<
	
$(OUTPUT)/src/libultra/libc/%.o : src/libultra/libc/%.c
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) -o $@ $<
	
$(OUTPUT)/src/libultra/libc/ll.o : src/libultra/libc/ll.c
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) -o $@ $<
	$(PYTHON) tools/set_o32abi_bit.py $@
	
$(OUTPUT)/src/libultra/libc/llcvt.o : src/libultra/libc/llcvt.c
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) -o $@ $<
	$(PYTHON) tools/set_o32abi_bit.py $@
	
$(OUTPUT)/src/libultra/audio/%.o : src/libultra/audio/%.c
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) -o $@ $<

$(OUTPUT)/%.o : %.s
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS) $< -o $@

$(OUTPUT)/%.o : %.bin
	@mkdir -p $(@D)
	$(LD) -r -b binary -o $@ $<

$(OUTPUT)/dukenukem.elf : $(OBJECTS)
	$(LD) -T undefined_syms.txt -T dukenukem.ld -Map dukenukem.map -o $(OUTPUT)/dukenukem.elf

all: dukenukem.bin

objects: $(OBJECTS)
