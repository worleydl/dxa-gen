#!/bin/bash

# Needs to point to your build folder
GEN_BIN="build_mgw/midi2ogg.exe"

$GEN_BIN hmq_dump/briefing.hmp staging_dxa/briefing.ogg 2
$GEN_BIN hmq_dump/credits.hmp staging_dxa/credits.ogg 2
$GEN_BIN hmq_dump/descent.hmq staging_dxa/descent.ogg 4
$GEN_BIN hmq_dump/endgame.hmq staging_dxa/endgame.ogg 3
$GEN_BIN hmq_dump/endlevel.hmq staging_dxa/endlevel.ogg 3
$GEN_BIN hmq_dump/game01.hmqq staging_dxa/game01.ogg 4
$GEN_BIN hmq_dump/game02.hmqq staging_dxa/game02.ogg 4
$GEN_BIN hmq_dump/game03.hmqq staging_dxa/game03.ogg 3
$GEN_BIN hmq_dump/game04.hmp staging_dxa/game04.ogg 2
$GEN_BIN hmq_dump/game05.hmp staging_dxa/game05.ogg 2
$GEN_BIN hmq_dump/game06.hmp staging_dxa/game06.ogg 2
$GEN_BIN hmq_dump/game07.hmp staging_dxa/game07.ogg 2
$GEN_BIN hmq_dump/game08.hmqq staging_dxa/game08.ogg 5
$GEN_BIN hmq_dump/game09.hmp staging_dxa/game09.ogg 2
$GEN_BIN hmq_dump/game10.hmp staging_dxa/game10.ogg 2
$GEN_BIN hmq_dump/game11.hmqq staging_dxa/game11.ogg 3
$GEN_BIN hmq_dump/game12.hmp staging_dxa/game12.ogg 2
$GEN_BIN hmq_dump/game13.hmqq staging_dxa/game13.ogg 3
$GEN_BIN hmq_dump/game14.hmqq staging_dxa/game14.ogg 3
$GEN_BIN hmq_dump/game15.hmp staging_dxa/game15.ogg 2
$GEN_BIN hmq_dump/game16.hmp staging_dxa/game16.ogg 2
$GEN_BIN hmq_dump/game17.hmp staging_dxa/game17.ogg 2
$GEN_BIN hmq_dump/game18.hmqq staging_dxa/game18.ogg 3
$GEN_BIN hmq_dump/game19.hmp staging_dxa/game19.ogg 2
$GEN_BIN hmq_dump/game20.hmp staging_dxa/game20.ogg 2
$GEN_BIN hmq_dump/game21.hmqq staging_dxa/game21.ogg 3
$GEN_BIN hmq_dump/game22.hmqq staging_dxa/game22.ogg 4

echo "Generation complete, zipping is still a manual step for now"
