;exmpl2
.extern MM
.extern ZZ
    .string "cd"
D1: .data -5
KK: clr r2
mcro mc
  jsr KK
  not D1
mcroend
.entry KK
red MM
    rts
        

