global StandartComparator
section .text

StandartComparator:

    vpcmpeqb ymm0, ymm1, ymm0 ; comparing 2 vectors
    vpmovmskb eax, ymm0       ; moving results of comparison in eax

    ret   