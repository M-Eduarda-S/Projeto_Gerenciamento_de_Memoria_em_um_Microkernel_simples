.section .text
.global context_switch

# a0 = &old->regs
# a1 = &new->regs

context_switch:

    # salva os ponteiros em registradores temporários não salvos ainda
    mv t5, a0
    mv t6, a1

    # ---------- SAVE OLD CONTEXT ----------

    sd ra,   0(t5)
    sd sp,   8(t5)

    sd gp,  16(t5)
    sd tp,  24(t5)

    sd t0,  32(t5)
    sd t1,  40(t5)
    sd t2,  48(t5)

    sd s0,  56(t5)
    sd s1,  64(t5)

    sd a0,  72(t5)
    sd a1,  80(t5)
    sd a2,  88(t5)
    sd a3,  96(t5)
    sd a4, 104(t5)
    sd a5, 112(t5)
    sd a6, 120(t5)
    sd a7, 128(t5)

    sd s2, 136(t5)
    sd s3, 144(t5)
    sd s4, 152(t5)
    sd s5, 160(t5)
    sd s6, 168(t5)
    sd s7, 176(t5)
    sd s8, 184(t5)
    sd s9, 192(t5)
    sd s10,200(t5)
    sd s11,208(t5)

    sd t3, 216(t5)
    sd t4, 224(t5)
    sd t5, 232(t5)   # salva t5 (usado como ponteiro do old), o valor original será restaurado depois 
    sd t6, 240(t5)   # salva t6 (ponteiro do new), também restaurado no ld

    # ---------- LOAD NEW CONTEXT ----------

    ld ra,   0(t6)
    ld sp,   8(t6)

    ld gp,  16(t6)
    ld tp,  24(t6)

    ld t0,  32(t6)
    ld t1,  40(t6)
    ld t2,  48(t6)

    ld s0,  56(t6)
    ld s1,  64(t6)

    ld a0,  72(t6)
    ld a1,  80(t6)
    ld a2,  88(t6)
    ld a3,  96(t6)
    ld a4, 104(t6)
    ld a5, 112(t6)
    ld a6, 120(t6)
    ld a7, 128(t6)

    ld s2, 136(t6)
    ld s3, 144(t6)
    ld s4, 152(t6)
    ld s5, 160(t6)
    ld s6, 168(t6)
    ld s7, 176(t6)
    ld s8, 184(t6)
    ld s9, 192(t6)
    ld s10,200(t6)
    ld s11,208(t6)

    ld t3, 216(t6)
    ld t4, 224(t6)
    ld t5, 232(t6)  # restaura t5 original da task
    ld t6, 240(t6)  # restaura t6 original da task

    ret