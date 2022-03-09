#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    void* func;
    void* arg;
} data_t;
typedef data_t (*func_t)(void*);

void stop() {
    exit(0);
}

#define stack_size (1024 * 1024) // 128MB stack (at 64bit) for 1M entries 
data_t stack[stack_size];
size_t stack_ptr = 0;
data_t pop() {
    if (stack_ptr == 0) stop();
    return stack[--stack_ptr];
}
void push(func_t func, void* arg) {
    stack[stack_ptr].func = func;
    stack[stack_ptr++].arg = arg;
}

#define heap_size (16 * 1024 * 1024) // 128MB heap (at 64bit) for 16M entries (for 2 binds per closure: 4M closures)
func_t heap[heap_size];
size_t heap_ptr = 0;
func_t* heap_alloc(size_t size) {
    int largeEnough = 0;
    while (!largeEnough) {
        largeEnough = 1;
        for (size_t i = 0; i < size; i++) {
            if (heap[heap_ptr + i] != 0) {
                largeEnough = 0;
                heap_ptr += i + 1;
                break;
            }
        }
        while (heap[heap_ptr] != 0)
            heap_ptr++;
    }
    func_t* alloced_ptr = &heap[heap_ptr];
    heap_ptr += size;
    return alloced_ptr;
}

data_t __call_closure(void* arg) {
    func_t binded_func = ((func_t)arg) + 1;
    while (binded_func)
        push(binded_func++, 0);
    data_t ret;
    ret.func = (func_t)arg;
    return ret;
}

int io_put = 0;

data_t ___IO_PUT_BEGIN(void* arg) {
    io_put = 0;
    return pop();
}
data_t ___IO_PUT_INC(void* arg) {
    io_put++;
    return pop();
}
data_t ___IO_PUT_DONE(void* arg) {
    putchar(io_put);
    return pop();
}
data_t ___IO_EOF(void* arg) {
    exit(0);
}
data_t _inc_c0(void* arg) {
    data_t v0 = pop();
    data_t v1 = pop();
    data_t v2 = pop();
    push(v0.func, v0.arg);
    push(v1.func, v1.arg);
    return v2;
}
data_t _inc(void* arg) {
    data_t v0 = pop();
    data_t v1 = pop();
    data_t v2 = pop();
    func_t* h0 = heap_alloc(5);
    h0[0] = _inc_c0;
    h0[1] = v0.func;
    h0[2] = v1.func;
    h0[3] = v2.func;
    h0[4] = 0;
    push(__call_closure, h0);
    return v1;
}
data_t _0(void* arg) {
    data_t v0 = pop();
    data_t v1 = pop();
    return v1;
}
data_t __gt__gt_(void* arg) {
    data_t v0 = pop();
    data_t v1 = pop();
    push(v1.func, v1.arg);
    return v0;
}
data_t ___IO_PUT_c0_l0(void* arg) {
    data_t v0 = pop();
    push(v0.func, v0.arg);
    push(___IO_PUT_INC, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t ___IO_PUT_c0(void* arg) {
    data_t v0 = pop();
    push(___IO_PUT_DONE, 0);
    push(___IO_PUT_c0_l0, 0);
    return v0;
}
data_t ___IO_PUT(void* arg) {
    data_t v0 = pop();
    data_t v1 = pop();
    push(v0.func, v0.arg);
    func_t* h0 = heap_alloc(3);
    h0[0] = ___IO_PUT_c0;
    h0[1] = v1.func;
    h0[2] = 0;
    push(__call_closure, h0);
    push(___IO_PUT_BEGIN, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _put(void* arg) {
    data_t v0 = pop();
    data_t v1 = pop();
    push(v0.func, v0.arg);
    push(v1.func, v1.arg);
    data_t ret; ret.func = ___IO_PUT; return ret;
}
data_t _io_start(void* arg) {
    data_t ret; ret.func = ___IO_EOF; return ret;
}
data_t _1(void* arg) {
    push(_0, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _2(void* arg) {
    push(_1, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _3(void* arg) {
    push(_2, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _4(void* arg) {
    push(_3, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _5(void* arg) {
    push(_4, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _6(void* arg) {
    push(_5, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _7(void* arg) {
    push(_6, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _8(void* arg) {
    push(_7, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _9(void* arg) {
    push(_8, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _10(void* arg) {
    push(_9, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _11(void* arg) {
    push(_10, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _12(void* arg) {
    push(_11, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _13(void* arg) {
    push(_12, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _14(void* arg) {
    push(_13, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _15(void* arg) {
    push(_14, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _16(void* arg) {
    push(_15, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _17(void* arg) {
    push(_16, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _18(void* arg) {
    push(_17, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _19(void* arg) {
    push(_18, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _20(void* arg) {
    push(_19, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _21(void* arg) {
    push(_20, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _22(void* arg) {
    push(_21, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _23(void* arg) {
    push(_22, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _24(void* arg) {
    push(_23, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _25(void* arg) {
    push(_24, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _26(void* arg) {
    push(_25, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _27(void* arg) {
    push(_26, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _28(void* arg) {
    push(_27, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _29(void* arg) {
    push(_28, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _30(void* arg) {
    push(_29, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _31(void* arg) {
    push(_30, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _32(void* arg) {
    push(_31, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _33(void* arg) {
    push(_32, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _34(void* arg) {
    push(_33, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _35(void* arg) {
    push(_34, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _36(void* arg) {
    push(_35, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _37(void* arg) {
    push(_36, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _38(void* arg) {
    push(_37, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _39(void* arg) {
    push(_38, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _40(void* arg) {
    push(_39, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _41(void* arg) {
    push(_40, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _42(void* arg) {
    push(_41, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _43(void* arg) {
    push(_42, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _44(void* arg) {
    push(_43, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _45(void* arg) {
    push(_44, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _46(void* arg) {
    push(_45, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _47(void* arg) {
    push(_46, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _48(void* arg) {
    push(_47, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _49(void* arg) {
    push(_48, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _50(void* arg) {
    push(_49, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _51(void* arg) {
    push(_50, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _52(void* arg) {
    push(_51, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _53(void* arg) {
    push(_52, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _54(void* arg) {
    push(_53, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _55(void* arg) {
    push(_54, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _56(void* arg) {
    push(_55, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _57(void* arg) {
    push(_56, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _58(void* arg) {
    push(_57, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _59(void* arg) {
    push(_58, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _60(void* arg) {
    push(_59, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _61(void* arg) {
    push(_60, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _62(void* arg) {
    push(_61, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _63(void* arg) {
    push(_62, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _64(void* arg) {
    push(_63, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _65(void* arg) {
    push(_64, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _66(void* arg) {
    push(_65, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _67(void* arg) {
    push(_66, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _68(void* arg) {
    push(_67, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _69(void* arg) {
    push(_68, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _70(void* arg) {
    push(_69, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _71(void* arg) {
    push(_70, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _72(void* arg) {
    push(_71, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _73(void* arg) {
    push(_72, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _74(void* arg) {
    push(_73, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _75(void* arg) {
    push(_74, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _76(void* arg) {
    push(_75, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _77(void* arg) {
    push(_76, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _78(void* arg) {
    push(_77, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _79(void* arg) {
    push(_78, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _80(void* arg) {
    push(_79, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _81(void* arg) {
    push(_80, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _82(void* arg) {
    push(_81, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _83(void* arg) {
    push(_82, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _84(void* arg) {
    push(_83, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _85(void* arg) {
    push(_84, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _86(void* arg) {
    push(_85, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _87(void* arg) {
    push(_86, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _88(void* arg) {
    push(_87, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _89(void* arg) {
    push(_88, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _90(void* arg) {
    push(_89, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _91(void* arg) {
    push(_90, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _92(void* arg) {
    push(_91, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _93(void* arg) {
    push(_92, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _94(void* arg) {
    push(_93, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _95(void* arg) {
    push(_94, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _96(void* arg) {
    push(_95, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _97(void* arg) {
    push(_96, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _98(void* arg) {
    push(_97, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _99(void* arg) {
    push(_98, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _100(void* arg) {
    push(_99, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _101(void* arg) {
    push(_100, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _102(void* arg) {
    push(_101, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _103(void* arg) {
    push(_102, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _104(void* arg) {
    push(_103, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _105(void* arg) {
    push(_104, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _106(void* arg) {
    push(_105, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _107(void* arg) {
    push(_106, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _108(void* arg) {
    push(_107, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _109(void* arg) {
    push(_108, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _110(void* arg) {
    push(_109, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _111(void* arg) {
    push(_110, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _112(void* arg) {
    push(_111, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _113(void* arg) {
    push(_112, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _114(void* arg) {
    push(_113, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _115(void* arg) {
    push(_114, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _116(void* arg) {
    push(_115, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _117(void* arg) {
    push(_116, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _118(void* arg) {
    push(_117, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _119(void* arg) {
    push(_118, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _120(void* arg) {
    push(_119, 0);
    data_t ret; ret.func = _inc; return ret;
}
data_t _cnewline(void* arg) {
    data_t ret; ret.func = _10; return ret;
}
data_t _cspace(void* arg) {
    data_t ret; ret.func = _32; return ret;
}
data_t _c!(void* arg) {
    data_t ret; ret.func = _33; return ret;
}
data_t _c.(void* arg) {
    data_t ret; ret.func = _46; return ret;
}
data_t _cD(void* arg) {
    data_t ret; ret.func = _68; return ret;
}
data_t _cH(void* arg) {
    data_t ret; ret.func = _72; return ret;
}
data_t _cT(void* arg) {
    data_t ret; ret.func = _84; return ret;
}
data_t _cW(void* arg) {
    data_t ret; ret.func = _87; return ret;
}
data_t _ca(void* arg) {
    data_t ret; ret.func = _97; return ret;
}
data_t _cd(void* arg) {
    data_t ret; ret.func = _100; return ret;
}
data_t _ce(void* arg) {
    data_t ret; ret.func = _101; return ret;
}
data_t _ci(void* arg) {
    data_t ret; ret.func = _105; return ret;
}
data_t _cl(void* arg) {
    data_t ret; ret.func = _108; return ret;
}
data_t _cn(void* arg) {
    data_t ret; ret.func = _110; return ret;
}
data_t _co(void* arg) {
    data_t ret; ret.func = _111; return ret;
}
data_t _cr(void* arg) {
    data_t ret; ret.func = _114; return ret;
}
data_t _cs(void* arg) {
    data_t ret; ret.func = _115; return ret;
}
data_t _ct(void* arg) {
    data_t ret; ret.func = _116; return ret;
}
data_t _cx(void* arg) {
    data_t ret; ret.func = _120; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0(void* arg) {
    push(_cnewline, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1(void* arg) {
    push(_c., 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0(void* arg) {
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, 0);
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1(void* arg) {
    push(_ct, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0(void* arg) {
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, 0);
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1(void* arg) {
    push(_cx, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0(void* arg) {
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, 0);
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1(void* arg) {
    push(_ce, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0(void* arg) {
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, 0);
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1(void* arg) {
    push(_cT, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0(void* arg) {
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, 0);
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1(void* arg) {
    push(_cspace, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0(void* arg) {
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, 0);
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1(void* arg) {
    push(_cn, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0(void* arg) {
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, 0);
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1(void* arg) {
    push(_ci, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0(void* arg) {
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, 0);
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1(void* arg) {
    push(_ce, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0(void* arg) {
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, 0);
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1(void* arg) {
    push(_cspace, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0(void* arg) {
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, 0);
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1(void* arg) {
    push(_ct, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0(void* arg) {
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, 0);
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1(void* arg) {
    push(_cs, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0(void* arg) {
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, 0);
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1(void* arg) {
    push(_ci, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0(void* arg) {
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, 0);
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1(void* arg) {
    push(_cspace, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0(void* arg) {
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, 0);
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1(void* arg) {
    push(_cs, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0(void* arg) {
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, 0);
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1(void* arg) {
    push(_ca, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0(void* arg) {
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, 0);
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1(void* arg) {
    push(_cD, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0(void* arg) {
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, 0);
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1(void* arg) {
    push(_cnewline, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0(void* arg) {
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, 0);
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1(void* arg) {
    push(_c!, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0(void* arg) {
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, 0);
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1(void* arg) {
    push(_cd, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0(void* arg) {
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, 0);
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1(void* arg) {
    push(_cl, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l0(void* arg) {
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, 0);
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0_l1(void* arg) {
    push(_cr, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l0(void* arg) {
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l0, 0);
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0_l1(void* arg) {
    push(_co, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l0(void* arg) {
    push(_main_l0_l0_l0_l0_l0_l0_l0_l0, 0);
    push(_main_l0_l0_l0_l0_l0_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0_l1(void* arg) {
    push(_cW, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l0(void* arg) {
    push(_main_l0_l0_l0_l0_l0_l0_l0, 0);
    push(_main_l0_l0_l0_l0_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l0_l0_l0_l0_l1(void* arg) {
    push(_cspace, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0_l0_l0(void* arg) {
    push(_main_l0_l0_l0_l0_l0_l0, 0);
    push(_main_l0_l0_l0_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l0_l0_l0_l1(void* arg) {
    push(_co, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0_l0(void* arg) {
    push(_main_l0_l0_l0_l0_l0, 0);
    push(_main_l0_l0_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l0_l0_l1(void* arg) {
    push(_cl, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0_l0(void* arg) {
    push(_main_l0_l0_l0_l0, 0);
    push(_main_l0_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l0_l1(void* arg) {
    push(_cl, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0_l0(void* arg) {
    push(_main_l0_l0_l0, 0);
    push(_main_l0_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l0_l1(void* arg) {
    push(_ce, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main_l0(void* arg) {
    push(_main_l0_l0, 0);
    push(_main_l0_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}
data_t _main_l1(void* arg) {
    push(_cH, 0);
    data_t ret; ret.func = _put; return ret;
}
data_t _main(void* arg) {
    push(_io_start, 0);
    push(_main_l0, 0);
    push(_main_l1, 0);
    data_t ret; ret.func = __gt__gt_; return ret;
}

struct func_str_pair {
    void* func;
    const char* str;
};
const struct func_str_pair funcToStr[] = {
    { ___IO_PUT_BEGIN, "__IO_PUT_BEGIN" },
    { ___IO_PUT_INC, "__IO_PUT_INC" },
    { ___IO_PUT_DONE, "__IO_PUT_DONE" },
    { ___IO_EOF, "__IO_EOF" },
    { _inc_c0, "inc_c0" },
    { _inc, "inc" },
    { _0, "0" },
    { __gt__gt_, ">>" },
    { ___IO_PUT_c0_l0, "__IO_PUT_c0_l0" },
    { ___IO_PUT_c0, "__IO_PUT_c0" },
    { ___IO_PUT, "__IO_PUT" },
    { _put, "put" },
    { _io_start, "io_start" },
    { _1, "1" },
    { _2, "2" },
    { _3, "3" },
    { _4, "4" },
    { _5, "5" },
    { _6, "6" },
    { _7, "7" },
    { _8, "8" },
    { _9, "9" },
    { _10, "10" },
    { _11, "11" },
    { _12, "12" },
    { _13, "13" },
    { _14, "14" },
    { _15, "15" },
    { _16, "16" },
    { _17, "17" },
    { _18, "18" },
    { _19, "19" },
    { _20, "20" },
    { _21, "21" },
    { _22, "22" },
    { _23, "23" },
    { _24, "24" },
    { _25, "25" },
    { _26, "26" },
    { _27, "27" },
    { _28, "28" },
    { _29, "29" },
    { _30, "30" },
    { _31, "31" },
    { _32, "32" },
    { _33, "33" },
    { _34, "34" },
    { _35, "35" },
    { _36, "36" },
    { _37, "37" },
    { _38, "38" },
    { _39, "39" },
    { _40, "40" },
    { _41, "41" },
    { _42, "42" },
    { _43, "43" },
    { _44, "44" },
    { _45, "45" },
    { _46, "46" },
    { _47, "47" },
    { _48, "48" },
    { _49, "49" },
    { _50, "50" },
    { _51, "51" },
    { _52, "52" },
    { _53, "53" },
    { _54, "54" },
    { _55, "55" },
    { _56, "56" },
    { _57, "57" },
    { _58, "58" },
    { _59, "59" },
    { _60, "60" },
    { _61, "61" },
    { _62, "62" },
    { _63, "63" },
    { _64, "64" },
    { _65, "65" },
    { _66, "66" },
    { _67, "67" },
    { _68, "68" },
    { _69, "69" },
    { _70, "70" },
    { _71, "71" },
    { _72, "72" },
    { _73, "73" },
    { _74, "74" },
    { _75, "75" },
    { _76, "76" },
    { _77, "77" },
    { _78, "78" },
    { _79, "79" },
    { _80, "80" },
    { _81, "81" },
    { _82, "82" },
    { _83, "83" },
    { _84, "84" },
    { _85, "85" },
    { _86, "86" },
    { _87, "87" },
    { _88, "88" },
    { _89, "89" },
    { _90, "90" },
    { _91, "91" },
    { _92, "92" },
    { _93, "93" },
    { _94, "94" },
    { _95, "95" },
    { _96, "96" },
    { _97, "97" },
    { _98, "98" },
    { _99, "99" },
    { _100, "100" },
    { _101, "101" },
    { _102, "102" },
    { _103, "103" },
    { _104, "104" },
    { _105, "105" },
    { _106, "106" },
    { _107, "107" },
    { _108, "108" },
    { _109, "109" },
    { _110, "110" },
    { _111, "111" },
    { _112, "112" },
    { _113, "113" },
    { _114, "114" },
    { _115, "115" },
    { _116, "116" },
    { _117, "117" },
    { _118, "118" },
    { _119, "119" },
    { _120, "120" },
    { _cnewline, "cnewline" },
    { _cspace, "cspace" },
    { _c!, "c!" },
    { _c., "c." },
    { _cD, "cD" },
    { _cH, "cH" },
    { _cT, "cT" },
    { _cW, "cW" },
    { _ca, "ca" },
    { _cd, "cd" },
    { _ce, "ce" },
    { _ci, "ci" },
    { _cl, "cl" },
    { _cn, "cn" },
    { _co, "co" },
    { _cr, "cr" },
    { _cs, "cs" },
    { _ct, "ct" },
    { _cx, "cx" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l0" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0_l1" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l0, "main_l0_l0_l0_l0_l0_l0_l0_l0_l0" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0_l1, "main_l0_l0_l0_l0_l0_l0_l0_l0_l1" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l0, "main_l0_l0_l0_l0_l0_l0_l0_l0" },
    { _main_l0_l0_l0_l0_l0_l0_l0_l1, "main_l0_l0_l0_l0_l0_l0_l0_l1" },
    { _main_l0_l0_l0_l0_l0_l0_l0, "main_l0_l0_l0_l0_l0_l0_l0" },
    { _main_l0_l0_l0_l0_l0_l0_l1, "main_l0_l0_l0_l0_l0_l0_l1" },
    { _main_l0_l0_l0_l0_l0_l0, "main_l0_l0_l0_l0_l0_l0" },
    { _main_l0_l0_l0_l0_l0_l1, "main_l0_l0_l0_l0_l0_l1" },
    { _main_l0_l0_l0_l0_l0, "main_l0_l0_l0_l0_l0" },
    { _main_l0_l0_l0_l0_l1, "main_l0_l0_l0_l0_l1" },
    { _main_l0_l0_l0_l0, "main_l0_l0_l0_l0" },
    { _main_l0_l0_l0_l1, "main_l0_l0_l0_l1" },
    { _main_l0_l0_l0, "main_l0_l0_l0" },
    { _main_l0_l0_l1, "main_l0_l0_l1" },
    { _main_l0_l0, "main_l0_l0" },
    { _main_l0_l1, "main_l0_l1" },
    { _main_l0, "main_l0" },
    { _main_l1, "main_l1" },
    { _main, "main" },
};

void print_str(func_t f, func_t* arg, int depth) {
    if (depth == 0) { printf("..."); return; }
    if (f == __call_closure) {
        printf("[");
        if (arg == 0) { printf("NULL]"); print_str(f, 0, depth - 1); return; }
        for(func_t *p = arg+1; *p; p++) {
            print_str(*p, 0, depth - 1);
            if (*(p + 1)) printf(",");
        }
        printf("]");
        print_str(*arg, 0, depth - 1);
    } else {
        for (size_t i = 0; i < sizeof(funcToStr) / sizeof(funcToStr[0]); i++)
            if (funcToStr[i].func == f) {
                printf("%s", funcToStr[i].str);
                return;
            }
        printf("?");
    }}

int main() {
    data_t next_section;
    next_section.func = _main;
    while(1) {
        print_str(next_section.func, next_section.arg, 5);
        printf(" ");
        for (int i = stack_ptr - 1; i >= 0; i--) { print_str(stack[i].func, stack[i].arg, 4); printf(" "); }
        printf("\n");
        next_section = ((func_t)next_section.func)(next_section.arg);
    }
}

