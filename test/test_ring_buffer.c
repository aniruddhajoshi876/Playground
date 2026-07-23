/*
 * test_ring_buffer.c
 *
 * Host-side test bench for ring_buffer.c
 * Compiled and run on your PC, not on the STM32. No HAL, no hardware.
 *
 * This version drives TWO independent ring buffers of DIFFERENT sizes at the
 * same time. If any global state is left in your module, or if a buffer ever
 * indexes outside its own storage, these tests are built to catch it.
 *
 * Build + run:   ./run.ps1
 */

#include <stdio.h>
#include <string.h>
#include "ring_buffer.h"

/* ------------------------------------------------------------------ */
/* bookkeeping                                                         */
/* ------------------------------------------------------------------ */

static int checks_run = 0;
static int checks_failed = 0;

#define CHECK_EQ(what, actual, expected)                                      \
    do {                                                                      \
        int _a = (int)(actual);                                               \
        int _e = (int)(expected);                                             \
        checks_run++;                                                         \
        if (_a != _e) {                                                       \
            checks_failed++;                                                  \
            printf("    FAIL  %-30s expected %3d, got %3d   (line %d)\n",     \
                   (what), _e, _a, __LINE__);                                 \
        } else {                                                              \
            printf("    ok    %-30s = %d\n", (what), _a);                     \
        }                                                                     \
    } while (0)

static void begin(const char *name)
{
    printf("\n[%s]\n", name);
}

/* ------------------------------------------------------------------ */
/* storage with guard bands                                            */
/*                                                                     */
/* Each buffer's bytes sit in the middle of a larger arena. The bytes  */
/* on either side are filled with 0xAA. Your code should never touch   */
/* them. If it writes at buffer[-1] or buffer[size], the guard check   */
/* turns red and tells you exactly which side ran over.                */
/* ------------------------------------------------------------------ */

#define GUARD      8
#define GUARD_BYTE 0xAA

#define SIZE_A 6
#define SIZE_B 4

static uint8_t arena_a[GUARD + SIZE_A + GUARD];
static uint8_t arena_b[GUARD + SIZE_B + GUARD];

static uint8_t *storage_a = arena_a + GUARD;
static uint8_t *storage_b = arena_b + GUARD;

static void arena_reset(void)
{
    memset(arena_a, GUARD_BYTE, sizeof arena_a);
    memset(arena_b, GUARD_BYTE, sizeof arena_b);
    memset(storage_a, 0, SIZE_A);
    memset(storage_b, 0, SIZE_B);
}

static int guards_intact(const uint8_t *arena, unsigned data_len, const char *name)
{
    int ok = 1;
    for (unsigned i = 0; i < GUARD; i++) {
        if (arena[i] != GUARD_BYTE) {
            printf("    FAIL  %s underran: wrote %u at %d bytes BELOW its storage\n",
                   name, arena[i], (int)(GUARD - i));
            ok = 0;
            break;
        }
    }
    for (unsigned i = 0; i < GUARD; i++) {
        const uint8_t *hi = arena + GUARD + data_len;
        if (hi[i] != GUARD_BYTE) {
            printf("    FAIL  %s overran: wrote %u at index size+%u\n", name, hi[i], i);
            ok = 0;
            break;
        }
    }
    return ok;
}

static void check_guards(void)
{
    checks_run++;
    int a = guards_intact(arena_a, SIZE_A, "buffer A");
    int b = guards_intact(arena_b, SIZE_B, "buffer B");
    if (a && b) printf("    ok    %-30s (nothing wrote out of bounds)\n", "guard bands intact");
    else        checks_failed++;
}

/* Prints one buffer's storage plus head/tail/count. */
static void dump(const char *label, ring_buffer *rb, const uint8_t *storage)
{
    printf("    %-18s size=%-2u head=%-2u tail=%-2u count=%-2u  [",
           label, rb->size, rb->head, rb->tail, rb->count);
    for (unsigned i = 0; i < rb->size; i++) printf("%4u", storage[i]);
    printf(" ]\n");
}

/* ------------------------------------------------------------------ */
/* reference model                                                     */
/* ------------------------------------------------------------------ */

typedef struct {
    uint8_t  q[16];
    unsigned n;
    unsigned cap;
} model_t;

static void  model_init(model_t *m, unsigned cap) { m->n = 0; m->cap = cap; }
static int   model_full(const model_t *m)  { return m->n == m->cap; }
static int   model_empty(const model_t *m) { return m->n == 0; }

static int model_push(model_t *m, uint8_t v)
{
    if (model_full(m)) return 0;
    m->q[m->n++] = v;
    return 1;
}

static uint8_t model_pop(model_t *m)
{
    uint8_t v = m->q[0];
    memmove(&m->q[0], &m->q[1], m->n - 1);
    m->n--;
    return v;
}

/* ------------------------------------------------------------------ */
/* tests                                                               */
/* ------------------------------------------------------------------ */

static void test_create_two(void)
{
    begin("create() two buffers of different sizes");
    arena_reset();

    ring_buffer a, b;
    create(&a, storage_a, SIZE_A);
    create(&b, storage_b, SIZE_B);

    dump("A after create", &a, storage_a);
    dump("B after create", &b, storage_b);

    CHECK_EQ("A size", a.size, SIZE_A);
    CHECK_EQ("B size", b.size, SIZE_B);
    CHECK_EQ("A count", a.count, 0);
    CHECK_EQ("B count", b.count, 0);

    /* Each struct must remember its OWN storage, not a shared array. */
    checks_run++;
    if (a.pxbuffer == storage_a && b.pxbuffer == storage_b && a.pxbuffer != b.pxbuffer) {
        printf("    ok    %-30s (A and B point at different arrays)\n", "pxbuffer wiring");
    } else {
        checks_failed++;
        printf("    FAIL  pxbuffer wiring: A->%p B->%p (expected %p and %p)\n",
               (void *)a.pxbuffer, (void *)b.pxbuffer, (void *)storage_a, (void *)storage_b);
    }
}

static void test_isolation(void)
{
    begin("pushing into A must not disturb B");
    arena_reset();

    ring_buffer a, b;
    create(&a, storage_a, SIZE_A);
    create(&b, storage_b, SIZE_B);

    for (unsigned i = 0; i < SIZE_A; i++) push(&a, (uint8_t)(0x50 + i));

    dump("A filled", &a, storage_a);
    dump("B untouched", &b, storage_b);

    CHECK_EQ("A count", a.count, SIZE_A);
    CHECK_EQ("B count still 0", b.count, 0);
    CHECK_EQ("B head still 0", b.head, 0);
    CHECK_EQ("B tail still 0", b.tail, 0);

    checks_run++;
    int clean = 1;
    for (unsigned i = 0; i < SIZE_B; i++) if (storage_b[i] != 0) clean = 0;
    if (clean) printf("    ok    %-30s (A's writes stayed in A)\n", "B storage untouched");
    else { checks_failed++; printf("    FAIL  B's storage was modified by pushes into A\n"); }

    check_guards();
}

static void test_sizes_respected(void)
{
    begin("each buffer fills to its OWN size, not a shared constant");
    arena_reset();

    ring_buffer a, b;
    create(&a, storage_a, SIZE_A);
    create(&b, storage_b, SIZE_B);

    for (unsigned i = 0; i < SIZE_A; i++) {
        char what[48];
        snprintf(what, sizeof what, "A push #%u accepted", i + 1);
        CHECK_EQ(what, push(&a, (uint8_t)(i + 1)), 1);
    }
    CHECK_EQ("A rejects push #7", push(&a, 0xEE), 0);

    for (unsigned i = 0; i < SIZE_B; i++) {
        char what[48];
        snprintf(what, sizeof what, "B push #%u accepted", i + 1);
        CHECK_EQ(what, push(&b, (uint8_t)(i + 1)), 1);
    }
    CHECK_EQ("B rejects push #5", push(&b, 0xEE), 0);

    dump("A full (6)", &a, storage_a);
    dump("B full (4)", &b, storage_b);

    CHECK_EQ("A count", a.count, SIZE_A);
    CHECK_EQ("B count", b.count, SIZE_B);

    check_guards();
}

static void test_interleaved_fifo(void)
{
    begin("interleaved traffic keeps each FIFO in order");
    arena_reset();

    ring_buffer a, b;
    create(&a, storage_a, SIZE_A);
    create(&b, storage_b, SIZE_B);

    /* Odd numbers into A, even numbers into B, alternating. */
    push(&a, 1); push(&b, 2);
    push(&a, 3); push(&b, 4);
    push(&a, 5); push(&b, 6);

    dump("A", &a, storage_a);
    dump("B", &b, storage_b);

    CHECK_EQ("A pop #1", pop(&a), 1);
    CHECK_EQ("B pop #1", pop(&b), 2);
    CHECK_EQ("A pop #2", pop(&a), 3);
    CHECK_EQ("B pop #2", pop(&b), 4);
    CHECK_EQ("A pop #3", pop(&a), 5);
    CHECK_EQ("B pop #3", pop(&b), 6);

    CHECK_EQ("A empty", a.count, 0);
    CHECK_EQ("B empty", b.count, 0);

    check_guards();
}

static void test_wraparound_each(void)
{
    begin("wraparound happens at each buffer's own size");
    arena_reset();

    ring_buffer a, b;
    create(&a, storage_a, SIZE_A);
    create(&b, storage_b, SIZE_B);

    /* Fill both, drain part of each, refill: forces tail ahead of head. */
    for (unsigned i = 0; i < SIZE_A; i++) push(&a, (uint8_t)(10 + i));  /* 10..15 */
    for (unsigned i = 0; i < SIZE_B; i++) push(&b, (uint8_t)(60 + i));  /* 60..63 */

    for (unsigned i = 0; i < 4; i++) pop(&a);   /* drops 10,11,12,13 */
    for (unsigned i = 0; i < 3; i++) pop(&b);   /* drops 60,61,62    */

    dump("A drained 4", &a, storage_a);
    dump("B drained 3", &b, storage_b);
    CHECK_EQ("A count", a.count, 2);
    CHECK_EQ("B count", b.count, 1);

    for (unsigned i = 0; i < 4; i++) push(&a, (uint8_t)(20 + i));  /* 20..23 */
    for (unsigned i = 0; i < 3; i++) push(&b, (uint8_t)(70 + i));  /* 70..72 */

    dump("A refilled", &a, storage_a);
    dump("B refilled", &b, storage_b);
    CHECK_EQ("A count refilled", a.count, SIZE_A);
    CHECK_EQ("B count refilled", b.count, SIZE_B);

    const uint8_t expect_a[SIZE_A] = {14, 15, 20, 21, 22, 23};
    const uint8_t expect_b[SIZE_B] = {63, 70, 71, 72};

    for (unsigned i = 0; i < SIZE_A; i++) {
        char what[48];
        snprintf(what, sizeof what, "A wrapped pop #%u", i + 1);
        CHECK_EQ(what, pop(&a), expect_a[i]);
    }
    for (unsigned i = 0; i < SIZE_B; i++) {
        char what[48];
        snprintf(what, sizeof what, "B wrapped pop #%u", i + 1);
        CHECK_EQ(what, pop(&b), expect_b[i]);
    }

    check_guards();
}

static void test_full_a_empty_b(void)
{
    begin("a FULL buffer and an EMPTY buffer look identical in head/tail");
    arena_reset();

    ring_buffer a, b;
    create(&a, storage_a, SIZE_A);
    create(&b, storage_b, SIZE_B);

    for (unsigned i = 0; i < SIZE_A; i++) push(&a, (uint8_t)(200 + i));

    dump("A (full)", &a, storage_a);
    dump("B (empty)", &b, storage_b);

    printf("    A: head==tail is %s, and A is FULL\n",  a.head == a.tail ? "true" : "false");
    printf("    B: head==tail is %s, and B is EMPTY\n", b.head == b.tail ? "true" : "false");

    CHECK_EQ("A count says full", a.count, SIZE_A);
    CHECK_EQ("B count says empty", b.count, 0);

    printf("    Note: only 'count' can tell these two states apart.\n");
}

static void test_reinit(void)
{
    begin("create() on a used buffer resets it");
    arena_reset();

    ring_buffer a;
    create(&a, storage_a, SIZE_A);
    push(&a, 1); push(&a, 2); push(&a, 3);
    dump("A used", &a, storage_a);

    create(&a, storage_a, SIZE_A);
    dump("A recreated", &a, storage_a);

    CHECK_EQ("count reset", a.count, 0);
    CHECK_EQ("head reset", a.head, 0);
    CHECK_EQ("tail reset", a.tail, 0);
}

static void test_pop_when_empty(void)
{
    begin("pop() on an empty buffer -- observe, no assertion");
    arena_reset();

    ring_buffer a;
    create(&a, storage_a, SIZE_A);

    push(&a, 0);            /* a legitimate zero byte */
    uint8_t real = pop(&a); /* real data */
    uint8_t fake = pop(&a); /* nothing there */

    printf("    pop() with a real 0 byte queued returned %u\n", real);
    printf("    pop() on an empty buffer returned       %u\n", fake);
    printf("    Q: standing outside, how does a caller tell these two apart?\n");
    printf("    Q: your push() reports failure with a bool. What should pop() do?\n");
}

/* Randomized interleaved traffic against two independent models. */
static void test_against_model(void)
{
    begin("randomized interleaved traffic vs two reference models");
    arena_reset();

    ring_buffer a, b;
    create(&a, storage_a, SIZE_A);
    create(&b, storage_b, SIZE_B);

    model_t ma, mb;
    model_init(&ma, SIZE_A);
    model_init(&mb, SIZE_B);

    unsigned seed = 987654321u;
    uint8_t next = 1;
    int reported = 0;

    for (unsigned step = 0; step < 500 && !reported; step++) {
        seed = seed * 1103515245u + 12345u;
        int pick_a  = ((seed >> 16) & 1);
        int do_push = ((seed >> 17) & 1);

        ring_buffer *rb = pick_a ? &a : &b;
        model_t     *m  = pick_a ? &ma : &mb;
        const char  *nm = pick_a ? "A" : "B";

        if (do_push) {
            if (next == 0) next = 1;          /* keep 0 out of the data */
            int got = push(rb, next);
            int exp = model_push(m, next);
            next++;
            if (got != exp) {
                printf("    FAIL  step %3u buf %s: push() returned %d, model says %d\n",
                       step, nm, got, exp);
                checks_failed++; reported = 1;
            }
        } else if (!model_empty(m)) {
            uint8_t got = pop(rb);
            uint8_t exp = model_pop(m);
            if (got != exp) {
                printf("    FAIL  step %3u buf %s: pop() returned %u, model says %u\n",
                       step, nm, got, exp);
                checks_failed++; reported = 1;
            }
        }

        if (!reported && (a.count != ma.n || b.count != mb.n)) {
            printf("    FAIL  step %3u: counts drifted. A=%u (model %u)  B=%u (model %u)\n",
                   step, a.count, ma.n, b.count, mb.n);
            checks_failed++; reported = 1;
        }
    }

    checks_run++;
    if (!reported) printf("    ok    %-30s\n", "500 interleaved ops matched");

    check_guards();
}

/* ------------------------------------------------------------------ */

int main(void)
{
    printf("ring_buffer test bench -- multiple instances\n");
    printf("buffer A holds %d bytes, buffer B holds %d bytes\n", SIZE_A, SIZE_B);
    printf("=============================================================\n");

    test_create_two();
    test_isolation();
    test_sizes_respected();
    test_interleaved_fifo();
    test_wraparound_each();
    test_full_a_empty_b();
    test_reinit();
    test_pop_when_empty();
    test_against_model();

    printf("\n=============================================================\n");
    printf("%d checks, %d failed\n", checks_run, checks_failed);
    return checks_failed != 0;
}
