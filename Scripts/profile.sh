#!/bin/bash

MOBILERT_DIR="/mnt/D/Android_Studio_Projects/MobileRayTracer"
BIN_DIR="${MOBILERT_DIR}/build_native/bin"
SCRIPTS_DIR="${MOBILERT_DIR}/Scripts"
PLOT_SCRIPTS_DIR="${SCRIPTS_DIR}/Plot_Scripts"
OBJS_DIR="${MOBILERT_DIR}/app/src/main/assets/WavefrontOBJs"

MOBILERT_SRCS="${MOBILERT_DIR}/app"
COMPONENTS_SRCS="${MOBILERT_DIR}/app"
GTK_INTERFACE_SRCS="${MOBILERT_DIR}/app/GTK_Interface"
SCENES_SRCS="${MOBILERT_DIR}/app/Scenes"

THIRDPARTY_HEADERS="${MOBILERT_DIR}/app/third_party"
GSL_HEADERS="${MOBILERT_DIR}/app/third_party/GSL/include"
GLM_HEADERS="${MOBILERT_DIR}/app/third_party/glm"
GTK_HEADERS="$(pkg-config --cflags gtk+-2.0)"
GTK_HEADERS="${GTK_HEADERS//-I/-isystem}"

if [ -z "${PLOT_GRAPHS}" ]; then
  PLOT_GRAPHS="Plot_Graphs"
fi
mkdir -p ${PLOT_GRAPHS}

for FOLDER in ${PLOT_GRAPHS[@]}
do
  FILES+=($(find ${FOLDER} -type f))
done

OBJ="${OBJS_DIR}/conference/conference.obj"
MTL="${OBJS_DIR}/conference/conference.mtl"

OBJ="${OBJS_DIR}/teapot/teapot.obj"
MTL="${OBJS_DIR}/teapot/teapot.mtl"

OBJ="${OBJS_DIR}/sponza/sponza.obj"
MTL="${OBJS_DIR}/sponza/sponza.mtl"

OBJ="${OBJS_DIR}/powerplant/powerplant.obj"
MTL="${OBJS_DIR}/powerplant/powerplant.mtl"

OBJ="${OBJS_DIR}/San_Miguel/san-miguel.obj"
MTL="${OBJS_DIR}/San_Miguel/san-miguel.mtl"

OBJ="${OBJS_DIR}/CornellBox/CornellBox-Empty-CO.obj"
MTL="${OBJS_DIR}/CornellBox/CornellBox-Empty-CO.mtl"
OBJ="${OBJS_DIR}/CornellBox/CornellBox-Empty-RG.obj"
MTL="${OBJS_DIR}/CornellBox/CornellBox-Empty-RG.mtl"
OBJ="${OBJS_DIR}/CornellBox/CornellBox-Empty-Squashed.obj"
MTL="${OBJS_DIR}/CornellBox/CornellBox-Empty-Squashed.mtl"
OBJ="${OBJS_DIR}/CornellBox/CornellBox-Empty-White.obj"
MTL="${OBJS_DIR}/CornellBox/CornellBox-Empty-White.mtl"
OBJ="${OBJS_DIR}/CornellBox/CornellBox-Glossy-Floor.obj"
MTL="${OBJS_DIR}/CornellBox/CornellBox-Glossy-Floor.mtl"
OBJ="${OBJS_DIR}/CornellBox/CornellBox-Glossy.obj"
MTL="${OBJS_DIR}/CornellBox/CornellBox-Glossy.mtl"
OBJ="${OBJS_DIR}/CornellBox/CornellBox-Mirror.obj"
MTL="${OBJS_DIR}/CornellBox/CornellBox-Mirror.mtl"
OBJ="${OBJS_DIR}/CornellBox/CornellBox-Original.obj"
MTL="${OBJS_DIR}/CornellBox/CornellBox-Original.mtl"
OBJ="${OBJS_DIR}/CornellBox/CornellBox-Sphere.obj"
MTL="${OBJS_DIR}/CornellBox/CornellBox-Sphere.mtl"
OBJ="${OBJS_DIR}/CornellBox/CornellBox-Water.obj"
MTL="${OBJS_DIR}/CornellBox/CornellBox-Water.mtl"
OBJ="${OBJS_DIR}/CornellBox/water.obj"
MTL="${OBJS_DIR}/CornellBox/water.mtl"

OBJ="${OBJS_DIR}/conference/conference.obj"
MTL="${OBJS_DIR}/conference/conference.mtl"

export ASAN_OPTIONS="suppressions=sanitizer_ignore.suppr:verbosity=1:strict_string_checks=1:detect_stack_use_after_return=1:check_initialization_order=1:strict_init_order=1:halt_on_error=0:detect_odr_violation=1"

export LSAN_OPTIONS="suppressions=sanitizer_ignore.suppr:verbosity=1:strict_string_checks=1"

SPP="1"
SPL="1"
WIDTH="900"
HEIGHT="900"
REP="1"
PRINT="false"
ASYNC="false"
SHOWIMAGE="false"
SEP="-"

THREADS="1"
REPETITIONS="10"

SHADERS="1 2"
SCENES="2"
ACCELERATORS="1 2"

function execute {
  THREAD="4"
  SHADER="1"
  SCENE="4"
  ACC="2"
  PRINT="true"
  SHOWIMAGE="true"
  ASYNC="true"
  SPP="1"
  SPL="1"

  echo ""
  echo "THREAD = "${THREAD}
  echo "SHADER = "${SHADER}
  echo "SCENE = "${SCENE}
  echo "ACC = "${ACC}

  #perf script report callgrind > perf.callgrind
  #kcachegrind perf.callgrind
  #perf stat \
  #perf record -g --call-graph 'fp' -- \
  ${BIN_DIR}/AppInterface \
            ${THREAD} ${SHADER} ${SCENE} ${SPP} ${SPL} ${WIDTH} ${HEIGHT} ${ACC} ${REP} \
            ${OBJ} ${MTL} ${PRINT} ${ASYNC} ${SHOWIMAGE}
  #perf report -g '' --show-nr-samples --hierarchy
}



function clangtidy {
  clang-tidy-5.0 \
	-analyze-temporary-dtors \
	-checks='*,-*llvm-header-guard*' \
	-header-filter='.*' \
  ${MOBILERT_SRCS}/MobileRT/*.*pp \
  ${MOBILERT_SRCS}/MobileRT/*/*.*pp \
  ${COMPONENTS_SRCS}/Components/*/*.*pp \
	${GTK_INTERFACE_SRCS}/*.*pp \
  ${SCENES_SRCS}/*.*pp \
	-- -std=c++14 -ferror-limit=1 \
  -I ${MOBILERT_SRCS} \
  -I ${COMPONENTS_SRCS} \
  -I ${GTK_INTERFACE_SRCS} \
  -I ${SCENES_SRCS} \
	-isystem ${THIRDPARTY_HEADERS} \
  -isystem ${GSL_HEADERS} \
  -isystem ${GLM_HEADERS} \
	-isystem /usr/include/c++/v1 \
  -isystem /usr/include/glib-2.0/gobject \
  -isystem /usr/include/gtk-2.0/gtk \
	${GTK_HEADERS} \
	2>&1 | tee ${SCRIPTS_DIR}/tidy.out
}

function profile {
  trap "exit" INT
  for R in `seq 1 ${REPETITIONS}`;
  do
    for THREAD in ${THREADS[@]}
    do
      for SHADER in ${SHADERS[@]}
      do
        for SCENE in ${SCENES[@]}
        do
          for ACC in ${ACCELERATORS[@]}
          do
            echo ""
            echo "REPETITION = ${R}"
            echo "THREAD = "${THREAD}
            echo "SHADER = "${SHADER}
            echo "SCENE = "${SCENE}
            echo "ACC = "${ACC}

            PLOT_FILE="SC${SCENE}${SEP}SH${SHADER}${SEP}A${ACC}${SEP}R${WIDTH}x${HEIGHT}"

            ${BIN_DIR}/AppInterface \
            ${THREAD} ${SHADER} ${SCENE} ${SPP} ${SPL} ${WIDTH} ${HEIGHT} ${ACC} ${REP} \
            ${OBJ} ${MTL} ${PRINT} ${ASYNC} ${SHOWIMAGE} \
            | awk -v threads="${THREAD}" -f ${PLOT_SCRIPTS_DIR}/parser_out.awk 2>&1 \
            | tee -a ${PLOT_GRAPHS}/${PLOT_FILE}.dat

          done
        done
      done
    done
    ((R++))
  done
}

PARAM1="time"
PARAM2="drawt"
PARAM3="draws"
PARAM4="test"
PARAM5="exec"
PARAM6="tidy"
PARAM7="gtest"

for P in ${@}
do
  case ${P} in
    ${PARAM1}) profile; sleep 2s ;;
    ${PARAM2}) . ${PLOT_SCRIPTS_DIR}/plot.sh 0;;
    ${PARAM3}) . ${PLOT_SCRIPTS_DIR}/plot.sh 1;;
    ${PARAM4}) awk -f "${PLOT_SCRIPTS_DIR}/parser_median.awk" "${PLOT_SCRIPTS_DIR}/test.dat"  ;;
    ${PARAM5}) execute ;;
    ${PARAM6}) clangtidy ;;
    ${PARAM7}) ${BIN_DIR}/GoogleTestd ;;
    *) echo ""
       echo "Wrong Parameter: ${P}"
       echo "The valid parameters are:"
       echo "${PARAM1} - Profile application and log the measured times."
       echo "${PARAM2} - Draw a graph with GNU Plot."
       break
       ;;
  esac
done
