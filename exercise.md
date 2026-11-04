I've finished the research: I checked open issues and PRs on 2026-11-02, read the related discussions, and read the historical fixes in this repo's git history. Below are 7 exercises, ordered easiest to hardest.

**Before you start: spoiler warnings**
- **Every linked page below contains spoilers.** Issue and PR descriptions mostly name the cause, and some include the fix. Read them only after you've attempted the exercise.
- **The collapsed tool output in this chat** contains the actual diffs I read to judge difficulty. Don't expand it.
- **`git log` and `git show` on these commits** reveal the answers through their titles and diffs.

**Setup for historical exercises:** check out the kernel as it was just before the fix, in a separate folder: `git worktree add ..\frt-exN <hash>^`. The `^` means "the parent", which is the buggy version. Build with the Windows simulator port (`portable/MSVC-MingW`) using your MinGW GCC. A small `main.c` and the template `FreeRTOSConfig.h` are enough. The FreeRTOS/FreeRTOS repo's `Demo/WIN32-MingW` is a ready-made starting point. For open exercises, use current `main`.

---

### 1. A task waiting for a notification gets "resumed" — Easy
1. **Link:** [PR #865](https://github.com/FreeRTOS/FreeRTOS-Kernel/pull/865) ⚠️ the description states the cause. Buggy version: `0640b2e48^`.
2. **Concept:** the difference between Blocked and Suspended. Also, which list a task waiting with no timeout actually lives on.
3. **Problem:** a task blocked forever in `ulTaskNotifyTake()` returns early, with nothing received, when another task calls `vTaskResume()` on it. `vTaskGetInfo()` also reports it in the wrong state.
4. **Prerequisites:** the task states diagram; a TCB's state and event list items. About 1 to 2 hours.
5. **Start in `tasks.c`:** `vTaskResume()` and the helper it calls to decide whether a task really is suspended; `xTaskGenericNotifyWait()`.
6. **Reproduce:**
   - Task W calls `ulTaskNotifyTake( pdTRUE, portMAX_DELAY )` and prints the return value.
   - The main task waits a few ticks, then calls `vTaskResume( W )`.
   - W should stay blocked. Check whether it does.
7. **Question:** list every way a task can end up on `xSuspendedTaskList`. For each one, what does `vTaskResume()` need to know to behave correctly?

### 2. `eTaskGetState()` reports the wrong state — Easy to moderate ⭐ best first exercise
1. **Link:** [PR #679](https://github.com/FreeRTOS/FreeRTOS-Kernel/pull/679) ⚠️ the description states the cause. Buggy version: `953c1ee6c^`.
2. **Concept:** the pending-ready list; what happens when a queue unblocks a task while the scheduler is suspended.
3. **Problem:** a task that a queue has already made ready is still reported as `eBlocked`.
4. **Prerequisites:** `listLIST_ITEM_CONTAINER`, `vTaskSuspendAll()`. About 2 hours.
5. **Start at:** `eTaskGetState()`, `xTaskRemoveFromEventList()` and `xTaskResumeAll()` in `tasks.c`; the container macros in `include/list.h`.
6. **Reproduce:** this is fully deterministic on the host, with no interrupts needed.
   - Task A blocks in `xQueueReceive()` with a long timeout.
   - The main task calls `vTaskSuspendAll()`, then `xQueueSend( q, &v, 0 )`, then prints `eTaskGetState( A )`, then calls `xTaskResumeAll()`.
   - A has already been made ready at that point, so the state should say so.
7. **Question:** a TCB has two list items. While the scheduler is suspended and a task gets unblocked, where does each of them live?

### 3. Equal-priority switching with time slicing off — Moderate
1. **Link:** [PR #568](https://github.com/FreeRTOS/FreeRTOS-Kernel/pull/568) ⚠️ the description states the intended rule. Buggy version: `1072988de^`. The same rule appears in other functions in a later fix ([PR #603](https://github.com/FreeRTOS/FreeRTOS-Kernel/pull/603), also a spoiler), which makes a good follow-up.
2. **Concept:** tick handling, preemption versus time slicing.
3. **Problem:** with `configUSE_TIME_SLICING = 0`, a task gets switched away from in favour of an equal-priority task that just woke up.
4. **Prerequisites:** `configUSE_PREEMPTION` and `configUSE_TIME_SLICING`, and the delayed lists. About 2 to 3 hours.
5. **Start at:** `xTaskIncrementTick()`, and the documented meaning of the two config options in `include/FreeRTOS.h` and on freertos.org.
6. **Reproduce:**
   - Create two tasks at the same priority. T1 loops on `vTaskDelay()` and counts its wake-ups. T2 spins forever.
   - Set preemption on and time slicing off.
   - Check whether T1 ever runs again after T2 starts spinning.
7. **Question:** first write down your own definition of "time slicing off". Then decide what should happen when an equal-priority task unblocks during a tick.

### 4. A stale "delay aborted" flag — Moderate (open, unconfirmed)
1. **Link:** [PR #1510](https://github.com/FreeRTOS/FreeRTOS-Kernel/pull/1510) ⚠️ the description states the cause and the fix.
   - **Status:** open, with no maintainer review and no linked issue.
   - **My check:** I read the current code paths and the reported behaviour looks plausible. I haven't run it, so treat it as a suspected bug, not a confirmed one.
2. **Concept:** timeouts (`TimeOut_t`, `xTaskCheckForTimeOut()`), `xTaskAbortDelay()`, and how event groups block differently from queues.
3. **Problem:** after an event-group wait is aborted, the task's *next* `xSemaphoreTake( m, portMAX_DELAY )` fails immediately.
4. **Prerequisites:** exercise 2, plus the queue receive loop in `queue.c`. About 3 to 4 hours.
5. **Start at:**
   - `xTaskAbortDelay()`, `xTaskCheckForTimeOut()` and `vTaskInternalSetTimeOutState()` in `tasks.c`
   - `xEventGroupWaitBits()` in `event_groups.c`
   - `xQueueSemaphoreTake()` in `queue.c`
6. **Reproduce:** set `INCLUDE_xTaskAbortDelay` to 1.
   - Task A calls `xEventGroupWaitBits( ..., portMAX_DELAY )`.
   - Task B takes a mutex, then calls `xTaskAbortDelay( A )`.
   - A then calls `xSemaphoreTake( mutex, portMAX_DELAY )`, while B releases the mutex 50 ticks later.
   - A should wait until then and get the mutex.
7. **Question:** which code paths *set* the "your wait was aborted" information, and which paths are *guaranteed to consume* it?

### 5. Deleting or suspending a task that's already been deleted — Moderate (open, unconfirmed)
1. **Link:** [Issue #1503](https://github.com/FreeRTOS/FreeRTOS-Kernel/issues/1503) ⚠️ the body fully explains the cause. Its fix is the open [PR #1507](https://github.com/FreeRTOS/FreeRTOS-Kernel/pull/1507). **Status:** neither has a maintainer response yet.
2. **Concept:** task deletion, the termination list, and the idle task's cleanup.
3. **Problem:** the idle task crashes some time after a task is deleted twice, or deleted and then suspended, before the idle task has had a chance to clean up.
4. **Prerequisites:** how self-deletion works, and the `eDeleted` state. About 3 hours.
5. **Start at:** `vTaskDelete()`, `vTaskSuspend()`, and the idle task's cleanup function in `tasks.c`. Also read the `eDeleted` documentation in `include/task.h`.
6. **Reproduce:**
   - A victim task at a priority above idle calls `vTaskDelete( NULL )`.
   - A second task at the same priority then calls `vTaskDelete( victim )`, or `vTaskSuspend( victim )`, before idle runs.
   - Let idle run, and watch for an assert or crash.
7. **Question:** is this a kernel bug or application misuse? Defend your answer using what the API documentation promises. Then ask whether the kernel should defend against misuse anyway.

### 6. Priority disinheritance after a mutex timeout — Moderate to hard
1. **Link:** [Issue #1337](https://github.com/FreeRTOS/FreeRTOS-Kernel/issues/1337) ⚠️ it names the exact faulty statement. The fix is [PR #1338](https://github.com/FreeRTOS/FreeRTOS-Kernel/pull/1338). Buggy version: `1dbc77697^`.
   - **Confirmed:** the bug was fixed and merged.
   - **Note:** the reporter couldn't reproduce it, so building the reproduction *is* the exercise.
2. **Concept:** priority inheritance, the ready lists, and the `uxTopReadyPriority` bitmap.
3. **Problem:** shortly after a higher-priority task's mutex take times out, the scheduler misbehaves. It may assert during task selection, or a ready task may stop being scheduled.
4. **Prerequisites:** `xTaskPriorityInherit()`; the port-optimised task selection macros in `portable/MSVC-MingW/portmacro.h`. The bug only appears with `configUSE_PORT_OPTIMISED_TASK_SELECTION = 1`, which the Windows port enables by default. About 4 to 6 hours.
5. **Start at:** `vTaskPriorityDisinheritAfterTimeout()`, `xTaskPriorityInherit()` and `taskSELECT_HIGHEST_PRIORITY_TASK()` in `tasks.c`.
6. **Reproduce:**
   - L (priority 1) takes a mutex and busy-loops while holding it.
   - H (priority 3) calls `xSemaphoreTake( m, 10 )`, so L inherits priority 3.
   - Have no other task at priority 3, and keep L running past H's timeout.
   - Enable `configASSERT` and watch what happens after the timeout.
7. **Question:** `uxTopReadyPriority` is a cache of the ready lists. Right after the disinherit, write down what the cache *should* contain and what it *actually* contains.

### 7. Wrong event-list position after `vTaskPrioritySet()` — Hard (open)
1. **Link:** [Issue #1364](https://github.com/FreeRTOS/FreeRTOS-Kernel/issues/1364) ⚠️ the body contains the fix. The fix is the open [PR #1380](https://github.com/FreeRTOS/FreeRTOS-Kernel/pull/1380), which has no review yet. The [forum thread](https://forums.freertos.org/t/why-vtaskpriorityset-doesnt-set-xeventlistitem-value-according-to-uxpriority/24895) is also a spoiler.
   - **Status:** the reporter showed it with a test, but they themselves called it a corner case.
   - **Note:** it was reported on a dual-core RP2040. I haven't checked whether it reproduces on a single core.
2. **Concept:** event lists sorted by priority, with priority inheritance on top.
3. **Problem:** a task that holds one mutex has inherited a higher priority. It then lowers its own base priority, and then blocks on a second, contended mutex. It never acquires that mutex, even though it's the highest-priority waiter.
4. **Prerequisites:** exercise 6, `vListInsert()` ordering, and `listSET_LIST_ITEM_VALUE`. About a day.
5. **Start at:**
   - `vTaskPrioritySet()`, `xTaskPriorityInherit()` and `vTaskPlaceOnEventList()` in `tasks.c`
   - `vListInsert()` in `list.c`
6. **Reproduce:** build the scenario from part 3 with 3 or 4 tasks and two mutexes. Print each waiter's event-item value with `listGET_LIST_ITEM_VALUE` before it blocks.
7. **Question:** an event list is sorted by item value. Who is responsible for keeping a task's item value consistent with its *effective* priority, and when?

---

**What I left out:**
- [Issue #1349](https://github.com/FreeRTOS/FreeRTOS-Kernel/issues/1349): maintainers concluded it isn't a bug. It's still worth reading later as a design discussion.
- PR #1408 and PR #1454: they change behaviour by design rather than fix bugs.
- The SMP-only issues, the port-specific issues, and the stream-buffer issue #1450: outside your focus areas.
- The `tasks.c:4103` analyzer warning from earlier: a false positive, so not an exercise.

**Recommended first exercise:** #2, `eTaskGetState()`. It exercises exactly what you're learning: list containers in `list.h`, the state-versus-event list item split in `tasks.c`, and a queue in `queue.c` waking a task. The reproduction is deterministic and needs only a few lines. #1 is slightly easier but mostly about notifications.

When you start one, tell me what you observe. If you get stuck, I'll give hints one step at a time, starting with questions.