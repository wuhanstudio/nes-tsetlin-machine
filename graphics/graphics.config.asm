; CHR ROM data
;
; The files below will be loaded into the game's chr banks directly, and available for your
; use.
;

.segment "CHR_00"

    ; We just put the ascii tiles into both sprites and tiles. If you want to get more clever you could do something else.
    .incbin "graphics/ascii.chr"

; Note: You can put your own separate chr files here to use them... we only use 3 in the demo. This is to avoid warnings,
; and make the rom a predictable size. Note that if you do this you'll have to tweak the engine to support it! There's
; hopefully a guide on how to do this in the repo.
.segment "CHR_01"
.segment "CHR_02"
.segment "CHR_03"
.segment "CHR_04"
.segment "CHR_05"
.segment "CHR_06"
.segment "CHR_07"
.segment "CHR_08"
.segment "CHR_09"
.segment "CHR_0A"
.segment "CHR_0B"
.segment "CHR_0C"
.segment "CHR_0D"
.segment "CHR_0E"
.segment "CHR_0F"
.segment "CHR_10"
.segment "CHR_11"
.segment "CHR_12"
.segment "CHR_13"
.segment "CHR_14"
.segment "CHR_15"
.segment "CHR_16"
.segment "CHR_17"
.segment "CHR_18"
.segment "CHR_19"
.segment "CHR_1A"
.segment "CHR_1B"
.segment "CHR_1C"
.segment "CHR_1D"
.segment "CHR_1E"
.segment "CHR_1F"
