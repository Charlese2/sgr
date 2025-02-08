#!/usr/bin/env python3

###
# Generates build files for the project.
# This file also includes the project configuration,
# such as compiler flags and the object matching status.
#
# Usage:
#   python3 configure.py
#   ninja
#
# Append --help to see available options.
###

import argparse
import sys
from pathlib import Path
from typing import Any, Dict, List

from tools.project import (
    Object,
    ProgressCategory,
    ProjectConfig,
    calculate_progress,
    generate_build,
    is_windows,
)

# Game versions
DEFAULT_VERSION = 0
VERSIONS = [
    "GS2E78",  # 0
]

parser = argparse.ArgumentParser()
parser.add_argument(
    "mode",
    choices=["configure", "progress"],
    default="configure",
    help="script mode (default: configure)",
    nargs="?",
)
parser.add_argument(
    "-v",
    "--version",
    choices=VERSIONS,
    type=str.upper,
    default=VERSIONS[DEFAULT_VERSION],
    help="version to build",
)
parser.add_argument(
    "--build-dir",
    metavar="DIR",
    type=Path,
    default=Path("build"),
    help="base build directory (default: build)",
)
parser.add_argument(
    "--binutils",
    metavar="BINARY",
    type=Path,
    help="path to binutils (optional)",
)
parser.add_argument(
    "--compilers",
    metavar="DIR",
    type=Path,
    help="path to compilers (optional)",
)
parser.add_argument(
    "--map",
    action="store_true",
    help="generate map file(s)",
)
parser.add_argument(
    "--debug",
    action="store_true",
    help="build with debug info (non-matching)",
)
if not is_windows():
    parser.add_argument(
        "--wrapper",
        metavar="BINARY",
        type=Path,
        help="path to wibo or wine (optional)",
    )
parser.add_argument(
    "--dtk",
    metavar="BINARY | DIR",
    type=Path,
    help="path to decomp-toolkit binary or source (optional)",
)
parser.add_argument(
    "--objdiff",
    metavar="BINARY | DIR",
    type=Path,
    help="path to objdiff-cli binary or source (optional)",
)
parser.add_argument(
    "--sjiswrap",
    metavar="EXE",
    type=Path,
    help="path to sjiswrap.exe (optional)",
)
parser.add_argument(
    "--verbose",
    action="store_true",
    help="print verbose output",
)
parser.add_argument(
    "--non-matching",
    dest="non_matching",
    action="store_true",
    help="builds equivalent (but non-matching) or modded objects",
)
parser.add_argument(
    "--no-progress",
    dest="progress",
    action="store_false",
    help="disable progress calculation",
)
args = parser.parse_args()

config = ProjectConfig()
config.version = str(args.version)
version_num = VERSIONS.index(config.version)

# Apply arguments
config.build_dir = args.build_dir
config.dtk_path = args.dtk
config.objdiff_path = args.objdiff
config.binutils_path = args.binutils
config.compilers_path = args.compilers
config.generate_map = args.map
config.non_matching = args.non_matching
config.sjiswrap_path = args.sjiswrap
config.progress = args.progress
if not is_windows():
    config.wrapper = args.wrapper
# Don't build asm unless we're --non-matching
if not config.non_matching:
    config.asm_dir = None

# Tool versions
config.binutils_tag = "2.42-1"
config.compilers_tag = "20240706"
config.dtk_tag = "v1.4.1"
config.objdiff_tag = "v2.4.0"
config.sjiswrap_tag = "v1.2.0"
config.wibo_tag = "0.6.11"

# Project
config.config_path = Path("config") / config.version / "config.yml"
config.check_sha_path = Path("config") / config.version / "build.sha1"
config.asflags = [
    "-mgekko",
    "--strip-local-absolute",
    "-I include",
    f"-I build/{config.version}/include",
    f"--defsym BUILD_VERSION={version_num}",
    f"--defsym VERSION_{config.version}",
]
config.ldflags = [
    "-fp hardware",
    "-nodefaults",
]
if args.debug:
    config.ldflags.append("-g")  # Or -gdwarf-2 for Wii linkers
if args.map:
    config.ldflags.append("-mapunused")
    # config.ldflags.append("-listclosure") # For Wii linkers

# Use for any additional files that should cause a re-configure when modified
config.reconfig_deps = []

# Optional numeric ID for decomp.me preset
# Can be overridden in libraries or objects
config.scratch_preset_id = None

# Base flags, common to most GC/Wii games.
# Generally leave untouched, with overrides added below.
cflags_base = [
    "-nodefaults",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    "-fp hardware",
    "-Cpp_exceptions off",
    # "-W all",
    "-O4,p",
    "-inline auto",
    '-pragma "cats off"',
    '-pragma "warn_notinlined off"',
    "-maxerrors 1",
    "-nosyspath",
    "-RTTI off",
    "-str reuse",
    "-multibyte",  # For Wii compilers, replace with `-enc SJIS`
    "-i include",
    f"-i build/{config.version}/include",
    f"-DVERSION={version_num}",
]

# Debug flags
if args.debug:
    # Or -sym dwarf-2 for Wii compilers
    cflags_base.extend(["-sym on", "-DDEBUG=1"])
else:
    cflags_base.append("-DNDEBUG=1")

# Metrowerks library flags
cflags_runtime = [
    *cflags_base,
    "-use_lmw_stmw on",
    "-str reuse,pool,readonly",
    "-gccinc",
    "-common off",
    "-inline auto,deferred",
    "-fp_contract on",
    "-i include/PowerPC_EABI_Support/Runtime",
    "-i include/PowerPC_EABI_Support/MSL_C/MSL_Common/Include",
    "-i include/PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Include",
    "-i include/stl",
    "-i include",
]

cflags_sdk = [
    *cflags_base,
    "-i include/PowerPC_EABI_Support/MSL_C/MSL_Common/Include",
    "-i include/PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Include",
    "-ir src/dolphin",
    "-i include/PowerPC_EABI_Support/Runtime",
]

cflags_game = [
    *cflags_base,
    "-RTTI on",
    "-Cpp_exceptions on",
    "-inline auto,deferred",
    "-str reuse,readonly",
    "-i include/PowerPC_EABI_Support/MSL_C/MSL_Common/Include",
    "-i src/Cranky",
    "-i include/PowerPC_EABI_Support/Runtime",
]

cflags_lib = [
    *cflags_base,
    "-i include/PowerPC_EABI_Support/MSL_C/MSL_Common/Include",
    "-i src/dolphin",
    "-i include/PowerPC_EABI_Support/Runtime",
]

# REL flags
cflags_rel = [
    *cflags_base,
    "-sdata 0",
    "-sdata2 0",
]

config.linker_version = "GC/1.3.2"


# Helper function for Dolphin libraries
def DolphinLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.2.5n",
        "cflags": cflags_sdk,
        "progress_category": "sdk",
        "objects": objects,
    }

def Lib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.3.2",
        "cflags": cflags_lib,
        "progress_category": "game",
        "objects": objects,
    }

# Helper function for REL script objects
def Rel(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.3.2",
        "cflags": cflags_rel,
        "progress_category": "game",
        "objects": objects,
    }


Matching = True                   # Object matches and should be linked
NonMatching = False               # Object does not match and should not be linked
Equivalent = config.non_matching  # Object should be linked when configured with --non-matching


# Object is only matching for specific versions
def MatchingFor(*versions):
    return config.version in versions


config.warn_missing_config = True
config.warn_missing_source = False
config.libs = [
    {
        "lib": "game",
        "mw_version": "GC/1.3.2",
        "cflags": cflags_game,
        "progress_category": "game",
        "host": True,
        "objects": [
            Object(NonMatching, "summoner.cpp"),
            Object(NonMatching, "effects.cpp"),
            Object(NonMatching, "gr.cpp"),
            Object(NonMatching, "gr_font.cpp"),
            Object(NonMatching, "gr_poly.cpp"),
            Object(NonMatching, "frametime.cpp"),
            Object(NonMatching, "gameplay.cpp"),
            Object(NonMatching, "levels.cpp"),
            Object(NonMatching, "effect_mem.cpp"),
            Object(NonMatching, "matrix.cpp"),
            Object(NonMatching, "stringpool.cpp"),
            Object(NonMatching, "new_interface.cpp"),
            Object(NonMatching, "gamewide.cpp"),
            Object(NonMatching, "parse.cpp"),
            Object(NonMatching, "gamesound.cpp"),
            Object(NonMatching, "character_instance.cpp"),
            Object(NonMatching, "skeleton.cpp"),
            Object(NonMatching, "vif_particles.cpp"),
            Object(NonMatching, "objects.cpp"),
            Object(NonMatching, "items.cpp"),
            Object(NonMatching, "spells_queue.cpp"),
            Object(NonMatching, "skills.cpp"),
            Object(NonMatching, "norlsallocpool.cpp"),
            Object(NonMatching, "characterinfo.cpp"),
            Object(NonMatching, "player.cpp"),
            Object(NonMatching, "footsteps.cpp"),
            Object(NonMatching, "boss_ai.cpp"),
            Object(NonMatching, "dc_menu.cpp"),
            Object(NonMatching, "level_unseen.cpp"),
            Object(NonMatching, "level_arena.cpp"),
            Object(NonMatching, "objectrender.cpp"),
            Object(NonMatching, "living_entity.cpp"),
            Object(NonMatching, "pathfinding.cpp"),
            Object(NonMatching, "combat_skills.cpp"),
            Object(NonMatching, "level_save.cpp"),
            Object(NonMatching, "gameload.cpp"),
            Object(NonMatching, "level_scripts.cpp"),
            Object(NonMatching, "new_fullscreen.cpp"),
            Object(NonMatching, "cutscene.cpp"),
            Object(NonMatching, "level_sequences.cpp"),
            Object(NonMatching, "water_ripples.cpp"),
            Object(NonMatching, "rain.cpp"),
            Object(NonMatching, "level_scripts_loadsave.cpp"),
            Object(NonMatching, "level_sequences_loadsave.cpp"),
            Object(NonMatching, "flipbook.cpp"),
            Object(NonMatching, "gamemem.cpp"),
            Object(Equivalent, "memory.cpp"),
            Object(NonMatching, "debug.cpp"),
            Object(NonMatching, "plainstring.cpp"),
            Object(NonMatching, "FileSystem.cpp"),
            Object(NonMatching, "PackFileSystem.cpp"),
            Object(NonMatching, "NGCSystem.cpp"),
            Object(NonMatching, "MemSystem.cpp"),
            Object(NonMatching, "InputSystem.cpp"),
            Object(NonMatching, "LevelManager.cpp"),
            Object(NonMatching, "RenderSystem.cpp"),
            Object(NonMatching, "gr_ngc.cpp"),
            Object(NonMatching, "S3dConvert.cpp"),
            Object(NonMatching, "Cranky/am/am.c"),
            Object(NonMatching, "lore.cpp"),
            Object(NonMatching, "quests.cpp"),
            Object(NonMatching, "new_help.cpp"),
            Object(NonMatching, "new_tutorial.cpp"),
            Object(NonMatching, "SoundSystem.cpp"),
            Object(NonMatching, "music.cpp"),
            Object(NonMatching, "sound_3d.cpp"),
            Object(NonMatching, "SoundConvert.cpp"),
            Object(NonMatching, "MusicSystem.cpp"),
            Object(NonMatching, "memcard.cpp"),
            Object(NonMatching, "console.cpp"),
        ],
    },
    {
        "lib": "Runtime.PPCEABI.H",
        "mw_version": config.linker_version,
        "cflags": cflags_runtime,
        "progress_category": "sdk",  # str | List[str]
        "objects": [
            Object(Matching, "PowerPC_EABI_Support/Runtime/global_destructor_chain.c"),
            Object(Matching, "PowerPC_EABI_Support/Runtime/__init_cpp_exceptions.cpp"),
            Object(Matching, "PowerPC_EABI_Support/Runtime/__mem.c"),
            Object(Matching, "PowerPC_EABI_Support/Runtime/CPlusLibPPC.cp"),
            Object(NonMatching, "PowerPC_EABI_Support/Runtime/runtime.c"),
            Object(NonMatching, "PowerPC_EABI_Support/Runtime/Gecko_ExceptionPPC.cp", extra_cflags = ["-Cpp_exceptions on"]),
            Object(NonMatching, "PowerPC_EABI_Support/Runtime/NMWException.cp", extra_cflags = ["-Cpp_exceptions on"]),
            Object(NonMatching, "PowerPC_EABI_Support/Runtime/GCN_mem_alloc.c"),
        ],
    },
    {
        "lib": "MSL",
        "mw_version": config.linker_version,
        "cflags": cflags_runtime,
        "progress_category": "sdk",  # str | List[str]
        "objects": [
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common/abort_exit.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL_C/MSL_Common/alloc.c", extra_cflags = ["-inline noauto"]),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common/errno.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common/ansi_files.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common/ctype.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common/arith.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common/buffer_io.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common/direct_io.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL_C/MSL_Common/file_io.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common/FILE_POS.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common/mbstring.c", extra_cflags = ["-inline noauto"]),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common/mem.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL_C/MSL_Common/mem_funcs.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common/misc_io.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL_C/MSL_Common/printf.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common/scanf.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common/string.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL_C/MSL_Common/strtoul.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common/wchar_io.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/ansi_fp.c", extra_cflags = ["-inline noauto"]),
            Object(NonMatching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/uart_console_io.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_acos.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_asin.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_atan2.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_exp.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_pow.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_rem_pio2.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_cos.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_rem_pio2.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_sin.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_tan.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_atan.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_ceil.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_copysign.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_cos.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_floor.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_frexp.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_ldexp.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_modf.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_nextafter.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_sin.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_tan.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_acos.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_asin.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_atan2.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_exp.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_pow.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_powf.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_tanf.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_sinf.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_cosf.c"),
            Object(Matching, "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_atanf.c"),
        ],
    },
    DolphinLib(
        "os",
        [
            Object(Matching, "dolphin/os/OS.c"),
            Object(Matching, "dolphin/os/__start.c"),
            Object(Matching, "dolphin/os/OSAlarm.c"),
            Object(Matching, "dolphin/os/OSAlloc.c"),
            Object(Matching, "dolphin/os/OSArena.c"),
            Object(Matching, "dolphin/os/OSAudioSystem.c"),
            Object(Matching, "dolphin/os/OSCache.c"),
            Object(Matching, "dolphin/os/OSContext.c"),
            Object(Matching, "dolphin/os/OSError.c"),
            Object(NonMatching, "dolphin/os/OSFont.c"),
            Object(Matching, "dolphin/os/OSInterrupt.c"),
            Object(Matching, "dolphin/os/OSLink.c"),
            Object(NonMatching, "dolphin/os/OSMemory.c"),
            Object(Matching, "dolphin/os/OSMutex.c"),
            Object(Matching, "dolphin/os/OSReboot.c"),
            Object(NonMatching, "dolphin/os/OSReset.c"),
            Object(NonMatching, "dolphin/os/OSResetSW.c"),
            Object(Matching, "dolphin/os/OSRtc.c"),
            Object(Matching, "dolphin/os/OSSync.c"),
            Object(Matching, "dolphin/os/OSThread.c"),
            Object(Matching, "dolphin/os/OSTime.c"),
            Object(NonMatching, "dolphin/os/OSTimer.c"),
            Object(NonMatching, "dolphin/os/time.dolphin.c"),
            Object(Matching, "dolphin/os/__ppc_eabi_init.c"),
        ],
    ),
    DolphinLib(
        "ai",
        [
            Object(Matching, "dolphin/ai/ai.c"),
        ]
    ),
    DolphinLib(
        "ar",
        [
            Object(NonMatching, "dolphin/ar/ar.c"),
            Object(Matching, "dolphin/ar/arq.c"),
        ]
    ),
    DolphinLib(
        "ax",
        [
            Object(Matching, "dolphin/ax/AX.c"),
            Object(Matching, "dolphin/ax/AXAlloc.c"),
            Object(NonMatching, "dolphin/ax/AXAux.c"),
            Object(NonMatching, "dolphin/ax/AXCL.c"),
            Object(NonMatching, "dolphin/ax/AXOut.c"),
            Object(Matching, "dolphin/ax/AXProf.c"),
            Object(NonMatching, "dolphin/ax/AXSPB.c"),
            Object(NonMatching, "dolphin/ax/AXVPB.c"),
            Object(NonMatching, "dolphin/ax/DSPCode.c"),
        ]
    ),
    DolphinLib(
        "axfx",
        [
            Object(NonMatching, "dolphin/axfx/axfx.c"),
        ]
    ),
    DolphinLib(
        "dvd",
        [
            Object(NonMatching, "dolphin/dvd/dvd.c"),
            Object(Matching, "dolphin/dvd/dvdfs.c"),
            Object(NonMatching, "dolphin/dvd/dvdlow.c"),
            Object(Matching, "dolphin/dvd/dvdqueue.c"),
            Object(Matching, "dolphin/dvd/dvderror.c"),
            Object(Matching, "dolphin/dvd/fstload.c"),
        ]
    ),
    DolphinLib(
        "dsp",
        [
            Object(NonMatching, "dolphin/dsp/dsp.c"),
            Object(NonMatching, "dolphin/dsp/dsp_debug.c"),
            Object(NonMatching, "dolphin/dsp/dsp_task.c"),
        ]
    ),
    DolphinLib(
        "dtk",
        [
            Object(Matching, "dolphin/dtk/dtk.c"),
        ]
    ),
    DolphinLib(
        "PPCArch",
        [
            Object(NonMatching, "dolphin/base/PPCArch.c"),

        ]
    ),
    DolphinLib(
        "gx",
        [
            Object(NonMatching, "dolphin/gx/GXInit.c"),
            Object(NonMatching, "dolphin/gx/GXTexture.c"),
            Object(NonMatching, "dolphin/gx/GXMisc.c"),
            Object(NonMatching, "dolphin/gx/GXGeometry.c"),
            Object(NonMatching, "dolphin/gx/GXFrameBuf.c"),
            Object(NonMatching, "dolphin/gx/GXDisplayList.c"),
            Object(NonMatching, "dolphin/gx/GXLight.c"),
            Object(NonMatching, "dolphin/gx/GXTransform.c"),
            Object(NonMatching, "dolphin/gx/GXFifo.c"),
            Object(NonMatching, "dolphin/gx/GXAttr.c"),
            Object(NonMatching, "dolphin/gx/GXPerf.c"),
            Object(NonMatching, "dolphin/gx/GXTev.c"),
            Object(NonMatching, "dolphin/gx/GXPixel.c"),
            Object(NonMatching, "dolphin/gx/GXBump.c"),
            Object(Matching, "dolphin/gx/GXStubs.c"),
        ]
    ),
    DolphinLib(
        "card",
        [
            Object(NonMatching, "dolphin/card/CARDBios.c"),
            Object(NonMatching, "dolphin/card/CARDBlock.c"),
            Object(NonMatching, "dolphin/card/CARDCheck.c"),
            Object(NonMatching, "dolphin/card/CARDCreate.c"),
            Object(NonMatching, "dolphin/card/CARDDelete.c"),
            Object(NonMatching, "dolphin/card/CARDDir.c"),
            Object(NonMatching, "dolphin/card/CARDFormat.c"),
            Object(NonMatching, "dolphin/card/CARDMount.c"),
            Object(NonMatching, "dolphin/card/CARDNet.c"),
            Object(NonMatching, "dolphin/card/CARDOpen.c"),
            Object(NonMatching, "dolphin/card/CARDRdwr.c"),
            Object(NonMatching, "dolphin/card/CARDRead.c"),
            Object(NonMatching, "dolphin/card/CARDStat.c"),
            Object(NonMatching, "dolphin/card/CARDUnlock.c"),
            Object(NonMatching, "dolphin/card/CARDWrite.c"),
        ]
    ),
    DolphinLib(
        "vi",
        [
            Object(NonMatching, "dolphin/vi/vi.c"),
        ]
    ),
    DolphinLib(
        "exi",
        [
            Object(NonMatching, "dolphin/exi/EXIBios.c"),
            Object(NonMatching, "dolphin/exi/EXIUart.c"),
        ]
    ),
    DolphinLib(
        "si",
        [
            Object(NonMatching, "dolphin/si/SIBios.c"),
            Object(NonMatching, "dolphin/si/SISamplingRate.c"),
        ]
    ),
    DolphinLib(
        "odenotstub",
        [
            Object(Matching, "dolphin/odenotstub/odenotstub.c"),
        ]
    ),
    DolphinLib(
        "amcstubs",
        [
            Object(Matching, "dolphin/amcstubs/AmcExi2Stubs.c"),
        ]
    ),
    DolphinLib(
        "mtx",
        [
            Object(NonMatching, "dolphin/mtx/mtx.c"),
            Object(NonMatching, "dolphin/mtx/mtx44.c"),
            Object(NonMatching, "dolphin/mtx/vec.c"),
            Object(NonMatching, "dolphin/mtx/quat.c"),
        ]
    ),
    DolphinLib(
        "db",
        [
            Object(Matching, "dolphin/db/db.c"),
        ]
    ),
    DolphinLib(
        "sp",
        [
            Object(NonMatching, "dolphin/sp/sp.c"),
        ]
    ),
    DolphinLib(
        "axart",
        [
            Object(Matching, "dolphin/axart/axart.c"),
            Object(Matching, "dolphin/axart/axartsound.c"),
            Object(Matching, "dolphin/axart/axartcents.c"),
            Object(Matching, "dolphin/axart/axartenv.c"),
            Object(Matching, "dolphin/axart/axartlfo.c"),
            Object(Matching, "dolphin/axart/axart3d.c"),
        ]
    ),
    DolphinLib(
        "mix",
        [
            Object(NonMatching, "dolphin/mix/mix.c"),
        ]
    ),
    DolphinLib(
        "hio",
        [
            Object(NonMatching, "dolphin/hio/hio.c"),
        ]
    ),
    DolphinLib(
        "pad",
        [
            Object(NonMatching, "dolphin/pad/Pad.c"),
        ]
    ),
    Lib(
        "Cranky",
        [
            Object(NonMatching, "Cranky/CrankyInput.cpp"),
            Object(NonMatching, "Cranky/CrankyFileManager.cpp"),
            Object(NonMatching, "Cranky/CrankyMemcard.cpp"),
        ]
    ),
]

# Optional extra categories for progress tracking
# Adjust as desired for your project
config.progress_categories = [
    ProgressCategory("game", "Game Code"),
    ProgressCategory("sdk", "SDK Code"),
]
config.progress_each_module = args.verbose

if args.mode == "configure":
    # Write build.ninja and objdiff.json
    generate_build(config)
elif args.mode == "progress":
    # Print progress and write progress.json
    calculate_progress(config)
else:
    sys.exit("Unknown mode: " + args.mode)
